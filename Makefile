MAIN_DIR = ./src/main
MAPS_DIR = ./src/mapsCode
OBJECTS_DIR = ./src/objectsCode
GLAD_DIR = ./src/include
SHADERS_DIR = ./src/shaders
EXTRA_DIR = ./src/extraCode
BUILD_DIR = build
MISLIBS_DIR = /home/edward/Documentos/CodigosC++/MisLibrerias
VPATH = $(MAIN_DIR):$(MAPS_DIR):$(OBJECTS_DIR):$(MISLIBS_DIR):$(GLAD_DIR):$(SHADERS_DIR)

INCLUDES = -I$(EXTRA_DIR) -I$(MAIN_DIR) -I$(MAPS_DIR) -I$(OBJECTS_DIR) -I$(MISLIBS_DIR) -I$(GLAD_DIR) -I$(SHADERS_DIR) -I/usr/include/assimp -I/usr/include/libserial
LIBRARIES = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lserial
#LIBRARIES += -L
COMPILER = g++
DEBUGER = gdb
FLAGSCPP = -c -Wall -g
FLAGSDEBUG = -g

PROJECT = $(notdir $(shell pwd))
SOURCES = $(wildcard $(MAIN_DIR)/*.cpp)
SOURCES += $(wildcard $(MAPS_DIR)/*.cpp)
SOURCES += $(wildcard $(OBJECTS_DIR)/*.cpp)
SOURCES += $(wildcard $(SHADERS_DIR)/shaders.cpp)
SOURCES += $(wildcard $(MISLIBS_DIR)/figure.cpp)
OBJECTS = $(addprefix $(BUILD_DIR)/, $(notdir $(SOURCES:.cpp=.o)))

$(BUILD_DIR)/$(PROJECT) : $(BUILD_DIR)/glad.o $(OBJECTS)
	$(info ---------------END COMPILATION---------------)
	$(info /-                                         -/)
	$(info ----------------START LINKING----------------)
	$(COMPILER) -o $@ $^ $(LIBRARIES) $(INCLUDES)

$(BUILD_DIR)/%.o : %.cpp | $(BUILD_DIR)
	$(COMPILER) $(FLAGSCPP) $(INCLUDES) $< -o $@

$(BUILD_DIR)/glad.o : $(GLAD_DIR)/glad.c
	$(COMPILER) $(FLAGSCPP) $(INCLUDES) $< -o $@

$(BUILD_DIR) :
	mkdir $(BUILD_DIR)

rebuild :
	make clean
	make

runProject :
	clear
	./$(BUILD_DIR)/$(PROJECT)

debugProject :
	$(DEBUGER) ./$(BUILD_DIR)/$(PROJECT)

info :
	$(info $(OBJECTS))

clean :
	clear
	rm ./$(BUILD_DIR)/*
