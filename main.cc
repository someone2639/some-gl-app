#include <libdragon.h>
#include <rdpq_debug.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/gl_integration.h>

#include <string>

#include <tiny_gltf.h>
using namespace tinygltf;

#include "2639_defs.h"
#include "Object2639.h"
#include "Level2639.h"
#include "camera.h"
#include "Timer.h"

#define RDPQ_LOG_FLAG_SHOWTRIS       0x00000001
extern int __rdpq_debug_log_flags;

rdpq_font_t *fnt1;

struct controller_data __attribute__((aligned(8))) gPressedButtons;
struct controller_data __attribute__((aligned(8))) gHeldButtons;

static surface_t zbuffer;

void printDebug() {
    
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


void render(Level2639 &level) {
    static const GLubyte bgColor[] = {0, 255, 229, 255};
    // static const GLubyte bgColor[] = {0, 0, 0, 255};

    gl_context_begin();

    glClearColor(
        bgColor[0] / 255.0f,
        bgColor[1] / 255.0f,
        bgColor[2] / 255.0f,
        bgColor[3] / 255.0f
    );

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);




    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

// LIGHTS
// TODO: make into class
    glEnable(GL_LIGHTING);
    static u32 lighttoggle = 0;

    if (ContRead(0, L)) {
        lighttoggle ^= 1;
    }

    static const GLfloat environment_color[] = { 1.0f, 1.0f, 1.0f, 1.f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, environment_color);

// i try to do a point light
    // static const GLfloat sunset_color[] = {0.62, 0.45, 0.05, 1.0};
    // glLightfv(GL_LIGHT0, GL_DIFFUSE, sunset_color);
    static const GLfloat sunset_color[] = {1.0, 1.0, 1.0, 1.0};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, sunset_color);

    // some tutorial told me the 4th value needs to be nonzero
    static const GLfloat light_pos[] = {0, 50.0f, 0, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

    float light_radius = 100.0f;
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 2.0f/light_radius);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 1.0f/(light_radius*light_radius));
    // static GLfloat sunset_diffuse[] = {0.62, 0.45, 0.05, 1.0};
    // glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, sunset_diffuse);
        
    if (lighttoggle) {
        glEnable(GL_LIGHT0);
    } else {
        glDisable(GL_LIGHT0);
        // glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, sunset_diffuse);
    }
    // static const GLfloat difLight0[4] = {0.5f, 0.5f, 0.5f, 1.0f};
    // glLightfv(GL_LIGHT0, GL_DIFFUSE, difLight0);

    glRotatef(0, 0, 1, 0);

    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_BLEND);

// LEVEL UPDATE
    level.update();

    gl_context_end();
}

int main() {
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
    glEnable(GL_MULTISAMPLE_ARB);
    glDisable(GL_MATRIX_PALETTE_ARB);


    fnt1 = rdpq_font_load("rom:/Pacifico.font64");
    // Level2639 level("rom:/alphatest2.glb");
    // Level2639 level("rom:/BOB_gltf_test.glb");
    // Level2639 level("rom:/cube.gltf");
    // Level2639 level("rom:/human_high.gltf");
    Level2639 level("rom:/BOB_gltf.gltf");
    // Level2639 level("rom:/human_low.gltf");

    Timer renderTimer = Timer::RegisterTimer("Render a Cube");
    Timer displayTimer = Timer::RegisterTimer("Display");
        // renderTimer.start();

    while (1) {
        controller_scan();
        gPressedButtons = get_keys_pressed();
        gHeldButtons = get_keys_down();

        displayTimer.start();

        surface_t *disp = display_get();
        rdpq_attach(disp, &zbuffer);


        // rdpq_debug_log(true);
        // __rdpq_debug_log_flags = RDPQ_LOG_FLAG_SHOWTRIS;
        render(level);
        // rdpq_debug_log(false);

        // rdpq_debug_stop();

        renderTimer.end();

        // rdpq_font_begin(RGBA32(0xED, 0xAE, 0x49, 0xFF));
        // printDebug();
        // printTimers();
        // rdpq_font_end();


        rdpq_detach_show();

        displayTimer.end();

        // break;
    }
}