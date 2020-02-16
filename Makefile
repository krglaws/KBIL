
CC := gcc
CFLAGS := -fPIC -I./include -std=c99

SRC := ./src
INC := ./include
BIN := ./bin

INCLOC := /usr/include/
LIBLOC := /usr/lib


KBILSRC := bigint.c
KBILTARG := bigint.o

$(KBILTARG): $(KBILSRC)
	$(CC) -c $< -o $@ $(CFLAGS)


STATICLIB := libkbil.a

$(STATICLIB): $(KBILTARG)
	ar rcs $@ $<


DYNAMICLIB := libkbil.so

$(DYNAMICLIB): $(KBILTARG)
	gcc -shared -o $@ $<	

 
.PHONY: all 
all: $(STATICLIB) $(DYNAMICLIB) 


.PHONY: install
install: $(STATICLIB) $(DYNAMICLIB)
	cp $(INC)/* $(INCLOC)
	cp $(STATICLIB) $(LIBLOC)
	cp $(DYNAMICLIB) $(LIBLOC)


.PHONY: uninstall
uninstall:
	if [ -d $(INCLOC) ]; then rm -rf $(INCLOC); fi;
	if [ -f $(LIBLOC)/$(STATICLIB) ]; then rm $(LIBLOC)/$(STATICLIB); fi;
	if [ -f $(LIBLOC)/$(DYNAMICLIB) ]; then rm $(LIBLOC)/$(DYNAMICLIB); fi;


.PHONY: clean
clean:
	if [ -f $(KBILTARG) ]; then rm $(KBILTARG); fi;
	if [ -f $(STATICLIB) ]; then rm $(STATICLIB); fi;
	if [ -f $(DYNAMICLIB) ]; then rm $(DYNAMICLIB); fi;


.PHONY: all
all:
	@echo "Building static libs..."
	$(MAKE) $(STATICLIB)
	@echo "Done."
	@echo "Building shared libs..."
	$(MAKE) $(DYNAMICLIB)
	@echo "Done."


