# makefile for Lua decompiler

INCLUDEDIRS=-I../include/ -I../../Leadwerks/include/
BINDIR=../bin

DEV_NULL= /dev/null
CP= cp -f
RM= rm -f

CC= gcc
CFLAGS= -O2 -Wall -DSRCVERSION=\"$(SRCVERSION)\" $(INCS) $(MYCFLAGS)
LDFLAGS= $(MYLDFLAGS)
LIBS= -lm $(MYLIBS)

ifeq ($(wildcard /dev/null),)
	DEV_NULL= NUL
	RM= cmd //c del //f //s //q
endif

SRCVERSION=UNKNOWN
GIT_STATUS:=$(shell git log -n 1 --format=format:%h 1>$(DEV_NULL) 2>$(DEV_NULL); echo $$?)
ifeq ($(GIT_STATUS),0)
	SRCVERSION:=$(shell git log -n 1 --format=format:%h)
endif

MYCFLAGS= 
MYOBJS=
MYLDFLAGS= 

INCS= $(INCLUDEDIRS)
OBJS= tex2dds.o
SRCS= tex2dds.c

T= tex2dds

all: $T

$T: $(OBJS) $(MYOBJS) $(LIBDIR)
	$(CC) -o $@ $(LDFLAGS) $^ $(LIBS)

debug:
	$(MAKE) all MYCFLAGS="-g -DDEBUG -D_DEBUG"

clean:
	$(RM) *.o *.exe
