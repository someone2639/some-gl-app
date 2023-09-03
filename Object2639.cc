#include <libdragon.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/gl_integration.h>
#include <malloc.h>
#include <math.h>

#include <vector>
#include <string>
#include <map>

#include <tiny_gltf.h>
using namespace tinygltf;

#include "2639_defs.h"
#include "glb_impl.h"
#include "Object2639.h"
#include "camera.h"
#include "swap.h"

void Object2639::loadVertexPositions(Model &model, Primitive &prim) {
    if (prim.attributes.count("POSITION") > 0) {
        const Accessor& vtxPosAccessor = model.accessors[prim.attributes["POSITION"]];
        const BufferView& vtxPosBufferView = model.bufferViews[vtxPosAccessor.bufferView];
        const Buffer &vtxPosBuffer = model.buffers[vtxPosBufferView.buffer];

        f32 *positions = (f32 *)(&vtxPosBuffer.data[
            vtxPosBufferView.byteOffset + vtxPosAccessor.byteOffset
        ]);
        f32_swap_endianness(
            positions,
            vtxPosAccessor.count * vtxPosAccessor.type
        );
        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(
            vtxPosAccessor.type,
            vtxPosAccessor.componentType,
            sizeof(f32) * vtxPosAccessor.type,
            positions
        );
    } else {
        glDisableClientState(GL_VERTEX_ARRAY);
    }
}

void Object2639::loadNormals(Model &model, Primitive &prim) {
    if (prim.attributes.count("NORMAL") > 0) {
        glEnableClientState(GL_NORMAL_ARRAY);

        const Accessor& normalAccessor = model.accessors[prim.attributes["NORMAL"]];
        const BufferView& normalBufferView = model.bufferViews[normalAccessor.bufferView];
        const Buffer &normalBuf = model.buffers[normalBufferView.buffer];

        f32 *normals = (f32 *)(&normalBuf.data[
            normalBufferView.byteOffset + normalAccessor.byteOffset
        ]);
        f32_swap_endianness(
            normals,
            normalAccessor.count * normalAccessor.type
        );
        glNormalPointer(
            normalAccessor.componentType,
            sizeof(f32) * normalAccessor.type,
            normals
        );
    } else {
        glDisableClientState(GL_NORMAL_ARRAY);
    }
}

void Object2639::loadColors(Model &model, Primitive &prim) {
    if (prim.attributes.count("COLOR_0") > 0) {
        glEnableClientState(GL_COLOR_ARRAY);

        const Accessor& colorAccessor = model.accessors[prim.attributes["COLOR_0"]];
        const BufferView& colorBufferView = model.bufferViews[colorAccessor.bufferView];
        const Buffer &colorBuf = model.buffers[colorBufferView.buffer];

        if (colorAccessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
            u16 *colors = (u16 *)(&colorBuf.data[
                colorBufferView.byteOffset + colorAccessor.byteOffset
            ]);
            u16_swap_endianness(
                colors,
                colorAccessor.count * colorAccessor.type
            );
            glColorPointer(
                colorAccessor.type,
                colorAccessor.componentType,
                sizeof(u16) * colorAccessor.type,
                colors
            );
        } else if (colorAccessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT) {
            f32 *colors = (f32 *)(&colorBuf.data[
                colorBufferView.byteOffset + colorAccessor.byteOffset
            ]);
            f32_swap_endianness(
                colors,
                colorAccessor.count * colorAccessor.type
            );
            glColorPointer(
                colorAccessor.type,
                colorAccessor.componentType,
                sizeof(float) * colorAccessor.type,
                colors
            );
        }
    } else {
        glDisableClientState(GL_COLOR_ARRAY);
    }
}

