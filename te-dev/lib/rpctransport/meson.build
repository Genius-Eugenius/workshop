# SPDX-License-Identifier: Apache-2.0

if get_option('rpc-transport-local')
    c_args += [ '-DENABLE_LOCAL_TRANSPORT' ]
endif
if get_option('rpc-transport-tcp')
    c_args += [ '-DENABLE_TCP_TRANSPORT' ]
endif
headers += files('rpc_transport.h')
sources += files('posix.c', 'win32.c')
