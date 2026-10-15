# Makefile for the Linux Terminal E-Commerce System (educational C project)

CC      ?= gcc
CFLAGS  ?= -Wall -Wextra -Wpedantic -std=c11 -O2
TARGET  := ecommerce
SRC     := src/ecommerce.c

.PHONY: all clean run setup-dirs help

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

run: $(TARGET)
	./$(TARGET)

# Create the directory tree the program expects (BASE_PATH).
# Override BASE_PATH at compile time if you want a different location:
#   make BASE_PATH_OVERRIDE=/tmp/ecommerce
setup-dirs:
	mkdir -p /home/george-emad/Desktop/ecommerce/{admin,sellers,customers}
	touch /home/george-emad/Desktop/ecommerce/admin/logs.txt
	echo "Product details" > /home/george-emad/Desktop/ecommerce/sellers/product1.txt
	@echo "Demo data created under /home/george-emad/Desktop/ecommerce"

clean:
	rm -f $(TARGET) *.o

help:
	@echo "Targets:"
	@echo "  make            - build $(TARGET)"
	@echo "  make run        - build and run"
	@echo "  make setup-dirs - create demo directory tree"
	@echo "  make clean      - remove build artifacts"
