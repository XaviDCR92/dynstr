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

#ifndef DYNSTR_H
#define DYNSTR_H

#include <stddef.h>

#ifdef __GNUC__

/* Since dynstr_append() might fail (as it is based on dynamic memory
 * allocation), these macros can be used to avoid boilerplate code.
 * However, C99 variadic macros might expand with a trailing comma
 * when no parameters are given after "format", a GNU extension is
 * needed to avoid this problem. */
#include <stdbool.h>

/**
 * Convenience macro that calls dynstr_append and returns NULL if failed.
 */
#define dynstr_append_or_ret_null(d, format, ...) if (dynstr_append(d, format, ## __VA_ARGS__)) return NULL;

/**
 * Convenience macro that calls dynstr_append and returns false if failed.
 */
#define dynstr_append_or_ret_false(d, format, ...) if (dynstr_append(d, format, ## __VA_ARGS__)) return false;

/**
 * Convenience macro that calls dynstr_append and returns its error code if failed.
 */
#define dynstr_append_or_ret(d, format, ...) {const int err = dynstr_append(d, format, ## __VA_ARGS__); if (err) return err;}

/**
 * Convenience macro that calls dynstr_append and returns zero if failed.
 */
#define dynstr_append_or_ret_zero(d, format, ...) if (dynstr_append(d, format, ## __VA_ARGS__)) return 0;

#endif /* __GNUC__ */

/**
 * Dynamic string type used for this library.
 * @note If needed, members can be safely read but should not be modified
 * outside this library.
 */
struct dynstr
{
    char *str; /**< Null-terminated string. */
    size_t len; /**< String length, null character not included. */
};

/**
 * Reportedly, it initializes an empty string with zero length.
 * @attention Always call this function when creating a dynstr instance.
 * @param d Dynamic string to be initialized.
 */
void dynstr_init(struct dynstr *d);

/**
 * This function takes a string literal in printf format and a variable
 * number of arguments, calculates its size and concatenates it into the
 * dynamic string.
 * @param d Dynamic string where new string will be appended.
 * @param format String literal in printf format.
 * @return Returns 0 if successful, 1 if alloc operation failed.
 */
int dynstr_append(struct dynstr *d, const char *format, ...);

/**
 * This function frees memory used by the dynamic string.
 * @param d Dynamic string to be freed.
 * @attention Attempting to call this function on an uninitialized or empty
 * instance is undefined behaviour.
 * @attention Parameters inside the struct are reset once memory is freed.
 */
void dynstr_free(struct dynstr *d);

#endif /* DYNSTR_H */
