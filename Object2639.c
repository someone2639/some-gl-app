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
    glColor3f(
        v->v.cn[0] / 255.0f,
        v->v.cn[1] / 255.0f,
        v->v.cn[2] / 255.0f
    );
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

static void _processSegment(gtGfx *g) {
    gtState *gs = g->obj.statep;

    u32 tc = gs->sp.triCount;

    gtTriN *ta = g->obj.trip;
    for (int i = 0; i < tc; i++) {
        Triangle(g->obj.vtxp, &ta[i]);
    }
}


void Object2639_Render(Object2639 *o) {
    glPushMatrix();

    glTranslatef(o->move.x, o->move.y, o->move.z);
    glRotatef(0, o->rotate.x, o->rotate.y, o->rotate.z);

    glScalef(o->scale.x, o->scale.y, o->scale.z);

    glBegin(GL_TRIANGLES);
    for (int i = 0; i < o->segmentCount; i++) {
        // assume this does correct glVertex3f calls
        _processSegment(&o->modelList[i]);
    }
    glEnd();


    glPopMatrix();
}

void Object2639_RenderList(Object2639 *o) {
    glPushMatrix();

    glTranslatef(o->move.x, o->move.y, o->move.z);
    glRotatef(0, o->rotate.x, o->rotate.y, o->rotate.z);

    glScalef(o->scale.x, o->scale.y, o->scale.z);

    // new difference
    glCallLists(o->segmentCount, GL_UNSIGNED_INT, o->lists);
    // glCallList(dl);

    glPopMatrix();
}

void Object2639_Register(Object2639 *o) {
    o->listStart = glGenLists(o->segmentCount);
    o->lists = malloc(sizeof(GLuint) * o->segmentCount);
    

    for (int i = 0; i < o->segmentCount; i++) {
        o->lists[i] = o->listStart + i;
        glNewList(o->listStart + i, GL_COMPILE);
        glBegin(GL_TRIANGLES);
        // this does correct glVertex3f calls
        _processSegment(&o->modelList[i]);
        glEnd();
        glEndList();
    }
}