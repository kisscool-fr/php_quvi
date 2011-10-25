/*
  +---------------------------------------------------------------------+
  | Author: KisSCoOl <kisscool@kisscool.net>                            |
  +---------------------------------------------------------------------+

*/

#ifndef PHP_QUVI_H
#define PHP_QUVI_H 1

#include <quvi/quvi.h>

#define PHP_QUVI_NAME       "quvi"
#define PHP_QUVI_VERSION    "0.1"


extern zend_module_entry quvi_module_entry;
#define phpext_quvi_ptr &quvi_module_entry

#endif /* PHP_QUVI_H */