void Object2639::loadUVs(Model &model, Primitive &prim) {
    if (prim.attributes.count("TEXCOORD_0") > 0) {
        const Accessor& texcoordAccessor = model.accessors[prim.attributes["TEXCOORD_0"]];
        const BufferView& texcoordBufferView = model.bufferViews[texcoordAccessor.bufferView];
        const Buffer &texcoordBuf = model.buffers[texcoordBufferView.buffer];


        f32 *texcoords = (f32 *)(&texcoordBuf.data[
            texcoordBufferView.byteOffset + texcoordAccessor.byteOffset
        ]);
        f32_swap_endianness(texcoords,
            texcoordAccessor.count * texcoordAccessor.type
        );
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glTexCoordPointer(
            texcoordAccessor.type,
            texcoordAccessor.componentType,
            sizeof(f32) * texcoordAccessor.type,
            texcoords
        );
    } else {
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
}

void Object2639::loadJoints(Model &model, Primitive &prim) {
    if (prim.attributes.count("JOINTS_0") > 0) {
        glEnableClientState(GL_MATRIX_INDEX_ARRAY_ARB);
        glMatrixMode(GL_MATRIX_PALETTE_ARB);
        const Accessor& jointAccessor = model.accessors[prim.attributes["JOINTS_0"]];
        const BufferView& jointBufferView = model.bufferViews[jointAccessor.bufferView];
        const Buffer &jointBuffer = model.buffers[jointBufferView.buffer];

        u8 *joints = (u8 *)(&jointBuffer.data[
            jointBufferView.byteOffset + jointAccessor.byteOffset
        ]);

        glMatrixIndexPointerARB(
            jointAccessor.type,
            jointAccessor.componentType,
            sizeof(u8) * jointAccessor.type,
            joints
        );
        glMatrixMode(GL_MODELVIEW);
    } else {
        glDisableClientState(GL_MATRIX_INDEX_ARRAY_ARB);
    }
}

void Object2639::drawTris(Model &model, Primitive &prim) {
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
    } else {
        assertf(0, "2639: Unimplemented Component Type %d", indexAccessor.componentType);
    }

    u32 mode = GL_TRIANGLES;
    switch (prim.mode) {
        case TINYGLTF_MODE_POINTS: mode = GL_POINTS; break;
        case TINYGLTF_MODE_LINE: mode = GL_LINE; break;
        case TINYGLTF_MODE_LINE_LOOP: mode = GL_LINE_LOOP; break;
        case TINYGLTF_MODE_LINE_STRIP: mode = GL_LINE_STRIP; break;
        case TINYGLTF_MODE_TRIANGLES: mode = GL_TRIANGLES; break;
        case TINYGLTF_MODE_TRIANGLE_STRIP: mode = GL_TRIANGLE_STRIP; break;
        case TINYGLTF_MODE_TRIANGLE_FAN: mode = GL_TRIANGLE_FAN; break;
    }
    glDrawElements(mode, indexAccessor.count, indexAccessor.componentType,
        &indexBuffer.data[
            indexBufferView.byteOffset + indexAccessor.byteOffset
        ]
    );
}

void Object2639::initializeInternalParams() {
    this->_initialized = 0;
    this->_displaylist = 0x2639;
    this->_texIndex = 0;
    this->texturePath = nullptr;
    this->_sprite = nullptr;

    for (int i = 0; i < O_TEXCOUNT; i++) {
        this->_texParams[i].tmem_addr = 0;
        this->_texParams[i].palette = 0;

        this->_texParams[i].s.translate = 0;
        this->_texParams[i].s.scale_log = 0;
        this->_texParams[i].s.repeats = REPEAT_INFINITE;
        this->_texParams[i].s.mirror = MIRROR_NONE;

        this->_texParams[i].t.translate = 0;
        this->_texParams[i].t.scale_log = 0;
        this->_texParams[i].t.repeats = REPEAT_INFINITE;
        this->_texParams[i].t.mirror = MIRROR_NONE;
        this->hasTexture[i] = false;
    }
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

    glEnable(GL_TEXTURE_2D);
    glCallList(this->_displaylist);

    glPopMatrix();
}


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

