CC     := g++
CFLAGS := -g -Wall
LIBS   := -lm

SRCDIR   := src
BUILDDIR := build
OUTDIR   := bin
TESTDIR  := tests
INCDIR   := include
TARGET   := bin/tinyrenderer

# main compilation -------------------------------------------------------------

SOURCES := $(shell find $(SRCDIR) -type f -name *.cpp)
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.cpp=.o))

all: $(OBJECTS)
	@mkdir -p $(OUTDIR)
	@echo "Linking..."
	@$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)
	@echo "Running Tinyrenderer..."
	@./$(TARGET)
	@eog output.tga

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BUILDDIR)
	@$(CC) -c $(CFLAGS) -I $(INCDIR) $< -o $@

# tests ------------------------------------------------------------------------

TESTSRC := $(shell find $(TESTDIR) -type f -name *.cpp)
TESTOUT := $(patsubst $(TESTDIR)/%,$(OUTDIR)/%, $(basename $(TESTSRC)))

.PHONY: test
test: $(TESTOUT)

$(OUTDIR)/%: $(TESTDIR)/%.cpp
	@mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) -I $(INCDIR) $< -o $@ $(LIBS)

# clean ------------------------------------------------------------------------

clean:
	-rm -r $(BUILDDIR) $(OUTDIR)
	-rm -f *.tga
