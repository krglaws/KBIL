
#include <stdio.h>
#include <stdint.h>
#include "bigint.h"

int main()
{
  bigint* a = BI_new(2000000000);
  bigint* b = BI_new(2000000000);
  bigint* c = BI_new(0);

  BI_print(a);
  printf("times\n");
  BI_print(b);

  BI_mul(c, a, b);

  printf("equals\n");
  BI_print(c);

  printf("squared equals\n");
  BI_mul(c, c, c);
  BI_print(c);

  printf("squared equals\n");
  BI_mul(c, c, c);
  //BI_print(c);

}

