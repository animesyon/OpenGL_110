// The 3ds file is made up ofchunks. As shown below, a chunk is composed of chunk's id, chunk's size and the chunk's data.
// The chunk's data might be another chunk or sub-chunk.
//
//                                            +-----------------------+     ^
//                                      index |     id (2 bytes)      |     |
//                                            +-----------------------+     |
//                                            |    size (4 bytes)     |     |
//                                            +-----------------------+     |     +-----------------------+     ^ 
//                                            |        string         |     |     |     id (2 bytes)      |     |
//                                            |        and/or         |     |     +-----------------------+     |            
//                                            |         data          |     |     |    size (4 bytes)     |     |
//                                            |                       |     |     +-----------------------+    size
//                                            |                       |     |     |        string         |     |
//                                            |                       |     |     |        and/or         |     |
//                                            |                       |   size    |         data          |     |
//                                            |                       |     |     |                       |     v
//                                            |                       |     |     +=======================+     -
//                                            |                       |     |     |     id (2 bytes)      |     ^
//                                            |                       |     |     +-----------------------+     |            
//                                            |                       |     |     |    size (4 bytes)     |     |
//                                            |                       |     |     +-----------------------+    size
//                                            |                       |     |     |        string         |     |
//                                            |                       |     |     |        and/or         |     |
//                                            |                       |     |     |         data          |     |
//                                            +-----------------------+     v     +-----------------------+     v

#include "queue.h"

#pragma once

// I get these constant from file Const3DS.pas

//----------------- List of all chunk IDs -------------------------------------

const unsigned int NULL_CHUNK                = 0x0000;

// Trick Chunk Flags For ChunkSyntax function
const unsigned int ChunkType                 = 0x0995;
const unsigned int ChunkUnique               = 0x0996;
const unsigned int NotChunk                  = 0x0997;
const unsigned int Container                 = 0x0998;
const unsigned int IsChunk                   = 0x0999;

// Dummy Chunk that sometimes appears in 3DS files created by prerelease 3D Studio R2
const unsigned int DUMMY                     = 0xffff;

// Trick Chunk Types For Open, Write, Close functions
const unsigned int POINT_ARRAY_ENTRY         = 0xf110;
const unsigned int POINT_FLAG_ARRAY_ENTRY    = 0xf111;
const unsigned int FACE_ARRAY_ENTRY          = 0xf120;
const unsigned int MSH_MAT_GROUP_ENTRY       = 0xf130;
const unsigned int TEX_VERTS_ENTRY           = 0xf140;
const unsigned int SMOOTH_GROUP_ENTRY        = 0xf150;
const unsigned int POS_TRACK_TAG_KEY         = 0xf020;
const unsigned int ROT_TRACK_TAG_KEY         = 0xf021;
const unsigned int SCL_TRACK_TAG_KEY         = 0xf022;
const unsigned int FOV_TRACK_TAG_KEY         = 0xf023;
const unsigned int ROLL_TRACK_TAG_KEY        = 0xf024;
const unsigned int COL_TRACK_TAG_KEY         = 0xf025;
const unsigned int MORPH_TRACK_TAG_KEY       = 0xf026;
const unsigned int HOT_TRACK_TAG_KEY         = 0xf027;
const unsigned int FALL_TRACK_TAG_KEY        = 0xf028;

// 3DS file Chunk IDs
const unsigned int M3DMAGIC                  = 0x4d4d;
const unsigned int SMAGIC                    = 0x2d2d;
const unsigned int LMAGIC                    = 0x2d3d;
const unsigned int MLIBMAGIC                 = 0x3daa;
const unsigned int MATMAGIC                  = 0x3dff;
const unsigned int M3D_VERSION               = 0x0002;
const unsigned int M3D_KFVERSION             = 0x0005;

