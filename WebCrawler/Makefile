TARGET = WebCrawler

BIN = bin

CC = gcc

CFLAGS = -Wextra -Iinclude
# Find the include path (usually /usr/include/postgresql)
CFLAGS += -I$(shell pg_config --includedir)
# Link the library
LDFLAGS = -L$(shell pg_config --libdir) -lpq

SRCS = $(wildcard src/*.c)

TESTS = $(wildcard tests/*.c)

build:
	mkdir -p bin
	$(CC) $(CFLAGS) $(SRCS) -o $(BIN)/$(TARGET) $(LDFLAGS)

test:
	@set -e; \
	for file in $(TESTS); do \
		echo "Compiling and running $$file..."; \
		$(CC) $(CFLAGS) $(filter-out src/main.c, $(SRCS)) $$file -o bin/temp_test $(LDFLAGS); \
		./bin/temp_test; \
	done
	@rm -f bin/temp_test

start: build
	@echo "Starting the web crawler..."
	./$(BIN)/$(TARGET)

clean:
	rm -rf $(BIN)
