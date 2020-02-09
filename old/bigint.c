
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "bigint.h"


#define ABS(n) (n < 0 ? (~n) + 1 : n)


static enum BI_error BI_errno = 0;


bigint* BI_new(int32_t val)
{
  bigint* bi;
  uint64_t abs;

  bi = calloc(1, sizeof(bigint));
  bi->len = 0;
  bi->maxlen = INIT_VAL_LEN;
  bi->val = calloc(1, bi->maxlen);

  bi->sign = val < 0 ? -1 : 1;
  abs = ABS(val);

  while (abs)
  {
    bi->val[bi->len++] = abs % 256;
    abs /= 256;
  } 

  if (val == 0) bi->len = 1;

  return bi;
}


void BI_free(bigint* bi)
{
  if (bi == NULL) return;
  free(bi->val);
  free(bi);
}


enum BI_error BI_add(bigint* a, bigint* b, bigint* c)
{
  if (a == NULL || b == NULL || c == NULL)
    return BI_errno = BIERR_NULLARG;

  int sign = 1;

  if (b->sign == -1 && c->sign == -1)
    sign = -1;
  else if (b->sign == -1 && BI_cmp_mag(b, c) == BI_GREATERTHAN)
    sign = -1;
  else if (c->sign == -1 && BI_cmp_mag(c, b) == BI_GREATERTHAN)
    sign = -1;

  bigint* temp = BI_new((int32_t) 0);

  int i = 0, j = 0, k = 0, carry = 0;

  while (i < b->len || j < c->len || carry)
  {
    if (temp->len == temp->maxlen)
    {
      temp->val = realloc(temp->val, temp->maxlen * 2);
      temp->maxlen *= 2;
    }

    int sum = (carry +
              (i < b->len ? b->val[i++] * b->sign : 0) +
              (j < c->len ? c->val[j++] * c->sign : 0));

    if (b->sign == 1 && sum < 0)
      temp->val[k++] = 256 - (ABS(sum) % 256);
    else
      temp->val[k++] = ABS(sum) % 256;

    if (sum > 255) carry = 1;
    else if (sum < -255) carry = -1;
    else if (b->sign == 1 && sum < 0) carry = -1;
    else carry = 0;

    if (temp->val[k-1]) temp->len = k;
  }

  if (a->maxlen != temp->maxlen)
  {
    a->val = realloc(a->val, temp->maxlen);
    a->maxlen = temp->maxlen;
  }

  a->sign = sign;
  a->len = temp->len;
  memcpy(a->val, temp->val, temp->maxlen);
  BI_free(temp);

  return BIERR_ZERO;
}


enum BI_error BI_sub(bigint* a, bigint* b, bigint* c)
{
  if (a == NULL || b == NULL || c == NULL)
    return BI_errno = BIERR_NULLARG;

  int csign = c->sign;
  c->sign *= -1;

  enum BI_error status = BI_add(a, b, c);

  if (c != a)
    c->sign = csign;

  return BIERR_ZERO;
}


enum BI_error BI_mul(bigint* a, bigint* b, bigint* c)
{
  if (a == NULL || b == NULL || c == NULL)
    return BI_errno = BIERR_NULLARG;

  int bsign = b->sign, csign = c->sign;
  int sign = bsign * csign;

  b->sign = 1;
  c->sign = 1;

  bigint* result = BI_new((int32_t) 0);
  bigint* counter = BI_new((int32_t) 0);
  BI_add(counter, counter, b);
  bigint* zero = BI_new((int32_t) 0);
  bigint* one = BI_new((int32_t) 1);

  while(BI_cmp(counter, zero) != BI_EQUAL)
  {
    BI_add(result, result, c);
    BI_sub(counter, counter, one);
  }

  BI_free(counter);
  BI_free(zero);
  BI_free(one);

  b->sign = bsign;
  c->sign = csign;

  if (a->maxlen != result->maxlen)
  {
    a->val = realloc(a->val, result->maxlen);
    a->maxlen = result->maxlen;
  }

  a->sign = sign;
  a->len = result->len;
  memcpy(a->val, result->val, result->maxlen);
  BI_free(result);

  return BIERR_ZERO;
}


enum BI_error BI_div(bigint* a, bigint* n, bigint* d)
{
  if (a == NULL || n == NULL || d == NULL)
    return BI_errno = BIERR_NULLARG;

  int sign = n->sign * d->sign;
  int nsign = n->sign; n->sign = 1;
  int dsign = d->sign; d->sign = 1;

  bigint* result = BI_new(0);
  if (BI_cmp(d, result) == BI_EQUAL)
    return BI_errno = BIERR_DIVZERO;

  bigint* temp = BI_new(0);
  BI_add(temp, temp, n);
  bigint* one = BI_new(1);

  enum BI_comparison cmp = BI_cmp(temp, d);

  while (cmp == BI_GREATERTHAN || cmp == BI_EQUAL)
  {
    BI_add(result, result, one);
    BI_sub(temp, temp, d);
    cmp = BI_cmp(temp, d);
  }

  BI_free(one);
  BI_free(temp);

  a->sign = sign;
  a->len = result->len;
  memcpy(a->val, result->val, result->maxlen);
  BI_free(result);

  if (n != a) n->sign = nsign;
  if (d != a) d->sign = dsign;

  return BIERR_ZERO;
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
