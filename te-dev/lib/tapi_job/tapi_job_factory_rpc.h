/* SPDX-License-Identifier: Apache-2.0 */
/** @file
 * @brief TAPI to handle RPC server job factory
 *
 * @defgroup tapi_job_factory_rpc RPC server job factory control (tapi_job_factory_rpc)
 * @ingroup tapi_job
 * @{
 *
 * TAPI to handle RPC server job factory.
 *
 */

#ifndef __TAPI_JOB_FACTORY_RPC_H__
#define __TAPI_JOB_FACTORY_RPC_H__

#include "rcf_rpc.h"

#ifdef __cplusplus
extern "C" {
#endif

struct tapi_job_factory_t;

/**
 * Create a job factory by the RPC server @p rpcs.
 *
 * @param rpcs          RPC server
 * @param[out] factory  Job factory handle
 *
 * @return 0
 *
 * @note The function never returns an error. Its return type is not void
 *       for legacy reasons. New code should never check the return value.
 */
extern te_errno tapi_job_factory_rpc_create(rcf_rpc_server *rpcs,
                                            struct tapi_job_factory_t **factory);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __TAPI_JOB_FACTORY_RPC_H__ */

/** @} <!-- END tapi_job_factory_rpc --> */
