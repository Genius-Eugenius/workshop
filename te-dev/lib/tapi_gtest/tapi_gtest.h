/* SPDX-License-Identifier: Apache-2.0 */
/** @file
 * @brief Wrapper for Google Test
 *
 * @defgroup tapi_gtest GTest support
 * @ingroup te_ts_tapi
 * @{
 *
 * Test API for run Google Test binaries
 *
 */

#ifndef __TAPI_GTEST_H__
#define __TAPI_GTEST_H__

#include "tapi_job.h"
#include "tapi_job_opt.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Internal implementation struct */
typedef struct tapi_gtest_impl {
    tapi_job_t *job;                /**< GTest job handler */
    tapi_job_channel_t *out[2];     /**< GTest stdour/stderr channels */
} tapi_gtest_impl;

/** Defaults for implementation struct */
#define TAPI_GTEST_IMPL_DEFAULTS (tapi_gtest_impl)  \
{                                                   \
    .job = NULL,                                    \
    .out = {0},                                     \
}

typedef struct tapi_gtest_opts {
    const char *gtest_filter;      /**< Concatenaion of Group and Test names */
    bool run_disabled;             /**< Force run disabled test */
    bool ipv4_only;                /**< Force use of IPv4 */
    bool no_col;                   /**< Disable colours in GTest output */
    tapi_job_opt_uint_t rand_seed; /**< Random seed */

    const char *dev_name;          /**< RDMA device name for GTest */
    tapi_job_opt_uint_t verbs_mtu; /**< MTU for RDMA QP */
} tapi_gtest_opts;

/** GTest handler */
typedef struct tapi_gtest {
    const char *bin;        /**< Path to GTest binary */
    const char *group;      /**< Group name in GTest */
    const char *name;       /**< Test name in GTest */
    tapi_gtest_opts opts;   /**< Options for Gtest binary */

    tapi_gtest_impl impl;   /**< Internal implementation struct */
} tapi_gtest;

/** Defaults for implementation for GTest handler */
#define TAPI_GTEST_DEFAULTS (tapi_gtest)       \
{                                              \
    .bin = NULL,                               \
    .group = NULL,                             \
    .name = NULL,                              \
    .opts.dev_name = NULL,                     \
    .opts.run_disabled = false,                \
    .opts.ipv4_only = false,                   \
    .opts.no_col = true,                       \
    .opts.verbs_mtu = TAPI_JOB_OPT_UINT_UNDEF, \
    .opts.rand_seed = TAPI_JOB_OPT_UINT_UNDEF, \
    .impl = TAPI_GTEST_IMPL_DEFAULTS,          \
}

/** A way for read gtest option from test arguments */
#define TEST_GTEST_PARAM(_gtest) (tapi_gtest)                             \
{                                                                         \
    .bin = TEST_STRING_PARAM(_gtest##_bin),                               \
    .group = TEST_STRING_PARAM(_gtest##_group),                           \
    .name = TEST_STRING_PARAM(_gtest##_name),                             \
    .opts.verbs_mtu = TAPI_JOB_OPT_UINT_UNDEF,                            \
    .opts.rand_seed = TE_OPTIONAL_UINT_VAL(TEST_INT_PARAM(te_rand_seed)), \
    .opts.ipv4_only = false,                                              \
    .opts.run_disabled = false,                                           \
    .opts.no_col = true,                                                  \
    .opts.dev_name = NULL,                                                \
    .impl = TAPI_GTEST_IMPL_DEFAULTS,                                     \
}

/** A way for read gtest option from test arguments */
#define TEST_GET_GTEST_PARAM(_gtest) \
    (_gtest) = TEST_GTEST_PARAM(_gtest)

/**
 * Create GTest
 *
 * @param gtest     GTest handler
 * @param factory   Job factory
 *
 * @return Status code
 */
extern te_errno tapi_gtest_init(tapi_gtest *gtest, tapi_job_factory_t *factory);

/**
 * Start GTest
 *
 * @param gtest     GTest handler
 *
 * @return Status code
 */
extern te_errno tapi_gtest_start(tapi_gtest *gtest);

/**
 * Stop GTest
 *
 * @param gtest     GTest handler
 *
 * @return Status code
 */
extern te_errno tapi_gtest_stop(tapi_gtest *gtest);

/**
 * Wait GTest and get result
 *
 * @param gtest         GTest handler
 * @param timeout_ms    Timeout for wait
 *
 * @return Status code
 */
extern te_errno tapi_gtest_wait(tapi_gtest *gtest, int timeout_ms);

/**
 * Cleanup GTest
 *
 * @param gtest         GTest handler
 *
 * @return Status code
 */
extern te_errno tapi_gtest_fini(tapi_gtest *gtest);

#ifdef __cplusplus
} /* extern "C" */
#endif

/**
 * @page tapi-gtest-scenarios GTest TAPI usage scenarios
 *
 * Wrap GTest
 * ------------
 *
 * \code{.c}
 * int
 * main(int argc, char *argv[])
 * {
 *     tapi_gtest gtest;
 *     tapi_job_factory_t *factory;
 *
 *     TEST_START;
 *     TEST_GET_GTEST_PARAM(gtest);
 *
 *     CHECK_RC(tapi_job_factory_rpc_create(rpcs, &factory));
 *     CHECK_RC(tapi_gtest_init(&gtest, factory));
 *     CHECK_RC(tapi_gtest_start(&gtest));
 *     CHECK_RC(tapi_gtest_wait(&gtest, TE_SEC2MS(10)));
 *
 *     TEST_SUCCESS;
 *
 * cleanup:
 *     CLEANUP_CHECK_RC(tapi_gtest_fini(&gtest));
 *     tapi_job_factory_destroy(factory);
 *     TEST_END;
 * }
 * \endcode
 */

#endif /* __TAPI_GTEST_H__ */

/**@} <!-- END tapi_gtest --> */
