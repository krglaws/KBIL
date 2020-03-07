
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <kbil.h>


#define ABS(n) (n < 0 ? (~n) + 1 : n)


#define MAX(a, b) (a > b ? a : b)


/* Only set BI_errno if it is not already set,
   or if it is being set to BI_EZERO */
#define SET_ERR(err)\
if (BI_errno != BI_EZERO || err == BI_EZERO)\
  BI_errno = err;


static enum BI_error BI_errno = 0;


bigint* BI_new_i(int num)
{
  bigint* bi = calloc(1, sizeof(bigint));
  
  long int abs = ABS(num);

  /* Calculate number of bytes needed to store num,
     and allocate */
  long int temp = abs;
  bi->len = 1;
  while (temp /= 256) bi->len++;
  bi->val = calloc(1, bi->len);
  bi->sign = num < 0 ? -1 : 1;

  /* Store num into bigint */
  for (int i = 0; i < bi->len; i++)
  {
    bi->val[i] = abs % 256;
    abs /= 256;
  }

  SET_ERR(BI_EZERO);
  return bi;
}


bigint* BI_new_b(bigint* num)
{
  if (num == NULL)
  {
    SET_ERR(BI_ENULLARG);
    return NULL;
  }

  // Create empty bigint
  bigint* bi = BI_new_i(0);

  // Set empty bigint to num's value
  BI_set_b(bi, num);

  SET_ERR(BI_EZERO);
  return bi;
}


bigint* BI_new_s(char* s, int base)
{
  if (s == NULL)
  {
    SET_ERR(BI_ENULLARG);
    return NULL;
  }

  if (base < 2 || base > 16)
  {
    SET_ERR(BI_EINVBASE);
    return NULL;
  }

  // Assume sign is positive
  int sign = 1;

  // Check if first char is '-'
  if (s[0] == '-')
  {
    sign = -1;
    s++;
  }

  // Create bigint result
  bigint* bi = BI_new_i(0);

  // Create bigint to store (s[i] * (base ^ i))
  bigint* prod = BI_new_i(0);

  // Iterate over string in reverse
  int slen = strlen(s);
  for (int i = slen - 1; i >= 0; i--)
  {
    // Check for invalid character, regardless of base
    if (s[i] < '0' || (s[i] > '9' &&
        s[i] < 'A') || s[i] > 'F')
    {
      BI_free(bi);
      BI_free(prod);
      SET_ERR(BI_EBADENC);
      return NULL;
    }

    // Check if character is out of bounds of the base
    if ((s[i] <= '9' && s[i] - '0' >= base) ||
        (s[i] <= 'F' && s[i] - 'A' >= base))
    {
      BI_free(bi);
      BI_free(prod);
      SET_ERR(BI_EBADENC);
      return NULL;
    }

    // Get current number in string 
    int curr = s[i] < '9' ? s[i] - '0' : (s[i] - 'A') + 10;

    // Compute curr * base ^ i, and add result to bi
    BI_pow_ii(prod, base, (slen - 1) - i);
    BI_mul_bi(prod, prod, curr);
    BI_add_bb(bi, bi, prod);
  }
  BI_free(prod);

  // Set sign
  bi->sign = sign;

  SET_ERR(BI_EZERO);
  return bi; 
}


bigint* BI_rand(unsigned int bits)
{
  // Number of bits at tail end  
  int rem = bits % 8;

  // Number of bytes
  int len = (bits / 8) + (rem ? 1 : 0);
  unsigned char val[len];

  // Store random value into each byte
  for (int i = 0; i < len; i++)
    val[i] = (unsigned char) rand();

  // Zero out unwanted bits
  if (rem)
    val[len-1] >>= (8 - rem);

  // Create new bigint, store random bytes into it
  bigint* bi = malloc(sizeof(bigint));

  bi->val = malloc(len);
  memcpy(bi->val, val, len);

  bi->len = len;

  // Random sign
  bi->sign = ((float) rand() / RAND_MAX) < 0.5 ? 1 : -1;

  return bi;
}


void BI_free(bigint* bi)
{
  if (bi == NULL)
    return;

  free(bi->val);
  free(bi);
}


