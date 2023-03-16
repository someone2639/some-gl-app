# renderstate is 0x2202
# (GT_CULL_BACK | GT_SHADING_SMOOTH | GT_TEXTURE | GT_ZBUFFER)
gtStateTemplate = """\
gtStateL %s_State = {
    .renderState = %s,
    .textureState = 0x0,
    .vtxCount = %d,
    .triCount = %d,
};

"""

gtStateLTemplate = """\
gtStateL %s = {
    .renderState = %s,
    .textureState = 0x0,
    .vtxCount = %d,
    .triCount = %d,
};

"""

object2639_Template = """\
Object2639 %s_Obj = {
    .move = {0, 0, 0},
    .rotate = {0, 0, 0},
    .scale = {1.0, 1.0, 1.0},

    .segmentCount = %d, // gfxlist count
    .modelList = (gtGfx*)&%s, // gfxlist name

    .texturePath = "rom:/colored.sprite",
};
"""

vtxTemplate = "    {{ {%d, %d, %d}, 0, {%f, %f}, {0x%X, 0x%X, 0x%X, 0x%X} }},"

vtxName = ""




