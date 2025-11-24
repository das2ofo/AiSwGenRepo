# Nome eseguibile = nome della cartella padre (sub_folderX)
TARGET := $(notdir $(shell dirname $(CURDIR)))
SRCS   := $(wildcard *.c)

$(TARGET): $(SRCS)
	@gcc -Wall -Wextra -o $(TARGET) $(SRCS)
	@echo ">>> Creato eseguibile $(TARGET) in $(CURDIR)"

clean:
	@rm -f $(TARGET)