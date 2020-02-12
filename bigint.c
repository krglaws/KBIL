
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "bigint.h"


#define ABS(n) (n < 0 ? (~n) + 1 : n)


static enum BI_error BI_errno = 0;


bigint* BI_new(int32_t num)
{
  bigint* bi;
  uint64_t abs;

  bi = calloc(1, sizeof(bigint));
  bi->len = 0;
  bi->val = calloc(1, 1);
  bi->sign = num < 0 ? -1 : 1;

  abs = ABS(num);

  while (abs)
  {
    bi->val[bi->len] = abs % 256;
    bi->val = realloc(bi->val, ++bi->len);
    abs /= 256;
  } 

  if (num == 0)
    bi->len = 1;

  return bi;
}


void BI_free(bigint* bi)
{
  if (bi == NULL) return;
  free(bi->val);
  free(bi);
}


enum BI_error BI_set_i(bigint* bi, int32_t num)
{
  if (bi == NULL)
    return BI_errno = BIERR_NULLARG;

  bigint* temp = BI_new(num);
  BI_set_bi(bi, temp);
  BI_free(temp);

  return BI_errno = BIERR_ZERO;
}


enum BI_error BI_set_bi(bigint* bi, bigint* num)
{
  if (bi == NULL || num == NULL)
    return BI_errno = BIERR_NULLARG;

  bi->sign = num->sign;
  bi->len = num->len;
  bi->val = realloc(bi->val, num->len);
  memcpy(bi->val, num->val, num->len);

  return BI_errno = BIERR_ZERO;
}


enum BI_error BI_add(bigint* res, bigint* a, bigint* b)
{
  if (res == NULL || a == NULL || b == NULL)
    return BI_errno = BIERR_NULLARG;

  int sign = 1;

  if (a->sign == -1 && b->sign == -1)
    sign = -1;
  else if (a->sign == -1 && BI_cmp_mag(a, b) == BI_GREATERTHAN)
    sign = -1;
  else if (b->sign == -1 && BI_cmp_mag(b, a) == BI_GREATERTHAN)
    sign = -1;

  bigint* temp = BI_new(0);

  int i = 0, j = 0, k = 0, carry = 0;

  while (i < a->len || j < b->len || carry)
  {
    if (k == temp->len)
      temp->val = realloc(temp->val, ++temp->len);

    int sum = (carry +
              (i < a->len ? a->val[i++] * a->sign : 0) +
              (j < b->len ? b->val[j++] * b->sign : 0));

    if (a->sign == 1 && sum < 0)
      temp->val[k++] = 256 - (ABS(sum) % 256);
    else
      temp->val[k++] = ABS(sum) % 256;

    if (sum > 255) carry = 1;
    else if (sum < -255) carry = -1;
    else if (a->sign == 1 && sum < 0) carry = -1;
    else carry = 0;

    if (temp->val[k-1]) temp->len = k;
  }

  BI_set_bi(res, temp);
  res->sign = sign;
  BI_free(temp);

  return BIERR_ZERO;
}


enum BI_error BI_sub(bigint* res, bigint* a, bigint* b)
{
  if (res == NULL || a == NULL || b == NULL)
    return BI_errno = BIERR_NULLARG;

  int bsign = b->sign;
  b->sign *= -1;

  enum BI_error status = BI_add(res, a, b);

  if (b != res)
    b->sign = bsign;

  return BIERR_ZERO;
}


enum BI_error BI_mul(bigint* res, bigint* a, bigint* b)
{
  if (res == NULL || a == NULL || b == NULL)
    return BI_errno = BIERR_NULLARG;

  int sign = a->sign * b->sign;

  int bsign = b->sign;
  b->sign = 1;

  bigint* product = BI_new(0);
  bigint* temp = BI_new(0);

  for (int i = 0; i < a->len; i++)
  {
    BI_set_i(temp, 0);

    for (int j = 0; j < a->val[i]; j++)
      BI_add(temp, temp, b);

    unsigned char* fullval = calloc(1, temp->len + i);
    memcpy(fullval + i, temp->val, temp->len);
    free(temp->val);
    temp->val = fullval;
    temp->len += i;

    BI_add(product, product, temp);
  }

  BI_free(temp);
  b->sign = bsign;
  BI_set_bi(res, product);
  res->sign = sign;
  BI_free(product);

  return BIERR_ZERO;
}


enum BI_error BI_pow(bigint* res, bigint* b, bigint* e)
{
  if (res == NULL || b == NULL || e == NULL)
    return BI_errno = BIERR_NULLARG;

  if (e->sign == -1)
  {
    BI_set_i(res, 0);
    return BI_errno = BIERR_ZERO;
  }

  bigint* product = BI_new(1);
  bigint* temp = BI_new(0);
  BI_set_bi(temp, b);
  bigint* zero = BI_new(0);
  bigint* two = BI_new(2);
  bigint* powcount = BI_new(0);
  BI_set_bi(powcount, e);
  bigint* currpow = BI_new(1);

