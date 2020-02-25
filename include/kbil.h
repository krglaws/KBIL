
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
  BIERR_ZERO,    // Success
  BIERR_NULLARG, // Null argument
  BIERR_INVBASE, // Invalid base
  BIERR_BADENC,  // Bad encoding
  BIERR_DIVZERO, // Divide by zero
  BIERR_TOOBIG   // Value is too big for operation
};

typedef struct bigint bigint;

struct bigint
{
  unsigned char* val;
  char sign;
  int len;
};

bigint* BI_new_i(int num);

bigint* BI_new_b(bigint* num);

bigint* BI_new_s(char* s, int base);

void BI_free(bigint* bi);

int BI_set_i(bigint* bi, int num);

int BI_set_b(bigint* bi, bigint* num);

int BI_rand(bigint* bi, unsigned int bits);

int BI_add_ii(bigint* res, int a, int b);

int BI_add_bi(bigint* res, bigint* a, int b);

int BI_add_bb(bigint* res, bigint* a, bigint* b);

int BI_inc(bigint* bi);

int BI_dec(bigint* bi);

int BI_sub_ii(bigint* res, int a, int b);

int BI_sub_bi(bigint* res, bigint* a, int b);

int BI_sub_ib(bigint* res, int a, bigint* b);

int BI_sub_bb(bigint* res, bigint* a, bigint* b);

int BI_mul_ii(bigint* res, int a, int b);

int BI_mul_bi(bigint* res, bigint* a, int i);

int BI_mul_bb(bigint* res, bigint* a, bigint* b);

int BI_pow_ii(bigint* res, int b, int e);

int BI_pow_bi(bigint* res, bigint* b, int e);

int BI_pow_ib(bigint* res, int b, bigint* e);

int BI_pow_bb(bigint* res, bigint* b, bigint* e);

int BI_div_mod_ii(bigint* res, bigint* rem, int n, int d);

int BI_div_mod_bi(bigint* res, bigint* rem, bigint* n, int d);

int BI_div_mod_ib(bigint* res, bigint* rem, int n, bigint* d);

int BI_div_mod_bb(bigint* res, bigint* rem, bigint* n, bigint* d);

int BI_div_ii(bigint* res, int n, int d);

int BI_div_bi(bigint* res, bigint* n, int d);

int BI_div_ib(bigint* res, int n, bigint* d);

int BI_div_bb(bigint* res, bigint* n, bigint* d);

int BI_mod_ii(bigint* res, int n, int m);

int BI_mod_bi(bigint* res, bigint* n, int m);

int BI_mod_ib(bigint* res, int n, bigint* m);

int BI_mod_bb(bigint* res, bigint* n, bigint* m);

enum BI_comparison BI_cmp_bi(bigint* a, int b);

enum BI_comparison BI_cmp_bb(bigint* a, bigint* b);

enum BI_comparison BI_cmp_mag_bi(bigint* a, int b);

enum BI_comparison BI_cmp_mag_bb(bigint* a, bigint* b);

char* BI_to_str(bigint* bi, int base);

int BI_to_int(bigint* bi, int* i);

int BI_print(bigint* bi);

void BI_perror(char* context);

#endif

