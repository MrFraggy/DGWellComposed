TARGET_NAME = project3
PATH_BUILD = bin
PATH_SRC = src
PATH_OBJ = obj

LIBRARIES = -lsfml-graphics \
			-lsfml-system \
			-lsfml-window


PATH_INCLUDE =	-Iinclude \
				-Iextlib/include

PATH_LIBRARIES = -Lextlib/lib/x64

#lister les fichiers à compiler
OBJ_FILES = $(PATH_OBJ)/main.o

GCC_OPTS = -std=c++11 -Wall

.PHONY: all

all: $(TARGET_NAME)

$(TARGET_NAME): $(OBJ_FILES)
	g++ $(GCC_OPTS) -o $(PATH_BUILD)/$(TARGET_NAME) $(OBJ_FILES) $(PATH_LIBRARIES) $(LIBRARIES)

$(PATH_OBJ)/%.o: $(PATH_SRC)/%.cpp
	@echo "Compiling $<"
	g++ $(GCC_OPTS) -c $< -o $@ $(PATH_INCLUDE)

clean:
	rm -rf $(PATH_OBJ)/*.o
