/*
  +---------------------------------------------------------------------+
  | Author: KisSCoOl <kisscool@kisscool.net>                            |
  +---------------------------------------------------------------------+

*/

#ifndef PHP_QUVI_H
#define PHP_QUVI_H 1

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"

#include <quvi/quvi.h>

#define PHP_QUVI_NAME       "quvi"
#define PHP_QUVI_VERSION    "0.2"

static void php_quvi_version(char *buf);
static void php_quvi_query_formats(char *url, char *buf);

PHP_MINIT_FUNCTION(quvi);
PHP_MSHUTDOWN_FUNCTION(quvi);
PHP_MINFO_FUNCTION(quvi);

PHP_FUNCTION(quvi);
PHP_FUNCTION(quvi_formats);
PHP_FUNCTION(quvi_support);
PHP_FUNCTION(quvi_version);

extern zend_module_entry quvi_module_entry;
#define phpext_quvi_ptr &quvi_module_entry

#endif /* PHP_QUVI_H */
