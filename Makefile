
ARCH?=x86
CC=gcc

ROOT_DIR=.

ifeq ($(ARCH),arm)
CC=gcc
BUILD_DIR=$(ROOT_DIR)/build_arm
else
CC=gcc
BUILD_DIR=$(ROOT_DIR)/build_x86
endif

SOURCE_FILE_DIR=$(ROOT_DIR)
INCLUDE_FILE_DIR=$(ROOT_DIR)

SOURCE_FILE=$(foreach dir,$(SOURCE_FILE_DIR),$(wildcard $(dir)/*.c))
INCLUDE_FILE=$(foreach dir,$(INCLUDE_FILE_DIR),$(wildcard $(dir)/*.h))

TARGET_NAME=pcm2wav
TARGET_DEPEND=$(patsubst %.c,$(BUILD_DIR)/%.o,$(notdir $(SOURCE_FILE)))
TARGET_INCLUDE=$(foreach dir,$(INCLUDE_FILE_DIR),$(patsubst %,-I%,$(dir)))
VPATH=$(SOURCE_FILE_DIR)

$(BUILD_DIR)/$(TARGET_NAME):$(TARGET_DEPEND)
	$(CC) $^ -o $@ $(TARGET_INCLUDE) -Wall -O0

$(BUILD_DIR)/%.o:%.c | creat_build
	$(CC) -c $< -o $@ $(TARGET_INCLUDE) -Wall -O0

.PHONY:clean creat_build
clean:
	rm -r $(BUILD_DIR)

creat_build:
	mkdir -p $(BUILD_DIR)