AC_PREREQ([2.69])
AC_INIT([C_Chess], [0.1], [tonemail@exemple.com])
AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])
AC_PROG_CC

# Vérifie que PKG_CHECK_MODULES est disponible
m4_pattern_forbid([^PKG_CHECK_MODULES])dnl

# Vérifie SDL2
PKG_CHECK_MODULES([SDL2], [sdl2 >= 2.0.22])

# Génère le Makefile
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
