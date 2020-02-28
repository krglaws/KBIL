
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include <kbil.h>


#define ABS(n) (n < 0 ? (~n) + 1 : n)

#define MAX(a, b) (a > b ? a : b)


static enum BI_error BI_errno = 0;


bigint* BI_new_i(int num)
{
  bigint* bi = calloc(1, sizeof(bigint));
  
  long int abs = ABS(num);
  long int temp = abs;
  bi->len = 1;
  while (temp /= 256) bi->len++;
  bi->val = calloc(1, bi->len);
  bi->sign = num < 0 ? -1 : 1;

  for (int i = 0; i < bi->len; i++)
  {
    bi->val[i] = abs % 256;
    abs /= 256;
  }

  BI_errno = BIERR_ZERO;
  return bi;
}


bigint* BI_new_b(bigint* num)
{
  if (num == NULL)
  {
    BI_errno = BIERR_NULLARG;
    return NULL;
  }

  bigint* bi = BI_new_i(0);

  BI_set_b(bi, num);

  BI_errno = BIERR_ZERO;
  return bi;
}


bigint* BI_new_s(char* s, int base)
{
  if (s == NULL)
  {
    BI_errno = BIERR_NULLARG;
    return NULL;
  }

  if (base < 2 || base > 16)
  {
    BI_errno = BIERR_INVBASE;
    return NULL;
  }
  int sign = 1;
  
  if (s[0] == '-')
  {
    sign = -1;
    s++;
  }

  bigint* bi = BI_new_i(0);
  bigint* prod = BI_new_i(0);
  int slen = strlen(s);
  for (int i = slen - 1; i >= 0; i--)
  {
    if (s[i] < '0' && s[i] > '9' &&
        s[i] < 'A' && s[i] > 'F')
    {
      BI_errno = BIERR_BADENC;
      return NULL;
    }
    if ((s[i] <= '9' && s[i] - '0' >= base) ||
        (s[i] <= 'F' && s[i] - 'A' >= base))
    {
      BI_errno = BIERR_BADENC;
      return NULL;
    }

    int curr = s[i] < '9' ? s[i] - '0' : (s[i] - 'A') + 10;

    BI_pow_ii(prod, base, (slen - 1) - i);
    BI_mul_bi(prod, prod, curr);
    BI_add_bb(bi, bi, prod);
  }
  BI_free(prod);

  bi->sign = sign;
  BI_errno = BIERR_ZERO;
  return bi; 
}


void BI_free(bigint* bi)
{
  if (bi == NULL)
  {
    return;
  }

  free(bi->val);
  free(bi);
}


int BI_set_i(bigint* bi, int num)
{
  if (bi == NULL)
  {
    BI_errno = BIERR_NULLARG;
    return -1;
  }

  bigint* temp = BI_new_i(num);
  BI_set_b(bi, temp);
  BI_free(temp);

  BI_errno = BIERR_ZERO;
  return 0;
}


int BI_set_b(bigint* bi, bigint* num)
{
  if (bi == NULL || num == NULL)
  {
    BI_errno = BIERR_NULLARG;
    return -1;
  }

  bi->sign = num->sign;
  bi->len = num->len;
  bi->val = realloc(bi->val, num->len);
  memcpy(bi->val, num->val, num->len);

  BI_errno = BIERR_ZERO;
  return 0;
}


int BI_rand(bigint* bi, unsigned int bits)
{
  if (bi == NULL)
  {
    BI_errno = BIERR_NULLARG;
    return -1;
  }

  int rem = bits % 8;
  int len = (bits / 8) + (rem ? 1 : 0);

  unsigned char val[len];

  for (int i = 0; i < len; i++)
    val[i] = (unsigned char) rand();

  if (rem)
    val[len-1] >>= (8 - rem);

  bi->val = realloc(bi->val, len);
  memcpy(bi->val, val, len);
  bi->len = len;
  bi->sign = ((float) rand() / RAND_MAX) < 0.5 ? 1 : -1;

  return 0;
}


