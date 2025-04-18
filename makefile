TARGET = GG

SRC = Gamyui/main.cpp

CXX = g++
CXXFLAGS = -std=c++17 -Wall -I. -IGamyui

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^

run: $(TARGET)
	cd Gamyui && ./GG

clean:
	rm -f $(TARGET)