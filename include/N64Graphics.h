#pragma once
#include "types.h"
/*
 * everything i need from gt.h and gbi.h
 */


typedef struct {
    short       ob[3];  /* x, y, z */
    unsigned short  flag;
    short       tc[2];  /* texture coord */
    unsigned char   cn[4];  /* color & alpha */
} Vtx_t;

typedef struct {
    short       ob[3];  /* x, y, z */
    unsigned short  flag;
    short       tc[2];  /* texture coord */
    signed char n[3];   /* normal */
    unsigned char   a;      /* alpha  */
} Vtx_tn;

typedef union {
    Vtx_t       v;  /* Use this one for colors  */
    Vtx_tn              n;  /* Use this one for normals */
    long long int   force_structure_alignment;
} Vtx;

typedef struct {
    struct {
        u32 w0;
        u32 w1;
    } words;
} Gfx;

#define GT_ZBUFFER		1
#define GT_TEXTURE		2	/* texture ON */
#define GT_CULL_BACK		4		/* reject backfaces */
#define GT_SHADING_SMOOTH	8	/* smooth shade ON */

/*
 * object state field: textureState
 *
 * The lower 3 bits of this flag word contain the texture tile number
 * to be used. All triangles of an object are rendered with the same
 * texture tile.
 */

/* 
 * object state field: flag
 *
 * This is a group of what would be pad bits. We use them for some
 * flag bits.
 */
#define GT_FLAG_NOMTX		0x01	/* don't load the matrix */
#define GT_FLAG_NO_XFM		0x02	/* load vtx, use verbatim */
#define GT_FLAG_XFM_ONLY	0x04	/* xform vtx, write to *TriN */

typedef struct {
    u32		renderState;	/* render state */
    u32		textureState;	/* texture state */
    u8		vtxCount;	/* how many verts? */
    u8		vtxV0;		/* where to load verts? */
    u8		triCount;	/* how many tris? */
    u8		flag;
    Gfx		*rdpCmds;	/* ptr (segment address) to RDP DL */
    Gfx		rdpOthermode;
    Mtx		transform;	/* the transform matrix to use */
} gtState_t;

typedef union {
    gtState_t		sp;
    long long int	force_structure_alignment;
} gtState;

typedef struct {
    u32		renderState;	/* render state */
    u32		textureState;	/* texture state */
    u8		vtxCount;	/* how many verts? */
    u8		vtxV0;		/* where to load verts? */
    u8		triCount;	/* how many tris? */
    u8		flag;
    Gfx		*rdpCmds;	/* ptr (segment address) to RDP DL */
    Gfx		rdpOthermode;
} gtStateL_t;

typedef union {
    gtStateL_t		sp;
    long long int	force_structure_alignment;
} gtStateL;

typedef struct {
    u32 vtxCount;
    u32 triCount;
} PacketInfo;

// typedef struct {
//     u8		v0, v1, v2, flag;	/* flag is which one for flat shade */
// } gtTriN;

typedef GLubyte gtTriN[3];

typedef enum {
    GT_CLEAR,		/* special gt mode, clears othermode state */
    GT_ALPHACOMPARE,
    GT_ZSRCSEL,
    GT_RENDERMODE,
    GT_ALPHADITHER,
    GT_RGBDITHER,
    GT_COMBKEY,
    GT_TEXTCONV,
    GT_TEXTFILT,
    GT_TEXTLUT,
    GT_TEXTLOD,
    GT_TEXTDETAIL,
    GT_TEXTPERSP,
    GT_CYCLETYPE,
    GT_PIPELINE
} gtStateOthermode_t;

#define gsDPClearOtherMode()						\
{									\
	_SHIFTL(G_RDPSETOTHERMODE, 24, 8), 0x0				\
}

#define gsDPEndDisplayList()	gsSPNoOp()

typedef struct {
    u32 gstatep;	/* global state, usually NULL */
    gtState	*statep;	/* if this is NULL, end object processing */
    float (*vtxp)[3];		/* if this is NULL, use points in buffer */
    gtTriN	*trip;		/* if this is NULL, use tris in buffer */
} gtGfx_t;


typedef union {
    gtGfx_t		obj;
    long long int	force_structure_alignment;
} gtGfx;

#define gsDPNoOp() {0, 0}
