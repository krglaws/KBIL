
#ifndef _BIGINT_H_
#define _BIGINT_H_

#define BI_BASE 256

#define INIT_VAL_LEN 16

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
  int maxlen;
};

bigint* BI_new(int32_t val);

void BI_free(bigint* bi);

enum BI_error BI_add(bigint* a, bigint* b, bigint* c);

enum BI_error BI_sub(bigint* a, bigint* b, bigint* c);

enum BI_error BI_mul(bigint* a, bigint* b, bigint* c);

enum BI_error BI_div(bigint* a, bigint* n, bigint* d);

enum BI_comparison BI_cmp(bigint* a, bigint* b);

enum BI_comparison BI_cmp_mag(bigint* a, bigint* b);

char* BI_to_str(bigint* bi, int base);

void BI_print(bigint* bi);

void BI_perror(char* context);

#endif