int BI_add_ii(bigint* res, int a, int b)
{
  if (res == NULL)
  {
    BI_errno = BIERR_NULLARG;
    return -1;
  }

  bigint* abi = BI_new_i(a);
  bigint* bbi = BI_new_i(b);

  int result = BI_add_bb(res, abi, bbi);

  BI_free(abi);
  BI_free(bbi);

  BI_errno = BIERR_ZERO;
  return 0;
}


int BI_add_bi(bigint* res, bigint* a, int b)
{
  if (res == NULL || a == NULL)
  {
    BI_errno = BIERR_NULLARG;
    return -1;
  }

  bigint* bbi = BI_new_i(b);

  int result = BI_add_bb(res, a, bbi);

  BI_free(bbi);

  return result;
}


int BI_add_bb(bigint* res, bigint* a, bigint* b)
{
  if (res == NULL || a == NULL || b == NULL)
  {
    BI_errno = BIERR_NULLARG;
    return -1;
  }

  int sign = 1, len = 1;

  enum BI_comparison mag = BI_cmp_mag_bb(a, b);

  if (a->sign == -1 && b->sign == -1)
  {
    sign = -1;
  }
  else if (a->sign == -1 && mag == BI_GREATERTHAN)
  {
    sign = -1;
  }
  else if (b->sign == -1 && mag == BI_LESSTHAN)
  {
    sign = -1;
  }

  int asign = a->sign;
  int bsign = b->sign;

  if (asign != bsign)
  {
    if (mag == BI_LESSTHAN)
    {
      a->sign = -1;
      b->sign = 1;
    }
    else
    {
      a->sign = 1;
      b->sign = -1;
    }
  }

  int templen = 1 + MAX(a->len, b->len);
  unsigned char tempval[templen];

  int i = 0, j = 0, k = 0, carry = 0;

  while (i < a->len || j < b->len || carry)
  {
    int sum, curra, currb;

    curra = (i < a->len ? a->val[i++] * a->sign : 0);
    currb = (j < b->len ? b->val[j++] * b->sign : 0);

    sum = carry + curra + currb;
    carry = 0;

    if (asign != bsign && sum < 0)
    {
      tempval[k] = 256 - (ABS(sum) % 256);
      carry = -1;
    }
    else
    {
      tempval[k] = ABS(sum) % 256;
      if (sum > 255)
      {
        carry = 1;
      }
      else if (sum < -255)
      {
        carry = -1;
      }
    }

    k++;

    if (tempval[k-1])
    {
      len = k;
    }
  }

  a->sign = asign;
  b->sign = bsign;

  res->len = len;
  res->sign = sign;
  free(res->val);
  res->val = calloc(1, len);
  memcpy(res->val, tempval, len);

  BI_errno = BIERR_ZERO;
  return 0;
}


int BI_inc(bigint* bi)
{
  return BI_add_bi(bi, bi, 1);
}


int BI_dec(bigint* bi)
{
  return BI_sub_bi(bi, bi, 1);
}


int BI_sub_ii(bigint* res, int a, int b)
{
  if (res == NULL)
  {
    BI_errno = BIERR_NULLARG;
    return -1;
  }

  bigint* abi = BI_new_i(a);
  bigint* bbi = BI_new_i(b);

  int result = BI_sub_bb(res, abi, bbi);

  BI_free(abi);
  BI_free(bbi);

  return result;
}


int BI_sub_bi(bigint* res, bigint* a, int b)
{
  if (res == NULL || a == NULL)
  {
    BI_errno = BIERR_NULLARG;
    return -1;
  }

  bigint* bbi = BI_new_i(b);

  int result = BI_sub_bb(res, a, bbi);

  BI_free(bbi);

  return result;
}


int BI_sub_ib(bigint* res, int a, bigint* b)
{
  if (res == NULL || b == NULL)
  {
    BI_errno = BIERR_NULLARG;
    return -1;
  }

  bigint* abi = BI_new_i(a);

  int result = BI_sub_bb(res, abi, b);

  BI_free(abi);

  return result;
}


