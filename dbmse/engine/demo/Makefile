CC=g++
#CXXFLAGS=-std=c++11
CXXFLAGS=-std=c++11 -g -O0

BINDIR=bin
OBJDIR=obj

SRC=../interface/interface.cpp pgetnextnode.cpp pselectnode.cpp pjoinnode.cpp demo.cpp pcrossproductnode.cpp pprojectnode.cpp puniquenode.cpp punionnode.cpp pjoinhashnode.cpp pjoinmergesortnode.cpp
OBJ=$(SRC:.cpp=.o)

all: $(BINDIR) $(OBJDIR) demo

valgrind: demo
	valgrind --leak-check=full --track-origins=yes ./demo

demo: $(OBJ)
	$(CC) $(addprefix $(OBJDIR)/, $(notdir $(OBJ))) $(CXXFLAGS) -o $(BINDIR)/$@

%.o: %.cpp
	$(CC) $(CXXFLAGS) -c $< -o $(OBJDIR)/$(notdir $@)

$(BINDIR):
	mkdir -p bin

$(OBJDIR):
	mkdir -p obj

.PHONY: clean

clean:
	rm -rfd $(BINDIR)
	rm -rfd $(OBJDIR)
