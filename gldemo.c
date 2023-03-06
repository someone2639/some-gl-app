#include <libdragon.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/gl_integration.h>
#include <malloc.h>
#include <math.h>

static surface_t zbuffer;

static const GLfloat environment_color[] = { 0.1f, 0.03f, 0.2f, 1.f };

void load_texture(GLenum target, sprite_t *sprite) {
    surface_t surf = sprite_get_lod_pixels(sprite, 0);
    glTexImageN64(target, 0, &surf);
}

GLuint tex[1];
void load_tex(char *path) {
    sprite_t *sprite = sprite_load(path);


    glGenTextures(1, tex);

    glBindTexture(GL_TEXTURE_2D, tex[0]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    load_texture(GL_TEXTURE_2D, sprite);
}


void draw_quad() {
    glBegin(GL_TRIANGLE_STRIP);
        glNormal3f(0, 1, 0);
        glTexCoord2f(0, 0);
        
        glVertex3f(-0.5f, 0, -0.5f);

        glTexCoord2f(0, 1);
        glVertex3f(-0.5f, 0, 0.5f);

        glTexCoord2f(1, 0);
        glVertex3f(0.5f, 0, -0.5f);

        glTexCoord2f(1, 1);
        glVertex3f(0.5f, 0, 0.5f);
    glEnd();
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
    float near_plane = 1.0f;
    float far_plane = 50.0f;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-near_plane*aspect_ratio, near_plane*aspect_ratio, -near_plane, near_plane, near_plane, far_plane);


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        1, 1, 0,
        0, 0, 0,
        0, 1, 0);
    glRotatef(0, 0, 1, 0);

    // static const GLfloat light_pos[][4] = {
    //     { 1, 0, 0, 0 },
    // };

    // static const GLfloat light_diffuse[][4] = {
    //     { 1.0f, 0.0f, 0.0f, 1.0f },
    // };

    // float light_radius = 10.0f;

    // glEnable(GL_LIGHT0);
    // glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse[0]);
    // glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 2.0f/light_radius);
    // glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 1.0f/(light_radius*light_radius));

    // glEnable(GL_LIGHTING);
    // GLfloat mat_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    // glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, mat_diffuse);

    // color part (not relevant)
    // glEnable(GL_COLOR_MATERIAL);
    // glColor3f(1, 0, 0);
    // texture part
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex[0]);

    draw_quad();

    glEnable(GL_CULL_FACE);
    glDisable(GL_BLEND);

    gl_context_end();

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
    load_tex("rom:/vend1.sprite");
    while (1) {
        render();
    }
}
