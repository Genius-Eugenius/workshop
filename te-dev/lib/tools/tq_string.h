/* SPDX-License-Identifier: Apache-2.0 */
/** @file
 * @brief Tail queue of strings (char *).
 *
 * @defgroup te_tools_tq_string Tail queue of strings
 * @ingroup te_tools
 * @{
 *
 * Definition of API for working with tail queue of strings.
 *
 *
 */

#ifndef __TE_TOOLS_TQ_STRING_H__
#define __TE_TOOLS_TQ_STRING_H__

#include "te_defs.h"
#include "te_queue.h"
#include "te_errno.h"

#ifdef __cplusplus
extern "C" {
#endif


/** Element of the list of strings */
typedef struct tqe_string {
    TAILQ_ENTRY(tqe_string)     links;  /**< List links */
    char                       *v;      /**< Value */
} tqe_string;

/** Head of the list of strings */
typedef TAILQ_HEAD(tqh_strings, tqe_string) tqh_strings;


/**
 * Free list of allocated strings.
 *
 * @param head          Head of the list
 * @param value_free    Function to be called to free value or NULL
 */
extern void tq_strings_free(tqh_strings *head, void (*value_free)(void *));

/**
 * Compare two tail queue of strings. Queues are equal, if each element
 * of the first queue is equal to the corresponding element of the
 * second queue.
 *
 * @param s1        The first tail queue
 * @param s2        The second tail queue
 *
 * @retval @c true     Equal
 * @retval @c false    Not equal
 */
extern bool tq_strings_equal(const tqh_strings *s1,
                                const tqh_strings *s2);

/**
 * Add a new string into the list, if no such string is already present
 * there.
 *
 * @param list      Head of the list
 * @param value     A new string to add
 * @param duplicate Whether to duplicate string
 *                  or not
 *
 * @return
 *          0 if string was added
 *          1 if string was already presented
 * @bug The return type of the function is actually boolean.
 */
extern te_errno tq_strings_add_uniq_gen(tqh_strings *list,
                                        const char *value,
                                        bool duplicate);

/**
 * Add a new string into the list, if no such string is already present
 * there. String value is not duplicated.
 *
 * @param list      Head of the list
 * @param value     A new string to add
 *
 * @return
 *          0 if string was added
 *          1 if string was already presented
 * @bug The return type of the function is actually boolean.
 */
extern te_errno tq_strings_add_uniq(tqh_strings *list, const char *value);

/**
 * Add a new string into the list, if no such string is already present
 * there. String value is duplicated.
 *
 * @param list      Head of the list
 * @param value     A new string to add
 *
 * @return
 *          0 if string was added
 *          1 if string was already presented
 * @bug The return type of the function is actually boolean.
 */
extern te_errno tq_strings_add_uniq_dup(tqh_strings *list,
                                        const char *value);

/**
 * Move values from source queue to destination queue.
 *
 * @param dst       Destination queue
 * @param src       Source queue
 *
 * @return Status code.
 */
extern te_errno tq_strings_move(tqh_strings *dst,
                                tqh_strings *src);

/**
 * Copy values from source queue to destination queue.
 *
 * @param dst       Destination queue
 * @param src       Source queue
 *
 * @return Status code.
 */
extern te_errno tq_strings_copy(tqh_strings *dst,
                                const tqh_strings *src);

/**
 * Copy members from source queue to destination queue but
 * let copied members to point to the same strings.
 *
 * @param dst       Destination queue
 * @param src       Source queue
 *
 * @return Status code.
 */
extern te_errno tq_strings_shallow_copy(tqh_strings *dst,
                                        const tqh_strings *src);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* !__TE_TOOLS_TQ_STRING_H__ */
/**@} <!-- END te_tools_tq_string --> */