// Mesh Chunk Ids
const unsigned int MDATA                     = 0x3d3d;
const unsigned int MESH_VERSION              = 0x3d3e;
const unsigned int COLOR_F                   = 0x0010;
const unsigned int COLOR_24                  = 0x0011;
const unsigned int LIN_COLOR_24              = 0x0012;
const unsigned int LIN_COLOR_F               = 0x0013;
const unsigned int INT_PERCENTAGE            = 0x0030;
const unsigned int FLOAT_PERCENTAGE          = 0x0031;
const unsigned int MASTER_SCALE              = 0x0100;
const unsigned int BIT_MAP                   = 0x1100;
const unsigned int USE_BIT_MAP               = 0x1101;
const unsigned int SOLID_BGND                = 0x1200;
const unsigned int USE_SOLID_BGND            = 0x1201;
const unsigned int V_GRADIENT                = 0x1300;
const unsigned int USE_V_GRADIENT            = 0x1301;
const unsigned int LO_SHADOW_BIAS            = 0x1400;
const unsigned int HI_SHADOW_BIAS            = 0x1410;
const unsigned int SHADOW_MAP_SIZE           = 0x1420;
const unsigned int SHADOW_SAMPLES            = 0x1430;
const unsigned int SHADOW_RANGE              = 0x1440;
const unsigned int SHADOW_FILTER             = 0x1450;
const unsigned int RAY_BIAS                  = 0x1460;
const unsigned int O_CONSTS                  = 0x1500;
const unsigned int AMBIENT_LIGHT             = 0x2100;
const unsigned int FOG                       = 0x2200;
const unsigned int USE_FOG                   = 0x2201;
const unsigned int FOG_BGND                  = 0x2210;
const unsigned int DISTANCE_CUE              = 0x2300;
const unsigned int USE_DISTANCE_CUE          = 0x2301;
const unsigned int LAYER_FOG                 = 0x2302;
const unsigned int USE_LAYER_FOG             = 0x2303;
const unsigned int DCUE_BGND                 = 0x2310;
const unsigned int DEFAULT_VIEW              = 0x3000;
const unsigned int VIEW_TOP                  = 0x3010;
const unsigned int VIEW_BOTTOM               = 0x3020;
const unsigned int VIEW_LEFT                 = 0x3030;
const unsigned int VIEW_RIGHT                = 0x3040;
const unsigned int VIEW_FRONT                = 0x3050;
const unsigned int VIEW_BACK                 = 0x3060;
const unsigned int VIEW_USER                 = 0x3070;
const unsigned int VIEW_CAMERA               = 0x3080;
const unsigned int VIEW_WINDOW               = 0x3090;
const unsigned int NAMED_OBJECT              = 0x4000;
const unsigned int OBJ_HIDDEN                = 0x4010;
const unsigned int OBJ_VIS_LOFTER            = 0x4011;
const unsigned int OBJ_DOESNT_CAST           = 0x4012;
const unsigned int OBJ_MATTE                 = 0x4013;
const unsigned int OBJ_FAST                  = 0x4014;
const unsigned int OBJ_PROCEDURAL            = 0x4015;
const unsigned int OBJ_FROZEN                = 0x4016;
const unsigned int OBJ_DONT_RCVSHADOW        = 0x4017;
const unsigned int N_TRI_OBJECT              = 0x4100;
const unsigned int POINT_ARRAY               = 0x4110;
const unsigned int POINT_FLAG_ARRAY          = 0x4111;
const unsigned int FACE_ARRAY                = 0x4120;
const unsigned int MSH_MAT_GROUP             = 0x4130;
const unsigned int OLD_MAT_GROUP             = 0x4131;
const unsigned int TEX_VERTS                 = 0x4140;
const unsigned int SMOOTH_GROUP              = 0x4150;
const unsigned int MESH_MATRIX               = 0x4160;
const unsigned int MESH_COLOR                = 0x4165;
const unsigned int MESH_TEXTURE_INFO         = 0x4170;
const unsigned int PROC_NAME                 = 0x4181;
const unsigned int PROC_DATA                 = 0x4182;
const unsigned int MSH_BOXMAP                = 0x4190;
const unsigned int N_D_L_OLD                 = 0x4400;
const unsigned int N_CAM_OLD                 = 0x4500;
const unsigned int N_DIRECT_LIGHT            = 0x4600;
const unsigned int DL_SPOTLIGHT              = 0x4610;
const unsigned int DL_OFF                    = 0x4620;
const unsigned int DL_ATTENUATE              = 0x4625;
const unsigned int DL_RAYSHAD                = 0x4627;
const unsigned int DL_SHADOWED               = 0x4630;
const unsigned int DL_LOCAL_SHADOW           = 0x4640;
const unsigned int DL_LOCAL_SHADOW2          = 0x4641;
const unsigned int DL_SEE_CONE               = 0x4650;
const unsigned int DL_SPOT_RECTANGULAR       = 0x4651;
const unsigned int DL_SPOT_OVERSHOOT         = 0x4652;
const unsigned int DL_SPOT_PROJECTOR         = 0x4653;
const unsigned int DL_EXCLUDE                = 0x4654;
const unsigned int DL_RANGE                  = 0x4655;

// Not used in R3
const unsigned int DL_SPOT_ROLL              = 0x4656;
const unsigned int DL_SPOT_ASPECT            = 0x4657;
const unsigned int DL_RAY_BIAS               = 0x4658;
const unsigned int DL_INNER_RANGE            = 0x4659;
const unsigned int DL_OUTER_RANGE            = 0x465a;
const unsigned int DL_MULTIPLIER             = 0x465b;
const unsigned int N_AMBIENT_LIGHT           = 0x4680;
const unsigned int N_CAMERA                  = 0x4700;
const unsigned int CAM_SEE_CONE              = 0x4710;
const unsigned int CAM_RANGES                = 0x4720;
const unsigned int HIERARCHY                 = 0x4f00;
const unsigned int PARENT_OBJECT             = 0x4f10;
const unsigned int PIVOT_OBJECT              = 0x4f20;
const unsigned int PIVOT_LIMITS              = 0x4f30;
const unsigned int PIVOT_ORDER               = 0x4f40;
const unsigned int XLATE_RANGE               = 0x4f50;
const unsigned int POLY_2D                   = 0x5000;

