# KBIL
### "Kyle's Big Integer Library"

I wrote this after I learned a little about RSA encryption. For some reason I really wanted to write my own library to handle big integers instead of using GNU MP or any other library. I did not work too hard to make it run as fast as possible, as I really just wanted it to work. And I think it does! But it is quite slow at the moment, so maybe some day I will try to make it more optimized.

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
