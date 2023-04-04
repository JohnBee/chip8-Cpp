.PHONY: all clean debug

MKDIR = mkdir -p


CXX=g++
INC=F:\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\include\

LOCAL_HEADERS=./include

EXE = chip8-emu.exe
BIN=./bin
BUILD_DIR=./build
SRC_DIR = ./src

DIRS = $(BUILD_DIR) $(BIN)

SOURCES = $(SRC_DIR)/chip8.cpp
OBJS = $(addprefix $(BUILD_DIR)/, $(addsuffix .o, $(basename $(notdir $(SOURCES)))))
LIBS = -L./lib 
CXXFLAGS =-fdiagnostics-color=always
CXXFLAGS += -std=c++11 -I$(INC) -I$(LOCAL_HEADERS)
CXXFLAGS += -g -Wall -Wformat


ifeq ($(BUILD),debug)   
# "Debug" build - no optimization, and debugging symbols
	CXXFLAGS += -DDEBUG -g
else
# "Release" build - optimization, and no debug symbols
	CFLAGS += -O2 -s -DNDEBUG
endif

ECHO_MESSAGE = "MinGW"
LIBS += -lmingw32 -lSDL2main -lSDL2

# CXXFLAGS += `pkg-config --cflags glfw3`
CFLAGS = $(CXXFLAGS)


##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------


$(BUILD_DIR)/%.o:$(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD_DIR)/%.o:$(IMGUI_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: dirs $(EXE)

$(EXE): $(OBJS)
	$(CXX) -o $(BIN)/$@ $^ $(CXXFLAGS) $(LIBS)

dirs:
	$(foreach var,$(DIRS),$(MKDIR) $(var);)
	
debug: 
	make "BUILD=debug"

clean:
	rm -f "$(BIN)/$(EXE)" $(OBJS)