// Flags in shaper AFile that tell whether polys make up an ok shape
const unsigned int SHAPE_OK                  = 0x5010;
const unsigned int SHAPE_NOT_OK              = 0x5011;
const unsigned int SHAPE_HOOK                = 0x5020;
const unsigned int PATH_3D                   = 0x6000;
const unsigned int PATH_MATRIX               = 0x6005;
const unsigned int SHAPE_2D                  = 0x6010;
const unsigned int M_SCALE                   = 0x6020;
const unsigned int M_TWIST                   = 0x6030;
const unsigned int M_TEETER                  = 0x6040;
const unsigned int M_FIT                     = 0x6050;
const unsigned int M_BEVEL                   = 0x6060;
const unsigned int XZ_CURVE                  = 0x6070;
const unsigned int YZ_CURVE                  = 0x6080;
const unsigned int INTERPCT                  = 0x6090;
const unsigned int DEFORM_LIMIT              = 0x60a0;

// Flags for Modeler options
const unsigned int USE_CONTOUR               = 0x6100;
const unsigned int USE_TWEEN                 = 0x6110;
const unsigned int USE_SCALE                 = 0x6120;
const unsigned int USE_TWIST                 = 0x6130;
const unsigned int USE_TEETER                = 0x6140;
const unsigned int USE_FIT                   = 0x6150;
const unsigned int USE_BEVEL                 = 0x6160;

// Viewport description chunks
const unsigned int VIEWPORT_LAYOUT_OLD       = 0x7000;
const unsigned int VIEWPORT_DATA_OLD         = 0x7010;
const unsigned int VIEWPORT_LAYOUT           = 0x7001;
const unsigned int VIEWPORT_DATA             = 0x7011;
const unsigned int VIEWPORT_DATA_3           = 0x7012;
const unsigned int VIEWPORT_SIZE             = 0x7020;
const unsigned int NETWORK_VIEW              = 0x7030;

// External Application Data
const unsigned int XDATA_SECTION             = 0x8000;
const unsigned int XDATA_ENTRY               = 0x8001;
const unsigned int XDATA_APPNAME             = 0x8002;
const unsigned int XDATA_STRING              = 0x8003;
const unsigned int XDATA_FLOAT               = 0x8004;
const unsigned int XDATA_DOUBLE              = 0x8005;
const unsigned int XDATA_SHORT               = 0x8006;
const unsigned int XDATA_LONG                = 0x8007;
const unsigned int XDATA_VOID                = 0x8008;
const unsigned int XDATA_GROUP               = 0x8009;
const unsigned int XDATA_RFU6                = 0x800a;
const unsigned int XDATA_RFU5                = 0x800b;
const unsigned int XDATA_RFU4                = 0x800c;
const unsigned int XDATA_RFU3                = 0x800d;
const unsigned int XDATA_RFU2                = 0x800e;
const unsigned int XDATA_RFU1                = 0x800f;
const unsigned int PARENT_NAME               = 0x80f0;

// Material Chunk IDs
const unsigned int MAT_ENTRY                 = 0xafff;
const unsigned int MAT_NAME                  = 0xa000;
const unsigned int MAT_AMBIENT               = 0xa010;
const unsigned int MAT_DIFFUSE               = 0xa020;
const unsigned int MAT_SPECULAR              = 0xa030;
const unsigned int MAT_SHININESS             = 0xa040;
const unsigned int MAT_SHIN2PCT              = 0xa041;
const unsigned int MAT_SHIN3PCT              = 0xa042;
const unsigned int MAT_TRANSPARENCY          = 0xa050;
const unsigned int MAT_XPFALL                = 0xa052;
const unsigned int MAT_REFBLUR               = 0xa053;
const unsigned int MAT_SELF_ILLUM            = 0xa080;
const unsigned int MAT_TWO_SIDE              = 0xa081;
const unsigned int MAT_DECAL                 = 0xa082;
const unsigned int MAT_ADDITIVE              = 0xa083;
const unsigned int MAT_SELF_ILPCT            = 0xa084;
const unsigned int MAT_WIRE                  = 0xa085;
const unsigned int MAT_SUPERSMP              = 0xa086;
const unsigned int MAT_WIRESIZE              = 0xa087;
const unsigned int MAT_FACEMAP               = 0xa088;
const unsigned int MAT_XPFALLIN              = 0xa08a;
const unsigned int MAT_PHONGSOFT             = 0xa08c;
const unsigned int MAT_WIREABS               = 0xa08e;
const unsigned int MAT_SHADING               = 0xa100;
const unsigned int MAT_TEXMAP                = 0xa200;
const unsigned int MAT_OPACMAP               = 0xa210;
const unsigned int MAT_REFLMAP               = 0xa220;
const unsigned int MAT_BUMPMAP               = 0xa230;
const unsigned int MAT_SPECMAP               = 0xa204;
const unsigned int MAT_USE_XPFALL            = 0xa240;
const unsigned int MAT_USE_REFBLUR           = 0xa250;
const unsigned int MAT_BUMP_PERCENT          = 0xa252;
const unsigned int MAT_MAPNAME               = 0xa300;
const unsigned int MAT_ACUBIC                = 0xa310;
const unsigned int MAT_SXP_TEXT_DATA         = 0xa320;
const unsigned int MAT_SXP_TEXT2_DATA        = 0xa321;
const unsigned int MAT_SXP_OPAC_DATA         = 0xa322;
const unsigned int MAT_SXP_BUMP_DATA         = 0xa324;
const unsigned int MAT_SXP_SPEC_DATA         = 0xa325;
const unsigned int MAT_SXP_SHIN_DATA         = 0xa326;
const unsigned int MAT_SXP_SELFI_DATA        = 0xa328;
const unsigned int MAT_SXP_TEXT_MASKDATA     = 0xa32a;
const unsigned int MAT_SXP_TEXT2_MASKDATA    = 0xa32c;
const unsigned int MAT_SXP_OPAC_MASKDATA     = 0xa32e;
const unsigned int MAT_SXP_BUMP_MASKDATA     = 0xa330;
const unsigned int MAT_SXP_SPEC_MASKDATA     = 0xa332;
const unsigned int MAT_SXP_SHIN_MASKDATA     = 0xa334;
const unsigned int MAT_SXP_SELFI_MASKDATA    = 0xa336;
const unsigned int MAT_SXP_REFL_MASKDATA     = 0xa338;
const unsigned int MAT_TEX2MAP               = 0xa33a;
const unsigned int MAT_SHINMAP               = 0xa33c;
const unsigned int MAT_SELFIMAP              = 0xa33d;
const unsigned int MAT_TEXMASK               = 0xa33e;
const unsigned int MAT_TEX2MASK              = 0xa340;
const unsigned int MAT_OPACMASK              = 0xa342;
const unsigned int MAT_BUMPMASK              = 0xa344;
const unsigned int MAT_SHINMASK              = 0xa346;
const unsigned int MAT_SPECMASK              = 0xa348;
const unsigned int MAT_SELFIMASK             = 0xa34a;
const unsigned int MAT_REFLMASK              = 0xa34c;
const unsigned int MAT_MAP_TILINGOLD         = 0xa350;
const unsigned int MAT_MAP_TILING            = 0xa351;
const unsigned int MAT_MAP_TEXBLUR_OLD       = 0xa352;
const unsigned int MAT_MAP_TEXBLUR           = 0xa353;
const unsigned int MAT_MAP_USCALE            = 0xa354;
const unsigned int MAT_MAP_VSCALE            = 0xa356;
const unsigned int MAT_MAP_UOFFSET           = 0xa358;
const unsigned int MAT_MAP_VOFFSET           = 0xa35a;
const unsigned int MAT_MAP_ANG               = 0xa35c;
const unsigned int MAT_MAP_COL1              = 0xa360;
const unsigned int MAT_MAP_COL2              = 0xa362;
const unsigned int MAT_MAP_RCOL              = 0xa364;
const unsigned int MAT_MAP_GCOL              = 0xa366;
const unsigned int MAT_MAP_BCOL              = 0xa368;