int BI_sub_bb(bigint* res, bigint* a, bigint* b)
{
  if (res == NULL || a == NULL || b == NULL)
  {
    BI_errno = BIERR_NULLARG;
    return -1;
  }

  int bsign = b->sign;
  b->sign *= -1;

  int status = BI_add_bb(res, a, b);

  if (b != res)
  {
    b->sign = bsign;
  }

  return status;
}


int BI_mul_ii(bigint* res, int a, int b)
{
  if (res == NULL)
  {
    BI_errno = BIERR_NULLARG;
    return -1;
  }

  bigint* abi = BI_new_i(a);
  bigint* bbi = BI_new_i(b);

  int result = BI_mul_bb(res, abi, bbi);

  BI_free(abi);
  BI_free(bbi);

  return result;
}


int BI_mul_bi(bigint* res, bigint* a, int b)
{
  if (res == NULL || a == NULL)
  {
    BI_errno = BIERR_NULLARG;
    return -1;
  }

  bigint* bbi = BI_new_i(b);

  int result = BI_mul_bb(res, a, bbi);

  BI_free(bbi);

  return result;
}


int BI_mul_bb(bigint* res, bigint* a, bigint* b)
{
  if (res == NULL || a == NULL || b == NULL)
  {
    BI_errno = BIERR_NULLARG;
    return -1;
  }

  int sign = a->sign * b->sign;

  int bsign = b->sign;
  b->sign = 1;

  bigint* product = BI_new_i(0);
  bigint* temp = BI_new_i(0);

  for (int i = 0; i < a->len; i++)
  {
    BI_set_i(temp, 0);

    for (int j = 0; j < a->val[i]; j++)
    {
      BI_add_bb(temp, temp, b);
    }

    unsigned char* fullval = calloc(1, temp->len + i);
    memcpy(fullval + i, temp->val, temp->len);
    free(temp->val);
    temp->val = fullval;
    temp->len += i;

    BI_add_bb(product, product, temp);
  }

  BI_free(temp);
  b->sign = bsign;
  BI_set_b(res, product);
  res->sign = sign;
  BI_free(product);

  BI_errno = BIERR_ZERO;
  return 0;
}


int BI_pow_ii(bigint* res, int b, int e)
{
  if (res == NULL)
  {
    BI_errno = BIERR_NULLARG;
    return -1;
  }

  bigint* bbi = BI_new_i(b);
  bigint* ebi = BI_new_i(e);

  int result = BI_pow_bb(res, bbi, ebi);

  BI_free(bbi);
  BI_free(ebi);

  return result;
}


int BI_pow_bi(bigint* res, bigint* b, int e)
{
  if (res == NULL || b == NULL)
  {
    BI_errno = BIERR_NULLARG;
    return -1;
  }

  bigint* ebi = BI_new_i(e);

  int result = BI_pow_bb(res, b, ebi);

  BI_free(ebi);

  return result;
}


int BI_pow_ib(bigint* res, int b, bigint* e)
{
  if (res == NULL || e == NULL)
  {
    BI_errno = BIERR_NULLARG;
    return -1;
  }

  bigint* bbi = BI_new_i(b);

  int result = BI_pow_bb(res, bbi, e);

  BI_free(bbi);

  return result;
}


int BI_pow_bb(bigint* res, bigint* b, bigint* e)
{
  if (res == NULL || b == NULL || e == NULL)
  {
    BI_errno = BIERR_NULLARG;
    return -1;
  }

  if (e->sign == -1)
  {
    BI_set_i(res, 0);
    BI_errno = BIERR_ZERO;
    return 0;
  }

  BI_set_i(res, 1);
  bigint* temp = BI_new_b(b);
  bigint* powcount = BI_new_b(e);
  bigint* currpow = BI_new_i(1);

  while (BI_cmp_bi(powcount, 0) != BI_EQUAL)
  {
    if (BI_cmp_bb(currpow, powcount) == BI_GREATERTHAN)
    {
      BI_set_i(currpow, 1);
      BI_set_b(temp, b);
    }

    BI_mul_bb(res, res, temp);
    BI_sub_bb(powcount, powcount, currpow);
    BI_mul_bi(currpow, currpow, 2);
    BI_mul_bb(temp, temp, temp);
  }

  BI_free(temp);
  BI_free(powcount);
  BI_free(currpow);

  BI_errno = BIERR_ZERO;
  return 0;
}


