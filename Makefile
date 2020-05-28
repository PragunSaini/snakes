CXX := g++
CXX_FLAGS := -std=c++11 -ggdb
LIBRARIES := -lsfml-graphics -lsfml-window -lsfml-system
WARNING := -Wall -Wextra

BIN := bin
SRC := src
INCLUDE := include

EXECUTABLE := main
# SOURCES := $(shell find $(SRC) -name *.cpp -execdir basename '{}' ';')
SOURCES := $(shell find $(SRC) -name *.cpp | cut -d'/' -f2- )
OBJS := $(patsubst %.cpp, %.o, $(SOURCES))
DEPENDS := $(patsubst %.cpp, %.d, $(SOURCES))
DIRS := bin bin/NN bin/Genetic


all: setup $(BIN)/$(EXECUTABLE)
	@echo -e "\n\e[92mAll done\033[0m"

setup:
	@mkdir -p $(DIRS)

-include $(DEPENDS:%=$(BIN)/%)

$(BIN)/$(EXECUTABLE): $(OBJS:%=$(BIN)/%)
	@echo -e "\n\e[96mBuilding the game...\033[0m"
	@echo -e "\e[95m"$(CXX) $(WARNING) $(CXX_FLAGS) -o $@ $^ $(LIBRARIES)"\033[0m"
	@$(CXX) $(WARNING) $(CXX_FLAGS) -o $@ $^ $(LIBRARIES)

$(BIN)/%.o: $(SRC)/%.cpp Makefile
	@echo -e "\e[95m\b"$(CXX) $(WARNING) $(CXX_FLAGS) -MMD -MP -o $@ -c $< -I$(INCLUDE)"\033[0m"
	@$(CXX) $(WARNING) $(CXX_FLAGS) -MMD -MP -o $@ -c $< -I$(INCLUDE)


clean:
	# $(RM) $(BIN)/*
	@rm -rf $(BIN)/*
	@echo -e "\e[96mClean done\033[0m"


run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)