// Keyframe Chunk IDs
const unsigned int KFDATA                    = 0xb000;
const unsigned int KFHDR                     = 0xb00a;
const unsigned int AMBIENT_NODE_TAG          = 0xb001;
const unsigned int OBJECT_NODE_TAG           = 0xb002;
const unsigned int CAMERA_NODE_TAG           = 0xb003;
const unsigned int TARGET_NODE_TAG           = 0xb004;
const unsigned int LIGHT_NODE_TAG            = 0xb005;
const unsigned int L_TARGET_NODE_TAG         = 0xb006;
const unsigned int SPOTLIGHT_NODE_TAG        = 0xb007;
const unsigned int KFSEG                     = 0xb008;
const unsigned int KFCURTIME                 = 0xb009;
const unsigned int NODE_HDR                  = 0xb010;
const unsigned int INSTANCE_NAME             = 0xb011;
const unsigned int PRESCALE                  = 0xb012;
const unsigned int PIVOT                     = 0xb013;
const unsigned int BOUNDBOX                  = 0xb014;
const unsigned int MORPH_SMOOTH              = 0xb015;
const unsigned int POS_TRACK_TAG             = 0xb020;
const unsigned int ROT_TRACK_TAG             = 0xb021;
const unsigned int SCL_TRACK_TAG             = 0xb022;
const unsigned int FOV_TRACK_TAG             = 0xb023;
const unsigned int ROLL_TRACK_TAG            = 0xb024;
const unsigned int COL_TRACK_TAG             = 0xb025;
const unsigned int MORPH_TRACK_TAG           = 0xb026;
const unsigned int HOT_TRACK_TAG             = 0xb027;
const unsigned int FALL_TRACK_TAG            = 0xb028;
const unsigned int HIDE_TRACK_TAG            = 0xb029;
const unsigned int NODE_ID                   = 0xb030;
const unsigned int CMAGIC                    = 0xc23d;
const unsigned int C_MDRAWER                 = 0xc010;
const unsigned int C_TDRAWER                 = 0xc020;
const unsigned int C_SHPDRAWER               = 0xc030;
const unsigned int C_MODDRAWER               = 0xc040;
const unsigned int C_RIPDRAWER               = 0xc050;
const unsigned int C_TXDRAWER                = 0xc060;
const unsigned int C_PDRAWER                 = 0xc062;
const unsigned int C_MTLDRAWER               = 0xc064;
const unsigned int C_FLIDRAWER               = 0xc066;
const unsigned int C_CUBDRAWER               = 0xc067;
const unsigned int C_MFILE                   = 0xc070;
const unsigned int C_SHPFILE                 = 0xc080;
const unsigned int C_MODFILE                 = 0xc090;
const unsigned int C_RIPFILE                 = 0xc0a0;
const unsigned int C_TXFILE                  = 0xc0b0;
const unsigned int C_PFILE                   = 0xc0b2;
const unsigned int C_MTLFILE                 = 0xc0b4;
const unsigned int C_FLIFILE                 = 0xc0b6;
const unsigned int C_PALFILE                 = 0xc0b8;
const unsigned int C_TX_STRING               = 0xc0c0;
const unsigned int C_CONSTS                  = 0xc0d0;
const unsigned int C_SNAPS                   = 0xc0e0;
const unsigned int C_GRIDS                   = 0xc0f0;
const unsigned int C_ASNAPS                  = 0xc100;
const unsigned int C_GRID_RANGE              = 0xc110;
const unsigned int C_RENDTYPE                = 0xc120;
const unsigned int C_PROGMODE                = 0xc130;
const unsigned int C_PREVMODE                = 0xc140;
const unsigned int C_MODWMODE                = 0xc150;
const unsigned int C_MODMODEL                = 0xc160;
const unsigned int C_ALL_LINES               = 0xc170;
const unsigned int C_BACK_TYPE               = 0xc180;
const unsigned int C_MD_CS                   = 0xc190;
const unsigned int C_MD_CE                   = 0xc1a0;
const unsigned int C_MD_SML                  = 0xc1b0;
const unsigned int C_MD_SMW                  = 0xc1c0;
const unsigned int C_LOFT_WITH_TEXTURE       = 0xc1c3;
const unsigned int C_LOFT_L_REPEAT           = 0xc1c4;
const unsigned int C_LOFT_W_REPEAT           = 0xc1c5;
const unsigned int C_LOFT_UV_NORMALIZE       = 0xc1c6;
const unsigned int C_WELD_LOFT               = 0xc1c7;
const unsigned int C_MD_PDET                 = 0xc1d0;
const unsigned int C_MD_SDET                 = 0xc1e0;
const unsigned int C_RGB_RMODE               = 0xc1f0;
const unsigned int C_RGB_HIDE                = 0xc200;
const unsigned int C_RGB_MAPSW               = 0xc202;
const unsigned int C_RGB_TWOSIDE             = 0xc204;
const unsigned int C_RGB_SHADOW              = 0xc208;
const unsigned int C_RGB_AA                  = 0xc210;
const unsigned int C_RGB_OVW                 = 0xc220;
const unsigned int C_RGB_OVH                 = 0xc230;
const unsigned int C_RGB_PICTYPE             = 0xc240;
const unsigned int C_RGB_OUTPUT              = 0xc250;
const unsigned int C_RGB_TODISK              = 0xc253;
const unsigned int C_RGB_COMPRESS            = 0xc254;
const unsigned int C_JPEG_COMPRESSION        = 0xc255;
const unsigned int C_RGB_DISPDEV             = 0xc256;
const unsigned int C_RGB_HARDDEV             = 0xc259;
const unsigned int C_RGB_PATH                = 0xc25a;
const unsigned int C_BITMAP_DRAWER           = 0xc25b;
const unsigned int C_RGB_FILE                = 0xc260;
const unsigned int C_RGB_OVASPECT            = 0xc270;
const unsigned int C_RGB_ANIMTYPE            = 0xc271;
const unsigned int C_RENDER_ALL              = 0xc272;
const unsigned int C_REND_FROM               = 0xc273;
const unsigned int C_REND_TO                 = 0xc274;
const unsigned int C_REND_NTH                = 0xc275;
const unsigned int C_PAL_TYPE                = 0xc276;
const unsigned int C_RND_TURBO               = 0xc277;
const unsigned int C_RND_MIP                 = 0xc278;
const unsigned int C_BGND_METHOD             = 0xc279;
const unsigned int C_AUTO_REFLECT            = 0xc27a;
const unsigned int C_VP_FROM                 = 0xc27b;
const unsigned int C_VP_TO                   = 0xc27c;
const unsigned int C_VP_NTH                  = 0xc27d;
const unsigned int C_REND_TSTEP              = 0xc27e;
const unsigned int C_VP_TSTEP                = 0xc27f;
const unsigned int C_SRDIAM                  = 0xc280;
const unsigned int C_SRDEG                   = 0xc290;
const unsigned int C_SRSEG                   = 0xc2a0;
const unsigned int C_SRDIR                   = 0xc2b0;
const unsigned int C_HETOP                   = 0xc2c0;
const unsigned int C_HEBOT                   = 0xc2d0;
const unsigned int C_HEHT                    = 0xc2e0;
const unsigned int C_HETURNS                 = 0xc2f0;
const unsigned int C_HEDEG                   = 0xc300;
const unsigned int C_HESEG                   = 0xc310;
const unsigned int C_HEDIR                   = 0xc320;
const unsigned int C_QUIKSTUFF               = 0xc330;
const unsigned int C_SEE_LIGHTS              = 0xc340;
const unsigned int C_SEE_CAMERAS             = 0xc350;
const unsigned int C_SEE_3D                  = 0xc360;
const unsigned int C_MESHSEL                 = 0xc370;
const unsigned int C_MESHUNSEL               = 0xc380;
const unsigned int C_POLYSEL                 = 0xc390;
const unsigned int C_POLYUNSEL               = 0xc3a0;
const unsigned int C_SHPLOCAL                = 0xc3a2;
const unsigned int C_MSHLOCAL                = 0xc3a4;
const unsigned int C_NUM_FORMAT              = 0xc3b0;
const unsigned int C_ARCH_DENOM              = 0xc3c0;
const unsigned int C_IN_DEVICE               = 0xc3d0;
const unsigned int C_MSCALE                  = 0xc3e0;
const unsigned int C_COMM_PORT               = 0xc3f0;
const unsigned int C_TAB_BASES               = 0xc400;
const unsigned int C_TAB_DIVS                = 0xc410;
const unsigned int C_MASTER_SCALES           = 0xc420;
const unsigned int C_SHOW_1STVERT            = 0xc430;
const unsigned int C_SHAPER_OK               = 0xc440;
const unsigned int C_LOFTER_OK               = 0xc450;
const unsigned int C_EDITOR_OK               = 0xc460;
const unsigned int C_KEYFRAMER_OK            = 0xc470;
const unsigned int C_PICKSIZE                = 0xc480;
const unsigned int C_MAPTYPE                 = 0xc490;
const unsigned int C_MAP_DISPLAY             = 0xc4a0;
const unsigned int C_TILE_XY                 = 0xc4b0;
const unsigned int C_MAP_XYZ                 = 0xc4c0;
const unsigned int C_MAP_SCALE               = 0xc4d0;
const unsigned int C_MAP_MATRIX_OLD          = 0xc4e0;
const unsigned int C_MAP_MATRIX              = 0xc4e1;
const unsigned int C_MAP_WID_HT              = 0xc4f0;
const unsigned int C_OBNAME                  = 0xc500;
const unsigned int C_CAMNAME                 = 0xc510;
const unsigned int C_LTNAME                  = 0xc520;
const unsigned int C_CUR_MNAME               = 0xc525;
const unsigned int C_CURMTL_FROM_MESH        = 0xc526;
const unsigned int C_GET_SHAPE_MAKE_FACES    = 0xc527;
const unsigned int C_DETAIL                  = 0xc530;
const unsigned int C_VERTMARK                = 0xc540;
const unsigned int C_MSHAX                   = 0xc550;
const unsigned int C_MSHCP                   = 0xc560;
const unsigned int C_USERAX                  = 0xc570;
const unsigned int C_SHOOK                   = 0xc580;
const unsigned int C_RAX                     = 0xc590;
const unsigned int C_STAPE                   = 0xc5a0;
const unsigned int C_LTAPE                   = 0xc5b0;
const unsigned int C_ETAPE                   = 0xc5c0;
const unsigned int C_KTAPE                   = 0xc5c8;
const unsigned int C_SPHSEGS                 = 0xc5d0;
const unsigned int C_GEOSMOOTH               = 0xc5e0;
const unsigned int C_HEMISEGS                = 0xc5f0;
const unsigned int C_PRISMSEGS               = 0xc600;
const unsigned int C_PRISMSIDES              = 0xc610;
const unsigned int C_TUBESEGS                = 0xc620;
const unsigned int C_TUBESIDES               = 0xc630;
const unsigned int C_TORSEGS                 = 0xc640;
const unsigned int C_TORSIDES                = 0xc650;
const unsigned int C_CONESIDES               = 0xc660;
const unsigned int C_CONESEGS                = 0xc661;
const unsigned int C_NGPARMS                 = 0xc670;
const unsigned int C_PTHLEVEL                = 0xc680;
const unsigned int C_MSCSYM                  = 0xc690;
const unsigned int C_MFTSYM                  = 0xc6a0;
const unsigned int C_MTTSYM                  = 0xc6b0;
const unsigned int C_SMOOTHING               = 0xc6c0;
const unsigned int C_MODICOUNT               = 0xc6d0;
const unsigned int C_FONTSEL                 = 0xc6e0;
const unsigned int C_TESS_TYPE               = 0xc6f0;
const unsigned int C_TESS_TENSION            = 0xc6f1;
const unsigned int C_SEG_START               = 0xc700;
const unsigned int C_SEG_END                 = 0xc705;
const unsigned int C_CURTIME                 = 0xc710;
const unsigned int C_ANIMLENGTH              = 0xc715;
const unsigned int C_PV_FROM                 = 0xc720;
const unsigned int C_PV_TO                   = 0xc725;
const unsigned int C_PV_DOFNUM               = 0xc730;
const unsigned int C_PV_RNG                  = 0xc735;
const unsigned int C_PV_NTH                  = 0xc740;
const unsigned int C_PV_TYPE                 = 0xc745;
const unsigned int C_PV_METHOD               = 0xc750;
const unsigned int C_PV_FPS                  = 0xc755;
const unsigned int C_VTR_FRAMES              = 0xc765;
const unsigned int C_VTR_HDTL                = 0xc770;
const unsigned int C_VTR_HD                  = 0xc771;
const unsigned int C_VTR_TL                  = 0xc772;
const unsigned int C_VTR_IN                  = 0xc775;
const unsigned int C_VTR_PK                  = 0xc780;
const unsigned int C_VTR_SH                  = 0xc785;

