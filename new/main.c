
#include <stdio.h>
#include <stdint.h>
#include "bigint.h"

int main()
{
  bigint* a = BI_new(1000000000);
  bigint* b = BI_new(2);
  bigint* c = BI_new(0);

  BI_print(a);
  printf("divided by\n");
  BI_print(b);

  BI_div(c, a, b);

  printf("equals\n");
  BI_print(c);
}

