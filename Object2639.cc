#include <libdragon.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/gl_integration.h>
#include <malloc.h>
#include <math.h>

#include <vector>
#include <string>

#include <tiny_gltf.h>
using namespace tinygltf;


#include "2639_defs.h"
#include "glb_impl.h"
#include "Object2639.h"
#include "camera.h"
#include "swap.h"

std::vector<Object2639> objectPool;

void Object2639::initializeInternalParams() {
    this->_initialized = 0;
    this->_displaylist = 0x2639;
    this->_texture[0] = 0;
    this->texturePath = nullptr;
    this->_sprite = nullptr;
}

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
    for (u32 i = 0; i < this->segmentCount; i++) {
        _processSegment(&this->modelList[i]);
    }
    glEnd();


    glPopMatrix();
}

void Object2639::renderList() {
    glPushMatrix();

    if ((this->move.x == 0) && (this->move.y == 0) && (this->move.z == 0)) {
    } else {
        glTranslatef(this->move.x, this->move.y, this->move.z);
    }

    glRotatef(this->rotate.x, 1, 0, 0);
    glRotatef(this->rotate.y, 0, 1, 0);
    glRotatef(this->rotate.z, 0, 0, 1);

    glScalef(this->scale.x, this->scale.y, this->scale.z);

    // if (this->texturePath != nullptr) {
    //     glBindTexture(GL_TEXTURE_2D, this->_texture[0]);
    // }

    // assert((glIsList(this->_displaylist) == GL_TRUE));
    glCallList(this->_displaylist);

    glPopMatrix();
}

// void Object2639::load() {
//     GLuint aa = glGenLists(1);
//     assert(aa != 0);

//     this->_displaylist = aa;

//     if (this->texturePath != NULL) {
//         this->_sprite = sprite_load(this->texturePath);
//         assert(this->_sprite != NULL);

//         glGenTextures(1, this->_texture);
//         glBindTexture(GL_TEXTURE_2D, this->_texture[0]);

//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

//         surface_t surf = sprite_get_lod_pixels(this->_sprite, 0);
//         glTexImageN64(GL_TEXTURE_2D, 0, &surf);
//     }
//     glNewList(this->_displaylist, GL_COMPILE);
//     glBegin(GL_TRIANGLES);
//     for (u32 i = 0; i < this->segmentCount; i++) {
//         _processSegment(&this->modelList[i]);
//     }
//     glEnd();
//     glEndList();
// }

Object2639::Object2639(std::initializer_list<float> m, std::initializer_list<float> r,
                       std::initializer_list<float> s, u32 segmentCount, gtGfx *modelList) {

    this->move = m;
    this->rotate = r;
    this->scale = s;

    this->segmentCount = segmentCount;
    this->modelList = modelList;
}

Object2639::Object2639() {
    this->move = {0, 0, 0};
    this->rotate = {0, 0, 0};
    this->scale = {1, 1, 1};

    this->segmentCount = 0;
    this->modelList = 0;

    this->initializeInternalParams();
}

Object2639::Object2639(std::string glb) : Object2639() {
    std::string err, warn;
    Model model;

    bool ret = loader.LoadBinaryFromFile(&model, &err, &warn, glb);
    assert(warn.empty());
    assert(err.empty());
    assert(ret);

    GLuint aa = glGenLists(1);
    this->_displaylist = aa;

    glNewList(this->_displaylist, GL_COMPILE);
    assert(glGetError() == 0);
    glBegin(GL_TRIANGLES);

    for (Mesh &mes : model.meshes) {
        for (Primitive &prim : mes.primitives) {
            const Accessor& accessor = model.accessors[prim.attributes["POSITION"]];

            const BufferView& bufferView = model.bufferViews[accessor.bufferView];
            const Buffer& buffer = model.buffers[bufferView.buffer];

            float* positions = (float *)(&buffer.data[bufferView.byteOffset + accessor.byteOffset]);


            f32_swap_endianness(positions, accessor.count * 3);

            glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
            glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(3, GL_FLOAT, sizeof(float) * 3, positions);
            const Accessor &indexAccessor = model.accessors[prim.indices];
            const BufferView& indexBufferView = model.bufferViews[indexAccessor.bufferView];
            const Buffer& indexBuffer = model.buffers[indexBufferView.buffer];
            if (indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT
             || indexAccessor.componentType == TINYGLTF_COMPONENT_TYPE_SHORT
            ) {
                u16 *bb = (u16*)&indexBuffer.data[
                    indexBufferView.byteOffset + indexAccessor.byteOffset
                ];
                u16_swap_endianness(bb, indexAccessor.count);
            }

            glDrawElements(GL_TRIANGLES, indexAccessor.count, indexAccessor.componentType,
                &indexBuffer.data[
                    indexBufferView.byteOffset + indexAccessor.byteOffset
                ]
            );

            // debug
            u16 *bb = (u16*)&indexBuffer.data[
                indexBufferView.byteOffset + indexAccessor.byteOffset
            ];
            for (size_t i = 0; i < indexAccessor.count; i++) {
                // glVertex3f(
                //     positions[bb[i * 3 + 0]],
                //     positions[bb[i * 3 + 1]],
                //     positions[bb[i * 3 + 2]]
                // );
                this->_DI.emplace_back(std::tuple<float, float, float>(
                    positions[bb[i * 3 + 0]],
                    positions[bb[i * 3 + 1]],
                    positions[bb[i * 3 + 2]]
                ));
            }
            this->_D = indexAccessor.count;
        }
    }
    glEnd();
    glEndList();
    assert(glGetError() == 0);
    assert((glIsList(this->_displaylist) == GL_TRUE));
    assert(this->_displaylist == aa);
    assert((glIsList(aa) == GL_TRUE));
}

void Object2639::RegisterModel(std::string s) {

    objectPool.emplace_back(Object2639(s));
}

void Object2639::update() {
    if (this->_initialized == 0) {
        if (this->init != nullptr) {
            this->init(this);
        }

        this->_initialized = 1;
        assert((glIsList(this->_displaylist) == GL_TRUE));
        return;
    }

    if (this->loop != nullptr) {
        this->loop(this);
    }
    assert((glIsList(this->_displaylist) == GL_TRUE));
    this->renderList();
}


void UpdateObjects() {
    for (Object2639 &i : objectPool) {
        i.update();
    }
}

