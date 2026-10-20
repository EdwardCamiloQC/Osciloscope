#==================================================
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# COLORS
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#==================================================
NORMAL = \033[0m
BLACK = \033[0,30m
RED = \033[0;31m
GREEN = \033[0;32m
ORANGE = \033[0;33m
BLUE = \033[0;34m
PURPLE = \033[0;35m
CYAN = \033[0;36m
LIGHT_GRAY = \033[0;37m
DARK_GRAY = \033[1;30m
LIGHT_RED = \033[1;31m
LIGHT_GREEN = \033[1;32m
YELLOW = \033[1;33m
LIGHT_BLUE = \033[1;34m
LIGHT_PURPLE = \033[1;35m
LIGHT_CYAN = \033[1;36m
WHITE = \033[1;37m

#==================================================
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# PATHS
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#==================================================
BUILD_DIR := build
SRC_DIR = ./src

VPATH =	$(SRC_DIR):\
		$(SRC_DIR)/domain/signals/:\
		$(SRC_DIR)/domain/VAOs/:\
		$(SRC_DIR)/domain/grids/:\
		$(SRC_DIR)/application/:\
		$(SRC_DIR)/infrastructure/:\
		$(SRC_DIR)/infrastructure/adapters/:\
		$(SRC_DIR)/frameworksAndDrivers/deviceInspector/:\
		$(SRC_DIR)/frameworksAndDrivers/gui/:\
		$(SRC_DIR)/frameworksAndDrivers/persistence/BBDD:\
		$(SRC_DIR)/frameworksAndDrivers/persistence/systemFile:\
		$(SRC_DIR)/frameworksAndDrivers/serialPort/:\
		$(SRC_DIR)/frameworksAndDrivers/shaderCompiler/:\
		$(SRC_DIR)/common/math/:\
		$(SRC_DIR)/common/dataStructures/:

#==================================================
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# VARIABLES
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#==================================================
PROJECT = $(notdir $(shell pwd))
HOST_ARCH := $(shell dpkg --print-architecture)
SYSROOT = /
RUN_EXECUTABLE := $(BUILD_DIR)/$(PROJECT)
COMPILER := g++
DEBUGER := gdb

ARCH ?= amd64
INSPECTOR ?= none
CONSTRUCTION ?= debug
GTK ?= gtk4

#==================================================
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# BUILD_DIR (GTK, ARCHITECTURE, INSPECTOR)
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#==================================================
ifeq ($(GTK), gtk3)
	ifeq ($(ARCH), amd64)
		ifeq ($(INSPECTOR), none)
			BUILD_DIR := $(BUILD_DIR)/$(GTK)/$(ARCH)/$(INSPECTOR)
		else ifeq ($(INSPECTOR), asan)
			BUILD_DIR := $(BUILD_DIR)/$(GTK)/$(ARCH)/$(INSPECTOR)
		else ifeq ($(INSPECTOR), valgrind)
			BUILD_DIR := $(BUILD_DIR)/$(GTK)/$(ARCH)/$(INSPECTOR)
		endif
	else ifeq ($(ARCH), i386)
		ifeq ($(INSPECTOR), none)
			BUILD_DIR := $(BUILD_DIR)/$(GTK)/$(ARCH)/$(INSPECTOR)
		else ifeq ($(INSPECTOR), asan)
			BUILD_DIR := $(BUILD_DIR)/$(GTK)/$(ARCH)/$(INSPECTOR)
		else ifeq ($(INSPECTOR), valgrind)
			BUILD_DIR := $(BUILD_DIR)/$(GTK)/$(ARCH)/$(INSPECTOR)
		endif
	else ifeq ($(ARCH), arm64)
		ifeq ($(INSPECTOR), none)
			BUILD_DIR := $(BUILD_DIR)/$(GTK)/$(ARCH)/$(INSPECTOR)
		else ifeq ($(INSPECTOR), asan)
			BUILD_DIR := $(BUILD_DIR)/$(GTK)/$(ARCH)/$(INSPECTOR)
		else ifeq ($(INSPECTOR), valgrind)
			BUILD_DIR := $(BUILD_DIR)/$(GTK)/$(ARCH)/$(INSPECTOR)
		endif
	else ifeq ($(ARCH), armhf)
		ifeq ($(INSPECTOR), none)
			BUILD_DIR := $(BUILD_DIR)/$(GTK)/$(ARCH)/$(INSPECTOR)
		else ifeq ($(INSPECTOR), asan)
			BUILD_DIR := $(BUILD_DIR)/$(GTK)/$(ARCH)/$(INSPECTOR)
		else ifeq ($(INSPECTOR), valgrind)
			BUILD_DIR := $(BUILD_DIR)/$(GTK)/$(ARCH)/$(INSPECTOR)
		endif
	endif
else ifeq ($(GTK), gtk4)
	ifeq ($(ARCH), amd64)
		ifeq ($(INSPECTOR), none)
			BUILD_DIR := $(BUILD_DIR)/$(GTK)/$(ARCH)/$(INSPECTOR)
		else ifeq ($(INSPECTOR), asan)
			BUILD_DIR := $(BUILD_DIR)/$(GTK)/$(ARCH)/$(INSPECTOR)
		else ifeq ($(INSPECTOR), valgrind)
			BUILD_DIR := $(BUILD_DIR)/$(GTK)/$(ARCH)/$(INSPECTOR)
		endif
	else ifeq ($(ARCH), i386)
		ifeq ($(INSPECTOR), none)
			BUILD_DIR := $(BUILD_DIR)/$(GTK)/$(ARCH)/$(INSPECTOR)
		else ifeq ($(INSPECTOR), asan)
			BUILD_DIR := $(BUILD_DIR)/$(GTK)/$(ARCH)/$(INSPECTOR)
		else ifeq ($(INSPECTOR), valgrind)
			BUILD_DIR := $(BUILD_DIR)/$(GTK)/$(ARCH)/$(INSPECTOR)
		endif
	else ifeq ($(ARCH), arm64)
		ifeq ($(INSPECTOR), none)
			BUILD_DIR := $(BUILD_DIR)/$(GTK)/$(ARCH)/$(INSPECTOR)
		else ifeq ($(INSPECTOR), asan)
			BUILD_DIR := $(BUILD_DIR)/$(GTK)/$(ARCH)/$(INSPECTOR)
		else ifeq ($(INSPECTOR), valgrind)
			BUILD_DIR := $(BUILD_DIR)/$(GTK)/$(ARCH)/$(INSPECTOR)
		endif
	else ifeq ($(ARCH), armhf)
		ifeq ($(INSPECTOR), none)
			BUILD_DIR := $(BUILD_DIR)/$(GTK)/$(ARCH)/$(INSPECTOR)
		else ifeq ($(INSPECTOR), asan)
			BUILD_DIR := $(BUILD_DIR)/$(GTK)/$(ARCH)/$(INSPECTOR)
		else ifeq ($(INSPECTOR), valgrind)
			BUILD_DIR := $(BUILD_DIR)/$(GTK)/$(ARCH)/$(INSPECTOR)
		endif
	endif
endif

#==================================================
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# COMPILER, DEBUGER, SYSROOT AND PKG_CONFIG_ENV (ARCHITECTURE)
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#==================================================
ifeq ($(ARCH), amd64)
	COMPILER = g++
	DEBUGER = gdb
	PKG_CONFIG_ENV = PKG_CONFIG_SYSROOT_DIR=$(SYSROOT) \
		PKG_CONFIG_LIBDIR=$(SYSROOT)/usr/lib/x86_64-linux-gnu/pkgconfig:$(SYSROOT)/usr/share/pkgconfig \
		PKG_CONFIG_PATH=""
else ifeq ($(ARCH), i386)
	COMPILER = i686-linux-gnu-g++
	DEBUGER = gdb -m32
	PKG_CONFIG_ENV = PKG_CONFIG_SYSROOT_DIR=$(SYSROOT) \
		PKG_CONFIG_LIBDIR=$(SYSROOT)/usr/lib/i386-linux-gnu/pkgconfig:$(SYSROOT)/usr/share/pkgconfig \
		PKG_CONFIG_PATH=""
else ifeq ($(ARCH), armhf)
	COMPILER = arm-linux-gnueabihf-g++
	DEBUGER = gdb-multiarch
	PKG_CONFIG_ENV = PKG_CONFIG_SYSROOT_DIR=$(SYSROOT) \
		PKG_CONFIG_LIBDIR=$(SYSROOT)/usr/lib/arm-linux-gnueabihf/pkgconfig:$(SYSROOT)/usr/share/pkgconfig \
		PKG_CONFIG_PATH=""
else ifeq ($(ARCH), arm64)
	COMPILER = aarch64-linux-gnu-g++
	DEBUGER = gdb-multiarch
	PKG_CONFIG_ENV = PKG_CONFIG_SYSROOT_DIR=$(SYSROOT) \
		PKG_CONFIG_LIBDIR=$(SYSROOT)/usr/lib/aarch64-linux-gnu/pkgconfig:$(SYSROOT)/usr/share/pkgconfig \
		PKG_CONFIG_PATH=""
endif

#==================================================
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# INSPECTOR
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#==================================================
ifeq ($(INSPECTOR), none)
	RUN_EXECUTABLE := $(BUILD_DIR)/$(PROJECT) 1> ./logs/out.log 2> ./logs/error.log
else ifeq ($(INSPECTOR), asan)
	RUN_EXECUTABLE := LSAN_OPTIONS=suppressions=lsan.supp $(BUILD_DIR)/$(PROJECT) 1> ./logs/out.log 2> ./logs/error.log
else ifeq ($(INSPECTOR), valgrind)
	RUN_EXECUTABLE := valgrind --leak-check=full $(BUILD_DIR)/$(PROJECT) 1> ./logs/out.log 2> ./logs/error.log
endif

#==================================================
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# SOURCES
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#==================================================
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
SOURCES += $(wildcard $(SRC_DIR)/domain/*/*.cpp)
SOURCES += $(wildcard $(SRC_DIR)/application/*.cpp)
SOURCES += $(wildcard $(SRC_DIR)/infrastructure/*.cpp)
SOURCES += $(wildcard $(SRC_DIR)/infrastructure/*/*.cpp)
SOURCES += $(wildcard $(SRC_DIR)/frameworksAndDrivers/*/*.cpp)
SOURCES += $(wildcard $(SRC_DIR)/frameworksAndDrivers/*/*/*.cpp)
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

INCLUDES = -I$(SRC_DIR)

LIBRARIES =

ifeq ($(CONSTRUCTION), debug)
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
else ifeq ($(CONSTRUCTION), release)
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

ifeq ($(ARCH), amd64)
	INCLUDES += $(shell $(PKG_CONFIG_ENV) pkg-config --cflags glib-2.0)
	INCLUDES += -I/usr/include/libserial
	INCLUDES += $(shell $(PKG_CONFIG_ENV) pkg-config --cflags libusb-1.0)
	ifeq ($(GTK), gtk3)
		INCLUDES += $(shell $(PKG_CONFIG_ENV) pkg-config --cflags gtk+-3.0)
		LIBRARIES += $(shell $(PKG_CONFIG_ENV) pkg-config --libs gtk+-3.0)
	else ifeq ($(GTK), gtk4)
		INCLUDES += $(shell $(PKG_CONFIG_ENV) pkg-config --cflags gtk4)
		LIBRARIES += $(shell $(PKG_CONFIG_ENV) pkg-config --libs gtk4)
	endif
	LIBRARIES += -lepoxy \
		-ludev \
		-lusb-1.0 \
		-lserial
else ifeq ($(ARCH), i386)
	INCLUDES += $(shell $(PKG_CONFIG_ENV) pkg-config --cflags glib-2.0)
	INCLUDES += -I/usr/include/libserial
	INCLUDES += $(shell $(PKG_CONFIG_ENV) pkg-config --cflags libusb-1.0)
	ifeq ($(GTK), gtk3)
		INCLUDES += $(shell $(PKG_CONFIG_ENV) pkg-config --cflags gtk+-3.0)
		LIBRARIES += $(shell $(PKG_CONFIG_ENV) pkg-config --libs gtk+-3.0)
	else ifeq ($(GTK), gtk4)
		INCLUDES += $(shell $(PKG_CONFIG_ENV) pkg-config --cflags gtk4)
		LIBRARIES += $(shell $(PKG_CONFIG_ENV) pkg-config --libs gtk4)
	endif
	LIBRARIES += -lepoxy \
		-ludev \
		-lusb-1.0 \
		-lserial
else ifeq ($(ARCH), arm64)
	INCLUDES += $(shell $(PKG_CONFIG_ENV) pkg-config --cflags glib-2.0)
	INCLUDES += -I/usr/include/libserial
	INCLUDES += $(shell $(PKG_CONFIG_ENV) pkg-config --cflags libusb-1.0)
	ifeq ($(GTK), gtk3)
		INCLUDES += $(shell $(PKG_CONFIG_ENV) pkg-config --cflags gtk+-3.0)
		LIBRARIES += $(shell $(PKG_CONFIG_ENV) pkg-config --libs gtk+-3.0)
	else ifeq ($(GTK), gtk4)
		INCLUDES += $(shell $(PKG_CONFIG_ENV) pkg-config --cflags gtk4)
		LIBRARIES += $(shell $(PKG_CONFIG_ENV) pkg-config --libs gtk4)
	endif
	LIBRARIES += -lepoxy \
		-ludev \
		-lusb-1.0 \
		-lserial
else ifeq ($(ARCH), armhf)
	INCLUDES += $(shell $(PKG_CONFIG_ENV) pkg-config --cflags glib-2.0)
	INCLUDES += -I/usr/include/libserial
	INCLUDES += $(shell $(PKG_CONFIG_ENV) pkg-config --cflags libusb-1.0)
	ifeq ($(GTK), gtk3)
		INCLUDES += $(shell $(PKG_CONFIG_ENV) pkg-config --cflags gtk+-3.0)
		LIBRARIES += $(shell $(PKG_CONFIG_ENV) pkg-config --libs gtk+-3.0)
	else ifeq ($(GTK), gtk4)
		INCLUDES += $(shell $(PKG_CONFIG_ENV) pkg-config --cflags gtk4)
		LIBRARIES += $(shell $(PKG_CONFIG_ENV) pkg-config --libs gtk4)
	endif
	LIBRARIES += -lepoxy \
		-ludev \
		-lusb-1.0 \
		-lserial
endif

#==================================================
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# CONSTRUCTION
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#==================================================
$(BUILD_DIR)/$(PROJECT): $(OBJECTS)
	@echo -e "$(LIGHT_CYAN)---------- START LINKING FOR $(ARCH) -----------\$(NORMAL)"
	$(COMPILER) -o $@ $^ $(FLAGS_LD) $(LIBRARIES)
	@echo -e "$(GREEN)"
	@cat ./resources/logos/logo.txt
	@echo -e "CONSTRUCTION SUCCESS $(NORMAL)"

$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	@echo -e "$(LIGHT_CYAN)COMPILATION($(ARCH))($(CONSTRUCTION)) -----> $(notdir $<)$(NORMAL)"
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
	rm -rf ./build
	clear

ifeq ($(ARCH), $(HOST_ARCH))

.PHONY: runProject
runProject:
	clear
	$(RUN_EXECUTABLE)

.PHONY: debugProject
debugProject:
	clear
	$(DEBUGER) $(RUN_EXECUTABLE)

.PHONY: genDocs
genDocs:
	doxygen Doxyfile

endif

.PHONY: package
package:
	@echo -e "$(LIGHT_CYAN)---------- PACKAGING FOR $(ARCH) -----------\$(NORMAL)"
	@mkdir -p ./pkg/$(GTK)/$(ARCH)/DEBIAN
	@mkdir -p ./pkg/$(GTK)/$(ARCH)/etc/$(PROJECT)
	@mkdir -p ./pkg/$(GTK)/$(ARCH)/usr/bin
	@mkdir -p ./pkg/$(GTK)/$(ARCH)/usr/lib/$(PROJECT)
	@mkdir -p ./pkg/$(GTK)/$(ARCH)/usr/share/applications
	@mkdir -p ./pkg/$(GTK)/$(ARCH)/usr/share/icons/hicolor/256x256/apps
	@mkdir -p ./pkg/$(GTK)/$(ARCH)/usr/share/$(PROJECT)/resources
	@cp -r ./resources/shaders ./pkg/$(GTK)/$(ARCH)/usr/share/$(PROJECT)/resources/
	@cp -r ./resources/styles ./pkg/$(GTK)/$(ARCH)/usr/share/$(PROJECT)/resources/
	@cp ./resources/logos/$(PROJECT).png ./pkg/$(GTK)/$(ARCH)/usr/share/icons/hicolor/256x256/apps/
	@cp ./build/$(GTK)/$(ARCH)/none/$(PROJECT) ./pkg/$(GTK)/$(ARCH)/usr/bin
	dpkg-deb --root-owner-group -Zgzip --build ./pkg/$(GTK)/$(ARCH) ./build/$(PROJECT)-$(GTK)-$(ARCH).deb
