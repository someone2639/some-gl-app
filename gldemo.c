#include <libdragon.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/gl_integration.h>
#include <malloc.h>
#include <math.h>


#include "2639_defs.h"
#include "Object2639.h"
#include "camera.h"

rdpq_font_t *fnt1;

struct controller_data gPressedButtons;
struct controller_data gHeldButtons;

static surface_t zbuffer;

static const GLfloat environment_color[] = { 0.1f, 0.03f, 0.2f, 1.f };

void load_texture(GLenum target, sprite_t *sprite) {
    surface_t surf = sprite_get_lod_pixels(sprite, 0);
    glTexImageN64(target, 0, &surf);
}

GLuint tex[1];

sprite_t *load_tex(char *path) {
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


void draw_quad() {
    glPushMatrix();
    glTranslatef(0, 325, 1000);
    glScalef(32, 32, 32);

    // // glRotatef(0, 0, 1, 0);

    glBegin(GL_TRIANGLE_STRIP);
        glNormal3f(0, 1, 0);
        glTexCoord2f(0, 0);
        
        glColor3f(1, 0, 0);
        glVertex3f(-0.5f, 0, -0.5f);

        glTexCoord2f(0, 1);
        glColor3f(0, 1, 0);
        glVertex3f(-0.5f, 0, 0.5f);

        glTexCoord2f(1, 0);
        glColor3f(0, 0, 1);
        glVertex3f(0.5f, 0, -0.5f);

        glTexCoord2f(1, 1);
        glColor3f(1, 1, 1);
        glVertex3f(0.5f, 0, 0.5f);
    glEnd();


    glPopMatrix();
}

extern Object2639 Title_Obj;
    extern Object2639 Test_Obj;

GLuint _list;

void dbg_drawtex(u32 x, u32 y, char *path) {
    sprite_t *sprite = load_tex("rom:/vend1.sprite");
    // debug sprite blit
    surface_t tex = sprite_get_pixels(sprite);
    // uint16_t *tex_pal = sprite_get_palette(&tex);
    rdpq_set_mode_copy(false);
    // rdpq_tex_load_tlut(tex_pal, 0, 256);
    rdpq_tex_blit(&tex, x, y, NULL);
}

void render() {
    surface_t *disp;
    RSP_WAIT_LOOP(200) {
        if ((disp = display_lock())) {
            break;
        }
    }

    rdpq_attach(disp, &zbuffer);

    gl_context_begin();

    glClearColor(environment_color[0], environment_color[1], environment_color[2], environment_color[3]);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    float aspect_ratio = (float)display_get_width() / (float)display_get_height();
    float near_plane = 10.0f;
    float far_plane = 400.0f;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-near_plane*aspect_ratio, near_plane*aspect_ratio, -near_plane, near_plane, near_plane, far_plane);


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    void CameraUpdate();
    CameraUpdate();
    // gluLookAt(
    //     -10, 0, 0,
    //     0, 0, 0,
    //     0, 1, 0);
    glRotatef(0, 0, 1, 0);
    // glScalef(2, 2, 2);

    // color part (not relevant)
    // glEnable(GL_COLOR_MATERIAL);

    // glEnable(GL_TEXTURE_2D);
    // glBindTexture(GL_TEXTURE_2D, tex[0]);


    // draw_quad();
    Object2639_RenderList(&Title_Obj);
    // Object2639_Render(&Test_Obj);



    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glDisable(GL_BLEND);

    gl_context_end();



        rdpq_font_begin(RGBA32(0xED, 0xAE, 0x49, 0xFF));

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


        char buf3[30];
        sprintf(buf3, "%d %d", ContRead(0, x), ContRead(0, y));

        rdpq_font_position(20, 50);
        rdpq_font_print(fnt1, buf);

        rdpq_font_position(20, 70);
        rdpq_font_print(fnt1, buf2);
        rdpq_font_position(20, 90);
        rdpq_font_print(fnt1, buf3);

        rdpq_font_end();


    rdpq_detach_show();
}

int main() {
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

    Object2639_Register(&Title_Obj);

    while (1) {
        controller_scan();
        gPressedButtons = get_keys_pressed();
        gHeldButtons = get_keys_down();


        render();
    }
}
