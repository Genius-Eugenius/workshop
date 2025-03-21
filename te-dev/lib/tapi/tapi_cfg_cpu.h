/* SPDX-License-Identifier: Apache-2.0 */
/** @file
 * @brief Test API to configure CPUs.
 *
 * Definition of API to configure CPUs.
 */

#ifndef __TE_TAPI_CFG_CPU_H__
#define __TE_TAPI_CFG_CPU_H__

#include "conf_api.h"
#include "te_defs.h"
#include "te_errno.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup tapi_conf_cpu CPU topology configuration of Test Agents
 * @ingroup tapi_conf
 * @{
 */

/** Unspecified CPU index used in tapi_cpu_index_t */
#define TAPI_CPU_ID_UNSPEC ((unsigned long)ULONG_MAX)

/** Factor to calculate number of jobs depending on CPU cores number. */
#define TAPI_CFG_CPU_NCORES_FACTOR "ncores"
/** Factor to calculate number of jobs depending on CPU threads number. */
#define TAPI_CFG_CPU_NPROC_FACTOR "nproc"

/** Identifier of a logical CPU (CPU thread) */
typedef struct tapi_cpu_index_t {
    unsigned long node_id;
    unsigned long package_id;
    unsigned long core_id;
    unsigned long thread_id;
} tapi_cpu_index_t;

/** CPU properties that can be requested when looking for a CPU */
typedef struct tapi_cpu_prop_t {
    bool isolated;
} tapi_cpu_prop_t;

/**
 * Grab a CPU on a test agent with requested index
 *
 * @param[in]  ta               Test Agent
 * @param[in]  cpu_id           CPU index
 *
 * @return Status code.
 */
te_errno tapi_cfg_cpu_grab_by_id(const char *ta,
                                 const tapi_cpu_index_t *cpu_id);

/**
 * Release a CPU on a test agent with requested index
 *
 * @param[in]  ta               Test Agent
 * @param[in]  cpu_id           CPU index
 *
 * @return Status code.
 */
te_errno tapi_cfg_cpu_release_by_id(const char *ta,
                                    const tapi_cpu_index_t *cpu_id);

/**
 * Grab a CPU on a test agent with requested properties (if specified)
 * as a resource and retrieve its index.
 *
 * @param[in]  ta               Test Agent
 * @param[in]  prop             CPU properties. May be @c NULL to grab
 *                              any available CPU
 * @param[out] cpu_id           Index of grabbed CPU
 *
 * @return Status code.
 */
te_errno tapi_cfg_cpu_grab_by_prop(const char *ta, const tapi_cpu_prop_t *prop,
                                   tapi_cpu_index_t *cpu_id);

/**
 * Get number of CPU cores on a test agent.
 *
 * @param[in]  ta               Test Agent name.
 * @param[out] size             Number of CPU cores (size of @p indices).
 * @param[out] indices          CPU core indices (might be @c NULL).
 *
 * @return Status code.
 */
extern te_errno tapi_cfg_get_cpu_cores(const char *ta, size_t *size,
                                       tapi_cpu_index_t **indices);

/**
 * Get all available CPU threads indices on a test agents.
 *
 * @param[in]  ta               Test Agent
 * @param[out] size             Number of CPU threads (size of @p indices)
 * @param[out] indices          CPU thread indices (might be @c NULL)
 */
extern te_errno tapi_cfg_get_all_threads(const char *ta,  size_t *size,
                                         tapi_cpu_index_t **indices);

/**
 * Get all available CPU NUMA nodes indices on a test agent.
 *
 * @param[in]  ta               Test Agent
 * @param[out] n_nodes          Number of NUMA nodes (size of @p nodes)
 * @param[out] nodes            CPU NUMA indices (may be @c NULL)
 */
extern te_errno tapi_cfg_cpu_get_nodes(const char *ta, size_t *n_nodes,
                                       tapi_cpu_index_t **nodes);

/**
 * Grab multiple CPUs on a test agent with requested properties (if specified)
 * and requested CPU topology (if specified) as a resource and retrieve
 * their indices.
 *
 * @param[in]  ta               Test Agent
 * @param[in]  prop             CPU properties. May be @c NULL to ignore CPU
 *                              properties
 * @param[in]  topology         CPU topology. Only CPUs with specified
 *                              node/package/core/thread ID will be grabbed.
 *                              May be @c NULL to ignore the restriction
 * @param[in]  n_cpus           Number of CPUs to grab
 * @param[out] cpu_ids          Indices of grabbed CPUs
 */
extern te_errno tapi_cfg_cpu_grab_multiple_with_id(const char *ta,
                                           const tapi_cpu_prop_t *prop,
                                           const tapi_cpu_index_t *topology,
                                           unsigned int n_cpus,
                                           tapi_cpu_index_t *cpu_ids);
/**
 * Wrapper of tapi_cfg_cpu_grab_multiple_with_id() which grabs CPUs on a single
 * NUMA node.
 */
extern te_errno tapi_cfg_cpu_grab_multiple_on_single_node(const char *ta,
                                           const tapi_cpu_prop_t *prop,
                                           unsigned int n_cpus,
                                           tapi_cpu_index_t *cpu_ids);

/**
 * Calculate number of jobs depending on number of CPU cores or threads.
 *
 * The syntax of the expression string:
 * @code
 * COUNT | (FACTOR? ("nproc" | "ncores") ("/" DIVISOR)? (("+" | "-") DISP)?
 *          ("<" MAX)? (">" MIN)?)`
 * @endcode
 * where @c COUNT - exact number of jobs,
 *       @c FACTOR - multiplier for @c nproc or @c ncores,
 *       @c nproc - number of CPU threads,
 *       @c ncores - number of CPU cores,
 *       @c DIVISOR - divisor for @c nproc or @c ncores,
 *       @c DISP - displacement from the calculated number,
 *       @c MAX - upper bound for jobs number,
 *       @c MIN - lower bound for jobs number.
 *
 * Examples:
 *
 * Expression  | nproc | ncores | Result
 * ------------|-------|--------|-------
 * `2`         |       |        | 2
 * `nproc`     | 8     |        | 8
 * `3ncores`   |       | 2      | 6
 * `2nproc/3`  | 6     |        | 4
 * `ncores-1`  |       |4       | 3
 * `nproc<16`  | 32    |        | 16
 * `2ncores>4` |       | 1      | 4
 *
 * In case of division, the value is truncated towards zero.
 * If the resulting value is less than @c 1, it is set to @c 1.
 *
 * @param[in]  ta        Test Agent.
 * @param[in]  expr      Expression for calculating the number of jobs.
 * @param[out] n_jobs    Calculated number of jobs.
 *
 * @return    Status code.
 */
extern te_errno tapi_cfg_cpu_calculate_numjobs(const char *ta,
                                               const char *expr,
                                               unsigned int *n_jobs);

/**@} <!-- END tapi_conf_cpu --> */

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* !__TE_TAPI_CFG_CPU_H__ */
