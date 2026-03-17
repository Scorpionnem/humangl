NAME :=	voxel


CXX :=	c++
CXXFLAGS :=	-g -MP -MMD -Wall -Wextra -Werror -std=c++17 # -fsanitize=address -fno-omit-frame-pointer
LFLAGS := -lSDL2 -lGL


INCLUDE_DIRS :=	inc/\
				external/glad\
				inc/Core\
				inc/Core/Resources\
				inc/Utils\
				inc/Math\
				inc/Math/Vec\
				inc/World\


SRCS :=	main\
		App\
		Core/Window\
		World/Chunk\
		Core/Resources/Shader\


INCLUDE_DIRS :=	$(addprefix -I, $(INCLUDE_DIRS))
SRCS :=	$(addprefix src/, $(SRCS))
SRCS :=	$(addsuffix .cpp, $(SRCS))

SRCS +=	external/glad/glad.cpp

OBJ_DIR :=	obj
OBJS =	$(SRCS:%.cpp=$(OBJ_DIR)/%.o)
DEPS =	$(SRCS:%.cpp=$(OBJ_DIR)/%.d)


compile:
	@make -j all --no-print-directory

all: $(NAME)

$(NAME): $(OBJS)
	@echo Compiling $(NAME)
	@$(CXX) $(CXXFLAGS) $(LFLAGS) $(INCLUDE_DIRS) -o $@ $^

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo Compiling $@
	@$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c $< -o $@

re: fclean compile

fclean: clean
	@echo Removed $(NAME)
	@rm -rf $(NAME)

clean:
	@echo Removed $(OBJ_DIR)
	@rm -rf $(OBJ_DIR)

.PHONY: all clean fclean re compile

-include $(DEPS)
