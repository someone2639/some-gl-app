#include <libdragon.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/gl_integration.h>
#include <malloc.h>
#include <math.h>

#include <string>

#include <tiny_gltf.h>
using namespace tinygltf;



#include "2639_defs.h"
#include "Object2639.h"
#include "camera.h"
#include "Timer.h"

rdpq_font_t *fnt1;

struct controller_data gPressedButtons;
struct controller_data gHeldButtons;

static surface_t zbuffer;

void load_texture(GLenum target, sprite_t *sprite) {
    surface_t surf = sprite_get_lod_pixels(sprite, 0);
    glTexImageN64(target, 0, &surf);
}

sprite_t *load_tex(GLuint *tex, char *path) {
    sprite_t *sprite = sprite_load(path);


    glGenTextures(1, tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    load_texture(GL_TEXTURE_2D, sprite);

    return sprite;
}

void get_tex(GLuint *tex, char *path) {
    sprite_t *sprite = sprite_load(path);


    glGenTextures(1, tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    load_texture(GL_TEXTURE_2D, sprite);
}

extern Object2639 Test_Obj;

void dbg_drawtex(u32 x, u32 y, char *path) {
    static GLuint tex[1];

    sprite_t *sprite = load_tex(tex, path);
    // debug sprite blit
    surface_t pxl = sprite_get_pixels(sprite);
    // uint16_t *tex_pal = sprite_get_palette(&tex);
    rdpq_set_mode_copy(false);
    // rdpq_tex_load_tlut(tex_pal, 0, 256);
    rdpq_tex_blit(&pxl, x, y, NULL);
}

void printDebug() {
    char buf[30];
    sprintf(buf, "%f %f %f",
        sCameraSpot.x,
        sCameraSpot.y,
        sCameraSpot.z
        );

    char buf2[30];
    sprintf(buf2, "%f %f %f",
        sCameraRPY.roll,
        sCameraRPY.pitch,
        sCameraRPY.yaw
        );


    extern std::vector<Object2639> objectPool;
    Object2639 &o = objectPool.back();
    char buf3[30];
    sprintf(buf3, "%lf %d", o._D, ContRead(0, y));

    rdpq_font_position(20, 50);
    rdpq_font_print(fnt1, buf);


    rdpq_font_position(20, 70);
    rdpq_font_print(fnt1, buf2);
    rdpq_font_position(20, 90);
    rdpq_font_print(fnt1, buf3);



    int k = 0;
    for (auto [v0, v1, v2] : o._DP) {
        char buf4[50];
        sprintf(buf4, "%f %f %f", v0, v1, v2);
        rdpq_font_position(20, 110 + k);
        rdpq_font_print(fnt1, buf4);
        k += 10;
    }

    k = 0;
    for (auto [v0, v1, v2] : o._DI) {
        char buf4[50];
        sprintf(buf4, "%f %f %f", v0, v1, v2);
        rdpq_font_position(100, 110 + k);
        rdpq_font_print(fnt1, buf4);
        k += 10;
    }
}

void printTimers() {
    int k = 0;

    for (Timer &tm : timerPool) {
        char buf[50];
        rdpq_font_position(100, 110 + k);

        sprintf(buf, "%s: %.3f ms", tm.name.c_str(), tm.elapsed_ms);
        rdpq_font_print(fnt1, buf);

        k += 10;
    }
}


void render() {
    static const GLubyte bgColor[] = {0, 255, 229, 255};

    gl_context_begin();

    glClearColor(
        bgColor[0] / 255.0f,
        bgColor[1] / 255.0f,
        bgColor[2] / 255.0f,
        bgColor[3] / 255.0f
    );

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    float aspect_ratio = (float)display_get_width() / (float)display_get_height();
    float near_plane = 80.0f;
    float far_plane = 800.0f;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-near_plane*aspect_ratio, near_plane*aspect_ratio, -near_plane, near_plane, near_plane, far_plane);


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    void CameraUpdate();
    CameraUpdate();

    glRotatef(0, 0, 1, 0);

    glEnable(GL_COLOR_MATERIAL);

    UpdateObjects();

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_BLEND);

    gl_context_end();
}

int main() {
    TinyGLTF _loader;
    std::string err;
    std::string warn;
	debug_init_isviewer();
	debug_init_usblog();
    
    dfs_init(DFS_DEFAULT_LOCATION);


    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 3, GAMMA_NONE, ANTIALIAS_RESAMPLE_FETCH_ALWAYS);
    zbuffer = surface_alloc(FMT_RGBA16, display_get_width(), display_get_height());

    gl_init();

    controller_init();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    glDisable(GL_LIGHTING);


    fnt1 = rdpq_font_load("rom:/Pacifico.font64");
    // bool ret = _loader.LoadBinaryFromFile(&model, &err, &warn, "rom:/untitled.glb"); // for binary glTF(.glb)
    // assert(warn.empty());
    // assert(err.empty());
    // assert(ret);

    Object2639::RegisterModel("rom:/BOB_gltf_test.glb");

    Timer renderTimer = Timer::RegisterTimer("Render a Cube");
    Timer displayTimer = Timer::RegisterTimer("Display");

    while (1) {
        controller_scan();
        gPressedButtons = get_keys_pressed();
        gHeldButtons = get_keys_down();

        displayTimer.start();

        surface_t *disp = display_get();
        rdpq_attach(disp, &zbuffer);

        renderTimer.start();
        render();
        renderTimer.end();

        rdpq_font_begin(RGBA32(0xED, 0xAE, 0x49, 0xFF));
        // printDebug();
        // printTimers();
        rdpq_font_end();
        rdpq_detach_show();

        displayTimer.end();
    }
}
