import sys, os

inVerts =False

inTris = False

vtxOffset = 0
shouldAddNow = False

countTris = 0
countVtx = 0

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

flag_fuzzer = 0

listOfVtxLoads = []
numTriLoads = 0

stateList = []

needMoreTris = False
triLoadCount = -1
#include "2639_defs.h"
#include "Object2639.h"
print("#include \"2639_defs.h\"")
print("#include \"Object2639.h\"")

print("extern Object2639 %s_Obj;" % sys.argv[2])
print("Object2639 *%s_objp = &%s_Obj;" % (sys.argv[2], sys.argv[2]))

with open(sys.argv[1]) as f:
    for line in f:
        if "Vtx" in line:
            inVerts = True
            countVtx = int(line.split("[")[1].split("]")[0])
            vtxName = line.split()[1].split("[")[0]
            outfile.write("static")
            outfile.write(line[:-1])
            continue

        if line[:-1] == "};" and inVerts:
            outfile.write(line[:-1])
            inVerts = False
            continue

        if inVerts:
            # TODO: convert texture coords to values between 0.0 and 1.0
            tokens = [int(i) for i in line.replace("{", " ")\
                         .replace("}", " ")\
                         .replace(",", " ")\
                         .split()]

            sAxis = (tokens[4] + 16) / 32.0 / 32.0
            tAxis = (tokens[5] + 16) / 32.0 / 32.0

            outfile.write(vtxTemplate % (
                tokens[0],
                tokens[1],
                tokens[2],

                sAxis,
                tAxis,

                tokens[6],
                tokens[7],
                tokens[8],
                tokens[9]
            ))

        if inTris:
            tokens = line.replace("(", " ")\
                         .replace(")", " ")\
                         .replace(",", " ")\
                         .split()

            if tokens[0] == "gsSPVertex":
                # outfile.write("// DBG VTX %d" % countTris)
                vtxName = ''.join(tokens[1:4])
                # if triLoadCount >= 0:
                stateList.append([
                    int(tokens[4]),
                    0,
                ])
                if len(stateList) == 2:
                    stateList[0][1] = countTris
                if countTris != 0:
                    # if stateList[-1][1] != 0:
                        # outfile.write("//IDIOT")
                    outfile.write("// %d" % countTris)
                    stateList[-2][1] = countTris
                # if len(stateList) > 1:
                #     stateList[-1][1] = countTris
                countTris = 0

                if triLoadCount >= 0:
                    outfile.write("};")
                needMoreTris = True
                triLoadCount += 1
                listOfVtxLoads.append(vtxName)
            if tokens[0] == "gsSP1Triangle":
                if needMoreTris:
                    outfile.write("gtTriN %s_tris_%d[] = {" % (sys.argv[2], triLoadCount))
                    needMoreTris = False
                countTris += 1
                outfile.write("    {%d, %d, %d}," % (
                    int(tokens[1]) + vtxOffset,
                    int(tokens[2]) + vtxOffset,
                    int(tokens[3]) + vtxOffset
                ))
            if tokens[0] == "gsSP2Triangles":
                countTris += 2
                # outfile.write("// 2tri %d" % countTris)
                if needMoreTris:
                    outfile.write("gtTriN %s_tris_%d[] = {" % (sys.argv[2], triLoadCount))
                    needMoreTris = False
                outfile.write("    {%d, %d, %d}, {%d, %d, %d}," % (
                    int(tokens[1]) + vtxOffset,
                    int(tokens[2]) + vtxOffset,
                    int(tokens[3]) + vtxOffset,

                    int(tokens[5]) + vtxOffset,
                    int(tokens[6]) + vtxOffset,
                    int(tokens[7]) + vtxOffset,
                ))
                flag_fuzzer += 2

        if "Gfx" in line:
            inTris = True
            numTriLoads+=1

        if line[:-1] == "};" and inTris:
            inTris = False
            outfile.write(line[:-1])
            break



stateList[-1][1] = countTris
for i, x in enumerate(stateList):
    if i == 0:
        outfile.write(gtStateTemplate % (
            sys.argv[2],
            "(GT_CULL_BACK | GT_SHADING_SMOOTH | GT_TEXTURE | GT_ZBUFFER)",
            x[0],
            x[1],
        ))
    else:
        outfile.write(gtStateLTemplate % (
            "%s_StateL_%d" % (sys.argv[2], i - 1),
            "(GT_CULL_BACK | GT_SHADING_SMOOTH | GT_TEXTURE | GT_ZBUFFER)",
            x[0],
            x[1],
        ))

gtGfxStart = "gtGfx %s_GfxList[] = {"

gtGfxTemplate = """\
    {
        (gtState *)%s,
        %s,
        %s,
    },"""

gtGfxEnd = "};"

outfile.write(gtGfxStart % sys.argv[2])
for i, x in enumerate(listOfVtxLoads):
    stateName = "&%s_StateL_%d" % (sys.argv[2], i - 1)
    if i == 0:
        stateName = "&%s_State" % sys.argv[2]
    outfile.write(gtGfxTemplate % (
        stateName,
        str(listOfVtxLoads[i]),
        "%s_tris_%d" % (sys.argv[2], i)
        )

    )

outfile.write(gtGfxEnd)

outfile.write(object2639_Template % (
    sys.argv[2],
    len(listOfVtxLoads),
    "%s_GfxList" % sys.argv[2]
))
