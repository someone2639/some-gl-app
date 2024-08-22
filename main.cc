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

enum {
    FONT_PACIFICO = 1,
};

#define RDPQ_LOG_FLAG_SHOWTRIS       0x00000001
extern int __rdpq_debug_log_flags;

rdpq_font_t *fnt1;

joypad_inputs_t __attribute__((aligned(8))) gPressedButtons;
joypad_buttons_t __attribute__((aligned(8))) gHeldButtons;

static surface_t zbuffer;

float gFPS;
void calculate_framerate(void) {
    static unsigned int curFrameTimeIndex = 0;
    static unsigned int frameTimes[30];
    unsigned int newTime = timer_ticks();
    unsigned int  oldTime = frameTimes[curFrameTimeIndex];
    frameTimes[curFrameTimeIndex] = newTime;

    curFrameTimeIndex++;
    if (curFrameTimeIndex >= 30) {
        curFrameTimeIndex = 0;
    }
    gFPS = (30 * 1000000.0f) / TIMER_MICROS(newTime - oldTime) / 2.0f;
}

void render(Level2639 &level) {
    static const GLubyte bgColor[] = {0, 255, 229};
    // static const GLubyte bgColor[] = {0, 0, 0, 255};

    gl_context_begin();

    glClearColor(
        bgColor[0] / 255.0f,
        bgColor[1] / 255.0f,
        bgColor[2] / 255.0f,
        1.0f
    );

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);




    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

// LIGHTS
// TODO: make into class
    glEnable(GL_LIGHTING);
    static u32 lighttoggle = 0;

    if (ContRead(0, l)) {
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

bool is_project_64() {
    return *((volatile u64*)0xb4000008u) == 0x00080008000C000Cull;
}

// Level2639 *level;

int main() {
    std::string err;
    std::string warn;
	debug_init_isviewer();
	debug_init_usblog();
    
    dfs_init(DFS_DEFAULT_LOCATION);

    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 3, GAMMA_NONE, ANTIALIAS_RESAMPLE_FETCH_ALWAYS);
    zbuffer = surface_alloc(FMT_RGBA16, display_get_width(), display_get_height());

    // audio_init(24000, 4);
    // mixer_init(2);  // Initialize up to 16 channels
    // mixer_ch_set_limits(0, 0, 32000, 0);
    timer_init();

    gl_init();

    joypad_init();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    glDisable(GL_LIGHTING);
    glEnable(GL_MULTISAMPLE_ARB);
    glDisable(GL_MATRIX_PALETTE_ARB);

    // fnt1 = rdpq_font_load("rom:/fonts/Pacifico.font64");
    // rdpq_text_register_font(1, fnt1);

    Level2639 level("rom:/levels/testBOB/");

    u32 beats = 0;

    int box_width = 320;
    int box_height = 240;
    rdpq_textparms_t myParms;
        myParms.line_spacing = 1;
        myParms.align = ALIGN_LEFT;
        myParms.valign = VALIGN_CENTER;
        myParms.width = box_width;
        myParms.height = box_height;
        myParms.wrap = WRAP_WORD;

    rdpq_font_t *fnt1 = rdpq_font_load("rom:/fonts/Pacifico.font64");

    rdpq_fontstyle_t myStyle;
        myStyle.color = RGBA32(0xED, 0xAE, 0x49, 0xFF);
    // rdpq_font_style(fnt1, 0, &myStyle);
    rdpq_text_register_font(FONT_PACIFICO, fnt1);



    while (1) {

        // displayTimer.start();
        calculate_framerate();

        // if (beats & 1) {
            // only do this at 30fps
            // TODO: move to a game tick
            joypad_poll();
            gPressedButtons = joypad_get_inputs(JOYPAD_PORT_1);
            gHeldButtons = joypad_get_buttons_pressed(JOYPAD_PORT_1);

            surface_t *disp = display_get();
            rdpq_attach(disp, &zbuffer);
            render(level);

            char text[50];
            sprintf(text, "FPS: %f;\n", gFPS);
            // debugf(text);


            disable_interrupts();
            int nbytes = rdpq_text_print(
                &myParms,
                FONT_PACIFICO,
                (320-box_width)/2,
                (240-box_height)/2,
                text
            ).utf8_text_advance;
            // int nbytes = rdpq_text_print(&myParms, 1, (320-box_width)/2, (240-box_height)/2, text);
            enable_interrupts();

            rdpq_detach_show();
        // }
        // else {
            // if (audio_can_write()) {        
            //     short *buf = audio_write_begin();
            //     mixer_poll(buf, audio_get_buffer_length());
            //     audio_write_end();
            // }
        // }

        beats ^= 1;
    }
}
