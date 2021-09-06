include ../Makefile.inc

SOURCES := $(wildcard *.c)
OBJECTS_DIR:=obj
OBJECTS := $(SOURCES:%.c=$(OBJECTS_DIR)/%.o)
TARGET  := application.out

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(LINKER) $(LINKER_FLAGS) $(OBJECTS) -o $(TARGET)

$(OBJECTS_DIR)/%.o : %.c $(OBJECTS_DIR)
	$(COMPILER) $< $(COMPILER_FLAGS) -o $@

$(OBJECTS_DIR):
	mkdir -p $(OBJECTS_DIR)

clean:
	@rm -rf $(OBJECTS) $(TARGET) $(OBJECTS_DIR)

.PHONY: all clean
