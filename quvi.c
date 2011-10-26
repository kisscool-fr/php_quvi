/*
  +---------------------------------------------------------------------+
  | Author: KisSCoOl <kisscool@kisscool.net>                            |
  +---------------------------------------------------------------------+

*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_quvi.h"

/**
 * Extension local function
 */
static void php_quvi_version(char *buf, int buflen)
{
    const char *vers ;
    vers = quvi_version(QUVI_VERSION);
    snprintf(buf, buflen, "%s", vers);
}

PHP_MINFO_FUNCTION(quvi)
{
    char vstr[128];
    php_quvi_version(vstr, sizeof(vstr));

    php_info_print_table_start();

    php_info_print_table_header(2, "quvi support", "enabled");
    php_info_print_table_row(2, "php_quvi version", PHP_QUVI_VERSION);
    php_info_print_table_row(2, "libquvi version", vstr);

    php_info_print_table_end();
}

PHP_FUNCTION(quvi_version)
{
    char vers[128];

    if (ZEND_NUM_ARGS()) {
        WRONG_PARAM_COUNT;
    }

    php_quvi_version(vers, sizeof(vers));
    RETURN_STRING(vers, 1);
}

PHP_FUNCTION(quvi)
{
    char *url, *host, *page_url, *page_title, *id, *media_url, *content_type, *file_suffix, *response_code, *format, *start_time, *thumbnail_url;
    double content_length, duration;
    int url_length;
    zval *link;
    quvi_media_t m;
    QUVIcode rc;
    quvi_t q;

    rc = quvi_init(&q);
    if (rc != QUVI_OK)
    {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "%s", quvi_strerror(q,rc));
        RETURN_NULL();
    }

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &url, &url_length) == FAILURE) {
        RETURN_NULL();
    }

    rc = quvi_parse(q, url, &m);
    if (rc != QUVI_OK)
    {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "%s", quvi_strerror(q,rc));
        quvi_close(&q);
        RETURN_NULL();
    }

    quvi_getprop(m, QUVIPROP_HOSTID, &host);
    quvi_getprop(m, QUVIPROP_PAGEURL, &page_url);
    quvi_getprop(m, QUVIPROP_PAGETITLE, &page_title);
    quvi_getprop(m, QUVIPROP_MEDIAID, &id);
    quvi_getprop(m, QUVIPROP_MEDIAURL, &media_url);
    quvi_getprop(m, QUVIPROP_MEDIACONTENTLENGTH, &content_length);
    quvi_getprop(m, QUVIPROP_MEDIACONTENTTYPE, &content_type);
    quvi_getprop(m, QUVIPROP_FILESUFFIX, &file_suffix);
    /*
    quvi_getprop(m, QUVIPROP_RESPONSECODE, &response_code);
    quvi_getprop(m, QUVIPROP_STARTTIME, &start_time);
    */
    quvi_getprop(m, QUVIPROP_MEDIADURATION, &duration);
    quvi_getprop(m, QUVIPROP_FORMAT, &format);
    quvi_getprop(m, QUVIPROP_MEDIATHUMBNAILURL, &thumbnail_url);

    array_init(return_value);

    add_assoc_string(return_value, "host", host, 1);
    add_assoc_string(return_value, "page_title", page_title, 1);
    add_assoc_string(return_value, "page_url", page_url, 1);
    /*
    add_assoc_long(return_value, "response_code", response_code);
    add_assoc_string(return_value, "start_time", start_time, 1);
    */
    add_assoc_double(return_value, "duration", duration);
    add_assoc_string(return_value, "id", id, 1);
    add_assoc_string(return_value, "format_requested", format, 1);
    add_assoc_string(return_value, "thumbnail_url", thumbnail_url, 1);

    ALLOC_INIT_ZVAL(link);
    array_init(link);
    add_assoc_double(link, "length_bytes", content_length);
    add_assoc_string(link, "content_type", content_type, 1);
    add_assoc_string(link, "file_suffix", file_suffix, 1);
    add_assoc_string(link, "url", media_url, 1);
    add_assoc_zval(return_value, "link", link);

    quvi_parse_close(&m);
    quvi_close(&q);
}

zend_function_entry quvi_functions[] = {
    PHP_FE(quvi,         NULL)
    PHP_FE(quvi_version, NULL)
    {NULL, NULL, NULL}
};

zend_module_entry quvi_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    PHP_QUVI_NAME,
    quvi_functions,
    NULL,            /* MINIT */
    NULL,            /* MSHUTDOWN */
    NULL,            /* RINIT */
    NULL,            /* RSHUTDOWN */
    PHP_MINFO(quvi), /* MINFO */
#if ZEND_MODULE_API_NO >= 20010901
    PHP_QUVI_VERSION,
#endif
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_QUVI
ZEND_GET_MODULE(quvi)
#endif
