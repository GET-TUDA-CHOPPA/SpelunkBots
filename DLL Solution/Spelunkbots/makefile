SRCDIR=Spelunkbots
INCDIR=Spelunkbots

BUILD=build

# ARCH=i686
ARCH=i686
CPP=$(ARCH)-w64-mingw32-g++
FLAGS=-std=c++0x -static -DLINUX_CROSS_COMPILE_FLAG
DLLTOOL=$(ARCH)-w64-mingw32-dlltool

OBJECTS= \
	$(BUILD)/SpelunkbotsConsoleOutput.o

all: build_dir Spelunkbots.dll Spelunkbots.lib

clean:
	rm -- $(BUILD)/*
	rm -- Spelunkbots.dll
	rm -- Spelunkbots.lib

build_dir:
	mkdir -p $(BUILD)

$(BUILD)/%.o: $(SRCDIR)/%.cpp
	$(CPP) $(FLAGS) -I $(INCDIR) -o $@ -c $<

Spelunkbots.dll: $(OBJECTS)
	$(CPP) $(FLAGS) -I $(INCDIR) -shared $(OBJECTS) $(SRCDIR)/Spelunkbots.cpp -o Spelunkbots.dll

Spelunkbots.lib: Spelunkbots.dll
	$(DLLTOOL) -l Spelunkbots.lib Spelunkbots.dll
