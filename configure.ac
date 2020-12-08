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
 progxx="[mpic++ mpiicpc clang++ g++ g++34]"
 progcc="[mpicc mpiicc clang gcc gcc34]"
 else
 echo non-MPI 
 progxx="[clang++ g++ g++34]"
 progcc="[clang gcc gcc34]"
fi

AC_PROG_CXX($progxx)
AC_PROG_CC($progcc)
AC_CHECK_HEADERS([mpi.h],AS_IF([test "$enable_mpi" != "no"],[AC_DEFINE([USE_MPI], [1], [mpi support])]))




AC_PROG_LIBTOOL

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
ROOTVER="`$ROOTSYS/bin/root-config --version`"
ROOTVER="${ROOTVER:0:1}"
AC_SUBST(ROOTLIBS)
AC_SUBST(ROOTVER)
AM_CONDITIONAL(HAVE_ROOT_PCM, test $ROOTVER -ge 6)



# check for get decoder
AC_ARG_WITH([get-decoder],
	AC_HELP_STRING([--with-get-decoder@<:@=PATH@:>@],
		       [PATH is path to get decoder home (without lib/include)]),
        [WITH_GET_DECODER=$withval],[WITH_GET_DECODER=no])
if test "$WITH_GET_DECODER" = "no"; then
# do nothing
echo HAVE_GET = 0
AC_SUBST([HAVE_GET],["0"])
AC_SUBST([LOOP_SUB_DIR],["param decoder ridf rdf hist"])
AC_SUBST([GET_DECODER_LIB],[""])
AC_SUBST([ARTGET_LIB],[""])
else
echo HAVE_GET = 1
AC_DEFINE([HAVE_GET_DECODER],[1],[Define to 1 if you have the get decoder])
AC_SUBST([HAVE_GET],["1"])
AC_SUBST([LOOP_SUB_DIR],["param decoder ridf rdf get hist"])
AC_SUBST([GET_DECODER_INCLUDE_DIR],["$WITH_GET_DECODER/include"])
AC_SUBST([GET_DECODER_INCLUDE],["-I$WITH_GET_DECODER/include"])
AC_SUBST([GET_DECODER_LIB],["-L$WITH_GET_DECODER/lib -lGETDecoder"])
AC_SUBST([ARTGET_LIB],["-L../loop/get/.libs -lartget"])
fi



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
                 sources/loop/get/Makefile
                 sources/loop/hist/Makefile
                 sources/loop/param/Makefile
                 sources/main/Makefile
                 sources/main/artemis-config
                 sources/mc/Makefile])
AC_OUTPUT