// Material chunks
const unsigned int C_WORK_MTLS               = 0xc790;            // Old-style -- now ignored
const unsigned int C_WORK_MTLS_2             = 0xc792;            // Old-style -- now ignored
const unsigned int C_WORK_MTLS_3             = 0xc793;            // Old-style -- now ignored
const unsigned int C_WORK_MTLS_4             = 0xc794;            // Old-style -- now ignored
const unsigned int C_WORK_MTLS_5             = 0xcb00;            // Old-style -- now ignored
const unsigned int C_WORK_MTLS_6             = 0xcb01;            // Old-style -- now ignored
const unsigned int C_WORK_MTLS_7             = 0xcb02;            // Old-style -- now ignored
const unsigned int C_WORK_MTLS_8             = 0xcb03;            // Old-style -- now ignored
const unsigned int C_WORKMTL                 = 0xcb04;
const unsigned int C_SXP_TEXT_DATA           = 0xcb10;
const unsigned int C_SXP_TEXT2_DATA          = 0xcb20;
const unsigned int C_SXP_OPAC_DATA           = 0xcb11;
const unsigned int C_SXP_BUMP_DATA           = 0xcb12;
const unsigned int C_SXP_SPEC_DATA           = 0xcb24;
const unsigned int C_SXP_SHIN_DATA           = 0xcb13;
const unsigned int C_SXP_SELFI_DATA          = 0xcb28;
const unsigned int C_SXP_TEXT_MASKDATA       = 0xcb30;
const unsigned int C_SXP_TEXT2_MASKDATA      = 0xcb32;
const unsigned int C_SXP_OPAC_MASKDATA       = 0xcb34;
const unsigned int C_SXP_BUMP_MASKDATA       = 0xcb36;
const unsigned int C_SXP_SPEC_MASKDATA       = 0xcb38;
const unsigned int C_SXP_SHIN_MASKDATA       = 0xcb3a;
const unsigned int C_SXP_SELFI_MASKDATA      = 0xc3cb;
const unsigned int C_SXP_REFL_MASKDATA       = 0xcb3e;
const unsigned int C_BGTYPE                  = 0xc7a1;
const unsigned int C_MEDTILE                 = 0xc7b0;