  while (BI_cmp(powcount, zero) != BI_EQUAL)
  {
    if (BI_cmp(currpow, powcount) == BI_GREATERTHAN)
    {
      BI_set_i(currpow, 1);
      BI_set_bi(temp, b);
    }

    BI_mul(product, product, temp);
    BI_sub(powcount, powcount, currpow);
    BI_mul(currpow, currpow, two);
    BI_mul(temp, temp, temp);
  }

  BI_free(temp);
  BI_free(zero);
  BI_free(two);
  BI_free(powcount);
  BI_free(currpow);
  BI_set_bi(res, product);
  BI_free(product);

  return BI_errno = BIERR_ZERO;
}


enum BI_error BI_div_mod(bigint* res, bigint* rem, bigint* n, bigint* d)
{
  if (n == NULL || d == NULL)
    return BI_errno = BIERR_NULLARG;

  int sign = n->sign * d->sign;
  int dsign = d->sign, nsign = n->sign;
  n->sign = 1;
  d->sign = 1;

  bigint* quotient = BI_new(0);
  if (BI_cmp(d, quotient) == BI_EQUAL)
    return BI_errno = BIERR_DIVZERO;

  bigint* temp = BI_new(0);
  BI_add(temp, temp, n);

  bigint* multiplier = BI_new(1);
  bigint* two = BI_new(2);
  bigint* subtractor = BI_new(0);
  BI_add(subtractor, subtractor, d);

  while (BI_cmp(temp, d) != BI_LESSTHAN)
  {
    if (BI_cmp(subtractor, temp) == BI_GREATERTHAN)
    {
      BI_set_bi(subtractor, d);
      BI_set_i(multiplier, 1);
      continue;
    }
    BI_sub(temp, temp, subtractor);
    BI_add(quotient, quotient, multiplier);

    BI_mul(subtractor, two, subtractor);
    BI_mul(multiplier, multiplier, two);
  }

  if (res)
  { 
    BI_set_bi(res, quotient);
    res->sign = sign;
  }

  if (rem)
    BI_set_bi(rem, temp);

  n->sign = nsign;
  d->sign = dsign;  

  BI_free(quotient);
  BI_free(temp);
  BI_free(multiplier);
  BI_free(two);
  BI_free(subtractor);

  return BIERR_ZERO;
}


enum BI_error BI_div(bigint* res, bigint* n, bigint* d)
{
  return BI_div_mod(res, NULL, n, d);
}


enum BI_error BI_mod(bigint* res, bigint* n, bigint* m)
{
  return BI_div_mod(NULL, res, n, m);
}


enum BI_comparison BI_cmp(bigint* a, bigint* b)
{
  if (a->sign > b->sign)
    return BI_GREATERTHAN;

  if (b->sign > a->sign)
    return BI_LESSTHAN;

  if (a->len > b->len)
    return BI_GREATERTHAN;

  if (b->len > a->len)
    return BI_LESSTHAN;

  for (int i = a->len-1; i >= 0; i--)
  {
    if (a->val[i] == b->val[i])
      continue;
    return a->val[i] > b->val[i] ? BI_GREATERTHAN : BI_LESSTHAN;
  }

  return BI_EQUAL; 
}


enum BI_comparison BI_cmp_mag(bigint* a, bigint* b)
{
  int a_sign = a->sign;
  int b_sign = b->sign;

  a->sign = 1;
  b->sign = 1;

  enum BI_comparison result = BI_cmp(a, b);

  a->sign = a_sign;
  b->sign = b_sign;

  return result;
}


char* BI_to_str(bigint* bi, int base)
{
  if (bi == NULL)
  {
    BI_errno = BIERR_NULLARG;
    return NULL;
  }

  if (base < 2 && base > 36)
  {
    BI_errno = BIERR_INVBASE;
    return NULL;
  }

  BI_errno = BIERR_NOTIMPL;
  return NULL;
}


void BI_print(bigint* bi)
{
  if (bi->sign == -1) printf("-");
  printf("[");
  for (int i = 0; i < bi->len; i++)
  {
    printf("%d", bi->val[i]);
    if (i < bi->len-1) printf(", ");
  }
  if (bi->len == 0) printf("0");
  printf("]\n");
}


void BI_perror(char* context)
{
  switch(BI_errno)
  {
    case BIERR_ZERO:
      fprintf(stderr, "%s: Success\n", context);
      break;
    case BIERR_NULLARG:
      fprintf(stderr, "%s: Null argument\n", context);
      break;
    case BIERR_INVBASE:
      fprintf(stderr, "%s: Invalid base\n", context);
      break;
    case BIERR_NOTIMPL:
      fprintf(stderr, "%s: Feature not yet implemented\n", context);
      break;
    case BIERR_DIVZERO:
      fprintf(stderr, "%s: Divide by zero\n", context);
      break;
    default:
      fprintf(stderr, "%s: Unknown error %d\n", context, BI_errno);
  }
}

