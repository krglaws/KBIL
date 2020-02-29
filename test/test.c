
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <kbil.h>


int BI_new_i_test()
{
  int result = 0;

  /* TEST 1 */
  bigint* bi = BI_new_i(256*256);

  if (bi->len != 3)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->len value: %d. Expected: 3\n", bi->len);
    result = -1;
  }

  if (bi->val[0] != 0)
  {
    fprintf(stderr, "TEST 1: Unexpected value at bigint->val[0]: %d. Expected: 0\n", bi->val[0]);
    result = -1;
  }

  if (bi->val[1] != 0)
  {
    fprintf(stderr, "TEST 1: Unexpected value at bigint->val[1]: %d. Expected: 0\n", bi->val[1]);
    result = -1;
  }

  if (bi->val[2] != 1)
  {
    fprintf(stderr, "TEST 1: Unexpected value at bigint->val[0]: %d. Expected: 1\n", bi->val[2]);
    result = -1;
  }
  BI_free(bi);

  return result;
}

int BI_new_b_test()
{
  int result = 0;

  /* TEST 1 */
  bigint* a = BI_new_i(256);
  bigint* b = BI_new_b(a);

  enum BI_comparison cmp = BI_cmp_bb(a, b);
  if (cmp != BI_EQUAL)
  {
    fprintf(stderr, "TEST 1: Unexpected comparison result: %d. Expected: %d\n", cmp, BI_EQUAL);
    result = -1;
  }
  BI_free(a);
  BI_free(b);

  return result;
}

int BI_new_s_test()
{
  int result = 0;

  /* TEST 1 */
  char* s = "10A0A";
  bigint* bi = BI_new_s(s, 16);

  if (bi->len != 3)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->len value: %d. Expected: 3\n", bi->len);
    result = -1;
  }

  if (bi->val[0] != 10)
  {
    fprintf(stderr, "TEST 1: Unexpected value at bigint->val[0]: %d. Expected: 10\n", bi->val[0]);
    result = -1;
  }

  if (bi->val[1] != 10)
  {
    fprintf(stderr, "TEST 1: Unexpected value at bigint->val[1]: %d. Expected: 10\n", bi->val[1]);
    result = -1;
  }

  if (bi->val[2] != 1)
  {
    fprintf(stderr, "TEST 1: Unexpected value at bigint->val[0]: %d. Expected: 1\n", bi->val[2]);
    result = -1;
}
  BI_free(bi);

  /* TEST 2 */
  s = "-1";
  bi = BI_new_s(s, 10);

  if (bi->len != 1)
  {
    fprintf(stderr, "TEST 2: Unexpected bigint->len value: %d. Expected: 1\n", bi->len);
    result = -1;
  }

  if (bi->val[0] != 1)
  {
    fprintf(stderr, "TEST 2: Unexpected bigint->val[0] value: %d. Expected: 1\n", bi->val[0]);
    result = -1;
  }

  if (bi->sign != -1)
  {
    fprintf(stderr, "TEST 2: Unexpected bigint->sign value: %d. Expected: -1\n", bi->sign);
    result = -1;
  }
  BI_free(bi);

  return result;
}

int BI_set_i_test()
{
  int result = 0;

  /* TEST 1 */
  bigint* bi = BI_new_i(0);
  BI_set_i(bi, 256);

  if (bi->len != 2)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->len value: %d. Expected: 2\n", bi->len);
    result = -1;
  }

  if (bi->val[0] != 0)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->val[0] value: %d. Expected: 0\n", bi->val[0]);
    result = -1;
  }

  if (bi->val[1] != 1)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->val[1] value: %d. Expected: 1\n", bi->val[1]);
    result = -1;
  }
  BI_free(bi);

  return result;
}

int BI_set_b_test()
{
  int result = 0;

  /* TEST 1 */
  bigint* b = BI_new_i(0);
  bigint* a = BI_new_i(256);
  BI_set_b(b, a);

  if (b->len != 2)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->len value: %d. Expected: 2\n", b->len);
    result = -1;
  }

  if (b->val[0] != 0)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->val[0] value: %d. Expected: 0\n", b->val[0]);
    result = -1;
  }

  if (b->val[1] != 1)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->val[1] value: %d. Expected: 1\n", b->val[1]);
    result = -1;
  }
  BI_free(a);
  BI_free(b);

  return result;
}

