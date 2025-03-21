/* SPDX-License-Identifier: Apache-2.0 */

#ifndef __REDIS_SRV_H__
#define __REDIS_SRV_H__

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

#endif /* !__REDIS_SRV_H__ */
