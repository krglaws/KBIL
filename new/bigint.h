
#ifndef _BIGINT_H_
#define _BIGINT_H_

#define BI_BASE 256

enum BI_comparison
{
  BI_EQUAL,
  BI_LESSTHAN,
  BI_GREATERTHAN
};

enum BI_error
{
  BIERR_ZERO,
  BIERR_NULLARG,
  BIERR_INVBASE,
  BIERR_NOTIMPL,
  BIERR_DIVZERO
};

typedef struct bigint bigint;

struct bigint
{
  unsigned char* val;
  char sign;
  int len;
};

bigint* BI_new(int32_t num);

void BI_free(bigint* bi);

enum BI_error BI_set_i(bigint* bi, int32_t num);

enum BI_error BI_set_bi(bigint* bi, bigint* num);

enum BI_error BI_add(bigint* res, bigint* a, bigint* b);

enum BI_error BI_sub(bigint* res, bigint* a, bigint* b);

enum BI_error BI_mul(bigint* res, bigint* a, bigint* b);

enum BI_error BI_pow(bigint* res, bigint* b, bigint* e);

enum BI_error BI_div_mod(bigint* res, bigint* rem, bigint* n, bigint* d);

enum BI_error BI_div(bigint* res, bigint* n, bigint* d);

enum BI_error BI_mod(bigint* res, bigint* n, bigint* m);

enum BI_comparison BI_cmp(bigint* a, bigint* b);

enum BI_comparison BI_cmp_mag(bigint* a, bigint* b);

char* BI_to_str(bigint* bi, int base);

void BI_print(bigint* bi);

void BI_perror(char* context);

#endif

