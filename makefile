
CXX=g++
DEBUG=no
STATIC=yes

EXEC=arenib_interface
SRC_DIR=src
OBJ_DIR=build
INC_DIR=include

SRC=$(wildcard $(SRC_DIR)/*.cpp) 
SRC_ROBOTS=$(wildcard $(SRC_DIR)/robots/*.cpp)
INC=$(wildcard $(INC_DIR)/*.hpp) $(wildcard $(INC_DIR)/robots/*.hpp)
OBJ=$(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o) $(SRC_ROBOTS:$(SRC_DIR)/robots/%.cpp=$(OBJ_DIR)/robots/%.o)
CXXFLAGS= -W -Wall -I$(INC_DIR) -D SFML_STATIC=1 -static -lstdc++ -static-libgcc
LDFLAGS=-static -lstdc++ -static-libgcc

OBJ+= resources.o
ifeq ($(DEBUG), yes)
  LDFLAGS += -lsfml-network-s-d -lsfml-graphics-s-d -lsfml-window-s-d -lsfml-system-s-d
  CXXFLAGES += -g
else
  LDFLAGS += -lsfml-network-s -lsfml-graphics-s -lsfml-window-s -lsfml-system-s
  CXXFLAGES += -s -Os
endif

all:$(EXEC)
ifeq ($(DEBUG), yes)
	@echo Generation en mode debug
else
	@echo Generation en mode release
endif


resources.o: resources.rc $(HEADER)
	windres resources.rc resources.o -v

build/%.o: $(SRC_DIR)/%.cpp $(INC)
	@echo Compilation C++ $< to $@
	g++  -c $< -o $@ $(CXXFLAGS)

  
$(EXEC):$(OBJ)
	@echo -----LINKAGE-----
	$(CXX) -o $@ $(OBJ) $(LDFLAGS)

.PHONY : clean run

run:
	$(EXEC)

clean:
	rm -rf $(OBJ_DIR)/*.o *.exe *~ */*~ $(CIBLE)
