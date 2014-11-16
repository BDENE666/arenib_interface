
CXX=g++
DEBUG=yes
STATIC=yes

EXEC=arenib_interface
SRC_DIR=src
OBJ_DIR=build
INC_DIR=include

SRC=$(wildcard $(SRC_DIR)/*.cpp) 
INC=$(wildcard $(INC_DIR)/*.hpp) 
OBJ=$(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
CXXFLAGS= -W -Wall -I$(INC_DIR) -D SFML_STATIC=1

OBJ+= resources.o
ifeq ($(DEBUG), yes)
  LDFLAGS= -lsfml-network-s-d -lsfml-graphics-s-d -lsfml-window-s-d -lsfml-system-s-d
  CXXFLAGES += -g
else
  LDFLAGS= -lsfml-network-s -lsfml-graphics-s -lsfml-window-s -lsfml-system-s
endif

all:$(EXEC)
ifeq ($(DEBUG), yes)
	@echo Generation en mode debug
else
	@echo Generation en mode release
endif


resources.o: resources.rc $(HEADER)
	windres resources.rc resources.o -v

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
