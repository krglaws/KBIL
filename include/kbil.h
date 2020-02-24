
#ifndef _BIGINT_H_
#define _BIGINT_H_

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

bigint* BI_new_i(int num);

bigint* BI_new_bi(bigint* num);

bigint* BI_new_s(char* s, int base);

void BI_free(bigint* bi);

int BI_set_i(bigint* bi, int num);

int BI_set_bi(bigint* bi, bigint* num);

int BI_rand(bigint* bi, unsigned int bytes);

int BI_add_ii(bigint* res, int a, int b);

int BI_add_bii(bigint* res, bigint* a, int b);

int BI_add_bibi(bigint* res, bigint* a, bigint* b);

int BI_inc(bigint* bi);

int BI_dec(bigint* bi);

int BI_sub_ii(bigint* res, int a, int b);

int BI_sub_bii(bigint* res, bigint* a, int b);

int BI_sub_bibi(bigint* res, bigint* a, bigint* b);

int BI_mul_ii(bigint* res, int a, int b);

int BI_mul_bii(bigint* res, bigint* a, int i);

int BI_mul_bibi(bigint* res, bigint* a, bigint* b);

int BI_pow_ii(bigint* res, int b, int e);

int BI_pow_bii(bigint* res, bigint* b, int e);

int BI_pow_bibi(bigint* res, bigint* b, bigint* e);

int BI_div_mod(bigint* res, bigint* rem, bigint* n, bigint* d);

int BI_div_ii(bigint* res, int n, int d);

int BI_div_bii(bigint* res, bigint* n, int d);

int BI_div_bibi(bigint* res, bigint* n, bigint* d);

int BI_mod_ii(bigint* res, int n, int m);

int BI_mod_bii(bigint* res, bigint* n, int m);

int BI_mod_bibi(bigint* res, bigint* n, bigint* m);

enum BI_comparison BI_cmp_bii(bigint* a, int b);

enum BI_comparison BI_cmp_bibi(bigint* a, bigint* b);

enum BI_comparison BI_cmp_mag_bii(bigint* a, int b);

enum BI_comparison BI_cmp_mag_bibi(bigint* a, bigint* b);

char* BI_to_str(bigint* bi, int base);

int BI_print(bigint* bi);

void BI_perror(char* context);

#endif

