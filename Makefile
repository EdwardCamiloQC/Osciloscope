#---------------------------------------------------
#---------------------------------------------------
#					COLORS
#---------------------------------------------------
#---------------------------------------------------
NORMAL = '\e[0m'
BLACK = '\e[0,30m'
RED = '\e[0;31m'
GREEN = '\e[0;32m'
ORANGE = '\e[0;33m'
BLUE = '\e[0;34m'
PURPLE = '\e[0;35m'
CYAN = '\e[0;36m'
LIGHT_GRAY = '\e[0;37m'
DARK_GRAY = '\e[1;30m'
LIGHT_RED = '\e[1;31m'
LIGHT_GREEN = '\e[1;32m'
YELLOW = '\e[1;33m'
LIGHT_BLUE = '\e[1;34m'
LIGHT_PURPLE = '\e[1;35m'
LIGHT_CYAN = '\e[1;36m'
WHITE = '\e[1;37m'

#---------------------------------------------------
#---------------------------------------------------
#					DIRECTIONS
#---------------------------------------------------
#---------------------------------------------------
MAIN_DIR = ./src/main
BUILD_DIR = build
SHADERS_DIR = ./src/shaders

VPATH = $(MAIN_DIR):$(SHADERS_DIR)

#---------------------------------------------------
#---------------------------------------------------
#					VARIABLES AND FLAGS
#---------------------------------------------------
#---------------------------------------------------
PROJECT = $(notdir $(shell pwd))
SOURCES = $(wildcard $(MAIN_DIR)/*.cpp)
SOURCES += $(wildcard $(SHADERS_DIR)/*cpp)
OBJECTS = $(addprefix $(BUILD_DIR)/, $(notdir $(SOURCES:.cpp=.o)))

COMPILER = g++
DEBUGER = gdb
FLAGS_CPP = -c -Wall
FLAGS_DEBUG = -g
FLAG_GTK3_C = `pkg-config --cflags gtk+-3.0`
FLAG_GTK3_L = `pkg-config --libs gtk+-3.0`
FLAG_GTK4_C = `pkg-config --cflags gtk4`
FLAG_GTK4_L = `pkg-config --libs gtk4`

INCLUDES = -I$(MAIN_DIR) -I$(SHADERS_DIR) -I/usr/include/libserial
LIBRARIES = -lepoxy -lX11 -lserial

#---------------------------------------------------
#---------------------------------------------------
#					CONSTRUCTION
#---------------------------------------------------
#---------------------------------------------------
$(BUILD_DIR)/$(PROJECT): $(OBJECTS)
	@echo -e "$(LIGHT_CYAN)---------- START LINKING-----------\$(NORMAL)"
	$(COMPILER) $(FLAG_GTK3_C) -o $@ $^ $(FLAG_GTK3_L) $(LIBRARIES) $(INCLUDES)
	@echo -e "$(GREEN) CONSTRUCTION SUCCESS $(NORMAL)"

$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	@echo -e "$(LIGHT_CYAN)----------COMPILATION $(notdir $<)----------$(NORMAL)"
	$(COMPILER) $(FLAGS_CPP) $(FLAGS_DEBUG) $(FLAG_GTK3_C) $(FLAG_GTK3_L) $(INCLUDES) $< -o $@

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

#---------------------------------------------------
#---------------------------------------------------
#					FUNCTIONS
#---------------------------------------------------
#---------------------------------------------------
clean:
	clear
	rm ./$(BUILD_DIR)/*

rebuild:
	make clean
	make

runProject:
	clear
	./$(BUILD_DIR)/$(PROJECT)

debugProject:
	clear
	gdb ./$(BUILD_DIR)/$(PROJECT)
