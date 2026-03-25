
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++20

collections: main.cpp
	$(CXX) $(CXXFLAGS) -o collections main.cpp

clean:
	rm -f collections