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

#if __STDC_VERSION__ < 199901L
#error C99 support is mandatory for dynstr
#endif /* __STDC_VERSION < 199901L */

void dynstr_init(struct dynstr *const d)
{
    memset(d, 0, sizeof *d);
}

enum dynstr_err dynstr_vappend(struct dynstr *const d, const char *const format, va_list ap)
{
    enum dynstr_err err = DYNSTR_OK;
    va_list apc;

    va_copy(apc, ap);

    {
        const int src_len = vsnprintf(NULL, 0, format, ap);

        if (src_len >= 0)
        {
            const size_t new_len = d->len + src_len + 1;

            d->str = realloc(d->str, new_len * sizeof *d->str);

            if (d->str)
            {
                if (vsprintf(d->str + d->len, format, apc) >= 0)
                {
                    d->len += src_len;
                }
                else
                {
                    err = DYNSTR_ERR_PRINTF;
                }
            }
            else
            {
                err = DYNSTR_ERR_ALLOC;
            }
        }
        else
        {
            err = DYNSTR_ERR_PRINTF;
        }
    }

    va_end(apc);
    return err;
}

enum dynstr_err dynstr_append(struct dynstr *const d, const char *const format, ...)
{
    va_list ap;
    enum dynstr_err err;

    va_start(ap, format);
    err = dynstr_vappend(d, format, ap);
    va_end(ap);
    return err;
}

enum dynstr_err dynstr_vprepend(struct dynstr *const d, const char *const format, va_list ap)
{
    enum dynstr_err err = DYNSTR_OK;
    va_list apc;

    va_copy(apc, ap);

    {
        const size_t src_len = vsnprintf(NULL, 0, format, ap);
        const size_t new_len = d->len + src_len + 1;

        d->str = realloc(d->str, new_len * sizeof *d->str);

        if (d->str && d->len)
        {
            /* Keep byte that will be removed by later call to vsprintf. */
            const char c = *d->str;

            for (size_t i = new_len - 1, j = d->len; j <= d->len; i--, j--)
            {
                d->str[i] = d->str[j];
            }

            if (vsprintf(d->str, format, apc) >= 0)
            {
                d->str[src_len] = c;
            }
            else
            {
                err = DYNSTR_ERR_PRINTF;
            }
        }
        else if (!d->len)
        {
            if (vsprintf(d->str + d->len, format, apc) < 0)
            {
                err = DYNSTR_ERR_PRINTF;
            }
        }
        else
        {
            err = DYNSTR_ERR_ALLOC;
        }

        d->len += src_len;
    }

    va_end(apc);
    return err;
}

enum dynstr_err dynstr_prepend(struct dynstr *const d, const char *const format, ...)
{
    va_list ap;
    enum dynstr_err err;

    va_start(ap, format);
    err = dynstr_vprepend(d, format, ap);
    return err;
}

enum dynstr_err dynstr_dup(struct dynstr *const dst, const struct dynstr *const src)
{
    if (!dst->str && !dst->len)
    {
        if (src->len && src->str)
        {
            const size_t sz = (src->len + 1) * sizeof *dst->str;
            dst->str = realloc(dst->str, sz);

            if (dst->str)
            {
                memcpy(dst->str, src->str, sz);
                dst->len = src->len;
            }
            else
            {
                return DYNSTR_ERR_ALLOC;
            }
        }
        else
        {
            return DYNSTR_ERR_SRC;
        }
    }
    else
    {
        return DYNSTR_ERR_INIT;
    }

    return DYNSTR_OK;
}

void dynstr_free(struct dynstr *const d)
{
    if (d->str)
    {
        free(d->str);
        dynstr_init(d);
    }
}
