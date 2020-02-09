
#include <stdio.h>
#include <stdint.h>
#include "bigint.h"

int main()
{
  bigint* a = BI_new((int32_t) 20000000);
  bigint* b = BI_new((int32_t) 2000000000);
  bigint* c = BI_new(0);

  BI_print(a);
  printf("times\n");
  BI_print(b);

  BI_mul(c, a, b);

  printf("equals\n");
  BI_print(c);
}

