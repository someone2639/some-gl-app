#include <libdragon.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/gl_integration.h>
#include <malloc.h>
#include <math.h>


#include "2639_defs.h"
#include "Object2639.h"
#include "camera.h"


static void Triangle(Vtx *vlist, gtTriN *t) {
    Vtx *v0 = &vlist[t->v0];
    Vtx *v1 = &vlist[t->v1];
    Vtx *v2 = &vlist[t->v2];


    glColor3f(
        v0->v.cn[0] / 255.0f,
        v0->v.cn[1] / 255.0f,
        v0->v.cn[2] / 255.0f
    );
    #define VCONV(s105) (((float)(s105))/(float)0x0020)
    glVertex3f(
        VCONV(v0->v.ob[0]),
        VCONV(v0->v.ob[1]),
        VCONV(v0->v.ob[2])
    );
    glVertex3f(
        VCONV(v1->v.ob[0]),
        VCONV(v1->v.ob[1]),
        VCONV(v1->v.ob[2])
    );
    glVertex3f(
        VCONV(v2->v.ob[0]),
        VCONV(v2->v.ob[1]),
        VCONV(v2->v.ob[2])
    );
}

static void _processSegment(gtGfx *g) {
    gtState *gs = g->obj.statep;

    u32 tc = gs->sp.triCount;

    for (int i = 0; i < tc; i++) {
        gtTriN *ta = g->obj.trip;
        Triangle(g->obj.vtxp, &ta[i]);
    }
}


void Object2639_Render(Object2639 *o) {
    glPushMatrix();

    glLoadIdentity();

    glScalef(o->scale.x, o->scale.y, o->scale.z);
    glRotatef(0, o->rotate.x, o->rotate.y, o->rotate.z);
    glTranslatef(o->move.x, o->move.y, o->move.z);

    glBegin(GL_TRIANGLES);
    for (int i = 0; i < o->segmentCount; i++) {
        _processSegment(&o->modelList[i]);
    }
    glEnd();


    glPopMatrix();
}
