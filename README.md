Sandbox
=======

Copyright (C) 2011-2013 Jerry Chen <mailto:onlyuser@gmail.com>

About:
------

Sandbox is a collection of experimental code.

**Currently, it includes the following:**

* test_backtrace -- test for backtrace
* test_coroutine -- test for coroutine
* test_functor   -- test for stl functor
* test_lexyacc   -- test for Lex-Yacc
* test_llvm      -- test for LLVM
* test_regex     -- test for posix regex
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

    <dt>"Lex & Yacc Tutorial"</dt>
    <dd>http://epaperpress.com/lexandyacc/</dd>

    <dt>"Kaleidoscope: Implementing a Language with LLVM"</dt>
    <dd>http://llvm.org/docs/tutorial/index.html</dd>

    <dt>"ticpp"</dt>
    <dd>http://code.google.com/p/ticpp/</dd>

    <dt>"tinyxml"</dt>
    <dd>http://sourceforge.net/projects/tinyxml/</dd>

    <dt>"Chapter 34. Boost.Variant"</dt>
    <dd>http://www.boost.org/doc/libs/1_52_0/doc/html/variant.html</dd>

    <dt>"Cooperative VisitorIFace: A Template Technique for VisitorIFace Creation"</dt>
    <dd>http://www.artima.com/cppsource/cooperative_visitor.html</dd>

    <dt>"Stack Backtracing Inside Your Program"</dt>
    <dd>http://www.linuxjournal.com/article/6391</dd>

    <dt>"Listing 3. Using the SA_SIGINFO Option"</dt>
    <dd>http://www.linuxjournal.com/files/linuxjournal.com/linuxjournal/articles/063/6391/6391l3.html</dd>

    <dt>"How to generate a stacktrace when my gcc C++ app crashes"</dt>
    <dd>http://stackoverflow.com/questions/77005/how-to-generate-a-stacktrace-when-my-gcc-c-app-crashes/77336#77336</dd>

    <dt>"C++ Code Snippet - Print Stack Backtrace Programmatically with Demangled Function Names"</dt>
    <dd>http://panthema.net/2008/0901-stacktrace-demangled/</dd>

    <dt>"How it's better to invoke gdb from program to print its stacktrace?"</dt>
    <dd>http://stackoverflow.com/questions/3151779/how-its-better-to-invoke-gdb-from-program-to-print-its-stacktrace/4611112#4611112</dd>

    <dt>"Stack trace for C++ using gcc"</dt>
    <dd>http://stackoverflow.com/questions/4636456/stack-trace-for-c-using-gcc/4732119#4732119</dd>

    <dt>"How do you capture a group with regex?"</dt>
    <dd>http://stackoverflow.com/questions/2577193/how-do-you-capture-a-group-with-regex</dd>
</dl>

Keywords:
---------

    stl, boost, functor, Lex, Yacc, LLVM, visitor, variant, tinyxml, ticpp, regex, backtrace
