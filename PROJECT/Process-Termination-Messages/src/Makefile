CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2

TARGET = process_monitor
TEST = termination_test

all: $(TARGET) $(TEST)

$(TARGET): main.c
	$(CC) $(CFLAGS) -o $(TARGET) main.c

$(TEST): termination_test.c
	$(CC) $(CFLAGS) -o $(TEST) termination_test.c

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET) $(TEST)

.PHONY: all run clean
