.SUFFIXES: .o .c
#============================================================

TARGET1 = example
C_OBJS1 = example.o 

C_SOURCES =  example.c rscombo.c
MY_INCLUDES = rscombo.h 

ALL_SOURCES = Makefile $(C_SOURCES) $(MY_INCLUDES)

CCX = g++
CXXFLAGS = 
SHELL:=/bin/bash

#============================================================
all: 	$(TARGET1) 

.o:.c	$(MY_INCLUDES)
	$(CCX)  -c  $(CXXFLAGS) $<  

$(TARGET1) :   $(C_OBJS1)
	$(CCX) $(CXXFLAGS)  $^ $(LIBDIRS)  -o $@

# Implicit rules: $@ = target name, $< = first prerequisite name, $^ = name of all prerequisites
#============================================================


clean:
	rm -f $(TARGET1)  $(C_OBJS1) 
