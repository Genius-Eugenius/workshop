/* SPDX-License-Identifier: Apache-2.0 */
/** @file
 * @brief TAPI TAD PPP
 *
 * @defgroup tapi_tad_ppp PPP
 * @ingroup tapi_tad_main
 * @{
 *
 */

#ifndef __TE_TAPI_PPP_H__
#define __TE_TAPI_PPP_H__


#if HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#if HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif

#include "te_stdint.h"
#include "te_defs.h"
#include "tad_common.h"
#include "asn_usr.h"
#include "tapi_tad.h"

#ifdef __cplusplus
extern "C" {
#endif


/**
 * Add PPP layer in CSAP specification.
 *
 * @param csap_spec     Location of CSAP specification pointer.
 *
 * @retval Status code.
 */
extern te_errno tapi_ppp_add_csap_layer(asn_value **csap_spec,
                                        uint16_t    protocol);

/**
 * Add PPP PDU as the last PDU to the last unit of the traffic
 * template or pattern.
 *
 * @param tmpl_or_ptrn  Location of ASN.1 value with traffic template or
 *                      pattern
 * @param pdu           Location for ASN.1 value pointer with added PDU
 * @param is_pattern    Is the first argument template or pattern
 * @param group_addr    Protocol.
 *
 * @return              Status code.
 */
extern te_errno tapi_ppp_add_pdu(asn_value          **tmpl_or_ptrn,
                                 asn_value          **pdu,
                                 bool is_pattern,
                                 uint16_t             protocol);


#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* !__TE_TAPI_PPP_H__ */

/**@} <!-- END tapi_tad_ppp --> */