int BI_rand_test()
{
  int result = 0;

  /* TEST 1 */
  bigint* b = BI_new_i(0);
  BI_rand(b, 9);

  if (b->len != 2)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->len value: %d. Expected: 2\n", b->len);
    result = -1;
  }

  if (b->val[1] != 1)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->val[1] value: %d. Expected: 1\n", b->val[1]);
    result = -1;
  }
  BI_free(b);

  return result;
}

int BI_add_ii_test()
{
  int result = 0;

  /* TEST 1 */
  bigint* bi = BI_new_i(0);
  BI_add_ii(bi, 128, 128);

  if (bi->len != 2)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->len value: %d. Expected: 2\n", bi->len);
    result = -1;
  }

  if (bi->val[0] != 0)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->val[0] value: %d. Expected: 0\n", bi->val[0]);
    result = -1;
  }

  if (bi->val[1] != 1)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->val[1] value: %d. Expected: 1\n", bi->val[1]);
    result = -1;
  }
  BI_free(bi);

  return result;
}

int BI_add_bi_test()
{
  int result = 0;

  /* TEST 1 */
  bigint* bi = BI_new_i(0);
  bigint* abi = BI_new_i(128);
  BI_add_bi(bi, abi, 128);

  if (bi->len != 2)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->len value: %d. Expected: 2\n", bi->len);
    result = -1;
  }

  if (bi->val[0] != 0)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->val[0] value: %d. Expected: 0\n", bi->val[0]);
    result = -1;
  }

  if (bi->val[1] != 1)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->val[1] value: %d. Expected: 1\n", bi->val[1]);
    result = -1;
  }
  BI_free(bi);
  BI_free(abi);

  return result;
}

int BI_add_bb_test()
{
  int result = 0;

  /* TEST 1 */
  bigint* bi = BI_new_i(0);
  bigint* abi = BI_new_i(128);
  bigint* cbi = BI_new_i(128);
  BI_add_bb(bi, abi, cbi);

  if (bi->len != 2)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->len value: %d. Expected: 2\n", bi->len);
    result = -1;
  }

  if (bi->val[0] != 0)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->val[0] value: %d. Expected: 0\n", bi->val[0]);
    result = -1;
  }

  if (bi->val[1] != 1)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->val[1] value: %d. Expected: 1\n", bi->val[1]);
    result = -1;
  }
  BI_free(bi);
  BI_free(abi);
  BI_free(cbi);

  return result;
}

int BI_inc_test()
{
  int result = 0;

  /* TEST 1 */
  bigint* b = BI_new_i(255);
  BI_inc(b);
  if (b->len != 2)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->len value: %d. Expected: 2\n", b->len);
    result = -1;
  }

  if (b->val[0] != 0)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->val[0] value: %d. Expected: 0\n", b->val[0]);
    result = -1;
  }

  if (b->val[1] != 1)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->val[1] value: %d. Expected: 1\n", b->val[1]);
    result = -1;
  }
  BI_free(b);

  return result;
}

int BI_dec_test()
{
  int result = 0;

  /* TEST 1 */
  bigint* b = BI_new_i(256);
  BI_dec(b);

  if (b->len != 1)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->len value: %d. Expected: 1\n", b->len);
    result = -1;
  }

  if (b->val[0] != 255)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->val[0] value: %d. Expected: 255\n", b->val[0]);
    result = -1;
  }
  BI_free(b);

  return result;
}

int BI_sub_ii_test()
{
  int result = 0;

  /* TEST 1 */
  bigint* b = BI_new_i(0);
  BI_sub_ii(b, 256, -256);

  if (b->len != 2)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->len value: %d. Expected: 2\n", b->len);
    result = -1;
  }

  if (b->val[0] != 0)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->val[0] value: %d. Expected: 0\n", b->val[0]);
    result = -1;
  }

  if (b->val[1] != 2)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->val[1] value: %d. Expected: 2\n", b->val[1]);
    result = -1;
  }
  BI_free(b);

  return result;
}

