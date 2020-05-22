CXX := g++
CXX_FLAGS := -std=c++11 -ggdb
LIBRARIES := -lsfml-graphics -lsfml-window -lsfml-system
WARNING := -Wall -Wextra

BIN := bin
SRC := src
INCLUDE := include

EXECUTABLE := main
SOURCES := $(shell find $(SRC) -name *.cpp -execdir basename '{}' ';')
OBJS := $(patsubst %.cpp, %.o, $(SOURCES))
DEPENDS := $(patsubst %.cpp, %.d, $(SOURCES))


all: $(BIN)/$(EXECUTABLE)
	@echo -e "\n\e[92mAll done\033[0m"

-include $(DEPENDS:%=$(BIN)/%)

$(BIN)/$(EXECUTABLE): $(OBJS:%=$(BIN)/%)
	@echo -e "\n\e[96mBuilding the game...\033[0m"
	@echo -e "\e[95m"$(CXX) $(WARNING) $(CXX_FLAGS) -o $@ $^ $(LIBRARIES)"\033[0m"
	@$(CXX) $(WARNING) $(CXX_FLAGS) -o $@ $^ $(LIBRARIES)

$(BIN)/%.o: $(SRC)/%.cpp Makefile
	@echo -e "\e[95m\b"$(CXX) $(WARNING) $(CXX_FLAGS) -MMD -MP -o $@ -c $< -I$(INCLUDE)"\033[0m"
	@$(CXX) $(WARNING) $(CXX_FLAGS) -MMD -MP -o $@ -c $< -I$(INCLUDE)


clean:
	$(RM) $(BIN)/*
	@echo -e "\e[96mClean done\033[0m"


run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)
