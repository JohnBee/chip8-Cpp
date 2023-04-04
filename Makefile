# .PHONY: all clean debug



# CC=g++
# CXXFLAGS=-fdiagnostics-color=always
# SRCDIR=./src
# IMGUIDIR=$(SRCDIR)/imgui
# BIN=./bin
# PROG=serialcommander.exe
# INC=-I./include
# LIBS = -L./lib 
# LIBS += -lglfw3 -lgdi32 -lopengl32 -limm32


# ifeq ($(BUILD),debug)   
# # "Debug" build - no optimization, and debugging symbols
# 	CXXFLAGS += -DDEBUG -g
# else
# # "Release" build - optimization, and no debug symbols
# 	CFLAGS += -O2 -s -DNDEBUG
# endif

# all: $(PROG)

# debug: 
# 	make "BUILD=debug"

# src = $(wildcard $(addsuffix /*.cpp,$(SRCDIR) $(IMGUIDIR)))

# obj = $(src:.c=.o)



# $(PROG): $(obj)
# 	$(CC) $(CXXFLAGS) $(INC) -o $(BIN)/$(PROG) $(obj) $(LIBS)

# %.o: %.c
# 	$(CC) $(CXXFLAGS) $(INC) -c -o $@ $^ $(LIBS)

# clean: 
# 	del $(BIN)/$(PROG)



.PHONY: all clean debug

MKDIR = mkdir -p


CXX=g++
INC=./include

EXE = serialcommander.exe
BIN=./bin
BUILD_DIR=./build
SRC_DIR = ./src

DIRS = $(BUILD_DIR) $(BIN)

SOURCES = $(SRC_DIR)/chip8.cpp
OBJS = $(addprefix $(BUILD_DIR)/, $(addsuffix .o, $(basename $(notdir $(SOURCES)))))
LIBS = -L./lib 
CXXFLAGS =-fdiagnostics-color=always
CXXFLAGS += -std=c++11 -I$(INC)
CXXFLAGS += -g -Wall -Wformat


ifeq ($(BUILD),debug)   
# "Debug" build - no optimization, and debugging symbols
	CXXFLAGS += -DDEBUG -g
else
# "Release" build - optimization, and no debug symbols
	CFLAGS += -O2 -s -DNDEBUG
endif

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