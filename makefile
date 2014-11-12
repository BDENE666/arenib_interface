
CXX=g++
DEBUG=no

EXEC=arenib_interface
SRC_DIR=src
OBJ_DIR=build
INC_DIR=include

SRC=$(wildcard $(SRC_DIR)/*.cpp) 
INC=$(wildcard $(INC_DIR)/*.hpp) 
OBJ=$(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
CXXFLAGS= -W -Wall -I$(INC_DIR)

ifeq ($(DEBUG), yes)
  LDFLAGS= -L"libwin32" -lsfml-network-s-d -lsfml-graphics-s-d -lsfml-window-s-d -lsfml-system-s-d
  CXXFLAGES += -g
else
  CXXFLAGS= -W -Wall -I$(INC_DIR)
  LDFLAGS= -L"libwin32" -lsfml-network-s -lsfml-graphics-s -lsfml-window-s -lsfml-system-s
endif

all:$(EXEC)
ifeq ($(DEBUG), yes)
	@echo Generation en mode debug
else
	@echo Generation en mode release
endif


build/%.o: $(SRC_DIR_CIBLE)/%.cpp $(INC_CIBLE) $(INC)
	@echo Compilation C++ $< to $@
	g++  -c $< -o $@ $(CXXFLAGS)

build/%.o: $(SRC_DIR)/%.cpp $(INC) $(INC_CIBLE)
	@echo Compilation C++ $< to $@
	g++  -c $< -o $@ $(CXXFLAGS)

$(EXEC):$(OBJ) $(OBJ_CIBLE)
	@echo -----LINKAGE-----
	$(CXX) -o $@ $(OBJ) $(OBJ_CIBLE) $(LDFLAGS)

.PHONY : clean run

run:
	$(EXEC)

clean:
	rm -rf $(OBJ_DIR)/*.o *.exe *~ */*~ $(CIBLE)
