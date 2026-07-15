CC = gcc

SRC = src/main.c src/disk.c

TARGET = minifs

all:
	$(CC) $(CFLAGE) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET) disk.img