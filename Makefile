.PHONY: all test run clean server client

#Compilation variables
CC = gcc
CFLAGS = -Wall -std=gnu11 -MMD -Wextra -ggdb
LDLIBS = -lreadline -lc

#Build directory
BUILD_DIR = build
SRC_DIR = src
SRC_DIR_SERVER = $(SRC_DIR)/server
SRC_DIR_CLIENT = $(SRC_DIR)/client
TEST_DIR = tests


#Executable name
TARGET_SERVER = server
TARGET_CLIENT = client
TARGET_TEST = $(TEST_DIR)/megaphone
TARGET_TEST_SERVER = $(TEST_DIR)/test_server
TARGET_TEST_CLIENT = $(TEST_DIR)/test_client

#Source files
SOURCES_SERVER =  $(filter-out $(SRC_DIR_SERVER)/$(TARGET_SERVER).c, \
		  $(wildcard $(SRC_DIR_SERVER)/*.c))
SOURCES_CLIENT = $(filter-out $(SRC_DIR_CLIENT)/$(TARGET_CLIENT).c, \
		 $(wildcard $(SRC_DIR_CLIENT)/*.c))
TEST_SOURCES = $(wildcard $(TEST_DIR)/*.c)

#Binaries names
OBJECTS_SERVER =  $(SOURCES_SERVER:%.c=$(BUILD_DIR)/%.o))
OBJECTS_CLIENT =  $(SOURCES_CLIENT:%.c=$(BUILD_DIR)/%.o))
TEST_OBJECTS = 	$(TEST_SOURCES:%.c=$(BUILD_DIR)/%.o)
TARGET_OBJECT_CLIENT = $(BUILD_DIR)/$(SRC_DIR_CLIENT)/$(TARGET_CLIENT).o
TARGET_OBJECT_SERVER = $(BUILD_DIR)/$(SRC_DIR_SERVER)/$(TARGET_SERVER).o


#Binaries dependencies
DEPENDENCIES = \
	       $(OBJECTS_SERVER:%.o=%.d) \
	       $(OBJECTS_CLIENT:%.o=%.d) \
	       $(TEST_OBJECTS:%.o=%.d) \
	       $(TARGET_OBJECT_CLIENT:%.o:%.d) \
	       $(TARGET_OBJECT_SERVER:%.o:%.d) 
		

#JOBS
all: $(TARGET)

run: $(TARGET)
	@./$(TARGET)

test: $(TEST_TARGET)
	@./$(TEST_TARGET)

-include $(DEPENDENCIES)

$(TARGET):  $(TARGET_OBJECT) $(OBJECTS)
	@$(CC) $(CFLAGS) $^ -o $(TARGET)  $(LDLIBS)

$(TEST_TARGET): $(TEST_OBJECTS) $(OBJECTS)
	@$(CC) $(CFLAGS) -o $(TEST_TARGET) $^



$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -o $@ -c $<

clean:
	@rm -rf $(BUILD_DIR)
	@rm -f $(TARGET) $(TEST_TARGET)

