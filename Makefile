CC     := g++
CFLAGS := -g -Wall
LIBS   := -lm

SRCDIR   := src
BUILDDIR := build
BINDIR   := bin
INCDIR   := include
TARGET   := bin/tinyrenderer

SOURCES := $(shell find $(SRCDIR) -type f -name *.cpp)
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.cpp=.o))

all: $(OBJECTS)
	@mkdir -p $(BINDIR)
	@echo "Linking..."
	@$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS) $(LIBS)
	@echo "Running Tinyrenderer..."
	@./$(TARGET)
	@eog output.tga

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(BUILDDIR)
	@$(CC) -c $(CFLAGS) -I $(INCDIR) $< -o $@

clean:
	-rm -r $(BUILDDIR) $(BINDIR)
	-rm -f *.tga