int BI_div_mod_ii(bigint* res, bigint* rem, int n, int d)
{
  if (res == NULL || rem == NULL)
  {
    BI_errno = BIERR_NULLARG;
    return -1;
  }

  bigint* nbi = BI_new_i(n);
  bigint* dbi = BI_new_i(d);

  int result = BI_div_mod_bb(res, rem, nbi, dbi);

  BI_free(nbi);
  BI_free(dbi);

  return result;
}


int BI_div_mod_bi(bigint* res, bigint* rem, bigint* n, int d)
{
  if (res == NULL || rem == NULL || n == NULL)
  {
    BI_errno = BIERR_NULLARG;
    return -1;
  }

  bigint* dbi = BI_new_i(d);

  int result = BI_div_mod_bb(res, rem, n, dbi);

  BI_free(dbi);

  return result;
}


int BI_div_mod_ib(bigint* res, bigint* rem, int n, bigint* d)
{
  if (res == NULL || rem == NULL || d == NULL)
  {
    BI_errno = BIERR_NULLARG;
    return -1;
  }

  bigint* nbi = BI_new_i(n);

  int result = BI_div_mod_bb(res, rem, nbi, d);

  BI_free(nbi);

  return result;
}


int BI_div_mod_bb(bigint* res, bigint* rem, bigint* n, bigint* d)
{
  if (res == NULL || rem == NULL || n == NULL || d == NULL)
  {
    BI_errno = BIERR_NULLARG;
    return -1;
  }

  int sign = n->sign * d->sign;
  int dsign = d->sign, nsign = n->sign;
  n->sign = 1;
  d->sign = 1;

  if (BI_cmp_bi(d, 0) == BI_EQUAL)
  {
    BI_errno = BIERR_DIVZERO;
    return -1;
  }

  BI_set_i(res, 0);
  if (rem != n)
    BI_set_b(rem, n);

  bigint* multiplier = BI_new_i(1);
  bigint* subtractor = BI_new_b(d);

  while (BI_cmp_bb(rem, d) != BI_LESSTHAN)
  {
    if (BI_cmp_bb(subtractor, rem) == BI_GREATERTHAN)
    {
      BI_set_b(subtractor, d);
      BI_set_i(multiplier, 1);
      continue;
    }

    BI_sub_bb(rem, rem, subtractor);
    BI_add_bb(res, res, multiplier);

    BI_mul_bi(subtractor, subtractor, 2);
    BI_mul_bi(multiplier, multiplier, 2);
  }

  res->sign = sign;

  n->sign = nsign;
  d->sign = dsign;  

  BI_free(multiplier);
  BI_free(subtractor);

  BI_errno = BIERR_ZERO;
  return 0;
}


int BI_div_ii(bigint* res, int n, int d)
{
  if (res == NULL)
  {
    BI_errno = BIERR_NULLARG;
    return -1;
  }

  bigint* nbi = BI_new_i(n);
  bigint* dbi = BI_new_i(d);

  int result = BI_div_bb(res, nbi, dbi);

  BI_free(nbi);
  BI_free(dbi);

  return result;
}


int BI_div_bi(bigint* res, bigint* n, int d)
{
  if (res == NULL || n == NULL)
  {
    BI_errno = BIERR_NULLARG;
    return -1;
  }

  bigint* dbi = BI_new_i(d);

  int result = BI_div_bb(res, n, dbi);

  BI_free(dbi);

  return result;
}


