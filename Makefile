NAME :=	humanGL


CXX :=	c++
CXXFLAGS :=	-g -MP -MMD -Wall -Wextra -Werror -std=c++17 -O3 #-fsanitize=address -fno-omit-frame-pointer
LFLAGS := -lSDL2 -lGL

EXTERNAL_DIR := external

INCLUDE_DIRS :=	inc/\
				external/imgui/\
				/usr/include/SDL2/\
				external/glad\
				inc/Core\
				inc/Core/Resources\
				inc/Utils\
				inc/Rig\
				inc/Math\
				inc/Math/Vec\
				inc/Math/Mat\
				inc/World\
				glad


SRCS :=	main\
		App\
		Core/Window\
		Core/Camera\
		Rig/Part\
		Rig/Rig\
		Rig/Timeline\
		Core/Resources/Shader\


INCLUDE_DIRS :=	$(addprefix -I, $(INCLUDE_DIRS))
SRCS :=	$(addprefix src/, $(SRCS))
SRCS :=	$(addsuffix .cpp, $(SRCS))


SRCS +=	external/glad/glad.cpp

IMGUI := $(EXTERNAL_DIR)/imgui
IMGUI_SRCS_RAW =	imgui.cpp\
					imgui_draw.cpp\
					imgui_widgets.cpp\
					imgui_tables.cpp\
					imgui_demo.cpp\
					backends/imgui_impl_opengl3.cpp\
					backends/imgui_impl_sdl2.cpp
IMGUI_SRCS = $(addprefix $(IMGUI)/, $(IMGUI_SRCS_RAW))

SRCS +=	$(IMGUI_SRCS)

OBJ_DIR :=	obj
OBJS =	$(SRCS:%.cpp=$(OBJ_DIR)/%.o)
DEPS =	$(SRCS:%.cpp=$(OBJ_DIR)/%.d)

TPUT 					= tput -T xterm-256color
_RESET 					:= $(shell $(TPUT) sgr0)
_BOLD 					:= $(shell $(TPUT) bold)
_ITALIC 				:= $(shell $(TPUT) sitm)
_UNDER 					:= $(shell $(TPUT) smul)
_GREEN 					:= $(shell $(TPUT) setaf 2)
_YELLOW 				:= $(shell $(TPUT) setaf 3)
_RED 					:= $(shell $(TPUT) setaf 1)
_GRAY 					:= $(shell $(TPUT) setaf 8)
_PURPLE 				:= $(shell $(TPUT) setaf 5)

all:
	@/bin/time --format='$(_GREEN)(%es)$(_RESET) Done' make timed_all --no-print-directory

build:
	@/bin/time --format='$(_GREEN)(%es)$(_RESET) Done' make timed_build --no-print-directory

timed_all:
	@make -j clone_dependencies --no-print-directory --silent
	@make -j compile --no-print-directory --silent

clone_dependencies: imgui glad

compile: $(NAME)

BUILD_DIR := build/
ASSETS_DIR := assets/
PACK_NAME := ft_vox.tar.gz

timed_build: timed_all
	@mkdir -p $(BUILD_DIR)
	@echo 'Copying $(_BOLD)$(NAME)$(_RESET) to $(_BOLD)$(BUILD_DIR)$(_RESET)'
	@cp $(NAME) $(BUILD_DIR)
	@echo 'Copying $(_BOLD)$(ASSETS_DIR)$(_RESET) to $(_BOLD)$(BUILD_DIR)$(_RESET)'
	@cp -r $(ASSETS_DIR) $(BUILD_DIR)
	@echo 'Packing $(_BOLD)$(PACK_NAME)$(_RESET)'
	@/bin/time --format='$(_GREEN)(%es)$(_RESET) Finished packing $(_BOLD)$(PACK_NAME)$(_RESET)' tar --directory=$(BUILD_DIR) -czf $(BUILD_DIR)$(PACK_NAME) $(NAME) $(ASSETS_DIR)

$(EXTERNAL_DIR):
	@mkdir -p external

imgui: $(EXTERNAL_DIR)
	@if ls external | grep -q "imgui"; then \
		printf ""; \
	else \
		echo "Downloading $(_BOLD)imgui$(_RESET)";\
		git clone https://github.com/ocornut/imgui.git $(IMGUI);\
		echo "Downloaded $(_BOLD)imgui$(_RESET)";\
	fi

glad: $(EXTERNAL_DIR)
	@if ls external | grep -q "glad"; then\
		printf "";\
	else \
		echo "Downloading $(_BOLD)glad$(_RESET)";\
		mkdir $(EXTERNAL_DIR)/glad;\
		cd $(EXTERNAL_DIR)/glad;\
		mkdir glad;\
		cd glad;\
		curl https://raw.githubusercontent.com/Manualouest/42_postCC/refs/heads/ft_scop/libs/glad/glad.h --output glad.h;\
		cd ..;\
		curl https://raw.githubusercontent.com/Manualouest/42_postCC/refs/heads/ft_scop/libs/glad/glad.c --output glad.cpp;\
		echo "Downloaded $(_BOLD)glad$(_RESET)";\
	fi

$(NAME): $(OBJS)
	@echo 'Linking $(_BOLD)$(NAME)$(_RESET)'
	@/bin/time --format='$(_GREEN)(%es)$(_RESET) Linked $(_BOLD)$(NAME)$(_RESET)' $(CXX) $(CXXFLAGS) $(LFLAGS) $(INCLUDE_DIRS) -o $@ $^

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo 'Compiling $(_BOLD)$<$(_RESET)'
	@/bin/time --format='$(_GREEN)(%es)$(_RESET) Compiled $(_BOLD)$@$(_RESET)' $(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c $< -o $@

re: fclean all

fclean: clean
	@echo 'Removed $(_BOLD)$(NAME)$(_RESET)'
	@rm -rf $(NAME)

clean:
	@echo 'Removed $(_BOLD)$(OBJ_DIR)$(_RESET)'
	@echo 'Removed $(_BOLD)$(BUILD_DIR)$(_RESET)'
	@rm -rf $(OBJ_DIR)
	@rm -rf $(BUILD_DIR)

.PHONY: all clean fclean re compile build external

-include $(DEPS)