// Contrast
const unsigned int C_LO_CONTRAST             = 0xc7d0;
const unsigned int C_HI_CONTRAST             = 0xc7d1;

// 3d frozen display
const unsigned int C_FROZ_DISPLAY            = 0xc7e0;

// Booleans
const unsigned int C_BOOLWELD                = 0xc7f0;
const unsigned int C_BOOLTYPE                = 0xc7f1;
const unsigned int C_ANG_THRESH              = 0xc900;
const unsigned int C_SS_THRESH               = 0xc901;
const unsigned int C_TEXTURE_BLUR_DEFAULT    = 0xc903;
const unsigned int C_MAPDRAWER               = 0xca00;
const unsigned int C_MAPDRAWER1              = 0xca01;
const unsigned int C_MAPDRAWER2              = 0xca02;
const unsigned int C_MAPDRAWER3              = 0xca03;
const unsigned int C_MAPDRAWER4              = 0xca04;
const unsigned int C_MAPDRAWER5              = 0xca05;
const unsigned int C_MAPDRAWER6              = 0xca06;
const unsigned int C_MAPDRAWER7              = 0xca07;
const unsigned int C_MAPDRAWER8              = 0xca08;
const unsigned int C_MAPDRAWER9              = 0xca09;
const unsigned int C_MAPDRAWER_ENTRY         = 0xca10;

