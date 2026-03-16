#==================================================
#==================================================
# COLORS
#==================================================
#==================================================
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

#==================================================
#==================================================
# PATHS
#==================================================
#==================================================
BUILD_DIR = build
SRC_DIR = ./src

VPATH =	$(SRC_DIR):\
		$(SRC_DIR)/modules/gui:\
		$(SRC_DIR)/modules/peripherals:\
		$(SRC_DIR)/utils/signal:\
		$(SRC_DIR)/utils/capturers:\
		$(SRC_DIR)/shaders

#==================================================
#==================================================
# VARIABLES
#==================================================
#==================================================
PROJECT = $(notdir $(shell pwd))
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
SOURCES += $(wildcard $(SRC_DIR)/modules/*/*.cpp)
SOURCES += $(wildcard $(SRC_DIR)/utils/*/*.cpp)
SOURCES += $(wildcard $(SRC_DIR)/shaders/*.cpp)
OBJECTS = $(addprefix $(BUILD_DIR)/, $(notdir $(SOURCES:.cpp=.o)))

COMPILER ?= g++
DEBUGER ?= gdb
BUILD ?= debug

GTK ?= gtk4

#==================================================
#==================================================
# CFLAGS, LDFLAGS AND INCLUDES
#==================================================
#==================================================
CFLAGS = 	-c \
			-fstack-protector-strong \
			-D_FORTIFY_SOURCE=2 \
			-std=c++17

FLAGS_LD =

INCLUDES =	-I$(SRC_DIR) \
			-I/usr/include/libserial

LIBRARIES =	-lepoxy \
			-lserial \
			-ludev \
			-lX11 \
#==================================================
#==================================================
#==================================================
#==================================================
ifeq ($(BUILD), debug)
	CFLAGS+= -Wall \
		-Wextra \
		-Wpedantic \
		-Werror \
		-g \
		-O0 \
		-fsanitize=address,undefined \
		-fno-omit-frame-pointer
	FLAGS_LD +=	-fsanitize=address,undefined
else ifeq ($(BUILD), release)
	CFLAGS = -O2 \
		-pipe \
		-fPIC \
		-fvisibility=hidden \
		-fPIE \
		-Wformat \
		-Wformat-security
	FLAGS_LD +=	-Wl,-z,relro \
		-Wl,-z,now \
		-Wl,--as-needed \
		-pie \
		-Wl,-rpath,'$$ORIGIN/../lib'
else
	$(error "No se selecionó si es para desarrollo o despliegue")
endif

ifeq ($(GTK), gtk3)
	INCLUDES += $(shell pkg-config --cflags gtk+-3.0)
	LIBRARIES += $(shell pkg-config --libs gtk+-3.0)
else ifeq ($(GTK), gtk4)
	INCLUDES += $(shell pkg-config --cflags gtk4)
	LIBRARIES += $(shell pkg-config --libs gtk4)
endif

#FLAGS_LD +=		'pkg-config --cflags --libs libsystemd'

#==================================================
#==================================================
# CONSTRUCTION
#==================================================
#==================================================
$(BUILD_DIR)/$(PROJECT): $(OBJECTS)
	@echo -e "$(LIGHT_CYAN)---------- START LINKING-----------\$(NORMAL)"
	$(COMPILER) -o $@ $^ $(FLAGS_LD) $(LIBRARIES)
	@echo -e "$(GREEN)"
	@cat logo.txt
	@echo -e "CONSTRUCTION SUCCESS $(NORMAL)"

$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	@echo -e "$(LIGHT_CYAN)COMPILATION($(BUILD)) -----> $(notdir $<)$(NORMAL)"
	$(COMPILER) $(CFLAGS) $(INCLUDES) $< -o $@

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

#==================================================
#==================================================
# FUNCTIONS
#==================================================
#==================================================
.PHONY: clean
clean:
	rm ./$(BUILD_DIR)/*
	clear

.PHONY:runTest
runTest:
	clear
	LSAN_OPTIONS=suppressions=lsan.supp ./$(BUILD_DIR)/$(PROJECT) 2> ./logs/error.log

.PHONY: runProject
runProject:
	clear
	./$(BUILD_DIR)/$(PROJECT)

.PHONY: runWithValgrind
runWithValgrind:
	clear
	valgrind --leak-check=full ./$(BUILD_DIR)/$(PROJECT)

.PHONY: debugProject
debugProject:
	clear
	gdb ./$(BUILD_DIR)/$(PROJECT)

.PHONY: docs
docs:
	doxygen Doxyfile
