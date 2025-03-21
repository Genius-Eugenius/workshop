/* SPDX-License-Identifier: Apache-2.0 */
/** @file
 * @brief Unix Test Agent
 *
 * Common declarations for Unix TA configuration
 *
 *
 */

#ifndef __TE_AGENTS_UNIX_CONF_BASE_CONF_COMMON_H_
#define __TE_AGENTS_UNIX_CONF_BASE_CONF_COMMON_H_

#include "te_config.h"
#if HAVE_CONFIG_H
#include "config.h"
#endif

#if HAVE_DIRENT_H
#include <dirent.h>
#endif

#if HAVE_STDLIB_H
#include <stdlib.h>
#endif
#if HAVE_LIMITS_H
#include <limits.h>
#endif

#include "te_defs.h"
#include "te_errno.h"

/* UNIX branching heritage: PATH_MAX can still be undefined here yet */
#if !defined(PATH_MAX)
#define PATH_MAX 1024
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Write requested value to system file.
 *
 * @param value     Null-terminated string containing the value.
 * @param format    Format string for path to the system file.
 * @param ...       Arguments for the format string.
 *
 * @return Status code.
 */
extern te_errno write_sys_value(const char *value, const char *format, ...);

/**
 * Read requested value from system file.
 *
 * @param value           Where to save the value.
 * @param len             Expected length, including null byte.
 * @param ignore_eaccess  If @c true, return success saving
 *                        empty string in @p value if the file
 *                        cannot be opened due to @c EACCES error.
 * @param format          Format string for path to the system file.
 * @param ...             Arguments for the format string.
 *
 * @return Status code.
 */
extern te_errno read_sys_value(char *value, size_t len,
                               bool ignore_eaccess,
                               const char *format, ...);

/**
 * Type of callback which may be passed to get_dir_list().
 * It is used to check whether a given item should be included
 * in the list.
 */
typedef bool (*include_callback_func)(const char *name, void *data);

/**
 * Obtain list of files in a given directory.
 *
 * @param path              Filesystem path.
 * @param buffer            Where to save the list.
 * @param length            Available space in @p buffer.
 * @param ignore_absence    If @c true, return success and
 *                          save empty string to @p buffer
 *                          if @p path does not exist.
 * @param include_callback  If not @c NULL, will be called for
 *                          each file name before including it
 *                          in the list. The file name will be
 *                          included only if this callback
 *                          returns @c true.
 * @param callback_data     Pointer which should be passed
 *                          to the callback as the second argument.
 * @param compar            Comparison function for sorting directory
 *                          entries (may be @c NULL).
 *
 * @return Status code.
 */
extern te_errno get_dir_list(const char *path, char *buffer, size_t length,
                             bool ignore_absence,
                             include_callback_func include_callback,
                             void *callback_data,
                             int (*compar)(const struct dirent **,
                                           const struct dirent **));

/**
 * Replace the given string by another string given
 * allocated from heap. Treat @p src with value @c NULL or
 * pointing to an empty string equally and replace @p dst
 * with @c NULL in this case.
 *
 * @param dst    String to be replaced.
 * @param src    Replacement string.
 *
 * @return Status code.
 */
extern te_errno string_replace(char **dst, const char *src);

/**
 * Initializes the list of instances to be empty.
 *
 * @param list  The list of instances.
 *
 * @return Status code.
 */
extern te_errno string_empty_list(char **list);

/**
 * Get kind of interface ("bond", "vlan", "team", etc).
 *
 * @param ifname      Interface name
 * @param value       Where to save interface kind
 *                    (should be of @c RCF_MAX_VAL length).
 *
 * @return Status code.
 */
extern te_errno get_interface_kind(const char *ifname, char *value);

/**
 * Initialize auxiliary configuration objects used for testing
 * Configurator.
 *
 * @return Status code.
 */
extern te_errno ta_unix_conf_selftest_init(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* !__TE_AGENTS_UNIX_CONF_BASE_CONF_COMMON_H_ */