int BI_set_i(bigint* bi, int num)
{
  if (bi == NULL)
  {
    SET_ERR(BI_ENULLARG);
    return -1;
  }

  /* Create temporary bigint from num,
     then call BI_set_b to make them equal */
  bigint* temp = BI_new_i(num);
  BI_set_b(bi, temp);
  BI_free(temp);

  SET_ERR(BI_EZERO);
  return 0;
}


int BI_set_b(bigint* bi, bigint* num)
{
  if (bi == NULL || num == NULL)
  {
    SET_ERR(BI_ENULLARG);
    return -1;
  }

  // Copy all values in 'num' over to 'bi'
  bi->sign = num->sign;
  bi->len = num->len;
  bi->val = realloc(bi->val, num->len);
  memcpy(bi->val, num->val, num->len);

  SET_ERR(BI_EZERO);
  return 0;
}


bigint* BI_add_ii(bigint* res, int a, int b)
{
  if (res == NULL)
  {
    SET_ERR(BI_ENULLARG);
    return NULL;
  }

  bigint* abi = BI_new_i(a);
  bigint* bbi = BI_new_i(b);

  bigint* result = BI_add_bb(res, abi, bbi);

  BI_free(abi);
  BI_free(bbi);

  SET_ERR(BI_EZERO);
  return 0;
}


bigint* BI_add_bi(bigint* res, bigint* a, int b)
{
  if (res == NULL || a == NULL)
  {
    SET_ERR(BI_ENULLARG);
    return NULL;
  }

  bigint* bbi = BI_new_i(b);

  bigint* result = BI_add_bb(res, a, bbi);

  BI_free(bbi);

  return result;
}


bigint* BI_add_bb(bigint* res, bigint* a, bigint* b)
{
  if (res == NULL || a == NULL || b == NULL)
  {
    SET_ERR(BI_ENULLARG);
    return NULL;
  }

  int sign = 1, len = 1;

  // Compare magnitude of 'a' with respect to 'b'
  enum BI_comparison mag = BI_cmp_mag_bb(a, b);

  // if both are negative, result is negative 
  if (a->sign == -1 && b->sign == -1)
    sign = -1;

  // if 'a' is negative and greater in magnitude, result is negative
  else if (a->sign == -1 && mag == BI_GREATERTHAN)
    sign = -1;

  // if 'b' is negative and greater in magnitude, result is negative
  else if (b->sign == -1 && mag == BI_LESSTHAN)
    sign = -1;

  // Save signs of 'a' and 'b'
  int asign = a->sign;
  int bsign = b->sign;

  /* If signs are not the same,
     make the larger magnitude positive,
     and the smaller magnitude negative */
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

  // create temporary result array
  int templen = 1 + MAX(a->len, b->len);
  unsigned char tempval[templen];

  int i = 0, j = 0, k = 0, carry = 0;

  while (i < a->len || j < b->len || carry)
  {
    int sum, curra, currb;

    // grab next values to add from 'a' and 'b'
    curra = (i < a->len ? a->val[i++] * a->sign : 0);
    currb = (j < b->len ? b->val[j++] * b->sign : 0);

    // compute sum
    sum = carry + curra + currb;
    carry = 0;

    /* If signs are not equal and sum is less than zero,
       wrap sum around and set carry to -1 */
    if (asign != bsign && sum < 0)
    {
      tempval[k] = 256 - (ABS(sum) % 256);
      carry = -1;
    }
    // otherwise, this is simple addition
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

    // keep track of result length
    if (tempval[k-1])
    {
      len = k;
    }
  }

  // restore signs
  a->sign = asign;
  b->sign = bsign;

  // copy result into 'res'
  res->len = len;
  res->sign = sign;
  free(res->val);
  res->val = calloc(1, len);
  memcpy(res->val, tempval, len);

  SET_ERR(BI_EZERO);
  return 0;
}


bigint* BI_inc(bigint* bi)
{
  return BI_add_bi(bi, bi, 1);
}


bigint* BI_dec(bigint* bi)
{
  return BI_sub_bi(bi, bi, 1);
}


