#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.59])
AC_INIT(artemis, 1.0.0, [artemis-dev@cns.s.u-tokyo.ac.jp])
AM_INIT_AUTOMAKE([foreign])
AC_CONFIG_SRCDIR([sources/main/main.cc])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_ARG_ENABLE([mpi],
	AC_HELP_STRING([--enable-mpi],[Use MPI (default is no)]),
	[],[enable_mpi=no])

if test "$enable_mpi" != "no" ; then
echo TEST MPI
CXX=mpiicpc
AC_PROG_CXX([mpiicpc])
AC_CHECK_HEADERS([mpi.h])
AC_DEFINE([USE_MPI], [1], [mpi support])
else
AC_PROG_CXX([clang++ g++ g++34])
AC_PROG_CC([clang gcc gcc34])
AC_PROG_LIBTOOL
fi

AC_CONFIG_MACRO_DIR([m4])

# Checks for libraries.
if test "x$ROOTSYS" = "x" ; then
AC_MSG_ERROR(ROOTSYS should be set)
fi

# check for yaml
AC_ARG_WITH([yaml-cpp],
	AC_HELP_STRING([--with-yaml-cpp@<:@=PATH@:>@],
		       [PATH is path to yaml-cpp home (without lib/include)]),
        [WITH_YAML_CPP=$withval],[WITH_YAML_CPP=yes])


if test "$WITH_YAML_CPP" = "yes" ; then
echo setting
AC_CHECK_LIB(yaml-cpp,main,[AC_DEFINE([HAVE_YAML_CPP],[1],[Define to 1 if you have the yaml-cpp library])],[AC_MSG_ERROR(*** yaml-cpp package is not installed or path is not correct ***)])
AC_SUBST([YAML_CPP_INCLUDE])
AC_SUBST([YAML_CPP_LIB],[-lyaml-cpp])
else
 echo
AC_DEFINE([HAVE_YAML_CPP],[1],[Define to 1 if you have the yaml-cpp library])
AC_SUBST([YAML_CPP_INCLUDE_DIR],["$WITH_YAML_CPP/include"])
AC_SUBST([YAML_CPP_INCLUDE],["-I$WITH_YAML_CPP/include"])
AC_SUBST([YAML_CPP_LIB],["-L$WITH_YAML_CPP/lib -lyaml-cpp"])
fi

ROOTLIBS="`$ROOTSYS/bin/root-config --glibs`"
AC_SUBST(ROOTLIBS)

# Checks for header files.
AC_CHECK_HEADERS([termios.h unistd.h])


AC_PATH_XTRA

if test "`uname -s`" = "Darwin" ; then
AC_SUBST(SOEXT,[dylib])
else
AC_SUBST(SOEXT,[so])
fi

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.
AC_HEADER_STDC
AC_FUNC_STAT

AC_CONFIG_FILES([Makefile
                 sources/Makefile
                 sources/commands/Makefile
                 sources/core/Makefile
                 sources/cont/Makefile
                 sources/loop/Makefile
                 sources/loop/decoder/Makefile
                 sources/loop/ridf/Makefile
                 sources/loop/rdf/Makefile
                 sources/loop/hist/Makefile
                 sources/loop/param/Makefile
                 sources/main/Makefile
                 sources/main/artemis-config
                 sources/mc/Makefile])
AC_OUTPUT
