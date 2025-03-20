# SPDX-License-Identifier: Apache-2.0

headers += files('tapi_env.h')
sources += files('tapi_env.c')
sources += lex_gen.process('env_lex.l')
sources += gram_gen.process('env_gram.y')
te_libs += [
    'tapi',
    'tapi_rpc',
]