bigint* BI_sub_ii(bigint* res, int a, int b)
{
  if (res == NULL)
  {
    SET_ERR(BI_ENULLARG);
    return NULL;
  }

  bigint* abi = BI_new_i(a);
  bigint* bbi = BI_new_i(b);

  bigint* result = BI_sub_bb(res, abi, bbi);

  BI_free(abi);
  BI_free(bbi);

  return result;
}


bigint* BI_sub_bi(bigint* res, bigint* a, int b)
{
  if (res == NULL || a == NULL)
  {
    SET_ERR(BI_ENULLARG);
    return NULL;
  }

  bigint* bbi = BI_new_i(b);

  bigint* result = BI_sub_bb(res, a, bbi);

  BI_free(bbi);

  return result;
}


bigint* BI_sub_ib(bigint* res, int a, bigint* b)
{
  if (res == NULL || b == NULL)
  {
    SET_ERR(BI_ENULLARG);
    return NULL;
  }

  bigint* abi = BI_new_i(a);

  bigint* result = BI_sub_bb(res, abi, b);

  BI_free(abi);

  return result;
}


bigint* BI_sub_bb(bigint* res, bigint* a, bigint* b)
{
  if (res == NULL || a == NULL || b == NULL)
  {
    SET_ERR(BI_ENULLARG);
    return NULL;
  }

  // if 'a' is the same reference as 'b', make a copy of 'b'
  if (a == b)
  {
    b = BI_new_b(a);
    b->sign *= -1;
    bigint* status = BI_add_bb(res, a, b);
    BI_free(b);
    return status;
  }

  // subtraction is just addition with b's sign flipped
  int bsign = b->sign;
  b->sign *= -1;
  bigint* status = BI_add_bb(res, a, b);

  if (b != res)
  {
    b->sign = bsign;
  }

  return status;
}


bigint* BI_mul_ii(bigint* res, int a, int b)
{
  if (res == NULL)
  {
    SET_ERR(BI_ENULLARG);
    return NULL;
  }

  bigint* abi = BI_new_i(a);
  bigint* bbi = BI_new_i(b);

  bigint* result = BI_mul_bb(res, abi, bbi);

  BI_free(abi);
  BI_free(bbi);

  return result;
}


bigint* BI_mul_bi(bigint* res, bigint* a, int b)
{
  if (res == NULL || a == NULL)
  {
    SET_ERR(BI_ENULLARG);
    return NULL;
  }

  bigint* bbi = BI_new_i(b);

  bigint* result = BI_mul_bb(res, a, bbi);

  BI_free(bbi);

  return result;
}


bigint* BI_mul_bb(bigint* res, bigint* a, bigint* b)
{
  if (res == NULL || a == NULL || b == NULL)
  {
    SET_ERR(BI_ENULLARG);
    return NULL;
  }

  // Calculate result sign
  int sign = a->sign * b->sign;

  // Save b sign and set to positive
  int bsign = b->sign;
  b->sign = 1;

  /* Allocate bigints to store product and
     temp multiplication result */
  bigint* product = BI_new_i(0);
  bigint* temp = BI_new_i(0);

  // Loop over each digit in 'a'
  for (int i = 0; i < a->len; i++)
  {
    BI_set_i(temp, 0);

    // Loop over digit and add 'b' to temp each time
    for (int j = 0; j < a->val[i]; j++)
    {
      BI_add_bb(temp, temp, b);
    }

    // Shift temp 'i' digits up in value and add to product
    unsigned char* fullval = calloc(1, temp->len + i);
    memcpy(fullval + i, temp->val, temp->len);
    free(temp->val);
    temp->val = fullval;
    temp->len += i;

    BI_add_bb(product, product, temp);
  }

  // Restore b's sign, store product into 'res', free up bigints
  BI_free(temp);
  b->sign = bsign;
  BI_set_b(res, product);
  res->sign = sign;
  BI_free(product);

  SET_ERR(BI_EZERO);
  return 0;
}


bigint* BI_pow_ii(bigint* res, int b, int e)
{
  if (res == NULL)
  {
    SET_ERR(BI_ENULLARG);
    return NULL;
  }

  bigint* bbi = BI_new_i(b);
  bigint* ebi = BI_new_i(e);

  bigint* result = BI_pow_bb(res, bbi, ebi);

  BI_free(bbi);
  BI_free(ebi);

  return result;
}


