SHELL := /bin/bash

include config.mk

LLIBNAME = lib$(LIBNAME)
LOC_SRC_DIR = src
LOC_BUILD_DIR = build
LOC_OBJ_DIR = $(LOC_BUILD_DIR)/obj
LOC_LIB_DIR = $(LOC_BUILD_DIR)/lib
CREATE_DIR = $(LOC_OBJ_DIR) $(LOC_LIB_DIR)

LIB_VERSION = $(VMAJ).$(VMIN)
UPSTREAM_VERSION = $(LIB_VERSION).$(VREV)
DEBIAN_REVISION = $(VDEB)
VERSION = $(UPSTREAM_VERSION)-$(DEBIAN_REVISION)

VLIBNAME = $(LLIBNAME)-$(LIB_VERSION)
SONAME = $(LLIBNAME).so.$(LIB_VERSION)
ANAME = $(LLIBNAME).a

GAC = gac
GACLIB = $(GAC)/build/lib/lib$(GAC).so

CGLM = $(GAC)/cglm

STATLIB = $(LOC_LIB_DIR)/$(LLIBNAME).a
DYNLIB = $(LOC_LIB_DIR)/$(LLIBNAME).so

SOURCES = $(wildcard $(LOC_SRC_DIR)/*.c)
OBJECTS := $(patsubst $(LOC_SRC_DIR)/%.c, $(LOC_OBJ_DIR)/%.o, $(SOURCES))

INCLUDES_DIR = -I$(CGLM)/include \
			   -I$(GAC)/include \
			   -I/usr/share/R/include \
			   -I.

LINK_DIR = -L/usr/local/lib\
		   -L$(CGLM)/.libs\
		   -L$(GAC)/build/lib

LINK_FILE = -lgac \
			-lcglm \
			-lm

CFLAGS = -Wall -fPIC \
		 -DLIBSMXUTILS_VERSION_UP=\"$(UPSTREAM_VERSION)\"
DEBUG_FLAGS = -g -O0

CC = gcc

all: directories $(STATLIB) $(DYNLIB)

# compile with dot stuff and debug flags
debug: CFLAGS += $(DEBUG_FLAGS)
debug: all

$(STATLIB): $(OBJECTS)
	ar -cq $@ $^

$(DYNLIB): $(OBJECTS)
	$(CC) -shared -Wl,-soname,$(SONAME) $^ -o $@ $(LINK_DIR) $(LINK_FILE)
	ln -sf $(LLIBNAME).so $(LOC_LIB_DIR)/$(SONAME)

# compile project
$(LOC_OBJ_DIR)/%.o: $(LOC_SRC_DIR)/%.c $(GACLIB)
	$(CC) $(CFLAGS) $(INCLUDES_DIR) -c $< -o $@ $(LINK_DIR) $(LINK_FILE)

directories: $(CREATE_DIR)

$(GACLIB):
	$(MAKE) -C $(GAC)

$(CREATE_DIR):
	mkdir -p $@

.PHONY: clean install uninstall doc cglm-check cglm-clean test $(TEST_DIRS)

install:

uninstall:

clean:
	rm -rf $(LOC_LIB_DIR)
	rm -rf $(LOC_OBJ_DIR)
	rm -rf $(LOC_BUILD_DIR)
	$(MAKE) clean -C $(GAC)

doc:
	doxygen .doxygen

cglm-check:
	$(MAKE) check -C $(CGLM)

cglm-clean:
	$(MAKE) clean -C $(CGLM)

test: $(TEST_DIRS)
$(TEST_DIRS):
	$(MAKE) run -C $@

