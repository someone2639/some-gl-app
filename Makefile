ROM_NAME = Engine2639

NO_COL  := \\033[0m
RED     := \\033[0;31m
GREEN   := \\033[0;32m
BLUE    := \\033[0;34m
YELLOW  := \\033[0;33m
BLINK   := \\033[32;5m

BUILD_DIR=build

# WARNINGS = -Wall -Wextra
WARNINGS = 
INCLUDES = -Iinclude/ -I/usr/include/tinygltf/
DEFINES = -DTINYGLTF_NO_STB_IMAGE \
		  -DTINYGLTF_NO_STB_IMAGE_WRITE \
		  -DTINYGLTF_NO_EXTERNAL_IMAGE

include $(N64_INST)/include/n64.mk
N64_CFLAGS += $(INCLUDES)
N64_CFLAGS += -Wno-missing-braces
N64_CFLAGS += -Wno-int-conversion


CXXFLAGS += $(WARNINGS) $(INCLUDES) $(DEFINES) --std=c++23 -O2

# replaces assets/ with filesystem/ in a cleaner way
assetpipe = $(foreach file, $(1), $(addprefix filesystem/,$(subst assets/,,$(file))))

c_src = $(wildcard *.c)
cc_src = $(wildcard *.cc) $(wildcard newtext/*.cc)

O_FILES := $(c_src:%.c=$(BUILD_DIR)/%.o) $(cc_src:%.cc=$(BUILD_DIR)/%.o)

assets_png = $(shell find assets/ -name "*.png")
assets_bin = $(shell find assets/ -name "*.bin")
assets_ttf = $(shell find assets/ -name "*.ttf")
assets_glb = $(shell find assets/ -name "*.glb")
assets_gltf = $(shell find assets/ -name "*.gltf")
assets_collision = $(shell find assets/ -name "*.collision")
assets_sound = $(shell find assets/ -name "*.wav")
assets_lvl = $(shell find assets/ -name "*.lvl")

assets_conv = $(call assetpipe, $(assets_png:%.png=%.sprite)) \
			  $(call assetpipe, $(assets_ttf:%.ttf=%.font64)) \
			  $(call assetpipe, $(assets_glb:%.glb=%.glb)) \
			  $(call assetpipe, $(assets_gltf:%.gltf=%.gltf)) \
			  $(call assetpipe, $(assets_bin:%.bin=%.bin)) \
			  $(call assetpipe, $(assets_collision:%.collision=%.collision)) \
			  $(call assetpipe, $(assets_sound:%.wav=%.wav64)) \
			  $(call assetpipe, $(assets_lvl:%.lvl=%.lvl))

MKSPRITE_FLAGS ?=

# i understand why this isnt a default, but im still disappointed :(
$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cc
	@mkdir -p $(dir $@)
	@printf "    [CXX] $<\n"
	$(CXX) -c $(CXXFLAGS) -o $@ $<

all: $(ROM_NAME).z64

test: $(ROM_NAME).z64
	ares $<
test2: $(ROM_NAME).z64
	simple64-gui $<
test-pl: $(ROM_NAME).z64
	parallel-launcher $<
load: $(ROM_NAME).z64
	cp $< /run/media/$(USER)/CF62-9261/

filesystem/:
	mkdir -p $@

filesystem/%.font64: assets/%.ttf | filesystem/
	@mkdir -p $(dir $@)
	@printf "    [FONT] $(YELLOW)$<$(GREEN) -> $(BLUE)$@$(NO_COL)\n"
	@$(N64_MKFONT) $(MKFONT_FLAGS) -o $(dir $@) "$<"

filesystem/%.glb: assets/%.glb | filesystem/
	@mkdir -p $(dir $@)
	@printf "    [GLB] $(YELLOW)$<$(GREEN) -> $(BLUE)$@$(NO_COL)\n"
	cp $< $@

filesystem/%.lvl: assets/%.lvl | filesystem/
	@mkdir -p $(dir $@)
	@printf "    [LVL] $(YELLOW)$<$(GREEN) -> $(BLUE)$@$(NO_COL)\n"
	cp $< $@

filesystem/%.bin: assets/%.bin | filesystem/
	@mkdir -p $(dir $@)
	@printf "    [BIN] $(YELLOW)$<$(GREEN) -> $(BLUE)$@$(NO_COL)\n"
	cp $< $@

filesystem/%.collision: assets/%.collision | filesystem/
	@mkdir -p $(dir $@)
	@printf "    [COLLISION] $(YELLOW)$<$(GREEN) -> $(BLUE)$@$(NO_COL)\n"
	$(N64_CC) -c -xc $(N64_CFLAGS) -I include/ -o /tmp/${@F}.o $<
	$(N64_LD) -e 0 -Trodata=0x00000000 -o /tmp/${@F}.elf /tmp/${@F}.o
	$(N64_OBJCOPY) -S -j .rodata -O binary /tmp/${@F}.elf $@

filesystem/%.gltf: assets/%.gltf | filesystem/
	@mkdir -p $(dir $@)
	@printf "    [GLTF] $(YELLOW)$<$(GREEN) -> $(BLUE)$@$(NO_COL)\n"
	cp $< $@

filesystem/%.sprite: assets/%.png | filesystem/
	@mkdir -p $(dir $@)
	@printf "    [SPRITE] $(YELLOW)$<$(GREEN) -> $(BLUE)$@$(NO_COL)\n"
	@$(N64_MKSPRITE) -f RGBA16 --compress -o "$(dir $@)" "$<"

filesystem/%.wav64: assets/%.wav | filesystem/
	@mkdir -p $(dir $@)
	@printf "    [AUDIO] $(YELLOW)$<$(GREEN) -> $(BLUE)$@$(NO_COL)\n"
	@$(N64_AUDIOCONV) -o $(dir $@) $<

$(BUILD_DIR)/$(ROM_NAME).dfs: filesystem/ $(assets_conv)
$(BUILD_DIR)/$(ROM_NAME).elf: $(O_FILES)

$(ROM_NAME).z64: N64_ROM_TITLE="Engine2639"
$(ROM_NAME).z64: $(BUILD_DIR)/$(ROM_NAME).dfs

clean:
	rm -rf filesystem/ $(BUILD_DIR) $(ROM_NAME).z64

-include $(wildcard $(BUILD_DIR)/*.d)

.PHONY: all clean

print-% : ; $(info $* is a $(flavor $*) variable set to [$($*)]) @true
