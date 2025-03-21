/* SPDX-License-Identifier: Apache-2.0 */
/** @file
 * @brief API to execute a program in a child process
 *
 * @defgroup te_tools_te_exec_child Execute a program in a child process
 * @ingroup te_tools
 * @{
 *
 * Routines to execute a program in a child process
 */

#ifndef __TE_EXEC_CHILD_H__
#define __TE_EXEC_CHILD_H__

#if HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#if HAVE_UNISTD_H
#include <unistd.h>
#endif
#if HAVE_SCHED_H
#include <sched.h>
#endif

#include "te_defs.h"
#include "logger_api.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TE_EXEC_CHILD_DEV_NULL_FD ((int *)-1)

/** Kinds of process parameters. */
typedef enum te_exec_param_type {
    /** Set CPU affinity */
    TE_EXEC_AFFINITY,
    /** Set process priority */
    TE_EXEC_PRIORITY,
    /** Set process working directory */
    TE_EXEC_WORKDIR,

    /** Last kind marker */
    TE_EXEC_END
} te_exec_param_type;

/**
 * Use @p te_exec_param_type in the new code.
 *
 * @deprecated
 */
typedef enum te_sched_param_type {
    /** Type to set CPU affinity */
    TE_SCHED_AFFINITY = TE_EXEC_AFFINITY,
    /** Type to set process priority */
    TE_SCHED_PRIORITY = TE_EXEC_PRIORITY,

    /** Last type of scheduling. */
    TE_SCHED_END = TE_EXEC_END
} te_sched_param_type;

/** Process parameter. */
typedef struct te_exec_param {
    /** Parameter kind */
    te_exec_param_type type;
    /** Parameter-specific data */
    void *data;
} te_exec_param;

/**
 * Use @p te_exec_param in the new code.
 *
 * @deprecated
 */
typedef te_exec_param te_sched_param;

/** Data specific for CPU affinity type (@p TE_EXEC_AFFINITY). */
typedef struct te_exec_affinity_param {
    /** Array of CPU IDs */
    int *cpu_ids;
    /** Array size */
    size_t cpu_ids_len;
} te_exec_affinity_param;

/**
 * Use @p te_exec_affinity_param in the new code.
 *
 * @deprecated
 */
typedef te_exec_affinity_param te_sched_affinity_param;

/** Data specific for priority type (@p TE_EXEC_PRIORITY). */
typedef struct te_exec_priority_param {
    /** Process priority. */
    int priority;
} te_exec_priority_param;

/**
 * Use @p te_exec_priority_param in the new code.
 *
 * @deprecated
 */
typedef te_exec_priority_param te_sched_priority_param;

/** Data specific for working directory type (@p TE_EXEC_WORKDIR). */
typedef struct te_exec_workdir_param {
    /** Process working directory. */
    char *workdir;
} te_exec_workdir_param;

/**
 * Function to base system()-like and popen()-like functions on it.
 * You MUST use uid parameter instead of "su - user -c", because su makes
 * one more fork, and you do not know how to kill this grandchild process.
 *
 * The function:
 * 1) creates pipes (if necessary) to communicate with a child process;
 * 2) fork()'s the child process;
 * 3) modifies standard file descriptors of the child process according
 *    to @p in_fd @p out_fd @p err_fd;
 * 4) executes specified program in the child process;
 *
 * @note if a file descriptor location is @c NULL, copy of the respective
 *       standard file descriptor will be inherited by the child;
 *       if a file descriptor location is @c TE_EXEC_CHILD_DEV_NULL_FD,
 *       the respective standard file descriptor of the child will be
 *       redirected to /dev/null
 *
 * If you use this fuction from agent, this process SHOULD be catched by
 * ta_waitpid() or killed by ta_kill_death().
 *
 * @param file         file to execute
 * @param argv         command line arguments terminated by @c NULL;
 *                     MUST NOT be @c NULL
 * @param envp         environment variables terminated by @c NULL;
 *                     use NULL to keep the current environment
 * @param uid          user id to execute the file; use -1 for default
 * @param in_fd        location to store file descriptor of pipe to stdin of
 *                     the process; see note
 * @param out_fd       location to store file descriptor of pipe to stdout of
 *                     the process; see note
 * @param out_fd       location to store file descriptor of pipe to stderr of
 *                     the process; see note
 * @param exec_param   Array of process parameters. The last element must
 *                     have the type TE_EXEC_END and data pointer to
 *                     @c NULL. May be @c NULL.
 *
 * @return pid value, positive on success, negative on failure
 */
extern pid_t te_exec_child(const char *file, char *const argv[],
                           char *const envp[], uid_t uid, int *in_fd,
                           int *out_fd, int *err_fd,
                           const te_exec_param *exec_param);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* !__TE_EXEC_CHILD_H__ */
/**@} <!-- END te_tools_te_exec_child --> */
