/*
   Copyright 2020 Xavier Del Campo Romero

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "dynstr.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>

void dynstr_init(struct dynstr *const d)
{
    memset(d, 0, sizeof *d);
}

int dynstr_append(struct dynstr *const d, const char *const format, ...)
{
    va_list ap;

    va_start(ap, format);
    const size_t src_len = vsnprintf(NULL, 0, format, ap);
    size_t new_len;
    va_end(ap);

    if (!d->str)
    {
        new_len = src_len + 1;
        d->str = calloc(src_len + 1, sizeof *d->str);
    }
    else
    {
        new_len = d->len + src_len + 1;
        d->str = realloc(d->str, (d->len + src_len) * sizeof *d->str);
    }

    if (d->str)
    {
        va_start(ap, format);
        vsprintf(d->str + d->len, format, ap);
        va_end(ap);
        d->len += src_len;
    }
    else
    {
        return 1;
    }

    return 0;
}

void dynstr_free(struct dynstr *const d)
{
    free(d->str);
    memset(d, 0, sizeof *d);
}
