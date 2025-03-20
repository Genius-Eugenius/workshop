# SPDX-License-Identifier: Apache-2.0

sources += files(
    'netconf.c',
)

te_libs += [
    'rpcserver',
]

dep_libnetconf = dependency('libnetconf2', required: true)
dep_libyang = dependency('libyang', required: true)
deps += [ dep_libnetconf, dep_libyang ]