int BI_sub_bi_test()
{
  int result = 0;

  /* TEST 1 */
  bigint* bi = BI_new_i(0);
  bigint* a = BI_new_i(256);
  BI_sub_bi(bi, a, 512);

  if (bi->len != 2)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->len value: %d. Expected: 2\n", bi->len);
    result = -1;
  }

  if (bi->sign != -1)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->sign value: %d. Expected: -1\n", bi->sign);
    result = -1;
  }

  if (bi->val[0] != 0)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->val[0] value: %d. Expected: 0\n", bi->val[0]);
    result = -1;
  }

  if (bi->val[1] != 1)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->val[1] value: %d. Expected: 1\n", bi->val[1]);
    result = -1;
  }
  BI_free(bi);
  BI_free(a);
 
  return result;
}

int BI_sub_ib_test()
{
  int result = 0;

  /* TEST 1 */
  bigint* bi = BI_new_i(-256);
  BI_sub_ib(bi, 0, bi);

  if (bi->len != 2)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->len value: %d. Expected: 2\n", bi->len);
    result = -1;
  }

  if (bi->sign != 1)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->sign value: %d. Expected: 1\n", bi->sign);
    result = -1;
  }

  if (bi->val[0] != 0)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->val[0] value: %d. Expected: 0\n", bi->val[0]);
    result = -1;
  }

  if (bi->val[1] != 1)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->val[1] value: %d. Expected: 1\n", bi->val[1]);
    result = -1;
  }
  BI_free(bi);

  return result;
}

int BI_sub_bb_test()
{
  int result = 0;

  /* TEST 1 */
  bigint* bi = BI_new_i(-256);
  BI_sub_bb(bi, bi, bi);

  if (bi->len != 1)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->len value: %d. Expected: 1\n", bi->len);
    result = -1;
  }

  if (bi->sign != 1)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->sign value: %d. Expected: 1\n", bi->sign);
    result = -1;
  }

  if (bi->val[0] != 0)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->val[0] value: %d. Expected: 0\n", bi->val[0]);
    result = -1;
  }
  BI_free(bi);

  return result;
}

int BI_mul_ii_test()
{
  int result = 0;

  /* TEST 1 */
  bigint* bi = BI_new_i(0);
  BI_mul_ii(bi, 256, 256);

  if (bi->len != 3)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->len value: %d. Expected: 3\n", bi->len);
    result = -1;
  }

  if (bi->val[0] != 0)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->val[0] value: %d. Expected: 0\n", bi->val[0]);
    result = -1;
  }

  if (bi->val[1] != 0)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->val[1] value: %d. Expected: 0\n", bi->val[1]);
    result = -1;
  }

  if (bi->val[2] != 1)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->val[2] value: %d. Expected: 1\n", bi->val[2]);
    result = -1;
  }
  BI_free(bi);

  return result;
}

int BI_mul_bi_test()
{
  int result = 0;

  /* TEST 1 */
  bigint* bi = BI_new_i(-256);
  BI_mul_bi(bi, bi, 256);

  if (bi->len != 3)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->len value: %d. Expected: 3\n", bi->len);
    result = -1;
  }

  if (bi->sign != -1)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->sign value: %d. Expected: -1\n", bi->sign);
    result = -1;
  }

  if (bi->val[0] != 0)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->val[0] value: %d. Expected: 0\n", bi->val[0]);
    result = -1;
  }

  if (bi->val[1] != 0)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->val[1] value: %d. Expected: 0\n", bi->val[1]);
    result = -1;
  }

  if (bi->val[2] != 1)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->val[2] value: %d. Expected: 1\n", bi->val[2]);
    result = -1;
  }
  BI_free(bi);

  return result;
}

int BI_mul_bb_test()
{
  int result = 0;

  /* TEST 1 */
  bigint* bi = BI_new_i(-256);
  BI_mul_bb(bi, bi, bi);

  if (bi->len != 3)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->len value: %d. Expected: 3\n", bi->len);
    result = -1;
  }

  if (bi->sign != 1)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->sign value: %d. Expected: 1\n", bi->sign);
    result = -1;
  }

  if (bi->val[0] != 0)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->val[0] value: %d. Expected: 0\n", bi->val[0]);
    result = -1;
  }

  if (bi->val[1] != 0)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->val[1] value: %d. Expected: 0\n", bi->val[1]);
    result = -1;
  }

  if (bi->val[2] != 1)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->val[2] value: %d. Expected: 1\n", bi->val[2]);
    result = -1;
  }
  BI_free(bi);

  return result;
}

