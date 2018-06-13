## require at least autoconf 2.62
AC_PREREQ(2.62)

## Process this file with autoconf to produce a configure script.
AC_INIT(Rzdd, m4_esyscmd_s([awk -e '/^Version:/ {print $2}' DESCRIPTION]))

## Set R_HOME, respecting an environment variable if one is set 
R_HOME=$(R RHOME)
if test -z "${R_HOME}"; then
  AC_MSG_ERROR([Could not determine R_HOME.])   
fi

## Use R to set CXX and CXXFLAGS
CXX=$(${R_HOME}/bin/R CMD config CXX)
CXXFLAGS=$("${R_HOME}/bin/R" CMD config CXXFLAGS)

AC_LANG(C++)
AC_REQUIRE_CPP
AC_PROG_CXX
AC_OPENMP


AC_SUBST([OPENMP_FLAG], ["${OPENMP_CXXFLAGS}"])
AC_CONFIG_FILES([src/Makevars])
AC_OUTPUT