int BI_div_ib(bigint* res, int n, bigint* d)
{
  if (res == NULL || d == NULL)
  {
    BI_errno = BIERR_NULLARG;
    return -1;
  }

  bigint* nbi = BI_new_i(n);

  int result = BI_div_bb(res, nbi, d);

  BI_free(nbi);

  return result;
}


int BI_div_bb(bigint* res, bigint* n, bigint* d)
{
  if (res == NULL || n == NULL || d == NULL)
  {
    BI_errno = BIERR_NULLARG;
    return -1;
  }

  bigint* tmp = BI_new_i(0);

  int result = BI_div_mod_bb(res, tmp, n, d);

  BI_free(tmp);

  return result;
}


int BI_mod_ii(bigint* res, int n, int m)
{
  if (res == NULL)
  {
    BI_errno = BIERR_NULLARG;
    return -1;
  }

  bigint* nbi = BI_new_i(n);
  bigint* mbi = BI_new_i(m);

  int result = BI_mod_bb(res, nbi, mbi);

  BI_free(nbi);
  BI_free(mbi);

  return result;
}


int BI_mod_bi(bigint* res, bigint* n, int m)
{
  if (res == NULL || n == NULL)
  {
    BI_errno = BIERR_NULLARG;
    return -1;
  }

  bigint* mbi = BI_new_i(m);

  int result = BI_mod_bb(res, n, mbi);

  BI_free(mbi);

  return result;
}


int BI_mod_ib(bigint* res, int n, bigint* m)
{
  if (res == NULL || m == NULL)
  {
    BI_errno = BIERR_NULLARG;
    return -1;
  }

  bigint* nbi = BI_new_i(n);

  int result = BI_mod_bb(res, nbi, m);

  BI_free(nbi);

  return result;
}


int BI_mod_bb(bigint* res, bigint* n, bigint* m)
{
  if (res == NULL || n == NULL || m == NULL)
  {
    BI_errno = BIERR_NULLARG;
    return -1;
  }

  bigint* tmp = BI_new_i(0);

  int result = BI_div_mod_bb(tmp, res, n, m);

  BI_free(tmp);

  return result;
}


enum BI_comparison BI_cmp_bi(bigint* a, int b)
{
  if (a == NULL)
  {
    BI_errno = BIERR_NULLARG;
    return -1;
  }

  bigint* bbi = BI_new_i(b);

  enum BI_comparison cmp = BI_cmp_bb(a, bbi);

  BI_free(bbi);

  return cmp;
}


enum BI_comparison BI_cmp_bb(bigint* a, bigint* b)
{
  if (a == NULL || b == NULL)
  {
    BI_errno = BIERR_NULLARG;
    return -1;
  }

  if (a->sign > b->sign)
  {
    return BI_GREATERTHAN;
  }

  if (b->sign > a->sign)
  {
    return BI_LESSTHAN;
  }

  if (a->len > b->len)
  {
    return BI_GREATERTHAN;
  }

  if (b->len > a->len)
  {
    return BI_LESSTHAN;
  }

  for (int i = a->len-1; i >= 0; i--)
  {
    if (a->val[i] == b->val[i])
    {
      continue;
    }

    return a->val[i] > b->val[i] ? BI_GREATERTHAN : BI_LESSTHAN;
  }

  return BI_EQUAL; 
}


enum BI_comparison BI_cmp_mag_bi(bigint* a, int b)
{
  if (a == NULL)
  {
    BI_errno = BIERR_NULLARG;
    return -1;
  }

  bigint* bbi = BI_new_i(b);

  enum BI_comparison cmp = BI_cmp_mag_bb(a, bbi);

  BI_free(bbi);

  return cmp;
}


enum BI_comparison BI_cmp_mag_bb(bigint* a, bigint* b)
{
  if (a == NULL || b == NULL)
  {
    BI_errno = BIERR_NULLARG;
    return -1;
  }

