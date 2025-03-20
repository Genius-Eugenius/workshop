# SPDX-License-Identifier: Apache-2.0

headers += files(
    'tapi_performance.h',
)
sources += files(
    'iperf.c',
    'iperf3.c',
    'performance_internal.c',
    'tapi_performance.c',
)
te_libs += [
    'tapi',
    'tapi_rpc',
    'tapi_job',
]

dep_jansson = dependency('jansson', required: false)
required_deps += 'jansson'
if not dep_jansson.found()
    missed_deps += 'jansson'
endif

deps += [
    dep_jansson,
]
