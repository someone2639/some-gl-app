BUILD_DIR=build

include $(N64_INST)/include/n64.mk
N64_CFLAGS += -Iinclude/
N64_CFLAGS += -Wno-missing-braces
N64_CFLAGS += -Wno-int-conversion

src = $(wildcard *.c)
assets_png = $(wildcard assets/*.png)
assets_ttf = $(wildcard assets/*.ttf)

assets_conv = $(addprefix filesystem/,$(notdir $(assets_png:%.png=%.sprite))) \
			  $(addprefix filesystem/,$(notdir $(assets_ttf:%.ttf=%.font64)))

MKSPRITE_FLAGS ?=

all: gldemo.z64

test: gldemo.z64
	ares $<

filesystem/%.font64: assets/%.ttf
	@mkdir -p $(dir $@)
	@echo "    [FONT] $@"
	@$(N64_MKFONT) $(MKFONT_FLAGS) -o filesystem "$<"

filesystem/%.sprite: assets/%.png
	@mkdir -p $(dir $@)
	@echo "    [SPRITE] $@"
	@$(N64_MKSPRITE) -f RGBA16 --compress -o "$(dir $@)" "$<"

$(BUILD_DIR)/gldemo.dfs: $(assets_conv)
$(BUILD_DIR)/gldemo.elf: $(src:%.c=$(BUILD_DIR)/%.o)

gldemo.z64: N64_ROM_TITLE="GL Demo"
gldemo.z64: $(BUILD_DIR)/gldemo.dfs

clean:
	rm -rf filesystem/ $(BUILD_DIR) gldemo.z64

-include $(wildcard $(BUILD_DIR)/*.d)

.PHONY: all clean

print-% : ; $(info $* is a $(flavor $*) variable set to [$($*)]) @true
