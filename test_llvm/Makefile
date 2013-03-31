CHILD_PATH = .
SUBPATHS = $(shell find $(CHILD_PATH) -mindepth 1 -maxdepth 1 -name "*" -type d | sort)

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
