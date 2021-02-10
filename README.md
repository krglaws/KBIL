# KBIL
### "Kyle's Big Integer Library"

I wrote this after I learned a little about RSA encryption. I really wanted to write my own library to handle the big integers that are required for encryption instead of using GNU MP or some other library. The main goal was to just get everything working. Unfortunately the code is painfully slow, so this is more of a toy/experiment than something that is actually useful.

## Demo
Here is an example of how to use KBIL:

```c
/* gcc main.c -lkbil -o pow */

#include <stdio.h>
#include <stdlib.h>
#include <kbil.h>


int main(int argc, char** argv)
{
  bigint* base = BI_new_s(argv[1], 10);
  bigint* e = BI_new_s(argv[2], 10);
  
  bigint* result = BI_new_i(0);
  BI_pow_bb(result, base, e);
  
  char* s = BI_to_str(result, 10);
  printf("%s\n", s);
  free(s);
  
  BI_free(base);
  BI_free(e);
  BI_free(result);
}
```

## Build and Install
```sh
$ git clone https://github.com/krglaws/KBIL
$ cd KBIL
$ make
$ sudo make install
```
