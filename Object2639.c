#include <libdragon.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/gl_integration.h>
#include <malloc.h>
#include <math.h>


#include "2639_defs.h"
#include "Object2639.h"
#include "camera.h"

static void _processSegment(gtGfx *g) {
    gtState *gs = g->obj.statep;


    glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
    glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);

    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, sizeof(Vtx), g->obj.vtxp);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_SHORT, sizeof(Vtx), g->obj.vtxp->v.tc);

    if (gs->sp.renderState & GT_TEXTURE) {
        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Vtx), g->obj.vtxp->v.cn);
    } else {
        glEnableClientState(GL_NORMAL_ARRAY);
        glNormalPointer(GL_BYTE, sizeof(Vtx), g->obj.vtxp->v.cn);
    }

    GLubyte (*tris)[3] = g->obj.trip;
    glDrawElements(GL_TRIANGLES, gs->sp.triCount * 3, GL_UNSIGNED_BYTE, tris);
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

    glCallList(o->segmentCount);

    glPopMatrix();
}

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
