========
php_quvi
========

php_quvi enable the support of libquvi in PHP.

libquvi is a small C library that can be used to parse flash media stream URLs. More infos on http://quvi.sourceforge.net/

-------
Compile
-------

First of all, you will need a full PHP development working environnement. You can check here http://devzone.zend.com/article/1021#Heading5 if you don't know how to do that.

Then, you will need the source of libquvi, available here http://quvi.sourceforge.net/, and of course, you'll need to build the lib before php_quvi.

At least, you can build the extension::

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

Very easy. Example::

    <?php
    $url = 'http://www.dailymotion.com/video/xgdy1i_birdy-nam-nam-the-parachute-ending-stranger-remix_music';
    $mediaInfos = quvi($url);yy

    echo $mediaInfos['page_title']; // should echoes "Birdy Nam Nam - The Parachute Ending (Stranger Remix)"

    // show all available infos
    var_dump($mediaInfos);

More complex example : use a HTML 5 player instead of Flash player::

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

Set default format to 'best' available::

    <?php
    // You can use the optionnal parameter of quvi()
    quvi('http://some.video.url', 'best');

    // or you can set the ini parameter
    ini_set('quvi.default_format_request', 'best');
    quvi('http://some.video.url');

----------------------
PHP function available
----------------------

(array)  quvi(string $url [, string $request_format]);

(string) quvi_formats(string $url);

(string) quvi_version();

------------------------
INI optionnal parameters
------------------------

* quvi.default_format_request : define the source format to request. Possible value : 'default' or 'best'. Default to 'default' (lowest quality available).

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