  int a_sign = a->sign;
  int b_sign = b->sign;

  a->sign = 1;
  b->sign = 1;

  enum BI_comparison result = BI_cmp_bb(a, b);

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

  if (base < 2 || base > 16)
  {
    BI_errno = BIERR_INVBASE;
    return NULL;
  }

  int len = 0;
  bigint* quo = BI_new_b(bi);
  do
  {
    BI_div_bi(quo, quo, base);
    len++;
    if (len > 1024)
    {
      BI_free(quo);
      BI_errno = BIERR_TOOBIG;
      return NULL;
    }
  } while (BI_cmp_bi(quo, 0) == BI_GREATERTHAN);
  BI_free(quo);

  bigint* num = BI_new_b(bi);
  bigint* rem = BI_new_i(0);
  bigint* den = BI_new_i(base);

  // add one for NULL terminator
  len++;

  // add one if signed
  if (bi->sign == -1)
  {
    len++;
  }

  char* s = calloc(1, len);
  int i;
  if (bi->sign == -1)
  {
    s[0] = '-';
    i = 1;
  }
  else i = 0;

  for (; i < len; i++)
  {
    BI_div_mod_bb(num, rem, num, den);
    int curr;
    BI_to_int(rem, &curr);
    if (curr < 9)
      s[i--] = curr + '0';
    else
      s[i--] = curr + 'A';
  }

  BI_free(num);
  BI_free(rem);
  BI_free(den);

  BI_errno = BIERR_ZERO;
  return s;
}


int BI_to_int(bigint* bi, int* i)
{
  if (bi == NULL || i == NULL)
  {
    BI_errno = BIERR_NULLARG;
    return -1;
  }

  if (BI_cmp_bi(bi, INT_MAX) == BI_GREATERTHAN)
  {
    BI_errno = BIERR_TOOBIG;
    return -1;
  }

  for (int j = 0; j < bi->len; j++)
  {
    *i += bi->val[j] * pow(256, j);
  }

  *i *= bi->sign;

  BI_errno = BIERR_ZERO;
  return 0;
}


int BI_print(bigint* bi)
{
  if (bi == NULL)
  {
    BI_errno = BIERR_NULLARG;
    return -1;
  }

  if (bi->sign == -1)
  {
    printf("-");
  }

  printf("[");
  for (int i = 0; i < bi->len; i++)
  {
    printf("%d", bi->val[i]);
    if (i < bi->len-1)
    {
      printf(", ");
    }
  }
  if (bi->len == 0)
  {
    printf("0");
  }
  printf("]\n");

  BI_errno = BIERR_ZERO;
  return 0;
}


void BI_perror(char* context)
{
  switch(BI_errno)
  {
    case BIERR_ZERO:
      if (context)
        fprintf(stderr, "%s: Success\n", context);
      else
        fprintf(stderr, "Success\n");
      break;
    case BIERR_NULLARG:
      if (context)
        fprintf(stderr, "%s: Null argument\n", context);
      else
        fprintf(stderr, "Null argument\n");
      break;
    case BIERR_INVBASE:
      if (context)
        fprintf(stderr, "%s: Invalid base\n", context);
      else
        fprintf(stderr, "Invalid base\n");
      break;
    case BIERR_BADENC:
      if (context)
        fprintf(stderr, "%s: Bad encoding\n", context);
      else
        fprintf(stderr, "Bad encoding\n");
      break;
    case BIERR_DIVZERO:
      if (context)
        fprintf(stderr, "%s: Divide by zero\n", context);
      else
        fprintf(stderr, "Divide by zero\n");
      break;
    case BIERR_TOOBIG:
      if (context)
        fprintf(stderr, "%s: Value is too big for operation\n", context);
      else
        fprintf(stderr, "Value is too big for operation\n");
    default:
      if (context)
        fprintf(stderr, "%s: Unknown error %d\n", context, BI_errno);
      else
        fprintf(stderr, "Unknown error %d\n", BI_errno);
  }
}