int BI_pow_ii_test()
{
  int result = 0;

  /* TEST */
  bigint* bi = BI_new_i(0);
  BI_pow_ii(bi, -2, 8);

  if (bi->len != 2)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->len value: %d. Expected: 2\n", bi->len);
    result = -1;
  }

  if (bi->sign != 1)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->sign value: %d. Expected: 1\n", bi->sign);
    result = -1;
  }

  if (bi->val[0] != 0)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->val[0] value: %d. Expected: 0\n", bi->val[0]);
    result = -1;
  }

  if (bi->val[1] != 1)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->val[1] value: %d. Expected: 1\n", bi->val[1]);
    result = -1;
  }
  BI_free(bi);

  return result;
}

int BI_pow_bi_test()
{
  int result = 0;

  /* TEST */
  bigint* bi = BI_new_i(-2);
  BI_pow_bi(bi, bi, 8);

  if (bi->len != 2)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->len value: %d. Expected: 2\n", bi->len);
    result = -1;
  }

  if (bi->sign != 1)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->sign value: %d. Expected: 1\n", bi->sign);
    result = -1;
  }

  if (bi->val[0] != 0)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->val[0] value: %d. Expected: 0\n", bi->val[0]);
    result = -1;
  }

  if (bi->val[1] != 1)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->val[1] value: %d. Expected: 1\n", bi->val[1]);
    result = -1;
  }
  BI_free(bi);

  return result;
}

int BI_pow_ib_test()
{
  int result = 0;

  /* TEST 1 */
  bigint* bi = BI_new_i(-8);
  BI_pow_ib(bi, 2, bi);

  if (bi->len != 1)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->len value: %d. Expected: 1\n", bi->len);
    result = -1;
  }

  if (bi->val[0] != 0)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->val[0]: %d. Expected 0\n", bi->val[0]);
    result = -1;
  }
  BI_free(bi);

  return result;
}

int BI_pow_bb_test()
{
  int result = 0;

  /* TEST 1 */
  bigint* bi = BI_new_i(2);
  BI_pow_bb(bi, bi, bi);

  if (bi->len != 1)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->len value: %d. Expected: 1\n", bi->len);
    result = -1;
  }

  if (bi->val[0] != 4)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->val[0] value: %d. Expected: 4\n", bi->val[0]);
    result = -1;
  }
  BI_free(bi);

  return result;
}

int BI_div_mod_ii_test()
{
  int result = 0;

  /* TEST 1 */
  bigint* quo = BI_new_i(0);
  bigint* rem = BI_new_i(0);
  BI_div_mod_ii(quo, rem, 512, 256);

  if (quo->len != 1)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->len value: %d. Expected: 1\n", quo->len);
    result = -1;
  }

  if (quo->val[0] != 2)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->val[0] value: %d. Expected: 2\n", quo->val[0]);
    result = -1;
  }

  if (rem->len != 1)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->len value: %d. Expected: 0\n", rem->len);
    result = -1;
  }

  if (rem->val[0] != 0)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->val[0] value: %d. Expected 0\n", rem->val[0]);
    result = -1;
  }
  BI_free(quo);
  BI_free(rem);

  return result;
}

int BI_div_mod_bi_test()
{
  int result = 0;

  /* TEST 1 */
  bigint* quo = BI_new_i(-512);
  bigint* rem = BI_new_i(0);
  BI_div_mod_bi(quo, rem, quo, 256);

  if (quo->len != 1)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->len value: %d. Expected: 1\n", quo->len);
    result = -1;
  }

  if (quo->sign != -1)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->sign value: %d. Expected: -1\n", quo->sign);
    result = -1;
  }

  if (quo->val[0] != 2)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->val[0] value: %d. Expected: 2\n", quo->val[0]);
    result = -1;
  }

  if (rem->len != 1)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->len value: %d. Expected: 1\n", rem->len);
    result = -1;
  }

  if (rem->val[0] != 0)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->val[0] value: %d. Expected 0\n", rem->val[0]);
    result = -1;
  }
  BI_free(quo);
  BI_free(rem);

  return result;
}

