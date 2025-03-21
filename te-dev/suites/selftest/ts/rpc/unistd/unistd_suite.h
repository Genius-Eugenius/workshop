/* SPDX-License-Identifier: Apache-2.0 */

#ifndef __UNISTD_SUITE_H__
#define __UNISTD_SUITE_H__
#ifndef TEST_START_VARS
/**
 * Test suite specific variables of the test @b main() function.
 */
#define TEST_START_VARS TEST_START_ENV_VARS
#endif
#ifndef TEST_START_SPECIFIC
/**
 * Test suite specific the first actions of the test.
 */
#define TEST_START_SPECIFIC TEST_START_ENV
#endif
#ifndef TEST_END_SPECIFIC
/**
 * Test suite specific part of the last action of the test @b main() function.
 */
#define TEST_END_SPECIFIC TEST_END_ENV
#endif

#include "te_config.h"

#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include "tapi_test.h"
#include "te_file.h"
#include "te_bufs.h"
#include "rcf_rpc.h"
#include "tapi_rpc_unistd.h"
#include "tapi_rpc_dirent.h"
#include "tapi_rpcsock_macros.h"
#include "tapi_env.h"
#include "tapi_mem.h"
#include "tapi_file.h"

static inline void
unistd_compare_and_fail(const void *exp_buf, size_t exp_len,
                        unsigned int n_copies,
                        const void *actual_buf, size_t actual_len)
{
    if (!te_compare_bufs(exp_buf, exp_len, n_copies, actual_buf, actual_len,
                         TE_LL_ERROR))
    {
        TEST_VERDICT("Buffers do not match");
    }
}

#endif /* !__UNISTD_SUITE_H__ */
