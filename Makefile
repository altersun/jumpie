BUILD_DIR := $(shell pwd)/build
C_EXES = $(patsubst ./source/%.c,./build/%.exe,$(wildcard ./source/*.c))
CXX_EXES += $(patsubst ./source/%.cpp,./build/%.exe,$(wildcard ./source/*.cpp))
INCLUDE := $(shell pwd)/include

.PHONY: all clean
all: $(C_EXES) $(CXX_EXES)

# Build cpp
./build/%.exe: ./source/%.cpp ./build
	@echo 'Building source file $<...'
	@$(CXX) --std=c++2a -I./ -fconcepts -g -o "$@" "$<" 
	@echo 'Built $@'
	@echo

# Build c
./build/%.exe: ./source/%.c ./build
	@echo 'Building source file $<...'
	@$(CC) -std=gnu17 -I./ -g -o "$@" "$<"
	@echo 'Built $@'
	@echo

./build:
	@mkdir -p ./build

clean:
	@rm -rf ./build/*.o* ./build/*.so ./build 
	@echo "All clean!"
