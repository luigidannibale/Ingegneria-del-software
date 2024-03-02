ODIR = ../obj
BDIR = ../bin

SRCS := $(shell find . -name "*.cpp")
OBJS := $(SRCS:%=$(ODIR)/%.o)
OBJS2 = $(OBJS:.cpp.o=.o)

CC=g++
# use this when connecting to Postgresql (library lpq)
#CFLAGS=-std=c++11 -I. -I$(IDIR) -I/usr/include/postgresql -lpq -lm
WXCONFIG=wx-config
WXFLAGS=$(shell $(WXCONFIG) --cxxflags)
WXLIBS=$(shell $(WXCONFIG) --libs)

CFLAGS=-std=c++20 -g -ggdb -fpermissive -L/usr/local/lib -I. $(WXFLAGS) -lm -lhiredis $(WXLIBS) #-Wall


TARGET_EXEC = chess


# c++ source
$(ODIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(CFLAGS)

$(BDIR)/$(TARGET_EXEC): $(OBJS2)
	$(CC) $(OBJS2) -o $@ $(LDFLAGS) $(CFLAGS)



.PHONY: clean

clean:
	rm -f $(BDIR)/*
	find $(ODIR) -type f -name "*.o" -delete
