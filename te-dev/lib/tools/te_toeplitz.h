/* SPDX-License-Identifier: Apache-2.0 */
/** @file
 * @brief API to calculate hash used by RSS
 *
 * @defgroup te_tools_te_toeplitz Toeplitz hash
 * @ingroup te_tools
 * @{
 *
 * Definition of API to calculate hash used by RSS
 *
 */

#ifndef __TE_TOOLS_TOEPLITZ_H__
#define __TE_TOOLS_TOEPLITZ_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "te_config.h"
#include "te_defs.h"
#include "te_stdint.h"
#include "te_sockaddr.h"

#if HAVE_STDLIB_H
#include <stdlib.h>
#endif


typedef struct te_toeplitz_hash_cache te_toeplitz_hash_cache;


/**
 * Calculate a Toeplitz hash of input data
 *
 * @param toeplitz_hash_cache Pre-constructed cache
 * @param input               Pointer to input data
 * @param pos                 Position of input data
 * @param datalen             Length of input data in bytes
 *
 * @retval RSS hash value of input data
 */
extern uint32_t te_toeplitz_hash_data(
    const te_toeplitz_hash_cache *toeplitz_hash_cache,
    const uint8_t *input, unsigned int pos, unsigned int datalen);


/**
 * Calculate a RSS hash using pre-calculated cache
 *
 * @param toeplitz_hash_cache Pre-constructed cache
 * @param addr_size           IPv4 / IPv6 address length
 * @param src_addr            Pointer to source address
 * @param src_port            Source port number in network byte order
 * @param dst_addr            Pointer to destination address
 * @param dst_port            Desination port number in network byte order
 *
 * @retval RSS hash value
 */
extern uint32_t te_toeplitz_hash(
    const te_toeplitz_hash_cache *toeplitz_hash_cache,
    unsigned int addr_size, const uint8_t *src_addr, uint16_t src_port,
    const uint8_t *dst_addr, uint16_t dst_port);

/** Known variants of Toeplitz hash algorithm */
typedef enum te_toeplitz_hash_variant {
    TE_TOEPLITZ_HASH_STANDARD,   /**< Standard implementation */
    TE_TOEPLITZ_HASH_SYM_OR_XOR, /**< Symmetric hash using OR and XOR */
} te_toeplitz_hash_variant;

/**
 * Calculate a RSS hash for address/port pairs using pre-calculated cache.
 *
 * @param toeplitz_hash_cache  Pre-constructed cache
 * @param src_addr             Source address/port
 * @param dst_addr             Destination address/port
 * @param hash_var             Hash variant
 * @param hash_out             Where to save computed hash
 *
 * @return Status code
 */
extern te_errno te_toeplitz_hash_sa(
    const te_toeplitz_hash_cache *toeplitz_hash_cache,
    const struct sockaddr *src_addr,
    const struct sockaddr *dst_addr,
    te_toeplitz_hash_variant hash_var,
    uint32_t *hash_out);

/**
 * Calculate a RSS hash using pre-calculated cache and symmetric
 * algorithm with OR and XOR.
 *
 * @param toeplitz_hash_cache  Pre-constructed cache
 * @param addr_size            IPv4 / IPv6 address length
 * @param src_addr             Pointer to source address
 * @param src_port             Source port number in network byte order
 * @param dst_addr             Pointer to destination address
 * @param dst_port             Desination port number in network byte order
 *
 * @return RSS hash value.
 */
extern uint32_t te_toeplitz_hash_sym_or_xor(
    const te_toeplitz_hash_cache *toeplitz_hash_cache,
    unsigned int addr_size, const uint8_t *src_addr, uint16_t src_port,
    const uint8_t *dst_addr, uint16_t dst_port);

/**
 * Free pre-constructed cache
 *
 * @param toeplitz_hash_cache  Pointer to cache
 */
extern void te_toeplitz_hash_fini(
    te_toeplitz_hash_cache *toeplitz_hash_cache);

/**
 * Pre-construct cache to use it for faster calculating of hash
 * used by Toeplitz hash function
 *
 * @param key        40-byte hash key
 *
 * @retval cache or NULL in case of error
 */
extern te_toeplitz_hash_cache *te_toeplitz_cache_init(const uint8_t *key);

/**
 * Pre-construct cache to use it for faster calculation of
 * Toeplitz hash function.
 *
 * @param key        Hash key
 * @param key_size   Length of the key in bytes
 *
 * @retval cache or NULL in case of error
 */
extern te_toeplitz_hash_cache *te_toeplitz_cache_init_size(
                                                    const uint8_t *key,
                                                    size_t key_size);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* !__TE_TOOLS_TOEPLITZ_H__ */
/**@} <!-- END te_tools_te_toeplitz --> */
