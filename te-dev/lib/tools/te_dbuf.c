/* SPDX-License-Identifier: Apache-2.0 */
/** @file
 * @brief API to deal with dynamic buffers
 *
 * Implementation of functions to work with dynamic buffers.
 *
 *
 */

#include "te_config.h"
#include "logger_api.h"
#include "te_defs.h"
#include "te_alloc.h"
#include "te_dbuf.h"

/* See description in te_dbuf.h. */
te_errno
te_dbuf_append(te_dbuf *dbuf, const void *data, size_t data_len)
{
    size_t   new_size;

    if ((dbuf->ptr == NULL && dbuf->size != 0) || dbuf->len > dbuf->size)
        TE_FATAL_ERROR("Corrupted dynamic buffer");

    new_size = dbuf->len + data_len;
    if (new_size > dbuf->size)
    {
        new_size += new_size * dbuf->grow_factor / 100;
        TE_REALLOC(dbuf->ptr, new_size);

        dbuf->size = new_size;
    }
    if (data != NULL)
        memcpy(&dbuf->ptr[dbuf->len], data, data_len);
    else
        memset(&dbuf->ptr[dbuf->len], 0, data_len);
    dbuf->len += data_len;
    return 0;
}

/* See description in te_dbuf.h. */
te_errno
te_dbuf_expand(te_dbuf *dbuf, size_t n)
{
    size_t   new_size;

    if ((dbuf->ptr == NULL && dbuf->size != 0) || dbuf->len > dbuf->size)
        TE_FATAL_ERROR("Corrupted dynamic buffer");

    new_size = dbuf->size + n;
    TE_REALLOC(dbuf->ptr, new_size);
    dbuf->size = new_size;

    return 0;
}

/* See description in te_dbuf.h. */
void
te_dbuf_cut(te_dbuf *dbuf, size_t start_index, size_t count)
{
    size_t end_index = start_index + count;

    assert(dbuf != NULL);

    if (start_index >= dbuf->len)
        return;

    if (end_index >= dbuf->len)
    {
        dbuf->len = start_index;
        return;
    }

    memmove(dbuf->ptr + start_index, dbuf->ptr + end_index,
            dbuf->len - end_index);

    dbuf->len -= count;
}

/* See description in te_dbuf.h. */
void
te_dbuf_free(te_dbuf *dbuf)
{
    dbuf->len = dbuf->size = 0;
    free(dbuf->ptr);
    dbuf->ptr = NULL;
}


/* See description in te_dbuf.h. */
void
te_dbuf_print(const te_dbuf *dbuf)
{
#define VALUES_IN_LINE 32
    char str[VALUES_IN_LINE * 3 + 1]; /* [* 3]: two-digit number + space.
                                         [+ 1]: string null-terminator. */
    size_t i;

    VERB("dbuf: ptr: %p, size: %u, len: %u",
         dbuf->ptr, dbuf->size, dbuf->len);
    for (i = 0; i < dbuf->len; i++)
    {
        sprintf(&str[(i % VALUES_IN_LINE) * 3], "%02x ", dbuf->ptr[i]);
        if (i > 0 && (i + 1) % VALUES_IN_LINE == 0)
            VERB(str);
    }
    if (i % VALUES_IN_LINE != 0)
        VERB(str);
#undef VALUES_IN_LINE
}
