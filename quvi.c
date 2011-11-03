/*
  +---------------------------------------------------------------------+
  | Author: KisSCoOl <kisscool@kisscool.net>                            |
  +---------------------------------------------------------------------+

*/

#include "php_quvi.h"

/**
 * Helpers
 */
static void php_quvi_version(char *buf)
{
    const char *vers;
    vers = quvi_version(QUVI_VERSION);
    php_sprintf(buf, "%s", vers);
}

static void php_quvi_query_formats(char *url, char *buf)
{
    char *formats;
    quvi_t q;

    quvi_init(&q);
    quvi_query_formats(q, url, &formats);

    php_sprintf(buf, "%s", formats);

    quvi_free(formats);
    quvi_close(&q);
}

/**
 * Zend mapping
 */
zend_function_entry quvi_functions[] = {
    PHP_FE(quvi,         NULL)
    PHP_FE(quvi_formats, NULL)
    PHP_FE(quvi_version, NULL)
    {NULL, NULL, NULL}
};

zend_module_entry quvi_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    PHP_QUVI_NAME,       /* extension name */
    quvi_functions,      /* zend_function_entry */
    PHP_MINIT(quvi),     /* MINIT */
    PHP_MSHUTDOWN(quvi), /* MSHUTDOWN */
    NULL,                /* RINIT */
    NULL,                /* RSHUTDOWN */
    PHP_MINFO(quvi),     /* MINFO */
#if ZEND_MODULE_API_NO >= 20010901
    PHP_QUVI_VERSION,    /* extension version */
#endif
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_QUVI
ZEND_GET_MODULE(quvi)
#endif

/* INI */
ZEND_INI_MH(php_quvi_ini_update_format_request)
{
    if (new_value_length == 0 || (strcmp(new_value, "default") != 0 && strcmp(new_value, "best") != 0))
    {
        return FAILURE;
    }

    return SUCCESS;
}

PHP_INI_BEGIN()
    PHP_INI_ENTRY("quvi.default_format_request", "default", PHP_INI_ALL, php_quvi_ini_update_format_request)
PHP_INI_END()

/* Process functions */
PHP_MINIT_FUNCTION(quvi)
{
    REGISTER_INI_ENTRIES();

    REGISTER_STRING_CONSTANT("QUVI_VERSION", PHP_QUVI_VERSION, CONST_CS | CONST_PERSISTENT);
    REGISTER_STRING_CONSTANT("LIBQUVI_VERSION", quvi_version(QUVI_VERSION), CONST_CS | CONST_PERSISTENT);
    REGISTER_STRING_CONSTANT("LIBQUVI_VERSION_LONG", quvi_version(QUVI_VERSION_LONG), CONST_CS | CONST_PERSISTENT);

    return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(quvi)
{
    UNREGISTER_INI_ENTRIES();

    return SUCCESS;
}

PHP_MINFO_FUNCTION(quvi)
{
    char vstr[128];
    php_quvi_version(vstr);

    php_info_print_table_start();
    php_info_print_table_header(2, "Quvi support", "enabled");
    php_info_print_table_row(2, "php_quvi version", PHP_QUVI_VERSION);
    php_info_print_table_row(2, "Quvi Library Version", vstr);
    php_info_print_table_end();

    DISPLAY_INI_ENTRIES();
}

/* Main functions */
PHP_FUNCTION(quvi_version)
{
    char vstr[128];

    if (ZEND_NUM_ARGS())
    {
        WRONG_PARAM_COUNT;
    }

    php_quvi_version(vstr);
    RETURN_STRING(vstr, 1);
}

PHP_FUNCTION(quvi_formats)
{
    char *url, avail_formats[128];
    int url_length;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &url, &url_length) == FAILURE)
    {
        RETURN_NULL();
    }

    if (url_length == 0)
    {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "empty url");
        RETURN_NULL();
    }

    php_quvi_query_formats(url, avail_formats);
    RETURN_STRING(avail_formats, 1);
}

PHP_FUNCTION(quvi)
{
    char *url, *format_requested = INI_STR("quvi.default_format_request");
    char *host, *page_url, *page_title, *id, *media_url, *content_type, *file_suffix, *response_code, *format, *start_time, *thumbnail_url;
    int url_length, format_requested_length = sizeof(format_requested) - 1;
    double content_length, duration;
    zval *link;
    quvi_media_t m;
    QUVIcode rc;
    quvi_t q;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s|s", &url, &url_length, &format_requested, &format_requested_length) == FAILURE)
    {
        RETURN_NULL();
    }

    rc = quvi_init(&q);
    if (rc != QUVI_OK)
    {
        php_error_docref(NULL TSRMLS_CC, E_ERROR, "%s", quvi_strerror(q,rc));
        RETURN_NULL();
    }

    quvi_setopt(q, QUVIOPT_FORMAT, format_requested);

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
    quvi_getprop(m, QUVIPROP_MEDIADURATION, &duration);
    quvi_getprop(m, QUVIPROP_FORMAT, &format);
    quvi_getprop(m, QUVIPROP_MEDIATHUMBNAILURL, &thumbnail_url);

    quvi_getprop(m, QUVIPROP_RESPONSECODE, &response_code);
    quvi_getprop(m, QUVIPROP_STARTTIME, &start_time);

    array_init(return_value);

    add_assoc_string(return_value, "host", host, 1);
    add_assoc_string(return_value, "page_title", page_title, 1);
    add_assoc_string(return_value, "page_url", page_url, 1);
    add_assoc_double(return_value, "duration", duration);
    add_assoc_string(return_value, "id", id, 1);
    add_assoc_string(return_value, "format_requested", format, 1);
    add_assoc_string(return_value, "thumbnail_url", thumbnail_url, 1);
    /*
    add_assoc_long(return_value, "response_code", response_code);
    add_assoc_string(return_value, "start_time", start_time, 1);
    */

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
