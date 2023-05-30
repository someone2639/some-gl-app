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

    if ((this->move.x == 0) && (this->move.y == 0) && (this->move.z == 0)) {
    } else {
        glTranslatef(this->move.x, this->move.y, this->move.z);
    }

    glRotatef(this->rotate.x, 1, 0, 0);
    glRotatef(this->rotate.y, 0, 1, 0);
    glRotatef(this->rotate.z, 0, 0, 1);

    glScalef(this->scale.x, this->scale.y, this->scale.z);

    if (this->texturePath != nullptr) {
        glBindTexture(GL_TEXTURE_2D, this->_texture[0]);
    }

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
    this->scale = {10, 10, 10};

    this->segmentCount = 0;
    this->modelList = 0;

    this->initializeInternalParams();

    this->init = nullptr;
    this->loop = nullptr;
}

Object2639::Object2639(std::string glb) : Object2639() {
    std::string err, warn;
    Model model;

    bool ret = loader.LoadBinaryFromFile(&model, &err, &warn, glb);
    assertf(err.empty(), err.c_str());
    assertf(warn.empty(), warn.c_str());
    assert(ret);

    GLuint aa = glGenLists(1);
    this->_displaylist = aa;

    glNewList(this->_displaylist, GL_COMPILE);
    assert(glGetError() == 0);
    glBegin(GL_TRIANGLES);

    for (Mesh &mes : model.meshes) {
        for (Primitive &prim : mes.primitives) {
            glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
            glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);

// Positions
            const Accessor& accessor = model.accessors[prim.attributes["POSITION"]];
            const BufferView& bufferView = model.bufferViews[accessor.bufferView];
            const Buffer &b = model.buffers[bufferView.buffer];

            f32 *positions = (f32 *)(&b.data[bufferView.byteOffset + accessor.byteOffset]);
            f32_swap_endianness(positions, accessor.count * 3);
            glEnableClientState(GL_VERTEX_ARRAY);
            glVertexPointer(3, GL_FLOAT, sizeof(f32) * 3, positions);

// Normals
            const Accessor& normalAccessor = model.accessors[prim.attributes["NORMAL"]];
            const BufferView& normalBufferView = model.bufferViews[normalAccessor.bufferView];
            const Buffer &normalBuf = model.buffers[bufferView.buffer];

            f32 *normals = (f32 *)(&normalBuf.data[
                normalBufferView.byteOffset + normalAccessor.byteOffset
            ]);
            f32_swap_endianness(normals, normalAccessor.count * 3);
            glEnableClientState(GL_NORMAL_ARRAY);
            glNormalPointer(GL_FLOAT, sizeof(f32) * 3, normals);

// Colors
            const Accessor& colorAccessor = model.accessors[prim.attributes["COLOR_0"]];
            const BufferView& colorBufferView = model.bufferViews[colorAccessor.bufferView];
            const Buffer &colorBuf = model.buffers[bufferView.buffer];

            u16 *colors = (u16 *)(&colorBuf.data[
                colorBufferView.byteOffset + colorAccessor.byteOffset
            ]);
            // u16_swap_endianness(colors, colorAccessor.count * 4);
            glEnableClientState(GL_COLOR_ARRAY);
            glColorPointer(4, GL_UNSIGNED_SHORT, sizeof(u16) * 4, colors);

// Texture Coords
            const Accessor& texcoordAccessor = model.accessors[prim.attributes["TEXCOORD_0"]];
            const BufferView& texcoordBufferView = model.bufferViews[texcoordAccessor.bufferView];
            const Buffer &texcoordBuf = model.buffers[bufferView.buffer];

            f32 *texcoords = (f32 *)(&texcoordBuf.data[
                texcoordBufferView.byteOffset + texcoordAccessor.byteOffset
            ]);
            f32_swap_endianness(texcoords, texcoordAccessor.count * 2);
            glEnableClientState(GL_TEXTURE_COORD_ARRAY);
            glTexCoordPointer(2, GL_FLOAT, sizeof(f32) * 2, texcoords);

// Triangle Indices
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

        this->_initialized = 1;
        assert((glIsList(this->_displaylist) == GL_TRUE));
        return;
    }

    if (this->loop != nullptr) {
        this->loop(this);
    }
    assert((glIsList(this->_displaylist) == GL_TRUE));
    this->render();
}


void UpdateObjects() {
    for (Object2639 &i : objectPool) {
        // i.texturePath = "rom:/misuzu.sprite";
        i.update();
    }
}

