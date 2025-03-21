/* SPDX-License-Identifier: Apache-2.0 */
/** @file
 * @brief wrk tool TAPI
 *
 * @defgroup tapi_wrk tool functions TAPI
 * @ingroup te_ts_tapi
 * @{
 *
 * TAPI to handle wrk tool.
 *
 */

#ifndef __TE_TAPI_WRK_H__
#define __TE_TAPI_WRK_H__

#include "te_errno.h"
#include "te_string.h"
#include "tapi_job.h"
#include "tapi_job_opt.h"
#include "te_mi_log.h"

#ifdef __cplusplus
extern "C" {
#endif

/** Number of entries in wrk latency distibution statistics */
#define TAPI_WRK_LATENCY_DISTR_ENTRIES 4

/** Maximum number of headers that can be set in wrk options */
#define TAPI_WRK_HEADERS_MAX 10

/** Maximum number of arguments that can be passed to the Lua script */
#define TAPI_WRK_SCRIPT_ARGS_MAX 16

/** wrk tool specific command line options */
typedef struct tapi_wrk_opt {
    /** Standard output logging level (default is @c TE_LL_RING). */
    te_log_level stdout_log_level;
    /** Standard error logging level (default is @c TE_LL_ERROR). */
    te_log_level stderr_log_level;
    /** Number of connections to keep open */
    unsigned int connections;
    /** Number of threads to use */
    unsigned int n_threads;
    /** Duration of test in seconds */
    unsigned int duration_s;
    /** Socket/request timeout in milliseconds */
    unsigned int timeout_ms;
    /**
     * Load Lua script file.
     * The file is created in TA only if script content is not NULL.
     */
    const char *script_content;
    /**
     * The file path is auto-generated only if
     * the path is @c NULL AND script_content is not @c NULL.
     */
    const char *script_path;

    /** Number of actual arguments in @a script_args. */
    size_t n_script_args;
    /** Script arguments. */
    const char *script_args[TAPI_WRK_SCRIPT_ARGS_MAX];

    /** Number of actual headers in @a headers */
    size_t n_headers;
    /** Request headers in "Name: Value" format (note the space). */
    const char *headers[TAPI_WRK_HEADERS_MAX];
    /** Print latency statistics */
    bool latency;
    /** Host to connect to */
    const char *host;

    /** wrk2: work rate (throughput) in requests/sec (total). */
    tapi_job_opt_uint_t rate;
    /** wrk2/patched: affinity list (for example 0,2,4,6). */
    const char *affinity;
} tapi_wrk_opt;

/** Default options initializer */
extern const tapi_wrk_opt tapi_wrk_default_opt;

/** Statistics for a single thread in wrk */
typedef struct tapi_wrk_thread_stats {
    /** Mean value (microseconds) */
    double mean;
    /** Standard deviation (microseconds) */
    double stdev;
    /** Max value (microseconds) */
    double max;
    /** Percentage of values within [mean - stdev ; mean + stdev] range */
    double within_stdev;
} tapi_wrk_thread_stats;

/** Entry of latency distribution statistics */
typedef struct tapi_wrk_latency_percentile {
    /** Percentile */
    double percentile;
    /** Latency value (microseconds) */
    double latency;
} tapi_wrk_latency_percentile;

/** Statistics of socket errors */
typedef struct tapi_wrk_socket_errors {
    /** Amount of connect socket errors */
    unsigned int connect;
    /** Amount of read socket errors */
    unsigned int read;
    /** Amount of write socket errors */
    unsigned int write;
    /** Amount of timeout socket errors */
    unsigned int timeout;
} tapi_wrk_socket_errors;

/** Statistics report of wrk */
typedef struct tapi_wrk_report {
    /** Latency in microseconds (for each thread) */
    tapi_wrk_thread_stats thread_latency;
    /** Requests per second (for each thread) */
    tapi_wrk_thread_stats thread_req_per_sec;
    /** Latency distribution */
    tapi_wrk_latency_percentile lat_distr[TAPI_WRK_LATENCY_DISTR_ENTRIES];
    /** Total requests count */
    unsigned int req_count;
    /** Requests per second */
    double req_per_sec;
    /** Bytes per second */
    double bps;
    /** Non-2xx or 3xx responses */
    unsigned int unexpected_resp;
    /** Socket errors */
    tapi_wrk_socket_errors socket_errors;
    /** Wrk arguments used */
    char *arguments;
    /** Truncated wrk arguments */
    char *truncated_arguments;
} tapi_wrk_report;

/** Information of a wrk tool */
typedef struct tapi_wrk_app {
    /** TAPI job handle */
    tapi_job_t *job;
    /** Output channel handles */
    tapi_job_channel_t *out_chs[2];
    /** wrk stdout content. */
    tapi_job_channel_t *std_out;
    /** wrk stderr content. */
    tapi_job_channel_t *std_err;
    /** Bytes per second filter */
    tapi_job_channel_t *bps_filter;
    /** Requests count filter (total) */
    tapi_job_channel_t *req_count_filter;
    /** Total requests filter (per second) */
    tapi_job_channel_t *req_total_filter;
    /** Latency per thread filter */
    tapi_job_channel_t *lat_filter;
    /** Requests per thread filter */
    tapi_job_channel_t *req_filter;
    /** Latency distribution filter */
    tapi_job_channel_t *lat_distr_filter;
    /** Non-2xx or 3xx responses filter */
    tapi_job_channel_t *unexpected_resp_filter;
    /** Socket errors filter */
    tapi_job_channel_t *socket_errors_filter;
    /** Arguents that are used when running the tool */
    te_vec wrk_args;
} tapi_wrk_app;

/**
 * Create wrk app. All needed information to run wrk is in @p opt
 *
 * @param[in]  factory      Job factory.
 * @param[in]  opt          Options of wrk tool.
 * @param[out] app          wrk app handle.
 *
 * @return Status code.
 */
extern te_errno tapi_wrk_create(tapi_job_factory_t *factory,
                                const tapi_wrk_opt *opt,
                                tapi_wrk_app **app);

/**
 * Start wrk.
 *
 * @param[in]  app          wrk app handle.
 *
 * @return Status code.
 */
extern te_errno tapi_wrk_start(tapi_wrk_app *app);

/**
 * Wait for wrk completion.
 *
 * @param[in]  app          wrk app handle.
 * @param[in]  timeout_ms   Wait timeout in milliseconds.
 *
 * @return Status code.
 */
extern te_errno tapi_wrk_wait(tapi_wrk_app *app, int timeout_ms);

/**
 * Get wrk report.
 *
 * @note    Field 'arguments' in the @p report points to a new allocated string
 *          that should be freed by user.
 *
 * @param[in]  app          wrk app handle.
 * @param[out] report       wrk statistics report.
 *
 * @return Status code.
 */
extern te_errno tapi_wrk_get_report(tapi_wrk_app *app, tapi_wrk_report *report);

/**
 * Send a signal to wrk.
 *
 * @param[in]  app          wrk app handle.
 * @param[in]  signo        Signal to send to wrk.
 *
 * @return Status code.
 */
extern te_errno tapi_wrk_kill(tapi_wrk_app *app, int signo);

/**
 * Destroy wrk app.
 *
 * @param[in]  app          wrk app handle.
 *
 * @return Status code.
 */
extern te_errno tapi_wrk_destroy(tapi_wrk_app *app);

/**
 * Add wrk report to MI logger.
 *
 * @param      logger       MI logger entity
 * @param[in]  report       wrk statistics report.
 */
extern void tapi_wrk_report_mi_log(te_mi_logger *logger,
                                   const tapi_wrk_report *report);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __TE_TAPI_WRK_H__ */

/**@} <!-- END tapi_wrk --> */
