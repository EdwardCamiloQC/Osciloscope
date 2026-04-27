#==================================================
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# COLORS
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# PATHS
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#==================================================
BUILD_DIR = build
SRC_DIR = ./src

VPATH =	$(SRC_DIR):\
		$(SRC_DIR)/application/:\
		$(SRC_DIR)/adapters/:\
		$(SRC_DIR)/entities/grids/:\
		$(SRC_DIR)/entities/signals/:\
		$(SRC_DIR)/entities/VAOs/:\
		$(SRC_DIR)/infrastructure/serialPort/:\
		$(SRC_DIR)/infrastructure/shaderCompiler/:\
		$(SRC_DIR)/infrastructure/gui/:\
		$(SRC_DIR)/common/math/:\
		$(SRC_DIR)/common/dataStructures/:

#==================================================
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# VARIABLES
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#==================================================
PROJECT = $(notdir $(shell pwd))

RUN_EXECUTABLE = ./$(BUILD_DIR)/$(PROJECT)

COMPILER ?= g++
DEBUGER ?= gdb
BUILD ?= debug
INSPECTOR ?= asan

GTK ?= gtk4

#==================================================
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# INSPECTOR
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#==================================================
ifeq ($(INSPECTOR), asan)
	RUN_EXECUTABLE = LSAN_OPTIONS=suppressions=lsan.supp ./$(BUILD_DIR)/$(PROJECT) 1> ./logs/out.log 2> ./logs/error.log
else ifeq ($(INSPECTOR), valgrind)
	RUN_EXECUTABLE = valgrind --leak-check=full ./$(BUILD_DIR)/$(PROJECT) 1> ./logs/out.log 2> ./logs/error.log
endif

#==================================================
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# SOURCES
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#==================================================
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
SOURCES += $(wildcard $(SRC_DIR)/application/*.cpp)
SOURCES += $(wildcard $(SRC_DIR)/adapters/*.cpp)
SOURCES += $(wildcard $(SRC_DIR)/entities/*/*.cpp)
SOURCES += $(wildcard $(SRC_DIR)/infrastructure/*/*.cpp)
SOURCES += $(wildcard $(SRC_DIR)/common/*/*.cpp)

#==================================================
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# OBJECTS
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#==================================================
OBJECTS = $(addprefix $(BUILD_DIR)/, $(notdir $(SOURCES:.cpp=.o)))

#==================================================
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# CFLAGS, LDFLAGS AND INCLUDES
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#==================================================
CFLAGS = 	-c \
			-fstack-protector-strong \
			-D_FORTIFY_SOURCE=2 \
			-std=c++17 \
			-MMD \
			-MP

FLAGS_LD =

INCLUDES =	-I$(SRC_DIR) \
			-I/usr/include/libserial

LIBRARIES =	-lepoxy \
			-lserial \
			-ludev \
			-lX11

ifeq ($(BUILD), debug)
	CFLAGS+= -Wall \
		-Wextra \
		-Wpedantic \
		-Werror \
		-g \
		-O0 \
		-fno-omit-frame-pointer
		ifeq ($(INSPECTOR), asan)
			CFLAGS += -fsanitize=address,undefined
			FLAGS_LD +=	-fsanitize=address,undefined
		endif
else ifeq ($(BUILD), release)
	CFLAGS += -O2 \
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
endif

ifeq ($(GTK), gtk3)
	INCLUDES += $(shell pkg-config --cflags gtk+-3.0)
	LIBRARIES += $(shell pkg-config --libs gtk+-3.0)
else ifeq ($(GTK), gtk4)
	INCLUDES += $(shell pkg-config --cflags gtk4)
	LIBRARIES += $(shell pkg-config --libs gtk4)
endif

#==================================================
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# CONSTRUCTION
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#==================================================
$(BUILD_DIR)/$(PROJECT): $(OBJECTS)
	@echo -e "$(LIGHT_CYAN)---------- START LINKING-----------\$(NORMAL)"
	$(COMPILER) -o $@ $^ $(FLAGS_LD) $(LIBRARIES)
	@echo -e "$(GREEN)"
	@cat ./resources/logos/logo.txt
	@echo -e "CONSTRUCTION SUCCESS $(NORMAL)"

$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	@echo -e "$(LIGHT_CYAN)COMPILATION($(BUILD)) -----> $(notdir $<)$(NORMAL)"
	$(COMPILER) $(CFLAGS) $(INCLUDES) $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

#==================================================
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# DEPENDENCIES
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#==================================================
DEP_FILES = $(OBJECTS:.o=.d)
-include $(DEP_FILES)

#==================================================
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# TAGS
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#==================================================
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
	clear

.PHONY:runTest
runTest:
	clear
	$(RUN_EXECUTABLE)

.PHONY: runDeployment
runDeployment:
	clear
	./$(BUILD_DIR)/$(PROJECT)

.PHONY: debugProject
debugProject:
	clear
	gdb ./$(BUILD_DIR)/$(PROJECT)

.PHONY: docs
docs:
	doxygen Doxyfile
