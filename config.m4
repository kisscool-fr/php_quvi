PHP_ARG_WITH(quvi, for quvi support,
    [  --with-quvi=[DIR]       Include quvi Support]
)

if test "$PHP_QUVI" != "no"; then
    SEARCH_PATH="/usr/local /usr"
    SEARCH_FOR="/include/quvi/quvi.h"
    if test -r $PHP_QUVI/$SEARCH_FOR ; then
        QUVI_DIR=$PHP_QUVI
    else
        AC_MSG_CHECKING([for quvi in default path])
        for i in $SEARCH_PATH; do
            if test -f $i/$SEARCH_FOR; then
                QUVI_DIR=$i
                AC_MSG_RESULT(found in $QUVI_DIR)
            fi
        done
    fi

    if test -z "$QUVI_DIR"; then
        AC_MSG_RESULT([not found])
        AC_MSG_ERROR([The required libquvi was not found. You can obtain the package from http://quvi.sourceforge.net/])
    fi

    PHP_ADD_INCLUDE($QUVI_DIR/include)

    LIBNAME=quvi
    LIBSYMBOL=quvi_version

    PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
    [
        PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $QUVI_DIR/lib, QUVI_SHARED_LIBADD)
        AC_DEFINE(HAVE_LIBQUVI, 1, [Have libquvi])
    ],[
        AC_MSG_ERROR([libquvi >= 0.2.16 not found])
    ],[
        -L$QUVI_DIR/lib -lm -ldl
    ])

    PHP_SUBST(QUVI_SHARED_LIBADD)

    PHP_NEW_EXTENSION(quvi, quvi.c, $ext_shared)
fi
