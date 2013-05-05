# Variations of a Flex-Bison parser
# -- based on "A COMPACT GUIDE TO LEX & YACC" by Tom Niemann
# Copyright (C) 2011 Jerry Chen <mailto:onlyuser@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.

SUBPATHS = \
    test_functor \
    test_lexyacc \
    test_llvm \
    test_regex \
    test_tinyxml \
    test_tokenize \
    test_unicode \
    test_variant \
    test_visitor #\
    test_backtrace \
    test_coroutine \
    test_ticpp

.DEFAULT_GOAL : all
all :
	@for i in $(SUBPATHS); do \
	echo "make $@ in $$i..."; \
	cd $$i; $(MAKE); done

.PHONY : test
test :
	@for i in $(SUBPATHS); do \
	echo "make $@ in $$i..."; \
	(cd $$i; $(MAKE) $@); done
	find . -name "*.test.*" | grep fail; if [ $$? -eq 0 ]; then exit 1; fi

.PHONY : clean
clean :
	@for i in $(SUBPATHS); do \
	echo "make $@ in $$i..."; \
	(cd $$i; $(MAKE) $@); done
