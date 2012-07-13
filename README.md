
Sandbox
=======

Copyright (C) 2011-2012 Jerry Chen <mailto:onlyuser@gmail.com>

About:
------

Sandbox is a collection of tests for 3rd party components used by XLang.

**Currently, it includes the following:**

* test_coroutine -- an example testing coroutine
* test_functor   -- an example testing stl functors
* test_ticpp     -- an example testing ticpp
* test_tinyxml   -- an example testing tinyxml
* test_visitor   -- an example testing the visitor pattern

Requirements:
-------------

Unix tools and 3rd party components (accessible from $PATH):

    gcc (with -std=c++0x support), coroutine, ticpp, tinyxml

**Environment variables:**

* $EXTERN_INCLUDE_PATH:

    <pre>
    where "coroutine/coroutine.h" resides
    where "ticpp/ticpp.h" resides
    where "tinyxml/tinyxml.h" resides
    </pre>

* $EXTERN_LIB_PATH -- where "libticppd.a" resides

Make targets:
-------------

    all, test, clean.

<table>
    <tr><th> target </th><th> action                        </th></tr>
    <tr><td> all    </td><td> make binaries                 </th></tr>
    <tr><td> test   </td><td> all + run tests               </th></tr>
    <tr><td> clean  </td><td> remove all intermediate files </th></tr>
</table>

References:
-----------

<dl>
    <dt>"Coroutines in C"</dt>
    <dd>http://www.chiark.greenend.org.uk/~sgtatham/coroutines.html</dd>

    <dt>"Function Objects"</dt>
    <dd>http://www.sgi.com/tech/stl/functors.html</dd>

    <dt>"ticpp"</dt>
    <dd>http://code.google.com/p/ticpp/</dd>

    <dt>"tinyxml"</dt>
    <dd>http://sourceforge.net/projects/tinyxml/</dd>

    <dt>"Cooperative Visitor: A Template Technique for Visitor Creation"</dt>
    <dd>http://www.artima.com/cppsource/cooperative_visitor.html</dd>
</dl>

Keywords:
---------

    stl, functor, visitor, tinyxml, ticpp