Object2639::Object2639(std::string directory, Model &model, Scene &s) : Object2639() {
    GLuint aa = glGenLists(1);
    this->_displaylist = aa;

    for (int i : s.nodes) {
        Node n = model.nodes[i];
        if (n.mesh == -1) continue;

        Mesh mes = model.meshes[n.mesh];

        for (Primitive &prim : mes.primitives) {
// TEXTURES
            int matIndex = prim.material;
            struct tinygltf::Material *m = &model.materials[matIndex];

            PbrMetallicRoughness p = m->pbrMetallicRoughness;
            TextureInfo ti = p.baseColorTexture;

            if (model.images.size() == 0) {
                continue;
            }
            Image *im = &model.images[ti.index];

            if (im == nullptr) {
                continue;
            }
            if (im->uri.empty()) {
                continue;
            }

            std::string texPath = directory;


            int startIdx = im->uri.find(".");
            texPath += im->uri.replace(startIdx, 7, ".sprite");

            sprite_t *materialSprite = sprite_load(texPath.c_str());
            assertf(materialSprite != NULL, texPath.c_str());

            this->_texParams[this->_texIndex].s.repeats = REPEAT_INFINITE;
            this->_texParams[this->_texIndex].s.repeats = REPEAT_INFINITE;

 
            glGenTextures(1, &this->_texture[this->_texIndex]);
            glBindTexture(GL_TEXTURE_2D, this->_texture[this->_texIndex]);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            Texture *tx = &model.textures[ti.index];
            Sampler *sm = &model.samplers[tx->sampler];
            if (sm->wrapS == TINYGLTF_TEXTURE_WRAP_MIRRORED_REPEAT) {
                this->_texParams[this->_texIndex].s.mirror = MIRROR_REPEAT;
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,  GL_MIRRORED_REPEAT_ARB);
            } else {
                this->_texParams[this->_texIndex].s.mirror = MIRROR_NONE;
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            }

            if (sm->wrapT == TINYGLTF_TEXTURE_WRAP_MIRRORED_REPEAT) {
                this->_texParams[this->_texIndex].t.mirror = MIRROR_REPEAT;
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,  GL_MIRRORED_REPEAT_ARB);
            } else {
                this->_texParams[this->_texIndex].t.mirror = MIRROR_NONE;
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            }

            glSpriteTextureN64(GL_TEXTURE_2D, materialSprite, &this->_texParams[this->_texIndex]);


            this->hasTexture[this->_texIndex] = true;
            this->_texIndex++;
        }

        glNewList(this->_displaylist, GL_COMPILE);
        assert(glGetError() == 0);


        Node HeadNode;

        for (int i : s.nodes) {
            if (model.nodes[i].skin != -1) {
                HeadNode = model.nodes[i];
            } else if (model.nodes[i].mesh != -1) {
                HeadNode = model.nodes[i];
            }
        }

        {
            Mesh mes = model.meshes[HeadNode.mesh];

            this->name = mes.name;

            for (Primitive &prim : mes.primitives) {
                std::string texPath = directory;

                glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
                glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
    // Material Settings
                int matIndex = prim.material;
                struct tinygltf::Material *m = &model.materials[matIndex];
                if (m->alphaMode == "BLEND") {
                    // deferredMeshes.emplace_back(mes);
                    glEnable(GL_BLEND);
                    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                }
                if (m->doubleSided == true) {
                    glDisable(GL_CULL_FACE);
                } else {
                    glEnable(GL_CULL_FACE);
                    glCullFace(GL_BACK);
                }

                loadVertexPositions(model, prim);
                loadNormals(model, prim);
                loadColors(model, prim);
                loadUVs(model, prim);
                // TODO: do after blended skinning gets implemented
                // loadJoints(model, prim);

    // Texture properties
                PbrMetallicRoughness p = m->pbrMetallicRoughness;
                TextureInfo ti = p.baseColorTexture;

                bool hasTexture = (ti.index != -1);

                if (hasTexture) {
                    Image *im = &model.images[ti.index];
                    int startIdx = im->uri.find(".");
                    texPath += im->uri.replace(startIdx, 7, ".sprite");
                }
                if (this->hasTexture[ti.index]) {
                    glBindTexture(GL_TEXTURE_2D, this->_texture[ti.index]);

                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                }

                if (hasTexture && texPath.find("decal") != std::string::npos) {
                    glDepthFunc(GL_EQUAL);
                }
                drawTris(model, prim);
                if (hasTexture && texPath.find("decal") != std::string::npos) {
                    glDepthFunc(GL_LESS);
                }
            }

            glEndList();
            assert(glGetError() == 0);
            assert((glIsList(this->_displaylist) == GL_TRUE));
            assert(this->_displaylist == aa);
            assert((glIsList(aa) == GL_TRUE));
        }
    }
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
    this->render();
}