bigint* BI_pow_bi(bigint* res, bigint* b, int e)
{
  if (res == NULL || b == NULL)
  {
    SET_ERR(BI_ENULLARG);
    return NULL;
  }

  bigint* ebi = BI_new_i(e);

  bigint* result = BI_pow_bb(res, b, ebi);

  BI_free(ebi);

  return result;
}


bigint* BI_pow_ib(bigint* res, int b, bigint* e)
{
  if (res == NULL || e == NULL)
  {
    SET_ERR(BI_ENULLARG);
    return NULL;
  }

  bigint* bbi = BI_new_i(b);

  bigint* result = BI_pow_bb(res, bbi, e);

  BI_free(bbi);

  return result;
}


bigint* BI_pow_bb(bigint* res, bigint* b, bigint* e)
{
  if (res == NULL || b == NULL || e == NULL)
  {
    SET_ERR(BI_ENULLARG);
    return NULL;
  }

  // If exponent is negative, return zero
  if (e->sign == -1)
  {
    BI_set_i(res, 0);
    SET_ERR(BI_EZERO);
    return 0;
  }

  // Bigint to store product
  bigint* prod = BI_new_i(1);

  // Temporary bigint that holds b^currpow
  bigint* temp = BI_new_b(b);

  // Copy 'e' to gradually count its value down to zero
  bigint* powcount = BI_new_b(e);

  // Current power of temp
  bigint* currpow = BI_new_i(1);

  // While powcount > 0
  while (BI_cmp_bi(powcount, 0) != BI_EQUAL)
  {
    // If currpow > powcount, reset currpow to 1 and temp to b
    if (BI_cmp_bb(currpow, powcount) == BI_GREATERTHAN)
    {
      BI_set_i(currpow, 1);
      BI_set_b(temp, b);
    }

    /* Multiply product * temp, subtract current power from powcount,
       double currpow, and square temp */
    BI_mul_bb(prod, prod, temp);
    BI_sub_bb(powcount, powcount, currpow);
    BI_mul_bi(currpow, currpow, 2);
    BI_mul_bb(temp, temp, temp);
  }

  // Store product into result, free up bigints
  BI_set_b(res, prod);
  BI_free(prod);
  BI_free(temp);
  BI_free(powcount);
  BI_free(currpow);

  SET_ERR(BI_EZERO);
  return 0;
}


bigint* BI_div_mod_ii(bigint* res, bigint* rem, int n, int d)
{
  if (res == NULL || rem == NULL)
  {
    SET_ERR(BI_ENULLARG);
    return NULL;
  }

  bigint* nbi = BI_new_i(n);
  bigint* dbi = BI_new_i(d);

  bigint* result = BI_div_mod_bb(res, rem, nbi, dbi);

  BI_free(nbi);
  BI_free(dbi);

  return result;
}


bigint* BI_div_mod_bi(bigint* res, bigint* rem, bigint* n, int d)
{
  if (res == NULL || rem == NULL || n == NULL)
  {
    SET_ERR(BI_ENULLARG);
    return NULL;
  }

  bigint* dbi = BI_new_i(d);

  bigint* result = BI_div_mod_bb(res, rem, n, dbi);

  BI_free(dbi);

  return result;
}


bigint* BI_div_mod_ib(bigint* res, bigint* rem, int n, bigint* d)
{
  if (res == NULL || rem == NULL || d == NULL)
  {
    SET_ERR(BI_ENULLARG);
    return NULL;
  }

  bigint* nbi = BI_new_i(n);

  bigint* result = BI_div_mod_bb(res, rem, nbi, d);

  BI_free(nbi);

  return result;
}


