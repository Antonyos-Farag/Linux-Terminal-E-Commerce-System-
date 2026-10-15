# Makefile for the Linux Terminal E-Commerce System (educational C project)

CC      ?= gcc
CFLAGS  ?= -Wall -Wextra -Wpedantic -std=c11 -O2

# Original program (preserved verbatim)
TARGET  := ecommerce
SRC     := src/ecommerce.c

# Cool edition (themed terminal UI — see docs/13-cool-ui.md)
TARGET_COOL := ecommerce-cool
SRC_COOL    := src/ecommerce_cool.c

.PHONY: all clean run run-cool setup-dirs help

all: $(TARGET) $(TARGET_COOL)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

$(TARGET_COOL): $(SRC_COOL)
	$(CC) $(CFLAGS) -o $(TARGET_COOL) $(SRC_COOL)

run: $(TARGET)
	./$(TARGET)

run-cool: $(TARGET_COOL)
	./$(TARGET_COOL)

# Create the directory tree the ORIGINAL program expects (hard-coded path).
# The cool edition auto-creates its own tree under $HOME/ecommerce
# (override with ECOMMERCE_HOME=... or -DBASE_PATH=...).
setup-dirs:
	mkdir -p /home/george-emad/Desktop/ecommerce/{admin,sellers,customers}
	touch /home/george-emad/Desktop/ecommerce/admin/logs.txt
	echo "Product details" > /home/george-emad/Desktop/ecommerce/sellers/product1.txt
	@echo "Demo data created under /home/george-emad/Desktop/ecommerce"

clean:
	rm -f $(TARGET) $(TARGET_COOL) *.o

help:
	@echo "Targets:"
	@echo "  make            - build both programs"
	@echo "  make run        - build + run the original"
	@echo "  make run-cool   - build + run the cool edition"
	@echo "  make setup-dirs - create demo data for the original"
	@echo "  make clean      - remove build artifacts"
