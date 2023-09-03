
/**
 * Sample Level:
 * // level_X.c
 * Level _ = {
 *     .gltf = "assets/BoB.gltf",
 *     .bgm = "assets/BoB.wav64",
 *     .Objects = {
 *          "assets/BoB/Object1.gltf",
 *          "assets/BoB/Object2.gltf",
 *     },
 * };
 */

typedef struct _LevelStruct {
    char *gltf;
    char *bgm;
    char **Objects;
} Level;
