/* SPDX-License-Identifier: Apache-2.0 */
/** @file
 * @brief PCI Configuration Model TAPI
 *
 * Implementation of test API for network configuration model
 * (doc/cm/cm_ovs.yml).
 *
 */

#define TE_LGR_USER      "Configuration TAPI"

#include "te_config.h"

#include "te_defs.h"
#include "te_errno.h"
#include "te_alloc.h"
#include "te_str.h"
#include "rcf_api.h"
#include "logger_api.h"
#include "conf_api.h"

#include "tapi_cfg.h"
#include "tapi_cfg_base.h"
#include "tapi_cfg_ovs.h"

const char *const tapi_cfg_ovs_cfg_name[] = {
    [TAPI_CFG_OVS_CFG_DPDK_ALLOC_MEM] = TAPI_OVS_OTHER_CFG ":dpdk-alloc-mem",
    [TAPI_CFG_OVS_CFG_DPDK_SOCKET_MEM] = TAPI_OVS_OTHER_CFG ":dpdk-socket-mem",
    [TAPI_CFG_OVS_CFG_DPDK_LCORE_MASK] = TAPI_OVS_OTHER_CFG ":dpdk-lcore-mask",
    [TAPI_CFG_OVS_CFG_DPDK_HUGEPAGE_DIR] =
        TAPI_OVS_OTHER_CFG ":dpdk-hugepage-dir",
    [TAPI_CFG_OVS_CFG_DPDK_SOCKET_LIMIT] =
        TAPI_OVS_OTHER_CFG ":dpdk-socket-limit",
    [TAPI_CFG_OVS_CFG_DPDK_EXTRA] = TAPI_OVS_OTHER_CFG ":dpdk-extra",
};

te_errno
tapi_cfg_ovs_convert_eal_args(int argc, const char *const *argv,
                              tapi_cfg_ovs_cfg *ovs_cfg)
{
    struct ovs_cfg_map_t {
        const char *eal_arg;
        tapi_cfg_ovs_cfg_type type;
    } cfg_map[] = {
        {"-m", TAPI_CFG_OVS_CFG_DPDK_ALLOC_MEM},
        {"--socket-mem", TAPI_CFG_OVS_CFG_DPDK_SOCKET_MEM},
        {"-c", TAPI_CFG_OVS_CFG_DPDK_LCORE_MASK},
        {"--huge-dir", TAPI_CFG_OVS_CFG_DPDK_HUGEPAGE_DIR},
        {"--socket-limit", TAPI_CFG_OVS_CFG_DPDK_SOCKET_LIMIT},
        {NULL, 0},
    };
    te_string dpdk_extra = TE_STRING_INIT;
    unsigned int j;
    int i;
    te_errno rc = 0;
    int ret;

    memset(ovs_cfg, 0, sizeof(*ovs_cfg));

    for (i = 0; i < argc; i++)
    {
        for (j = 0; j < TE_ARRAY_LEN(cfg_map); j++)
        {
            if (cfg_map[j].eal_arg == NULL)
            {
                rc = te_string_append_shell_args_as_is(&dpdk_extra, argv[i],
                                                       NULL);
            }
            else if (strcmp(argv[i], cfg_map[j].eal_arg) == 0)
            {
                if (i + 1 >= argc)
                {
                    ERROR("EAL argument '%s' with no value", argv[i]);
                    rc = TE_EINVAL;
                }
                else if (ovs_cfg->values[cfg_map[j].type] != NULL)
                {
                    ERROR("Duplicated EAL argument '%s'", argv[i]);
                    rc = TE_EINVAL;
                }
                else
                {
                    ret = te_asprintf(&ovs_cfg->values[cfg_map[j].type], "%s",
                                      argv[++i]);
                    if (ret < 0)
                        rc = TE_EFAIL;
                    else
                        break;
                }
            }
            if (rc != 0)
            {
               ERROR("Failed to construct DPDK EAL args");
               goto out;
            }
        }
    }

    if (dpdk_extra.ptr != NULL)
    {
        ret = te_asprintf(&ovs_cfg->values[TAPI_CFG_OVS_CFG_DPDK_EXTRA], "%s",
                          dpdk_extra.ptr);
        rc = (ret < 0) ? TE_EFAIL : 0;
        if (rc != 0)
        {
            ERROR("Failed to construct extra DPDK EAL args");
            goto out;
        }
    }

out:
    te_string_free(&dpdk_extra);
    if (rc != 0)
    {
        for (j = 0; j < TE_ARRAY_LEN(ovs_cfg->values); j++)
            free(ovs_cfg->values[i]);

        memset(ovs_cfg, 0, sizeof(*ovs_cfg));
    }

    return TE_RC(TE_TAPI, rc);
}

te_errno
tapi_cfg_ovs_clear_rules(const char *ta, const char *bridge)
{
    te_errno      rc;
    char         *bridge_name = NULL;
    unsigned int  i;
    unsigned int  n_rules;
    cfg_handle   *rules;

    if (bridge == NULL)
    {
        rc = tapi_cfg_ovs_default_bridge(ta, &bridge_name);
        if (rc != 0)
        {
            ERROR("Failed to find default bridge on TA %s: %r", ta, rc);
            return rc;
        }
        bridge = bridge_name;
    }

    rc = cfg_find_pattern_fmt(&n_rules, &rules,
                              "/agent:%s/ovs:/bridge:%s/flow:*", ta, bridge);
    free(bridge_name);
    if (rc != 0)
    {
        ERROR("Failed to find flow rules on TA %s OvS bridge %s: %r",
              ta, bridge, rc);
        return rc;
    }

    for (i = 0; i < n_rules; i++)
    {
        rc = cfg_del_instance(rules[i], FALSE);
        if (rc != 0)
        {
            ERROR("Failed to remove flow rule with handle 0x%x: %r",
                  rules[i], rc);
            break;
        }
    }

    free(rules);
    return rc;
}


