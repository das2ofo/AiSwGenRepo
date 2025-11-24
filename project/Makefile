# Trova tutte le cartelle che contengono file .c
SUBDIRS := $(shell find . -type f -name '*.c' -exec dirname {} \; | sort -u)

# Target principale: compila ogni sottocartella
all:
	@echo ">>> Creato eseguibile $(TARGET) in $(CURDIR)"
	@for dir in $(SUBDIRS); do \
        echo ">>> Compilo $$dir"; \
        $(MAKE) -C $$dir; \
    done
