#include <libdragon.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/gl_integration.h>
#include <malloc.h>
#include <math.h>


#include "2639_defs.h"
#include "Object2639.h"
#include "camera.h"

// static void VtxConv(float v[3]) {
//     // glColor3f(
//     //     v->v.cn[0] / 255.0f,
//     //     v->v.cn[1] / 255.0f,
//     //     v->v.cn[2] / 255.0f
//     // );
//     glVertex3f(v[0], v[1], v[2]);
// }

// static void Triangle(float (*vlist)[3], gtTriN t) {
//     float *v0 = vlist[t[0]];
//     float *v1 = vlist[t[1]];
//     float *v2 = vlist[t[2]];


//     VtxConv(v0);
//     VtxConv(v1);
//     VtxConv(v2);
// }

static void _processSegment(gtGfx *g) {
    gtState *gs = g->obj.statep;


    glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
    glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);

    glEnableClientState(GL_VERTEX_ARRAY);
    // glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    // glEnableClientState(GL_NORMAL_ARRAY);
    // glEnableClientState(GL_COLOR_ARRAY);

    glVertexPointer(3, GL_SHORT, 10, g->obj.vtxp);

    GLubyte (*tris)[3] = g->obj.trip;
    glDrawElements(GL_TRIANGLES, gs->sp.triCount * 3, GL_UNSIGNED_BYTE, tris);
}

    // for (int i = 0; i < tc; i++) {
    //     Triangle(g->obj.vtxp, &tris[i]);
    // }

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
    // glCallLists(o->segmentCount, GL_UNSIGNED_INT, o->lists);
    glCallList(o->segmentCount);

    glPopMatrix();
}

// void Object2639_Register(Object2639 *o) {
//     o->listStart = glGenLists(o->segmentCount);
//     o->lists = malloc(sizeof(GLuint) * o->segmentCount);
    

//     for (int i = 0; i < o->segmentCount; i++) {
//         o->lists[i] = o->listStart + i;
//         glNewList(o->listStart + i, GL_COMPILE);
//         glBegin(GL_TRIANGLES);
//         // this does correct glVertex3f calls
//         _processSegment(&o->modelList[i]);
//         glEnd();
//         glEndList();
//     }
// }

void Object2639_Register(Object2639 *o) {
    // o->listStart = glGenLists(o->segmentCount);
    // o->lists = malloc(sizeof(GLuint) * o->segmentCount);
    
    o->listStart = glGenLists(1);

    glNewList(o->segmentCount, GL_COMPILE);
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < o->segmentCount; i++) {
        // glNewList(o->listStart + i, GL_COMPILE);
        // o->lists[i] = o->listStart + i;
        // this does correct glVertex3f calls
        _processSegment(&o->modelList[i]);
    }
    glEnd();
    glEndList();
}