static uint64_t
allocate_ovs_flow_cookie(const char *ta, const char *bridge)
{
    te_errno rc;
    uint64_t next;

    rc = cfg_get_uint64(&next, "/agent:%s/ovs:/bridge:%s/next_flow_cookie:",
                        ta, bridge);
    if (rc != 0)
    {
        TE_FATAL_ERROR("Failed to get next flow cookie for TA %s bridge %s: %r",
                       ta, bridge, rc);
    }

    rc = cfg_set_instance_fmt(CFG_VAL(UINT64, next + 1),
                              "/agent:%s/ovs:/bridge:%s/next_flow_cookie:",
                              ta, bridge);
    if (rc != 0)
    {
        TE_FATAL_ERROR("Failed to set next flow cookie for TA %s bridge %s: %r",
                       ta, bridge, rc);
    }

    return next;
}

te_errno
tapi_cfg_ovs_add_rule(const char *ta, const char *bridge, uint64_t *cookie,
                      const char *fmt, ...)
{
    te_errno                rc;
    char                   *bridge_name = NULL;
    va_list                 va;
    char                    user_rule[RCF_MAX_VAL];
    uint64_t                new_cookie;

    if (bridge == NULL)
    {
        rc = tapi_cfg_ovs_default_bridge(ta, &bridge_name);
        if (rc != 0)
        {
            ERROR("Failed to find default bridge on TA %s: %r", ta, rc);
            return rc;
        }
        bridge = bridge_name;
    }

    va_start(va, fmt);
    rc = te_vsnprintf(user_rule, sizeof(user_rule), fmt, va);
    va_end(va);
    if (rc != 0)
    {
        ERROR("Failed to format user OvS flow rule: %r", rc);
        free(bridge_name);
        return rc;
    }

    rc = cfg_get_uint64(&new_cookie, "/agent:%s/ovs:/bridge:%s/next_flow_cookie:",
                        ta, bridge);
    if (rc != 0)
    {
        TE_FATAL_ERROR("Failed to get next flow cookie for TA %s bridge %s: %r",
                       ta, bridge, rc);
    }

    new_cookie = allocate_ovs_flow_cookie(ta, bridge);

    rc = cfg_add_instance_fmt(NULL, CVT_STRING, user_rule,
                              "/agent:%s/ovs:/bridge:%s/flow:0x%"PRIx64,
                              ta, bridge, new_cookie);
    free(bridge_name);
    if (rc != 0)
    {
        ERROR("Failed to add OvS flow rule with cookie 0x%"PRIx64" %r",
              new_cookie, rc);
        return rc;
    }

    if (cookie != NULL)
        *cookie = new_cookie;

    return 0;
}

te_errno
tapi_cfg_ovs_flow_rule_get(const char *ta, const char *bridge, uint64_t cookie,
                           ovs_flow_rule *rule)
{
    te_errno  rc;
    char     *rule_str;

    rc = cfg_get_string(&rule_str, "/agent:%s/ovs:/bridge:%s/flow:0x%"PRIx64,
                        ta, bridge, cookie);
    if (rc != 0)
    {
        ERROR("Failed to get OvS flow rule: %r", rc);
        return rc;
    }

    rc = ovs_flow_rule_parse(rule_str, rule);
    free(rule_str);
    if (rc != 0)
    {
        ERROR("Failed to parse OvS flow rule: %r", rc);
        return rc;
    }

    return 0;
}

te_errno
tapi_cfg_ovs_del_rule(const char *ta, const char *bridge, uint64_t cookie)
{
    te_errno  rc;
    char     *bridge_name = NULL;

    if (bridge == NULL)
    {
        rc = tapi_cfg_ovs_default_bridge(ta, &bridge_name);
        if (rc != 0)
        {
            ERROR("Failed to find default bridge on TA %s: %r", ta, rc);
            return rc;
        }
        bridge = bridge_name;
    }

    rc = cfg_del_instance_fmt(FALSE, "/agent:%s/ovs:/bridge:%s/flow:0x%"PRIx64,
                              ta, bridge, cookie);
    free(bridge_name);

    return rc;
}

te_errno
tapi_cfg_ovs_default_bridge(const char *ta, char **bridge_name)
{
    te_errno      rc;
    cfg_handle   *bridges;
    unsigned int  n_bridges;

    rc = cfg_find_pattern_fmt(&n_bridges, &bridges,
                              "/agent:%s/ovs:/bridge:*", ta);
    if (rc != 0)
    {
        ERROR("Failed to find OvS bridges on TA %s: %r", ta, rc);
        return rc;
    }

    if (n_bridges == 0)
    {
        ERROR("TA %s does not have an OvS bridge", ta);
        return TE_RC(TE_TAPI, TE_ENOENT);
    }
    else if (n_bridges > 1)
    {
        ERROR("TA %s has too many OvS bridges (%u)", ta, n_bridges);
        free(bridges);
        return TE_RC(TE_TAPI, TE_ETOOMANY);
    }

    rc = cfg_get_inst_name(bridges[0], bridge_name);
    free(bridges);
    if (rc != 0)
    {
        ERROR("Failed to extract the name of OvS bridge with handle 0x%x: %r",
              bridges[0], rc);
        return rc;
    }

    return 0;
}