bigint* BI_div_mod_bb(bigint* res, bigint* rem, bigint* n, bigint* d)
{
  if (res == NULL || rem == NULL || n == NULL || d == NULL)
  {
    SET_ERR(BI_ENULLARG);
    return NULL;
  }

  // Check for divide by zero
  if (BI_cmp_bi(d, 0) == BI_EQUAL)
  {
    SET_ERR(BI_EDIVZERO);
    return NULL;
  }

  // Calculate result sign
  int sign = n->sign * d->sign;

  // Bigint to store quotient
  bigint* quotient = BI_new_i(0);

  // Bigint to store remainder
  bigint* remainder = BI_new_b(n);
  remainder->sign = 1;

  // Copy d into denominator and make positive
  bigint* denominator = BI_new_b(d);
  denominator->sign = 1;

  bigint* multiplier = BI_new_i(1);
  bigint* subtractor = BI_new_b(denominator);

  while (BI_cmp_bb(remainder, denominator) != BI_LESSTHAN)
  {
    if (BI_cmp_bb(subtractor, remainder) == BI_GREATERTHAN)
    {
      BI_set_b(subtractor, denominator);
      BI_set_i(multiplier, 1);
      continue;
    }

    BI_sub_bb(remainder, remainder, subtractor);
    BI_add_bb(quotient, quotient, multiplier);

    BI_mul_bi(subtractor, subtractor, 2);
    BI_mul_bi(multiplier, multiplier, 2);
  }

  if (quotient->val[quotient->len - 1] == 0)
  {
    quotient->sign = 1;
  }
  else
  {
    quotient->sign = sign;
  }

  BI_set_b(res, quotient);
  BI_set_b(rem, remainder);

  BI_free(quotient);
  BI_free(remainder);
  BI_free(denominator);
  BI_free(multiplier);
  BI_free(subtractor);

  SET_ERR(BI_EZERO);
  return 0;
}


bigint* BI_div_ii(bigint* res, int n, int d)
{
  if (res == NULL)
  {
    SET_ERR(BI_ENULLARG);
    return NULL;
  }

  bigint* nbi = BI_new_i(n);
  bigint* dbi = BI_new_i(d);

  bigint* result = BI_div_bb(res, nbi, dbi);

  BI_free(nbi);
  BI_free(dbi);

  return result;
}


bigint* BI_div_bi(bigint* res, bigint* n, int d)
{
  if (res == NULL || n == NULL)
  {
    SET_ERR(BI_ENULLARG);
    return NULL;
  }

  bigint* dbi = BI_new_i(d);

  bigint* result = BI_div_bb(res, n, dbi);

  BI_free(dbi);

  return result;
}


bigint* BI_div_ib(bigint* res, int n, bigint* d)
{
  if (res == NULL || d == NULL)
  {
    SET_ERR(BI_ENULLARG);
    return NULL;
  }

  bigint* nbi = BI_new_i(n);

  bigint* result = BI_div_bb(res, nbi, d);

  BI_free(nbi);

  return result;
}


bigint* BI_div_bb(bigint* res, bigint* n, bigint* d)
{
  if (res == NULL || n == NULL || d == NULL)
  {
    SET_ERR(BI_ENULLARG);
    return NULL;
  }

  bigint* tmp = BI_new_i(0);

  bigint* result = BI_div_mod_bb(res, tmp, n, d);

  BI_free(tmp);

  return result;
}


bigint* BI_mod_ii(bigint* res, int n, int m)
{
  if (res == NULL)
  {
    SET_ERR(BI_ENULLARG);
    return NULL;
  }

  bigint* nbi = BI_new_i(n);
  bigint* mbi = BI_new_i(m);

  bigint* result = BI_mod_bb(res, nbi, mbi);

  BI_free(nbi);
  BI_free(mbi);

  return result;
}


bigint* BI_mod_bi(bigint* res, bigint* n, int m)
{
  if (res == NULL || n == NULL)
  {
    SET_ERR(BI_ENULLARG);
    return NULL;
  }

  bigint* mbi = BI_new_i(m);

  bigint* result = BI_mod_bb(res, n, mbi);

  BI_free(mbi);

  return result;
}


bigint* BI_mod_ib(bigint* res, int n, bigint* m)
{
  if (res == NULL || m == NULL)
  {
    SET_ERR(BI_ENULLARG);
    return NULL;
  }

  bigint* nbi = BI_new_i(n);

  bigint* result = BI_mod_bb(res, nbi, m);

  BI_free(nbi);

  return result;
}