int BI_div_mod_ib_test()
{
  int result = 0;

  /* TEST 1 */
  bigint* quo = BI_new_i(256);
  bigint* rem = BI_new_i(0);
  BI_div_mod_ib(quo, rem, 512, quo);

  if (quo->len != 1)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->len value: %d. Expected: 1\n", quo->len);
    result = -1;
  }

  if (quo->val[0] != 2)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->val[0] value: %d. Expected: 2\n", quo->val[0]);
    result = -1;
  }

  if (rem->len != 1)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->len value: %d. Expected: 1\n", rem->len);
    result = -1;
  }

  if (rem->val[0] != 0)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->val[0] value: %d. Expected: 0\n", rem->val[0]);
    result = -1;
  }
  BI_free(quo);
  BI_free(rem);

  return result;
}

int BI_div_mod_bb_test()
{
  int result = 0;

  /* TEST 1 */
  bigint* quo = BI_new_i(256*256);
  bigint* rem = BI_new_i(0);
  BI_div_mod_bb(quo, rem, quo, quo);

  if (quo->len != 1)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->len value: %d. Expected: 1\n", quo->len);
    result = -1;
  }

  if (quo->val[0] != 1)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->val[0] value: %d. Expected: 1\n", quo->val[0]);
    result = -1;
  }

  if (rem->len != 1)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->len value: %d. Expected: 1\n", rem->len);
    result = -1;
  }

  if (rem->val[0] != 0)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->val[0] value: %d. Expected: 0\n", rem->val[0]);
    result = -1;
  }
  BI_free(quo);
  BI_free(rem);

  return result;
}

int BI_div_ii_test()
{
  int result = 0;

  /* TEST 1 */
  bigint* quo = BI_new_i(0);
  BI_div_ii(quo, 256*256, 256);

  if (quo->len != 2)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->len value: %d. Expected: 2\n", quo->len);
    result = -1;
  }

  if (quo->val[0] != 0)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->val[0] value: %d. Expected: 0\n", quo->val[0]);
    result = -1;
  }

  if (quo->val[1] != 1)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->val[1] value: %d. Expected: 1\n", quo->val[1]);
    result = -1;
  }
  BI_free(quo);

  return result;
}

int BI_div_bi_test()
{
  int result = 0;

  /* TEST 1 */
  bigint* quo = BI_new_i(-256*256);
  BI_div_bi(quo, quo, 256);

  if (quo->len != 2)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->len value: %d. Expected: 2\n", quo->len);
    result = -1;
  }

  if (quo->val[0] != 0)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->val[0] value: %d. Expected: 0\n", quo->val[0]);
    result = -1;
  }

  if (quo->val[1] != 1)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->val[1] value: %d. Expected: 1\n", quo->val[1]);
    result = -1;
  }
  BI_free(quo);

  return result;
}

int BI_div_ib_test()
{
  int result = 0;

  /* TEST 1 */
  bigint* quo = BI_new_i(256);
  BI_div_ib(quo, 256, quo);

  if (quo->len != 1)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->len value: %d. Expected: 1\n", quo->len);
    result = -1;
  }

  if (quo->val[0] != 1)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->val[0] value: %d. Expected: 1\n", quo->val[0]);
    result = -1;
  }
  BI_free(quo);

  return result;
}

int BI_div_bb_test()
{
  int result = 0;

  /* TEST 1 */
  bigint* quo = BI_new_i(256);
  BI_div_bb(quo, quo, quo);

  if (quo->len != 1)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->len value: %d. Expected: 1\n", quo->len);
    result = -1;
  }

  if (quo->val[0] != 1)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->val[0] value: %d. Expected: 1\n", quo->val[0]);
    result = -1;
  }
  BI_free(quo);

  return result;
}

int BI_mod_ii_test()
{
  int result = 0;

  /* TEST 1 */
  bigint* rem = BI_new_i(0);
  BI_mod_ii(rem, 7, 2);

  if (rem->len != 1)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->len value: %d. Expected: 1\n", rem->len);
    result = -1;
  }

  if (rem->val[0] != 1)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->val[0] value: %d. Expected: 1\n", rem->val[0]);
    result = -1;
  }
  BI_free(rem);

  return result;
}

int BI_mod_bi_test()
{
  int result = 0;

  /* TEST 1 */
  bigint* rem = BI_new_i(256);
  BI_mod_bi(rem, rem, 255);

  if (rem->len != 1)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->val[0] value: %d. Expected: 1\n", rem->len);
    result = -1;
  }

  if (rem->val[0] != 1)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->val[0] value: %d. Expected: 1\n", rem->val[0]);
    result = -1;
  }
  BI_free(rem);

  return result;
}

