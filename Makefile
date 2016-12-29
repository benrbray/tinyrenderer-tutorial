CC     := g++
CFLAGS := -std=c++11 -g -Wall -Wunused-variable
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

.PHONY: test
test: test-vec2f test-vec2i test-vec3f test-model

test-vec2f: tests/test-vec2f.cpp
	@mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) -I $(INCDIR) $< -o $(OUTDIR)/test-vec2f $(LIBS)

test-vec2i: tests/test-vec2i.cpp
	@mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) -I $(INCDIR) $< -o $(OUTDIR)/test-vec2i $(LIBS)

test-vec3f: tests/test-vec3f.cpp
	@mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) -I $(INCDIR) $< -o $(OUTDIR)/test-vec3f $(LIBS)

test-model: tests/test-model.cpp $(BUILDDIR)/model.o
	@mkdir -p $(OUTDIR)
	$(CC) $(CFLAGS) -I $(INCDIR) $^ -o $(OUTDIR)/test-model $(LIBS)

# clean ------------------------------------------------------------------------

clean:
	-rm -r $(BUILDDIR) $(OUTDIR)
	-rm -f *.tga
