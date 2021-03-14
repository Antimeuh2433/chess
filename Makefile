CXX = g++
CXXFLAGS = -Wall

SRCDIR = src
BINDIR = bin

SRCFILES = $(wildcard $(SRCDIR)/*.cpp)
OBJFILES = $(SRCFILES:$(SRCDIR)/%.cpp=$(BINDIR)/%.o)
EXE = $(BINDIR)/chess

all: $(EXE)

debug: CXXFLAGS += -g -O0
debug: $(EXE)

release: CXXFLAGS += -O3
release: $(EXE)

$(EXE): $(BINDIR) $(OBJFILES)
	$(CXX) $(CXXFLAGS) -o $(EXE) $(OBJFILES)

$(BINDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	del /Q bin/*.o
	del /Q bin/chess.exe

.PHONY: all debug release