int BI_mod_ib_test()
{
  int result = 0;

  bigint* rem = BI_new_i(256);
  BI_mod_ib(rem, 255, rem);

  if (rem->len != 1)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->val[0] value: %d. Expected: 1\n", rem->len);
    result = -1;
  }

  if (rem->val[0] != 255)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->val[0] value: %d. Expected: 255\n", rem->val[0]);
    result = -1;
  }
  BI_free(rem);

  return result;
}

int BI_mod_bb_test()
{
  int result = 0;

  /* TEST 1 */
  bigint* rem = BI_new_i(1024);
  BI_mod_bb(rem, rem, rem);

  if (rem->len != 1)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->len value: %d. Expected: 1\n", rem->len);
    result = -1;
  }

  if (rem->val[0] != 0)
  {
    fprintf(stderr, "TEST 1: Unexpected bigint->val[0] value: %d. Expected: 0\n", rem->val[0]);
    result = -1;
  }
  BI_free(rem);

  return result;
}

int BI_cmp_bi_test()
{
  int result = 0;

  /* TEST 1 */
  bigint* bi = BI_new_i(1234);

  enum BI_comparison cmp = BI_cmp_bi(bi, 1234);
  if (cmp != BI_EQUAL)
  {
    fprintf(stderr, "TEST 1: Unexpected comparison result: %d. Expected: %d\n", cmp, BI_EQUAL);
    result = -1;
  }

  /* TEST 2 */
  cmp = BI_cmp_bi(bi, 1233);
  if (cmp != BI_GREATERTHAN)
  {
    fprintf(stderr, "TEST 2: Unexpected comparison result: %d. Expected: %d\n", cmp, BI_GREATERTHAN);
    result = -1;
  }

  /* TEST 3 */
  cmp = BI_cmp_bi(bi, 1235);
  if (cmp != BI_LESSTHAN)
  {
    fprintf(stderr, "TEST 3: Unexpected comparison result: %d. Expected: %d\n", cmp, BI_LESSTHAN);
    result = -1;
  }
  BI_free(bi);

  return result;
}

int BI_cmp_bb_test()
{
  int result = 0;

  /* TEST 1 */
  bigint* bi = BI_new_i(1234);
  bigint* a = BI_new_i(1234);

  enum BI_comparison cmp = BI_cmp_bb(bi, a);
  if (cmp != BI_EQUAL)
  {
    fprintf(stderr, "TEST 1: Unexpected comparison result: %d. Expected: %d\n", cmp, BI_EQUAL);
    result = -1;
  }

  /* TEST 2 */
  BI_set_i(a, 1233);
  cmp = BI_cmp_bb(bi, a);
  if (cmp != BI_GREATERTHAN)
  {
    fprintf(stderr, "TEST 2: Unexpected comparison result: %d. Expected: %d\n", cmp, BI_GREATERTHAN);
    result = -1;
  }

  /* TEST 3 */
  BI_set_i(a, 1235);
  cmp = BI_cmp_bb(bi, a);
  if (cmp != BI_LESSTHAN)
  {
    fprintf(stderr, "TEST 3: Unexpected comparison result: %d. Expected: %d\n", cmp, BI_LESSTHAN);
    result = -1;
  }
  BI_free(bi);
  BI_free(a);

  return result;
}

int BI_cmp_mag_bi_test()
{
  int result = 0;

  bigint* bi = BI_new_i(1234);

  /* TEST 1 */
  enum BI_comparison cmp = BI_cmp_mag_bi(bi, -1234);
  if (cmp != BI_EQUAL)
  {
    fprintf(stderr, "TEST 1: Unexpected comparison result: %d. Expected: %d\n", cmp, BI_EQUAL);
    result = -1;
  }

  /* TEST 2 */
  cmp = BI_cmp_mag_bi(bi, -1233);
  if (cmp != BI_GREATERTHAN)
  {
    fprintf(stderr, "TEST 2: Unexpected comparison result: %d. Expected: %d\n", cmp, BI_GREATERTHAN);
    result = -1;
  }

  /* TEST 3 */
  cmp = BI_cmp_mag_bi(bi, -1235);
  if (cmp != BI_LESSTHAN)
  {
    fprintf(stderr, "TEST 3: Unexpected comparison result: %d. Expected: %d\n", cmp, BI_LESSTHAN);
    result = -1;
  }
  BI_free(bi);

  return result;
}

