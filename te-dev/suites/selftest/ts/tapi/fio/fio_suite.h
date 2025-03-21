/* SPDX-License-Identifier: Apache-2.0 */
/** @file
 * @brief TAPI fio tests
 *
 * Common includes and definitions.
 */

#ifndef __FIO_SUITE_H__
#define __FIO_SUITE_H__

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
 * Test suite specific part of the last action of the test @b main()
 * function.
 */
#define TEST_END_SPECIFIC TEST_END_ENV
#endif

#include "tapi_test.h"
#include "tapi_env.h"
#include "tapi_job_factory_rpc.h"
#include "tapi_fio.h"

#endif /* !__FIO_SUITE_H__ */
