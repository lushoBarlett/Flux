SRC := src
BIN := bin
# TODO: choose libraries depending on the target machine
LIBS := -lglfw3 -lGL -ldl -lX11 -lpthread -lGLEW -lm
FLAGS := -Wall -g -std=c++20

.PHONY: flux
flux: $(BIN)/flux

$(BIN)/flux: $(wildcard $(SRC)/*)
	@mkdir -p bin
	$(CXX) $(SRC)/main.cpp $(SRC)/renderer.cpp -o $(BIN)/flux $(FLAGS) $(LIBS)