int BI_cmp_mag_bb_test()
{
  int result = 0;
  bigint* bi = BI_new_i(1234);
  bigint* a = BI_new_i(-1234);

  /* TEST 1 */
  enum BI_comparison cmp = BI_cmp_mag_bb(bi, a);
  if (cmp != BI_EQUAL)
  {
    fprintf(stderr, "TEST 1: Unexpected comparison result: %d. Expected: %d\n", cmp, BI_EQUAL);
    result = -1;
  }

  /* TEST 2 */
  BI_set_i(a, -1233);
  cmp = BI_cmp_mag_bb(bi, a);
  if (cmp != BI_GREATERTHAN)
  {
    fprintf(stderr, "TEST 2: Unexpected comparison result: %d. Expected: %d\n", cmp, BI_GREATERTHAN);
    result = -1;
  }

  /* TEST 3 */
  BI_set_i(a, -1235);
  cmp = BI_cmp_mag_bb(bi, a);
  if (cmp != BI_LESSTHAN)
  {
    fprintf(stderr, "TEST 3: Unexpected comparison result: %d. Expected: %d\n", cmp, BI_LESSTHAN);
    result = -1;
  }
  BI_free(bi);
  BI_free(a);

  return result;
}

int BI_to_str_test()
{
  int result = 0;

  /* TEST 1 */
  bigint* bi = BI_new_i(1234);
  char* s = BI_to_str(bi, 10);
  if (strcmp(s, "1234") != 0)
  {
    fprintf(stderr, "TEST 1: Unexpected string value: %s. Expected: 1234\n", s);
    result = -1;
  }
  free(s);

  /* TEST 2 */
  BI_set_i(bi, -1234);
  s = BI_to_str(bi, 10); 
  if (strcmp(s, "-1234") != 0)
  {
    fprintf(stderr, "TEST 2: Unexpected string value: %s. Expected: -1234\n", s);
    result = -1;
  }
  free(s);

  /* TEST 3 */
  BI_set_i(bi, 255);
  s = BI_to_str(bi, 16);
  if (strcmp(s, "FF") != 0)
  {
    fprintf(stderr, "TEST 3: Unexpected string value %s. Expected: FF\n", s);
    result = -1;
  }
  free(s);

  return result;
}

int BI_to_int_test()
{
  int result = 0;

  /* TEST 1 */
  bigint* bi = BI_new_i(0);
  int i = -1;
  BI_to_int(bi, &i);

  if (i != 0)
  {
    fprintf(stderr, "TEST 1: Unexpected int value: %d. Expected: 0\n", i);
    result = -1;
  }

  /* TEST 2 */
  BI_set_i(bi, 1234);
  BI_to_int(bi, &i);
  if (i != 1234)
  {
    fprintf(stderr, "TEST 2: Unexpected int value: %d. Expected: 1234\n", i);
    result = -1;
  }

  /* TEST 3 */
  BI_set_i(bi, -1);
  BI_to_int(bi, &i);
  if (i != -1)
  {
    fprintf(stderr, "TEST 3: Unexpected int value: %d. Expected -1\n", i);
    result = -1;
  }

  return result;
}

