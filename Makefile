
CC := gcc
CFLAGS := -fPIC -Iinclude -std=c99

SRC := ./src
INC := ./include
BIN := ./bin

INCLOC := /usr/include
LIBLOC := /usr/lib

default: all


KBILSRC := $(SRC)/kbil.c
KBILTARG := $(BIN)/kbil.o

$(KBILTARG): $(KBILSRC)
	$(CC) -c $< -o $@ $(CFLAGS)


STATICLIB := libkbil.a

$(STATICLIB): $(KBILTARG)
	ar rcs $@ $<


DYNAMICLIB := libkbil.so

$(DYNAMICLIB): $(KBILTARG)
	gcc -shared -o $@ $<	


.PHONY: install
install: $(STATICLIB) $(DYNAMICLIB)
	@cp $(INC)/* $(INCLOC)
	@cp $(STATICLIB) $(LIBLOC)
	@cp $(DYNAMICLIB) $(LIBLOC)


.PHONY: uninstall
uninstall:
	@if [ -f $(INCLOC)/kbil.h ]; then rm -rf $(INCLOC)/kbil.h; fi;
	@if [ -f $(LIBLOC)/$(STATICLIB) ]; then rm $(LIBLOC)/$(STATICLIB); fi;
	@if [ -f $(LIBLOC)/$(DYNAMICLIB) ]; then rm $(LIBLOC)/$(DYNAMICLIB); fi;


.PHONY: clean
clean:
	@if [ -f $(KBILTARG) ]; then rm $(KBILTARG); fi;
	@if [ -f $(STATICLIB) ]; then rm $(STATICLIB); fi;
	@if [ -f $(DYNAMICLIB) ]; then rm $(DYNAMICLIB); fi;


.PHONY: all 
all: 
	@echo "building static library..."
	$(MAKE) $(STATICLIB)
	@echo "done."
	@echo "building dynamic library..."
	$(MAKE) $(DYNAMICLIB)
	@echo "done."

