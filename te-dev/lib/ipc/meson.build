# SPDX-License-Identifier: Apache-2.0

server_sources = [
    'ipc_common.c',
    'portmap_common.c',
    'portmap_server.c',
    'server.c',
]
te_lib_ipcserver = library('ipcserver', server_sources, install: install_lib,
                           include_directories: includes)
dep_lib_ipcserver = declare_dependency(link_with: te_lib_ipcserver,
                                      include_directories: includes)

headers += files(
    'ipc_client.h',
    'ipc_server.h',
)
sources += files(
    'client.c',
    'ipc_common.c',
    'portmap_common.c',
)
