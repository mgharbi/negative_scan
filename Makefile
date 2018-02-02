CXX=g++
MOC=moc
CXXFLAGS=-Wall -std=c++11

SOURCES:=$(wildcard src/*.cpp)
SOURCES:=$(filter-out src/main.cpp,$(SOURCES))
MOC_HEADERS=hello_cls.h
EXECUTABLE=bin/main

QT ?= /usr/local/Cellar/qt/5.10.0_1
INCDIRS=-Iinclude `pkg-config --cflags Qt5Core Qt5Gui Qt5Widgets`
LIBS=`pkg-config --libs Qt5Core Qt5Gui Qt5Widgets`

INCDIRS += `pkg-config --cflags libraw`
LIBS += `pkg-config --libs libraw`

# Change postfixes
MOC_SOURCES=$(MOC_HEADERS:.h=.moc.cpp)
OBJECTS=$(subst src/,build/,$(SOURCES:.cpp=.o)) #$(MOC_SOURCES:.cpp=.o)

all: bin_dir $(EXECUTABLE)
	@echo Done!

$(EXECUTABLE): src/main.cpp $(OBJECTS)
	@echo Compiling $@
	@$(CXX) $^  $(CXXFLAGS) $(INCDIRS) $(LIBS) -o $@

# Generate object files, rule to change postfix
build/%.o: src/%.cpp build_dir
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) $(INCDIRS) -c $< -o $@

# # Generate cc from h via Qt's Meta Object Compiler, rule to change postfix
# %.moc.cc: %.h
# 	$(MOC) $(INCDIRS) $< -o $@

.PHONY: build_dir clean

build_dir:
	@echo Creating build directory
	@mkdir -p build

bin_dir:
	@echo Creating bin directory
	@mkdir -p bin

clean:
	rm -r build
	rm -r bin

# # Generate ctags file for all included files (autocomplete and jump to source)
# tags:
# 	gcc -M $(INCDIRS) $(SOURCES) | \
# 	sed -e 's/[\\ ]/\n/g' | \
# 	sed -e '/^$$/d' -e '/\.o:[ \t]*$$/d' | \
# 	ctags -L - --c++-kinds=+p --fields=+iaS --extra=+q
