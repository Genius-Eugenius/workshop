# SPDX-License-Identifier: Apache-2.0

headers += files(
    'ta_job.h',
)

sources += files(
    'ta_job.c',
)

te_libs += [
    'agentlib',
    'rpc_types',
    'tools',
]

dep_pcre = dependency('libpcre', required: false)
required_deps += 'libpcre'
if not dep_pcre.found()
    missed_deps += 'libpcre'
endif

deps += [
    dep_pcre,
]