bigint* BI_mod_bb(bigint* res, bigint* n, bigint* m)
{
  if (res == NULL || n == NULL || m == NULL)
  {
    SET_ERR(BI_ENULLARG);
    return NULL;
  }

  bigint* tmp = BI_new_i(0);

  bigint* result = BI_div_mod_bb(tmp, res, n, m);

  BI_free(tmp);

  return result;
}


enum BI_comparison BI_cmp_bi(bigint* a, int b)
{
  if (a == NULL)
  {
    SET_ERR(BI_ENULLARG);
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
    SET_ERR(BI_ENULLARG);
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

  if (a->sign == 1 && a->len > b->len)
  {
    return BI_GREATERTHAN;
  }

  if (b->sign == 1 && b->len > a->len)
  {
    return BI_LESSTHAN;
  }

  if (a->sign == -1 && a->len > b->len)
  {
    return BI_LESSTHAN;
  }

  if (b->sign == -1 && b->len > a->len)
  {
    return BI_GREATERTHAN;
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
    SET_ERR(BI_ENULLARG);
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
    SET_ERR(BI_ENULLARG);
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
    SET_ERR(BI_ENULLARG);
    return NULL;
  }

  if (base < 2 || base > 16)
  {
    SET_ERR(BI_EINVBASE);
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
      SET_ERR(BI_ETOOBIG);
      return NULL;
    }
  } while (BI_cmp_bi(quo, 0) != BI_EQUAL);
  BI_free(quo);

  bigint* num = BI_new_b(bi);
  bigint* rem = BI_new_i(0);
  bigint* den = BI_new_i(base);

  len++;

  if (bi->sign == -1)
    len++;

  char* s = calloc(1, len);
  int stop = 0;
  if (bi->sign == -1)
  {
    s[0] = '-';
    stop = 1;
  }

  for (int i = len - 2; i >= stop; i--)
  {
    BI_div_mod_bb(num, rem, num, den);

    int curr;
    BI_to_int(rem, &curr);

    if (curr < 10)
      s[i] = curr + '0';
    else
      s[i] = (curr - 10) + 'A';
  }

  BI_free(num);
  BI_free(rem);
  BI_free(den);

  SET_ERR(BI_EZERO);
  return s;
}


static int __pow(int base, int exp)
{
  if (exp < 0)
    return 0;

  int result = 1;
  while (exp--)
    result *= base;

  return result;
}


int BI_to_int(bigint* bi, int* i)
{
  if (bi == NULL || i == NULL)
  {
    SET_ERR(BI_ENULLARG);
    return -1;
  }

  if (BI_cmp_bi(bi, INT_MAX) == BI_GREATERTHAN ||
      BI_cmp_bi(bi, INT_MIN) == BI_LESSTHAN)
  {
    SET_ERR(BI_ETOOBIG);
    return -1;
  }

  *i = 0;

  for (int j = 0; j < bi->len; j++)
  {
    *i += bi->val[j] * __pow(256, j);
  }

  *i *= bi->sign;

  SET_ERR(BI_EZERO);
  return 0;
}


int BI_print(bigint* bi)
{
  if (bi == NULL)
  {
    SET_ERR(BI_ENULLARG);
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

  SET_ERR(BI_EZERO);
  return 0;
}


void BI_perror(char* context)
{
  switch(BI_errno)
  {
    case BI_EZERO:
      if (context)
        fprintf(stderr, "%s: Success\n", context);
      else
        fprintf(stderr, "Success\n");
      break;
    case BI_ENULLARG:
      if (context)
        fprintf(stderr, "%s: Null argument\n", context);
      else
        fprintf(stderr, "Null argument\n");
      break;
    case BI_EINVBASE:
      if (context)
        fprintf(stderr, "%s: Invalid base\n", context);
      else
        fprintf(stderr, "Invalid base\n");
      break;
    case BI_EBADENC:
      if (context)
        fprintf(stderr, "%s: Bad encoding\n", context);
      else
        fprintf(stderr, "Bad encoding\n");
      break;
    case BI_EDIVZERO:
      if (context)
        fprintf(stderr, "%s: Divide by zero\n", context);
      else
        fprintf(stderr, "Divide by zero\n");
      break;
    case BI_ETOOBIG:
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
