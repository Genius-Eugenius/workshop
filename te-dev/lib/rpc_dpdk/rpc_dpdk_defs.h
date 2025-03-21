/* SPDX-License-Identifier: Apache-2.0 */
/** @file
 * @brief DPDK RPC definitions
 *
 * DPDK RPC related definitions
 *
 */

#ifndef __RPC_DPDK_DEFS_H__
#define __RPC_DPDK_DEFS_H__

#ifdef __cplusplus
extern "C" {
#endif

#define RPC_RTE_ETH_NAME_MAX_LEN 64U

#define RPC_RTE_RETA_GROUP_SIZE 64U

#define RPC_RSS_HASH_KEY_LEN_DEF 40U

#define RPC_RTE_EPOLL_PER_THREAD -1

#define TARPC_RTE_MBUF_F_RX_IP_CKSUM_MASK \
    ((UINT64_C(1) << TARPC_RTE_MBUF_F_RX_IP_CKSUM_BIT0) | \
     (UINT64_C(1) << TARPC_RTE_MBUF_F_RX_IP_CKSUM_BIT1))

#define TARPC_RTE_MBUF_F_RX_IP_CKSUM_UNKNOWN 0

#define TARPC_RTE_MBUF_F_RX_IP_CKSUM_BAD \
    (UINT64_C(1) << TARPC_RTE_MBUF_F_RX_IP_CKSUM_BIT0)

#define TARPC_RTE_MBUF_F_RX_IP_CKSUM_GOOD \
    (UINT64_C(1) << TARPC_RTE_MBUF_F_RX_IP_CKSUM_BIT1)

#define TARPC_RTE_MBUF_F_RX_IP_CKSUM_NONE \
    ((UINT64_C(1) << TARPC_RTE_MBUF_F_RX_IP_CKSUM_BIT0) | \
     (UINT64_C(1) << TARPC_RTE_MBUF_F_RX_IP_CKSUM_BIT1))

#define TARPC_RTE_MBUF_F_RX_L4_CKSUM_MASK \
    ((UINT64_C(1) << TARPC_RTE_MBUF_F_RX_L4_CKSUM_BIT0) | \
     (UINT64_C(1) << TARPC_RTE_MBUF_F_RX_L4_CKSUM_BIT1))

#define TARPC_RTE_MBUF_F_RX_L4_CKSUM_UNKNOWN 0

#define TARPC_RTE_MBUF_F_RX_L4_CKSUM_BAD \
    (UINT64_C(1) << TARPC_RTE_MBUF_F_RX_L4_CKSUM_BIT0)

#define TARPC_RTE_MBUF_F_RX_L4_CKSUM_GOOD \
    (UINT64_C(1) << TARPC_RTE_MBUF_F_RX_L4_CKSUM_BIT1)

#define TARPC_RTE_MBUF_F_RX_L4_CKSUM_NONE \
    ((UINT64_C(1) << TARPC_RTE_MBUF_F_RX_L4_CKSUM_BIT0) | \
     (UINT64_C(1) << TARPC_RTE_MBUF_F_RX_L4_CKSUM_BIT1))

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __RPC_DPDK_DEFS_H__ */
