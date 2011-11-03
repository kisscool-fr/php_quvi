========
php_quvi
========

php_quvi enable the support of libquvi in PHP.

libquvi is a small C library that can be used to parse flash media stream URLs. More infos on http://quvi.sourceforge.net/

-------
Compile
-------

First of all, you will need a full PHP development working environnement. You can check here http://devzone.zend.com/article/1021#Heading5 if you don't know how to do that.

Then, you will need the source of libquvi, available here http://quvi.sourceforge.net/, and of course, you'll need to build it before php_quvi.

When everything is ready, you can build the extension::

    phpize
    ./configure --with-quvi
    make

And at the end, install it (as root)::

    make install

-------------
Configuration
-------------

Add the extension into your extension enable list (or load it with dl() into your PHP script)::

    extension=quvi.so

If needed, restart your web server.

-----
Usage
-----

Very easy. Short example::

    <?php
    $url = 'http://www.dailymotion.com/video/xgdy1i_birdy-nam-nam-the-parachute-ending-stranger-remix_music';
    $mediaInfos = quvi($url);yy

    echo $mediaInfos['page_title']; // should echoes "Birdy Nam Nam - The Parachute Ending (Stranger Remix)"

    // show all available infos
    var_dump($mediaInfos);

Get all available formats for a media::

    <?php
    $url = '';
    $availableFormats = quvi_formats($url);

    // show an array with all available formats for the media
    var_dump($availableFormats);

Then, get a specific info from one of available formats (we supposed that we have at least 2 different formats in the array $availableFormats)::

    $media = quvi($url, $availableFormats[1]);

    // show all info for format $availableFormats[1] of media $url
    var_dump($media);

Complex example (^^) : use a HTML 5 player instead of Flash player::

    <html>
        <body>
            <?php
            $url = 'http://www.dailymotion.com/video/xjnhzd_kazantip-duplex-18-06-2011_music';
            $media = quvi($url);
            ?>
            <video controls preload="none">
                <source src="<?php echo $media['link']['url'] ?>" type="<?php echo $media['link']['content_type'] ?>" />
            </video>
        </body>
    </html>

How to set default format to 'best' available::

    <?php
    // For one video, you can use the optionnal parameter of quvi()
    quvi('http://some.video.url', 'best');

    // or if you have a lot of video, you can set the ini parameter
    ini_set('quvi.default_format_request', 'best');
    quvi('http://some.video.url');

----------------------
PHP function available
----------------------

(array)  quvi(string $url [, string $request_format = 'default']);

(array)  quvi_formats(string $url [, string $delimiter = '|']);

(string) quvi_version();

------------------------
INI optionnal parameters
------------------------

* quvi.default_format_request : define the default source format to request. Possible value : 'default', 'best'. Default to 'default' (lowest quality available).
* quvi.format_delimiter : define the delimiter of the string return by libquvi. By default, it's the character | (single pipe). This value should not be change while libquvi query_formats() return a piped string delimiter, but maybe this will change in the future ...

-----------------------
PHP constants available
-----------------------

* (string) QUVI_VERSION         : php_quvi version      (ie: 0.1)
* (string) LIBQUVI_VERSION      : libquvi short version (ie: 0.2.16.2)
* (string) LIBQUVI_VERSION_LONG : libquvi full version  (ie: 0.2.16.2 on 2011-10-08 for x86_64-redhat-linux-gnu (in))

------------------
Build environnment
------------------

This extension is develop, build and test on::

    Linux Fedora 15 x86_64
    PHP 5.3.8 (php-devel.x86_64 / 5.3.8-3.fc15)
    libquvi 0.2.16.2 (quvi-devel.x86_64 / 0.2.16.2-1.fc15)

If you have success (or fail) with build on other environnment, please let me know :)

--------------------
Why this extension ?
--------------------

I've always want to know how to develop a PHP extension, but I've never found a nice and useful C library that need to be add as a PHP extension.

Then, I've found libquvi, an easy, small and useful C library.

When I've start this personal project, libquvi already has support for Perl and Python, but not for PHP. So, I said to me that it's very unfair, and I start this extension to give an answer to this problem :)

-------------
What's next ?
-------------

Win32 support.

Follow and add support for new libquvi development.

Recode the extension as a PHP 5 object, with keeping the existing working.

And yes, if you have the skill, help is welcome :)

-------
License
-------

Like libquvi, php_quvi is free software, licensed under the LGPLv2.1+.
