Sandbox
=======

Copyright (C) 2011-2012 Jerry Chen <mailto:onlyuser@gmail.com>

About:
------

Sandbox is a collection of experimental code.

**Currently, it includes the following:**

* test_coroutine -- test for coroutine
* test_functor   -- test for stl functor
* test_ticpp     -- test for ticpp
* test_tinyxml   -- test for tinyxml
* test_variant   -- test for boost variant
* test_visitor   -- test for visitor pattern

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

    <dt>"Chapter 34. Boost.Variant"</dt>
    <dd>http://www.boost.org/doc/libs/1_52_0/doc/html/variant.html</dd>

    <dt>"Cooperative VisitorIFace: A Template Technique for VisitorIFace Creation"</dt>
    <dd>http://www.artima.com/cppsource/cooperative_visitor.html</dd>
</dl>

Keywords:
---------

    stl, boost, functor, visitor, variant, tinyxml, ticpp