// system options
const unsigned int C_BACKUP_FILE             = 0xca20;
const unsigned int C_DITHER_256              = 0xca21;
const unsigned int C_SAVE_LAST               = 0xca22;
const unsigned int C_USE_ALPHA               = 0xca23;
const unsigned int C_TGA_DEPTH               = 0xca24;
const unsigned int C_REND_FIELDS             = 0xca25;
const unsigned int C_REFLIP                  = 0xca26;
const unsigned int C_SEL_ITEMTOG             = 0xca27;
const unsigned int C_SEL_RESET               = 0xca28;
const unsigned int C_STICKY_KEYINF           = 0xca29;
const unsigned int C_WELD_THRESHOLD          = 0xca2a;
const unsigned int C_ZCLIP_POINT             = 0xca2b;
const unsigned int C_ALPHA_SPLIT             = 0xca2c;
const unsigned int C_KF_SHOW_BACKFACE        = 0xca30;
const unsigned int C_OPTIMIZE_LOFT           = 0xca40;
const unsigned int C_TENS_DEFAULT            = 0xca42;
const unsigned int C_CONT_DEFAULT            = 0xca44;
const unsigned int C_BIAS_DEFAULT            = 0xca46;
const unsigned int C_DXFNAME_SRC             = 0xca50;
const unsigned int C_AUTO_WELD               = 0xca60;
const unsigned int C_AUTO_UNIFY              = 0xca70;
const unsigned int C_AUTO_SMOOTH             = 0xca80;
const unsigned int C_DXF_SMOOTH_ANG          = 0xca90;
const unsigned int C_SMOOTH_ANG              = 0xcaa0;

