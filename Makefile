BUILD_DIR := $(shell pwd)/build
EXE := "$(BUILD_DIR)/ringbuf.exe"
OBJS = $(patsubst ./source/%.c,./build/%.o,$(wildcard ./source/*.c))
OBJS += $(patsubst ./source/%.cpp,./build/%.opp,$(wildcard ./source/*.cpp))
INCLUDE := $(shell pwd)/include

.PHONY: all clean
all: $(EXE)

run: $(EXE)
	@$(EXE)

$(EXE): $(OBJS)
	@echo 'Linking source file(s) $(OBJS) together into $@...'
	@$(CXX) -o "$@" $(OBJS) -Wl,-rpath,`realpath $(BUILD_DIR)`
	@echo "Built $@"

# Build cpp
./build/%.opp: ./source/%.cpp ./build
	@echo 'Building source file $<...'
	@$(CXX) --std=c++2a -I $(INCLUDE) -fconcepts -g -c -o "$@" "$<" 
	@echo 'Built $@'
	@echo

# Build c
./build/%.o: ./source/%.c ./build
	@echo 'Building source file $<...'
	@$(CC) -std=gnu17 -I $(INCLUDE) -g -c -o "$@" "$<"
	@echo 'Built $@'
	@echo

./build:
	@mkdir -p ./build

clean:
	@rm -rf ./build/*.o* ./build/*.so ./build 
	@echo "All clean!"
