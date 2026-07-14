CXX = g++
CXXFLAGS = -std=c++17 -O2
CHUNK_SIZE ?= 100000

all: split_program merge_program

split_program: split.cpp
	$(CXX) $(CXXFLAGS) split.cpp -o split_program

merge_program: merge.cpp
	$(CXX) $(CXXFLAGS) merge.cpp -o merge_program

run: all
	rm -f run*.txt output.txt
	./split_program $(CHUNK_SIZE)
	./merge_program

clean:
	rm -f split_program merge_program run*.txt output.txt