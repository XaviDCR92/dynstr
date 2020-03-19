dynstr
======

Minimal library that provides dynamic strings using plain C99. Convenience
macros are also provided if GNU C is used.

# Usage

```
#include <stdio.h>
#include "dynstr.h"

int main(const int argc, const char *argv[])
{
    struct dynstr s;

    dynstr_init(&s);
    dynstr_append(&s, "Hello");
    dynstr_append(&s, " from %s:%d\n", __func__, __LINE__);
    printf("%s", s.str);
    dynstr_free(&s);

    return 0;
}
```
Output:

```
Hello from main:10
```
# License
See LICENSE file.
