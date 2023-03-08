#include <libdragon.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/gl_integration.h>
#include <malloc.h>
#include <math.h>


#include "2639_defs.h"
#include "Object2639.h"
#include "camera.h"

static void VtxConv(Vtx *v) {
    // #define VCONV(s105) (((float)(s105))/32.0f)
    #define VCONV(s105) (((float)(s105)))
    glColor3f(
        v->v.cn[0] / 255.0f,
        v->v.cn[1] / 255.0f,
        v->v.cn[2] / 255.0f
    );
    // glVertex3f(
    //     VCONV(v->v.ob[0]),
    //     VCONV(v->v.ob[1]),
    //     VCONV(v->v.ob[2])
    // );
    glVertex3f(v->v.ob[0], v->v.ob[1], v->v.ob[2]);
}

static void Triangle(Vtx *vlist, gtTriN *t) {
    Vtx *v0 = &vlist[t->v0];
    Vtx *v1 = &vlist[t->v1];
    Vtx *v2 = &vlist[t->v2];


    VtxConv(v0);
    VtxConv(v1);
    VtxConv(v2);
}

static void TestTriangle(Vtx *vlist) {
    Vtx *v0 = &vlist[0];
    Vtx *v1 = &vlist[1];
    Vtx *v2 = &vlist[2];
    Vtx *v3 = &vlist[3];


    VtxConv(v0);
    VtxConv(v1);
    VtxConv(v2);
    VtxConv(v3);
}

static void _processSegment(gtGfx *g) {
    gtState *gs = g->obj.statep;

    u32 tc = gs->sp.triCount;

    gtTriN *ta = g->obj.trip;
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < tc; i++) {
        Triangle(g->obj.vtxp, &ta[i]);
        // TestTriangle(g->obj.vtxp);
    }
    glEnd();
}


void Object2639_Render(Object2639 *o) {
    glPushMatrix();

    glTranslatef(o->move.x, o->move.y, o->move.z);
    glRotatef(0, o->rotate.x, o->rotate.y, o->rotate.z);

    glScalef(o->scale.x, o->scale.y, o->scale.z);

    // glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i < o->segmentCount; i++) {
    // for (int i = 0; i < 20; i++) {
        // assume this does correct glVertex3f calls
        _processSegment(&o->modelList[i]);
    }


    glPopMatrix();
}
