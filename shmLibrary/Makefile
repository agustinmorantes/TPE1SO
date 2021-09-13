include ../Makefile.inc

SOURCES := $(wildcard *.c)
OBJECTS_DIR:=obj
OBJECTS := $(SOURCES:%.c=$(OBJECTS_DIR)/%.o)

all: $(OBJECTS)

$(OBJECTS_DIR)/%.o : %.c $(OBJECTS_DIR)
	$(COMPILER) $< -o $@ $(COMPILER_FLAGS)

$(OBJECTS_DIR):
	mkdir -p $(OBJECTS_DIR)

clean:
	@rm -rf $(OBJECTS) $(TARGET) $(OBJECTS_DIR)

.PHONY: all clean
