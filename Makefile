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


CXXFLAGS += $(WARNINGS) $(INCLUDES) $(DEFINES)


c_src = $(wildcard *.c)
cc_src = $(wildcard *.cc)

O_FILES := $(c_src:%.c=$(BUILD_DIR)/%.o) $(cc_src:%.cc=$(BUILD_DIR)/%.o)

assets_png = $(wildcard assets/*.png)
assets_bin = $(wildcard assets/*.bin)
assets_ttf = $(wildcard assets/*.ttf)
assets_glb = $(wildcard assets/*.glb)
assets_gltf = $(wildcard assets/*.gltf)

assets_conv = $(addprefix filesystem/,$(notdir $(assets_png:%.png=%.sprite))) \
			  $(addprefix filesystem/,$(notdir $(assets_ttf:%.ttf=%.font64))) \
			  $(addprefix filesystem/,$(notdir $(assets_glb:%.glb=%.glb))) \
			  $(addprefix filesystem/,$(notdir $(assets_gltf:%.gltf=%.gltf))) \
			  $(addprefix filesystem/,$(notdir $(assets_bin:%.bin=%.bin)))

MKSPRITE_FLAGS ?=

# i understand why this isnt a default, but im still disappointed :(
$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cc
	@mkdir -p $(dir $@)
	@echo "    [CXX] $<"
	$(CXX) -c $(CXXFLAGS) -o $@ $<

all: gldemo.z64

test: gldemo.z64
	ares $<
test2: gldemo.z64
	simple64-gui $<

filesystem/%.font64: assets/%.ttf
	@mkdir -p $(dir $@)
	@echo "    [FONT] $@"
	@$(N64_MKFONT) $(MKFONT_FLAGS) -o filesystem "$<"

filesystem/%.glb: assets/%.glb
	@mkdir -p $(dir $@)
	@echo "    [GLB] $@"
	cp $< $@

filesystem/%.bin: assets/%.bin
	@mkdir -p $(dir $@)
	@echo "    [BIN] $@"
	cp $< $@

filesystem/%.gltf: assets/%.gltf
	@mkdir -p $(dir $@)
	@echo "    [GLTF] $@"
	cp $< $@

filesystem/%.sprite: assets/%.png
	@mkdir -p $(dir $@)
	@echo "    [SPRITE] $@"
	@$(N64_MKSPRITE) -f RGBA16 --compress -o "$(dir $@)" "$<"

$(BUILD_DIR)/gldemo.dfs: $(assets_conv)
$(BUILD_DIR)/gldemo.elf: $(O_FILES)

gldemo.z64: N64_ROM_TITLE="GL Demo"
gldemo.z64: $(BUILD_DIR)/gldemo.dfs

clean:
	rm -rf filesystem/ $(BUILD_DIR) gldemo.z64

-include $(wildcard $(BUILD_DIR)/*.d)

.PHONY: all clean

print-% : ; $(info $* is a $(flavor $*) variable set to [$($*)]) @true
