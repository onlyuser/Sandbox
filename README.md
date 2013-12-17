[![Build Status](https://secure.travis-ci.org/onlyuser/Sandbox.png)](http://travis-ci.org/onlyuser/Sandbox)

Sandbox
=======

Copyright (C) 2011-2013 Jerry Chen <mailto:onlyuser@gmail.com>

About:
------

Sandbox is a collection of experimental code and tutorials.

**Currently, it includes the following:**

* test_backtrace -- test for backtrace
* test_c         -- test for c-language curiosities
* test_coroutine -- test for coroutine
* test_functor   -- test for stl functor
* test_lexyacc   -- test for Lex-Yacc
* test_llvm      -- test for LLVM
* test_math      -- test for math libraries
* test_opengl    -- test for OpenGL
* test_pthread   -- test for pthread
* test_regex     -- test for posix regex
* test_ticpp     -- test for ticpp
* test_tinyxml   -- test for tinyxml
* test_tokenize  -- test for stl tokenize
* test_variant   -- test for boost variant
* test_visitor   -- test for visitor pattern

Requirements:
-------------

Unix tools and 3rd party components (accessible from $PATH):

    gcc (with -std=c++0x support), boost, llvm, ticpp, tinyxml, coroutine

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

    <dt>"Universal References in C++11 - Scott Meyers"</dt>
    <dd>http://isocpp.org/blog/2012/11/universal-references-in-c11-scott-meyers</dd>

    <dt>"POSIX thread (pthread) libraries"</dt>
    <dd>http://www.cs.cmu.edu/afs/cs/academic/class/15492-f07/www/pthreads.html</dd>

    <dt>"Setting up an OpenGL development environment in Ubuntu Linux"</dt>
    <dd>http://www.codeproject.com/Articles/182109/Setting-up-an-OpenGL-development-environment-in-Ub</dd>

    <dt>"The Official Guide to Learning OpenGL, Version 1.1 - Chapter 1 - Introduction to OpenGL"</dt>
    <dd>http://www.glprogramming.com/red/chapter01.html</dd>

    <dt>"OpenGL Programming/Modern OpenGL Introduction"</dt>
    <dd>http://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Introduction</dd>

    <dt>"Anton's OpenGL 4 Tutorials"</dt>
    <dd>http://antongerdelan.net/opengl/</dd>

    <dt>"OpenGL Mathematics (GLM) - A C++ mathematics library for graphics programming - Code samples"</dt>
    <dd>http://glm.g-truc.net/0.9.4/code.html</dd>
</dl>

Keywords:
---------

    stl, boost, functor, Lex, Yacc, OpenGL, glm, LLVM, visitor, variant, tinyxml, ticpp, regex, backtrace
