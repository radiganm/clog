### Makefile.am
### Copyright 2017 Mac Radigan
### All Rights Reserved

AM_LDFLAGS  = -pthread                                                     \
              -L/usr/lib -ldl -lm                                          \
              -L/usr/lib/x86_64-linux-gnu                                  \
              -lstdc++fs                                                    

AM_CPPFLAGS = -I$(top_srcdir)/include                                      \
              -std=c++1z                                                   \
              -O3
AM_FCFLAGS  = -Jmod

clog_ldadd             = libclog.la                                        \
                         -ldl -lm                                          \
                         -lrt                                              \
                         -lstdc++fs                                        \
                         -lboost_system                                    \
                         -lboost_thread                                    \
                         -lboost_program_options                            

bin_PROGRAMS           = test_clog

test_clog_SOURCES      = src/tests/test_clog.cc
test_clog_LDADD        = $(clog_ldadd)

lib_LTLIBRARIES        = libclog.la                                         

libclog_la_SOURCES     = src/packages/common/util.cc                       \
                         src/packages/log/Syslog.cc                        \
                         src/packages/log/Logger.cc                         
libclog_la_LIBADD      = -L/usr/lib/x86_64-linux-gnu -lrt
libclog_la_LDFLAGS     = -export-symbols-regex '^clog_[^_]' -fPIC          \
                         -shared -pthread                                  \
                         -L/usr/lib/x86_64-linux-gnu -lrt                   

pkginclude_HEADERS     = include/packages/common/util.h                    \
                         include/packages/log/Syslog.h                     \
                         include/packages/log/Logger.h                      

### *EOF*