// Special network-use chunks
const unsigned int C_NET_USE_VPOST           = 0xcc00;
const unsigned int C_NET_USE_GAMMA           = 0xcc10;
const unsigned int C_NET_FIELD_ORDER         = 0xcc20;
const unsigned int C_BLUR_FRAMES             = 0xcd00;
const unsigned int C_BLUR_SAMPLES            = 0xcd10;
const unsigned int C_BLUR_DUR                = 0xcd20;
const unsigned int C_HOT_METHOD              = 0xcd30;
const unsigned int C_HOT_CHECK               = 0xcd40;
const unsigned int C_PIXEL_SIZE              = 0xcd50;
const unsigned int C_DISP_GAMMA              = 0xcd60;
const unsigned int C_FBUF_GAMMA              = 0xcd70;
const unsigned int C_FILE_OUT_GAMMA          = 0xcd80;
const unsigned int C_FILE_IN_GAMMA           = 0xcd82;
const unsigned int C_GAMMA_CORRECT           = 0xcd84;
const unsigned int C_APPLY_DISP_GAMMA        = 0xcd90;            // OBSOLETE
const unsigned int C_APPLY_FBUF_GAMMA        = 0xcda0;            // OBSOLETE
const unsigned int C_APPLY_FILE_GAMMA        = 0xcdb0;            // OBSOLETE
const unsigned int C_FORCE_WIRE              = 0xcdc0;
const unsigned int C_RAY_SHADOWS             = 0xcdd0;
const unsigned int C_MASTER_AMBIENT          = 0xcde0;
const unsigned int C_SUPER_SAMPLE            = 0xcdf0;
const unsigned int C_OBJECT_MBLUR            = 0xce00;
const unsigned int C_MBLUR_DITHER            = 0xce10;
const unsigned int C_DITHER_24               = 0xce20;
const unsigned int C_SUPER_BLACK             = 0xce30;
const unsigned int C_SAFE_FRAME              = 0xce40;
const unsigned int C_VIEW_PRES_RATIO         = 0xce50;
const unsigned int C_BGND_PRES_RATIO         = 0xce60;
const unsigned int C_NTH_SERIAL_NUM          = 0xce70;

// Video Post
const unsigned int VPDATA                    = 0xd000;
const unsigned int P_QUEUE_ENTRY             = 0xd100;
const unsigned int P_QUEUE_IMAGE             = 0xd110;
const unsigned int P_QUEUE_USEIGAMMA         = 0xd114;
const unsigned int P_QUEUE_PROC              = 0xd120;
const unsigned int P_QUEUE_SOLID             = 0xd130;
const unsigned int P_QUEUE_GRADIENT          = 0xd140;
const unsigned int P_QUEUE_KF                = 0xd150;
const unsigned int P_QUEUE_MOTBLUR           = 0xd152;
const unsigned int P_QUEUE_MB_REPEAT         = 0xd153;
const unsigned int P_QUEUE_NONE              = 0xd160;
const unsigned int P_QUEUE_RESIZE            = 0xd180;
const unsigned int P_QUEUE_OFFSET            = 0xd185;
const unsigned int P_QUEUE_ALIGN             = 0xd190;
const unsigned int P_CUSTOM_SIZE             = 0xd1a0;
const unsigned int P_ALPH_NONE               = 0xd210;
const unsigned int P_ALPH_PSEUDO             = 0xd220;            // Old Chunk
const unsigned int P_ALPH_OP_PSEUDO          = 0xd221;            // Old Chunk
const unsigned int P_ALPH_BLUR               = 0xd222;            // Replaces pseudo 
const unsigned int P_ALPH_PCOL               = 0xd225;
const unsigned int P_ALPH_C0                 = 0xd230;
const unsigned int P_ALPH_OP_KEY             = 0xd231;
const unsigned int P_ALPH_KCOL               = 0xd235;
const unsigned int P_ALPH_OP_NOCONV          = 0xd238;
const unsigned int P_ALPH_IMAGE              = 0xd240;
const unsigned int P_ALPH_ALPHA              = 0xd250;
const unsigned int P_ALPH_QUES               = 0xd260;
const unsigned int P_ALPH_QUEIMG             = 0xd265;
const unsigned int P_ALPH_CUTOFF             = 0xd270;
const unsigned int P_ALPHANEG                = 0xd280;
const unsigned int P_TRAN_NONE               = 0xd300;
const unsigned int P_TRAN_IMAGE              = 0xd310;
const unsigned int P_TRAN_FRAMES             = 0xd312;
const unsigned int P_TRAN_FADEIN             = 0xd320;
const unsigned int P_TRAN_FADEOUT            = 0xd330;
const unsigned int P_TRANNEG                 = 0xd340;
const unsigned int P_RANGES                  = 0xd400;
const unsigned int P_PROC_DATA               = 0xd500;

class C3dsFile
{
private:
	unsigned char* data;
	unsigned char* buffer;
	unsigned int index, size;
	
public:
	C3dsFile();
	~C3dsFile();

	bool Open(wchar_t* filename);

	void GetChunkName(unsigned short id, wchar_t* str, size_t count);
	void GetChunkName(unsigned char* buffer, wchar_t* str, size_t count);
	unsigned char* ReadMainChunk(unsigned short* id, unsigned int* size);
	int GetSubChunkCount(unsigned short id);
	void ReadChunkData(unsigned char* buffer, unsigned int* size, int* column, CQueue* que);
	bool HasStringData(unsigned short id);

	bool IsEnd();
	void Reset(unsigned char* buffer, wchar_t** str);
	unsigned char* Read(unsigned short* id);

};