int main()
{
  int result = 0;

  printf("Running BI_new_i_test()...\n");
  if (BI_new_i_test())
    result = -1;
  printf("Done.\n");

  printf("Running BI_new_b_test()...\n");
  if (BI_new_b_test())
    result = -1;
  printf("Done.\n");

  printf("Running BI_new_s_test()...\n");
  if (BI_new_s_test())
    result = -1;
  printf("Done.\n");

  printf("Running BI_set_i_test()...\n");
  if (BI_set_i_test())
    result = -1;
  printf("Done.\n");

  printf("Running BI_set_b_test()...\n");
  if (BI_set_b_test())
    result = -1;
  printf("Done.\n");

  printf("Running BI_rand_test()...\n");
  if (BI_rand_test())
    result = -1;
  printf("Done.\n");

  printf("Running BI_add_ii_test()...\n");
  if (BI_add_ii_test())
    result = -1;
  printf("Done.\n");

  printf("Running BI_add_bi_test()...\n");
  if (BI_add_bi_test())
    result = -1;
  printf("Done.\n");

  printf("Running BI_add_bb_test()...\n");
  if (BI_add_bb_test())
    result = -1;
  printf("Done.\n");

  printf("Running BI_inc_test()...\n");
  if (BI_inc_test())
    result = -1;
  printf("Done.\n");

  printf("Running BI_dec_test()...\n");
  if (BI_dec_test())
    result = -1;
  printf("Done.\n");

  printf("Running BI_sub_ii_test()...\n");
  if (BI_sub_ii_test())
    result = -1;
  printf("Done.\n");

  printf("Running BI_sub_bi_test()...\n");
  if (BI_sub_bi_test())
    result = -1;
  printf("Done.\n");

  printf("Running BI_sub_ib_test()...\n");
  if (BI_sub_ib_test())
    result = -1;
  printf("Done.\n");

  printf("Running BI_sub_bb_test()...\n");
  if (BI_sub_bb_test())
    result = -1;
  printf("Done.\n");

  printf("Running BI_mul_ii_test()...\n");
  if (BI_mul_ii_test())
    result = -1;
  printf("Done.\n");

  printf("Running BI_mul_bi_test()...\n");
  if (BI_mul_bi_test())
    result = -1;
  printf("Done.\n");

  printf("Running BI_mul_bb_test()...\n");
  if (BI_mul_bb_test())
    result = -1;
  printf("Done.\n");

  printf("Running BI_pow_ii_test()...\n");
  if (BI_pow_ii_test())
    result = -1;
  printf("Done.\n");

  printf("Running BI_pow_bi_test()...\n");
  if (BI_pow_bi_test())
    result = -1;
  printf("Done.\n");

  printf("Running BI_pow_ib_test()...\n");
  if (BI_pow_ib_test())
    result = -1;
  printf("Done.\n");

  printf("Running BI_pow_bb_test()...\n");
  if (BI_pow_bb_test())
    result = -1;
  printf("Done.\n");

  printf("Running BI_div_mod_ii_test()...\n");
  if (BI_div_mod_ii_test())
    result = -1;
  printf("Done.\n");

  printf("Running BI_div_mod_bi_test()...\n");
  if (BI_div_mod_bi_test())
    result = -1;
  printf("Done.\n");

  printf("Running BI_div_mod_ib_test()...\n");
  if (BI_div_mod_ib_test())
    result = -1;
  printf("Done.\n");

  printf("Running BI_div_mod_bb_test()...\n");
  if (BI_div_mod_bb_test())
    result = -1;
  printf("Done.\n");

  printf("Running BI_div_ii_test()...\n");
  if (BI_div_ii_test())
    result = -1;
  printf("Done.\n");

  printf("Running BI_div_bi_test()...\n");
  if (BI_div_bi_test())
    result = -1;
  printf("Done.\n");

  printf("Running BI_div_ib_test()...\n");
  if (BI_div_ib_test())
    result = -1;
  printf("Done.\n");

  printf("Running BI_div_bb_test()...\n");
  if (BI_div_bb_test())
    result = -1;
  printf("Done.\n");

  printf("Running BI_mod_ii_test()...\n");
  if (BI_mod_ii_test())
    result = -1;
  printf("Done.\n");

  printf("Running BI_mod_bi_test()...\n");
  if (BI_mod_bi_test())
    result = -1;
  printf("Done.\n");

  printf("Running BI_mod_ib_test()...\n");
  if (BI_mod_ib_test())
    result = -1;
  printf("Done.\n");

  printf("Running BI_mod_bb_test()...\n");
  if (BI_mod_bb_test())
    result = -1;
  printf("Done.\n");

  printf("Running BI_cmp_bi_test()...\n");
  if (BI_cmp_bi_test())
    result = -1;
  printf("Done.\n");

  printf("Running BI_cmp_bb_test()...\n");
  if (BI_cmp_bb_test())
    result = -1;
  printf("Done.\n");

  printf("Running BI_cmp_mag_bi_test()...\n");
  if (BI_cmp_mag_bi_test())
    result = -1;
  printf("Done.\n");

  printf("Running BI_cmp_mag_bb_test()...\n");
  if (BI_cmp_mag_bb_test())
    result = -1;
  printf("Done.\n");

  printf("Running BI_to_str_test()...\n");
  if (BI_to_str_test())
    result = -1;
  printf("Done.\n");

  printf("Running BI_to_int_test()...\n");
  if (BI_to_int_test())
    result = -1;
  printf("Done.\n");

  return result;
}

