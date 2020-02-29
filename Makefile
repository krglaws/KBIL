
CC := gcc
CFLAGS := -fPIC -Iinclude -std=c99
DEBFLAGS := -Iinclude -std=c99 -g

SRC := ./src
INC := ./include
TST := ./test
BIN := ./bin

INCLOC := /usr/include
LIBLOC := /usr/lib

default: all


KBILDEPS := $(SRC)/kbil.c
KBILTARG := $(BIN)/kbil.o

$(KBILTARG): $(KBILDEPS)
	$(CC) -c $< -o $@ $(CFLAGS)


KBILTESTDEPS := $(KBILDEPS)
KBILTESTTARG := $(BIN)/kbiltests.out

$(KBILTESTTARG): $(TST)/test.c $(KBILTESTDEPS)
	$(CC) $^ -o $@ $(DEBFLAGS)


STATICLIB := libkbil.a

$(STATICLIB): $(KBILTARG)
	ar rcs $@ $<


DYNAMICLIB := libkbil.so

$(DYNAMICLIB): $(KBILTARG)
	gcc -shared -o $@ $<


.PHONY: build_tests
build_tests: $(KBILTESTTARG)


.PHONY: run_tests
run_tests: build_tests
	@echo "Running tests..."
	@echo "======================"
	@echo "----------------------"
	@./bin/kbiltests.out
	@if [ $? = 0 ]; then echo "Tests failed."; else echo "All tests passed."; fi;
	@echo "----------------------"
	@echo "======================"
	@echo "Done."


.PHONY: uninstall
uninstall:
	@if [ -f $(INCLOC)/kbil.h ]; then rm -rf $(INCLOC)/kbil.h; fi;
	@if [ -f $(LIBLOC)/$(STATICLIB) ]; then rm $(LIBLOC)/$(STATICLIB); fi;
	@if [ -f $(LIBLOC)/$(DYNAMICLIB) ]; then rm $(LIBLOC)/$(DYNAMICLIB); fi;


.PHONY: clean
clean:
	@if [ -f $(KBILTARG) ]; then rm $(KBILTARG); fi;
	@if [ -f $(KBILTESTTARG) ]; then rm $(KBILTESTTARG); fi;
	@if [ -f $(STATICLIB) ]; then rm $(STATICLIB); fi;
	@if [ -f $(DYNAMICLIB) ]; then rm $(DYNAMICLIB); fi;


.PHONY: all 
all: run_tests 
	@echo "building static library..."
	$(MAKE) $(STATICLIB)
	@echo "done."
	@echo "building dynamic library..."
	$(MAKE) $(DYNAMICLIB)
	@echo "done."


.PHONY: install
install: all
	@cp $(INC)/* $(INCLOC)
	@cp $(STATICLIB) $(LIBLOC)
	@cp $(DYNAMICLIB) $(LIBLOC)
	@echo "Installed successfully."

