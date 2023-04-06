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

void Object2639::render() {
    glPushMatrix();

    glTranslatef(this->move.x, this->move.y, this->move.z);

    // if ((this->rotate.x == 0.0f)
    //     && (this->rotate.y == 0.0f)
    //     && (this->rotate.z == 0.0f)
    // ) {

    // } else {
    // }
    // TODO: have it only apply rotation if nonzero
    // glRotatef(0, this->rotate.x, this->rotate.y, this->rotate.z);

    glScalef(this->scale.x, this->scale.y, this->scale.z);

    glBegin(GL_TRIANGLES);
    for (int i = 0; i < this->segmentCount; i++) {
        _processSegment(&this->modelList[i]);
    }
    glEnd();


    glPopMatrix();
}

void Object2639::renderList() {
    glPushMatrix();

    glTranslatef(this->move.x, this->move.y, this->move.z);

    glRotatef(this->rotate.x, 1, 0, 0);
    glRotatef(this->rotate.y, 0, 1, 0);
    glRotatef(this->rotate.z, 0, 0, 1);

    glScalef(this->scale.x, this->scale.y, this->scale.z);

    if (this->texturePath != NULL) {
        glBindTexture(GL_TEXTURE_2D, this->_texture[0]);
    }
    glCallList(this->segmentCount);

    glPopMatrix();
}

void Object2639::load() {
    this->_displaylist = glGenLists(1);

    if (this->texturePath != NULL) {
        this->_sprite = sprite_load(this->texturePath);
        assert(this->_sprite != NULL);

        glGenTextures(1, this->_texture);
        glBindTexture(GL_TEXTURE_2D, this->_texture[0]);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        surface_t surf = sprite_get_lod_pixels(this->_sprite, 0);
        glTexImageN64(GL_TEXTURE_2D, 0, &surf);
    }
    glNewList(this->segmentCount, GL_COMPILE);
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < this->segmentCount; i++) {
        _processSegment(&this->modelList[i]);
    }
    glEnd();
    glEndList();
}

Object2639::Object2639(std::initializer_list<float> m, std::initializer_list<float> r,
                       std::initializer_list<float> s, u32 segmentCount, gtGfx *modelList) {

    float *move = (float *) m.begin();
    float *rotate = (float *) r.begin();
    float *scale = (float *) s.begin();

    this->move = m;

    this->rotate.x = rotate[0];
    this->rotate.y = rotate[1];
    this->rotate.z = rotate[2];

    this->scale.x = scale[0];
    this->scale.y = scale[1];
    this->scale.z = scale[2];

    this->segmentCount = segmentCount;
    this->modelList = modelList;
}
