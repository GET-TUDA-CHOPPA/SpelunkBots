SRCDIR=SpelunkBot
INCDIR=SpelunkBot

BUILD=build

# ARCH=x86_64
ARCH=i686
CPP=$(ARCH)-w64-mingw32-g++
DLLTOOL=$(ARCH)-w64-mingw32-dlltool
FLAGS=-std=c++14 -DSPELUNKBOT_EXPORTS -static

OBJECTS= \
	$(BUILD)/BasicBotExampleOne.o \
	$(BUILD)/Bot.o \
	$(BUILD)/DiscoveryDan.o \
	$(BUILD)/FishBot.o \
	$(BUILD)/GoldDigger.o \
	$(BUILD)/IBot.o \
	$(BUILD)/Indie.o \
	$(BUILD)/JordanBot.o \
	$(BUILD)/NotSoSolidSnake.o \
	$(BUILD)/SeanBean.o \
	$(BUILD)/stdafx.o \
	$(BUILD)/MoveRightBot.o

$(BUILD)/%.o: $(SRCDIR)/%.cpp
	$(CPP) $(FLAGS) -I $(INCDIR) -shared -o $@ -c $<

Bot.dll: $(OBJECTS)
	$(CPP) $(FLAGS) -I $(INCDIR) -shared $(OBJECTS) $(SRCDIR)/dllmain.cpp -o Bot.dll

Bot.lib: Bot.dll
	$(DLLTOOL) -l Bot.lib Bot.dll

all: Bot.dll Bot.lib

clean:
	rm -- Bot.dll Bot.lib $(BUILD)/*
