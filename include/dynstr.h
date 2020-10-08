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

#if __STDC_VERSION__ < 199901L
#error C99 support is mandatory for dynstr
#endif /* __STDC_VERSION < 199901L */

#include <stdarg.h>
#include <stddef.h>
#include <stdbool.h>

/* Since dynstr_append() might fail (as it is based on dynamic memory
 * allocation), these macros can be used to avoid boilerplate code. */

/**
 * Convenience macro that calls dynstr_append and returns NULL if failed.
 */
#define dynstr_append_or_ret_null(...) \
   do {if (dynstr_append(__VA_ARGS__) != DYNSTR_OK) return NULL;} while (0)

/**
 * Convenience macro that calls dynstr_append and returns false if failed.
 */
#define dynstr_append_or_ret_false(...) \
   do {if (dynstr_append(__VA_ARGS__) != DYNSTR_OK) return false;} while (0)

/**
 * Convenience macro that calls dynstr_append and returns its error code if failed.
 */
#define dynstr_append_or_ret(...) \
   { \
      const enum dynstr_err err = dynstr_append(__VA_ARGS__);  \
      if (err != DYNSTR_OK) return err; \
   }

/**
 * Convenience macro that calls dynstr_append and returns zero if failed.
 */
#define dynstr_append_or_ret_zero(...) \
   do {if (dynstr_append(__VA_ARGS__) != DYNSTR_OK) return 0;} while (0)

/**
 * Convenience macro that calls dynstr_append and returns one if failed.
 */
#define dynstr_append_or_ret_nonzero(...) \
   do {if (dynstr_append(__VA_ARGS__) != DYNSTR_OK) return 1;} while (0)

/**
 * Convenience macro that calls dynstr_prepend and returns NULL if failed.
 */
#define dynstr_prepend_or_ret_null(...) \
   do {if (dynstr_prepend(__VA_ARGS__) != DYNSTR_OK) return NULL;} while (0)

/**
 * Convenience macro that calls dynstr_prepend and returns false if failed.
 */
#define dynstr_prepend_or_ret_false(...) \
   do {if (dynstr_prepend(__VA_ARGS__) != DYNSTR_OK) return false;} while (0)

/**
 * Convenience macro that calls dynstr_prepend and returns its error code if failed.
 */
#define dynstr_prepend_or_ret(...) \
   { \
      const enum dynstr_err err = dynstr_prepend(__VA_ARGS__);  \
      if (err != DYNSTR_OK) return err; \
   }

/**
 * Convenience macro that calls dynstr_prepend and returns zero if failed.
 */
#define dynstr_prepend_or_ret_zero(...) \
   do {if (dynstr_prepend(__VA_ARGS__) != DYNSTR_OK) return 0;} while (0)

/**
 * Convenience macro that calls dynstr_prepend and returns one if failed.
 */
#define dynstr_prepend_or_ret_nonzero(...) \
   do {if (dynstr_prepend(__VA_ARGS__) != DYNSTR_OK) return 1;} while (0)

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
 * List of errors that this library might return.
 * @note Following the tradition, success code is guaranteed to always be
 * zero.
 * @attention Error codes are guaranteed to be non-zero, but their ordering
 * might change, so please use descriptive error names instead of their integer
 * equivalents.
 */
enum dynstr_err
{
   DYNSTR_OK, /**< Operation was successful. */
   DYNSTR_ERR_ALLOC, /**< Alloc operation failed. */
   DYNSTR_ERR_INIT, /**< Dynamic string was not initialized. */
   DYNSTR_ERR_SRC /**< Source string has invalid parameters. */
};

/**
 * This function initializes string pointer to NULL and sets zero length.
 * @attention Always call this function before using an instance.
 * For future compatibility, please avoid initializing instances by calling
 * @c memset or similar and use this function instead.
 * @param d Dynamic string to be initialized.
 */
void dynstr_init(struct dynstr *d);

/**
 * This function takes a string literal in printf format and a variable
 * number of arguments, calculates its size and concatenates it into the
 * dynamic string.
 * @param d Dynamic string where new string will be appended.
 * @param format String literal in printf format.
 * @return Returns one of the following error codes:
 *    # DYNSTR_OK if successful.
 *    # DYNSTR_ERR_ALLOC if no more memory is available.
 */
enum dynstr_err dynstr_append(struct dynstr *d, const char *format, ...);

/**
 * This function takes a string literal in printf format and a variable
 * argument list, calculates its size and concatenates it into the
 * dynamic string.
 * @param d Dynamic string where new string will be appended.
 * @param format String literal in printf format.
 * @param ap Variable argument list.
 * @return Returns one of the following error codes:
 *    # DYNSTR_OK if successful.
 *    # DYNSTR_ERR_ALLOC if no more memory is available.
 */
enum dynstr_err dynstr_vappend(struct dynstr *d, const char *format, va_list ap);

/**
 * This function takes a string literal in printf format and a variable
 * number of arguments, calculates its size and prepends it into the
 * beginning of the dynamic string.
 * @param d Dynamic string where new string will be prepended.
 * @param format String literal in printf format.
 * @return Returns one of the following error codes:
 *    # DYNSTR_OK if successful.
 *    # DYNSTR_ERR_ALLOC if no more memory is available.
 */
enum dynstr_err dynstr_prepend(struct dynstr *d, const char *format, ...);

/**
 * This function takes a string literal in printf format and a variable
 * argument list, calculates its size and prepends it into the beginning of the
 * dynamic string.
 * @param d Dynamic string where new string will be prepended.
 * @param format String literal in printf format.
 * @param ap Variable argument list.
 * @return Returns one of the following error codes:
 *    # DYNSTR_OK if successful.
 *    # DYNSTR_ERR_ALLOC if no more memory is available.
 */
enum dynstr_err dynstr_vprepend(struct dynstr *d, const char *format, va_list ap);

/**
 * This function duplicates a dynamic string to another instance.
 * @attention Destination instance must be initialized before calling
 * this function.
 * @attention Since this function performs a deep copy, please remember to
 * free the source instance if no longer used to avoid memory leaks.
 * @return Returns one of the following error codes:
 *    # DYNSTR_OK if successful.
 *    # DYNSTR_ERR_ALLOC if no more memory is available.
 *    # DYNSTR_ERR_INIT if destination dynamic string was either not
 *      initialized or already contains data.
 *    # DYNSTR_ERR_SRC if source dynamic string has no length or data.
 * @note This function has the same effect as calling:
 * @code
 *    dynstr_append(&dst, "%s", src.str);
 * @endcode
 * However, the implementation for dynstr_dup() should be faster as it
 * does not rely on vsnprintf(), as opposed to dynstr_append(). If in
 * doubt, always profile your code.
 */
enum dynstr_err dynstr_dup(struct dynstr *dst, const struct dynstr *src);

/**
 * This function frees memory used by the dynamic string.
 * @param d Dynamic string to be freed.
 * @note This function does nothing on empty, initialized instances.
 * @attention Calling this function on an uninitialized instance leads to
 * undefined behaviour.
 * @attention Once memory is freed, @ref dynstr_init is called so it can be
 * used again.
 */
void dynstr_free(struct dynstr *d);

#endif /* DYNSTR_H */
