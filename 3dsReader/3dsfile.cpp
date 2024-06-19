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

#include "framework.h"
#include "3dsfile.h"

// contructor
C3dsFile::C3dsFile()
{
	data = buffer = NULL;
	index = 0;
}

// destructor
C3dsFile::~C3dsFile()
{
	if (data != NULL) {
		delete[] data;
		data = NULL;
	}
}

// This will open the file and store the whole content into a memmory.
bool C3dsFile::Open(wchar_t* filename)
{
	HANDLE handle;
	WIN32_FIND_DATA finddata;
	FILE* pf;
	errno_t err;
	long long size;

	handle = FindFirstFile(filename, &finddata);
	size = (long long)finddata.nFileSizeHigh * ((long long)MAXDWORD + 1) + (long long)finddata.nFileSizeLow;
	FindClose(handle);

	if ((err = _wfopen_s(&pf, filename, L"rb")) != 0) return false;

	if (data != NULL)
		delete[] data;

	data = new unsigned char[size];

	fread(data, sizeof(unsigned char), size, pf);

	fclose(pf);

	return true;
}

// This will convert the chunk's id into a string.
void C3dsFile::GetChunkName(unsigned short id, wchar_t* str, size_t count)
{
	switch(id) {
	case NULL_CHUNK             : swprintf_s(str, count, L"%s [0x%04X]", L"NULL_CHUNK", id);             break;
	case ChunkType              : swprintf_s(str, count, L"%s [0x%04X]", L"ChunkType", id);              break;
	case ChunkUnique            : swprintf_s(str, count, L"%s [0x%04X]", L"ChunkUnique", id);            break;
	case NotChunk               : swprintf_s(str, count, L"%s [0x%04X]", L"NotChunk", id);               break;
	case Container              : swprintf_s(str, count, L"%s [0x%04X]", L"Container", id);              break;
	case IsChunk                : swprintf_s(str, count, L"%s [0x%04X]", L"IsChunk", id);                break;
	case DUMMY                  : swprintf_s(str, count, L"%s [0x%04X]", L"DUMMY", id);                  break;
	case POINT_ARRAY_ENTRY      : swprintf_s(str, count, L"%s [0x%04X]", L"POINT_ARRAY_ENTRY", id);      break;
	case POINT_FLAG_ARRAY_ENTRY : swprintf_s(str, count, L"%s [0x%04X]", L"POINT_FLAG_ARRAY_ENTRY", id); break;
	case FACE_ARRAY_ENTRY       : swprintf_s(str, count, L"%s [0x%04X]", L"FACE_ARRAY_ENTRY", id);       break;
	case MSH_MAT_GROUP_ENTRY    : swprintf_s(str, count, L"%s [0x%04X]", L"MSH_MAT_GROUP_ENTRY", id);    break;
	case TEX_VERTS_ENTRY        : swprintf_s(str, count, L"%s [0x%04X]", L"TEX_VERTS_ENTRY", id);        break;
	case SMOOTH_GROUP_ENTRY     : swprintf_s(str, count, L"%s [0x%04X]", L"SMOOTH_GROUP_ENTRY", id);     break;
	case POS_TRACK_TAG_KEY      : swprintf_s(str, count, L"%s [0x%04X]", L"POS_TRACK_TAG_KEY", id);      break;
	case ROT_TRACK_TAG_KEY      : swprintf_s(str, count, L"%s [0x%04X]", L"ROT_TRACK_TAG_KEY", id);      break;
	case SCL_TRACK_TAG_KEY      : swprintf_s(str, count, L"%s [0x%04X]", L"SCL_TRACK_TAG_KEY", id);      break;
	case FOV_TRACK_TAG_KEY      : swprintf_s(str, count, L"%s [0x%04X]", L"FOV_TRACK_TAG_KEY", id);      break;
	case ROLL_TRACK_TAG_KEY     : swprintf_s(str, count, L"%s [0x%04X]", L"ROLL_TRACK_TAG_KEY", id);     break;
	case COL_TRACK_TAG_KEY      : swprintf_s(str, count, L"%s [0x%04X]", L"COL_TRACK_TAG_KEY", id);      break;
	case MORPH_TRACK_TAG_KEY    : swprintf_s(str, count, L"%s [0x%04X]", L"MORPH_TRACK_TAG_KEY", id);    break;
	case HOT_TRACK_TAG_KEY      : swprintf_s(str, count, L"%s [0x%04X]", L"HOT_TRACK_TAG_KEY", id);      break;
	case FALL_TRACK_TAG_KEY     : swprintf_s(str, count, L"%s [0x%04X]", L"FALL_TRACK_TAG_KEY", id);     break;
	case M3DMAGIC               : swprintf_s(str, count, L"%s [0x%04X]", L"M3DMAGIC", id);               break;
	case SMAGIC                 : swprintf_s(str, count, L"%s [0x%04X]", L"SMAGIC", id);                 break;
	case LMAGIC                 : swprintf_s(str, count, L"%s [0x%04X]", L"LMAGIC", id);                 break;
	case MLIBMAGIC              : swprintf_s(str, count, L"%s [0x%04X]", L"MLIBMAGIC", id);              break;
	case MATMAGIC               : swprintf_s(str, count, L"%s [0x%04X]", L"MATMAGIC", id);               break;
	case M3D_VERSION            : swprintf_s(str, count, L"%s [0x%04X]", L"M3D_VERSION", id);            break;
	case M3D_KFVERSION          : swprintf_s(str, count, L"%s [0x%04X]", L"M3D_KFVERSION", id);          break;
	case MDATA                  : swprintf_s(str, count, L"%s [0x%04X]", L"MDATA", id);                  break;
	case MESH_VERSION           : swprintf_s(str, count, L"%s [0x%04X]", L"MESH_VERSION", id);           break;
	case COLOR_F                : swprintf_s(str, count, L"%s [0x%04X]", L"COLOR_F", id);                break;
	case COLOR_24               : swprintf_s(str, count, L"%s [0x%04X]", L"COLOR_24", id);               break;
	case LIN_COLOR_24           : swprintf_s(str, count, L"%s [0x%04X]", L"LIN_COLOR_24", id);           break;
	case LIN_COLOR_F            : swprintf_s(str, count, L"%s [0x%04X]", L"LIN_COLOR_F", id);            break;
	case INT_PERCENTAGE         : swprintf_s(str, count, L"%s [0x%04X]", L"INT_PERCENTAGE", id);         break;
	case FLOAT_PERCENTAGE       : swprintf_s(str, count, L"%s [0x%04X]", L"FLOAT_PERCENTAGE", id);       break;
	case MASTER_SCALE           : swprintf_s(str, count, L"%s [0x%04X]", L"MASTER_SCALE", id);           break;
	case BIT_MAP                : swprintf_s(str, count, L"%s [0x%04X]", L"BIT_MAP", id);                break;
	case USE_BIT_MAP            : swprintf_s(str, count, L"%s [0x%04X]", L"USE_BIT_MAP", id);            break;
	case SOLID_BGND             : swprintf_s(str, count, L"%s [0x%04X]", L"SOLID_BGND", id);             break;
	case USE_SOLID_BGND         : swprintf_s(str, count, L"%s [0x%04X]", L"USE_SOLID_BGND", id);         break;
	case V_GRADIENT             : swprintf_s(str, count, L"%s [0x%04X]", L"V_GRADIENT", id);             break;
	case USE_V_GRADIENT         : swprintf_s(str, count, L"%s [0x%04X]", L"USE_V_GRADIENT", id);         break;
	case LO_SHADOW_BIAS         : swprintf_s(str, count, L"%s [0x%04X]", L"LO_SHADOW_BIAS", id);         break;
	case HI_SHADOW_BIAS         : swprintf_s(str, count, L"%s [0x%04X]", L"HI_SHADOW_BIAS", id);         break;
	case SHADOW_MAP_SIZE        : swprintf_s(str, count, L"%s [0x%04X]", L"SHADOW_MAP_SIZE", id);        break;
	case SHADOW_SAMPLES         : swprintf_s(str, count, L"%s [0x%04X]", L"SHADOW_SAMPLES", id);         break;
	case SHADOW_RANGE           : swprintf_s(str, count, L"%s [0x%04X]", L"SHADOW_RANGE", id);           break;
	case SHADOW_FILTER          : swprintf_s(str, count, L"%s [0x%04X]", L"SHADOW_FILTER", id);          break;
	case RAY_BIAS               : swprintf_s(str, count, L"%s [0x%04X]", L"RAY_BIAS", id);               break;
	case O_CONSTS               : swprintf_s(str, count, L"%s [0x%04X]", L"O_CONSTS", id);               break;
	case AMBIENT_LIGHT          : swprintf_s(str, count, L"%s [0x%04X]", L"AMBIENT_LIGHT", id);          break;
	case FOG                    : swprintf_s(str, count, L"%s [0x%04X]", L"FOG", id);                    break;
	case USE_FOG                : swprintf_s(str, count, L"%s [0x%04X]", L"USE_FOG", id);                break;
	case FOG_BGND               : swprintf_s(str, count, L"%s [0x%04X]", L"FOG_BGND", id);               break;
	case DISTANCE_CUE           : swprintf_s(str, count, L"%s [0x%04X]", L"DISTANCE_CUE", id);           break;
	case USE_DISTANCE_CUE       : swprintf_s(str, count, L"%s [0x%04X]", L"USE_DISTANCE_CUE", id);       break;
	case LAYER_FOG              : swprintf_s(str, count, L"%s [0x%04X]", L"LAYER_FOG", id);              break;
	case USE_LAYER_FOG          : swprintf_s(str, count, L"%s [0x%04X]", L"USE_LAYER_FOG", id);          break;
	case DCUE_BGND              : swprintf_s(str, count, L"%s [0x%04X]", L"DCUE_BGND", id);              break;
	case DEFAULT_VIEW           : swprintf_s(str, count, L"%s [0x%04X]", L"DEFAULT_VIEW", id);           break;
	case VIEW_TOP               : swprintf_s(str, count, L"%s [0x%04X]", L"VIEW_TOP", id);               break;
	case VIEW_BOTTOM            : swprintf_s(str, count, L"%s [0x%04X]", L"VIEW_BOTTOM", id);            break;
	case VIEW_LEFT              : swprintf_s(str, count, L"%s [0x%04X]", L"VIEW_LEFT", id);              break;
	case VIEW_RIGHT             : swprintf_s(str, count, L"%s [0x%04X]", L"VIEW_RIGHT", id);             break;
	case VIEW_FRONT             : swprintf_s(str, count, L"%s [0x%04X]", L"VIEW_FRONT", id);             break;
	case VIEW_BACK              : swprintf_s(str, count, L"%s [0x%04X]", L"VIEW_BACK", id);              break;
	case VIEW_USER              : swprintf_s(str, count, L"%s [0x%04X]", L"VIEW_USER", id);              break;
	case VIEW_CAMERA            : swprintf_s(str, count, L"%s [0x%04X]", L"VIEW_CAMERA", id);            break;
	case VIEW_WINDOW            : swprintf_s(str, count, L"%s [0x%04X]", L"VIEW_WINDOW", id);            break;
	case NAMED_OBJECT           : swprintf_s(str, count, L"%s [0x%04X]", L"NAMED_OBJECT", id);           break;
	case OBJ_HIDDEN             : swprintf_s(str, count, L"%s [0x%04X]", L"OBJ_HIDDEN", id);             break;
	case OBJ_VIS_LOFTER         : swprintf_s(str, count, L"%s [0x%04X]", L"OBJ_VIS_LOFTER", id);         break;
	case OBJ_DOESNT_CAST        : swprintf_s(str, count, L"%s [0x%04X]", L"OBJ_DOESNT_CAST", id);        break;
	case OBJ_MATTE              : swprintf_s(str, count, L"%s [0x%04X]", L"OBJ_MATTE", id);              break;
	case OBJ_FAST               : swprintf_s(str, count, L"%s [0x%04X]", L"OBJ_FAST", id);               break;
	case OBJ_PROCEDURAL         : swprintf_s(str, count, L"%s [0x%04X]", L"OBJ_PROCEDURAL", id);         break;
	case OBJ_FROZEN             : swprintf_s(str, count, L"%s [0x%04X]", L"OBJ_FROZEN", id);             break;
	case OBJ_DONT_RCVSHADOW     : swprintf_s(str, count, L"%s [0x%04X]", L"OBJ_DONT_RCVSHADOW", id);     break;
	case N_TRI_OBJECT           : swprintf_s(str, count, L"%s [0x%04X]", L"N_TRI_OBJECT", id);           break;
	case POINT_ARRAY            : swprintf_s(str, count, L"%s [0x%04X]", L"POINT_ARRAY", id);            break;
	case POINT_FLAG_ARRAY       : swprintf_s(str, count, L"%s [0x%04X]", L"POINT_FLAG_ARRAY", id);       break;
	case FACE_ARRAY             : swprintf_s(str, count, L"%s [0x%04X]", L"FACE_ARRAY", id);             break;
	case MSH_MAT_GROUP          : swprintf_s(str, count, L"%s [0x%04X]", L"MSH_MAT_GROUP", id);          break;
	case OLD_MAT_GROUP          : swprintf_s(str, count, L"%s [0x%04X]", L"OLD_MAT_GROUP", id);          break;
	case TEX_VERTS              : swprintf_s(str, count, L"%s [0x%04X]", L"TEX_VERTS", id);              break;
	case SMOOTH_GROUP           : swprintf_s(str, count, L"%s [0x%04X]", L"SMOOTH_GROUP", id);           break;
	case MESH_MATRIX            : swprintf_s(str, count, L"%s [0x%04X]", L"MESH_MATRIX", id);            break;
	case MESH_COLOR             : swprintf_s(str, count, L"%s [0x%04X]", L"MESH_COLOR", id);             break;
	case MESH_TEXTURE_INFO      : swprintf_s(str, count, L"%s [0x%04X]", L"MESH_TEXTURE_INFO", id);      break;
	case PROC_NAME              : swprintf_s(str, count, L"%s [0x%04X]", L"PROC_NAME", id);              break;
	case PROC_DATA              : swprintf_s(str, count, L"%s [0x%04X]", L"PROC_DATA", id);              break;
	case MSH_BOXMAP             : swprintf_s(str, count, L"%s [0x%04X]", L"MSH_BOXMAP", id);             break;
	case N_D_L_OLD              : swprintf_s(str, count, L"%s [0x%04X]", L"N_D_L_OLD", id);              break;
	case N_CAM_OLD              : swprintf_s(str, count, L"%s [0x%04X]", L"N_CAM_OLD", id);              break;
	case N_DIRECT_LIGHT         : swprintf_s(str, count, L"%s [0x%04X]", L"N_DIRECT_LIGHT", id);         break;
	case DL_SPOTLIGHT           : swprintf_s(str, count, L"%s [0x%04X]", L"DL_SPOTLIGHT", id);           break;
	case DL_OFF                 : swprintf_s(str, count, L"%s [0x%04X]", L"DL_OFF", id);                 break;
	case DL_ATTENUATE           : swprintf_s(str, count, L"%s [0x%04X]", L"DL_ATTENUATE", id);           break;
	case DL_RAYSHAD             : swprintf_s(str, count, L"%s [0x%04X]", L"DL_RAYSHAD", id);             break;
	case DL_SHADOWED            : swprintf_s(str, count, L"%s [0x%04X]", L"DL_SHADOWED", id);            break;
	case DL_LOCAL_SHADOW        : swprintf_s(str, count, L"%s [0x%04X]", L"DL_LOCAL_SHADOW", id);        break;
	case DL_LOCAL_SHADOW2       : swprintf_s(str, count, L"%s [0x%04X]", L"DL_LOCAL_SHADOW2", id);       break;
	case DL_SEE_CONE            : swprintf_s(str, count, L"%s [0x%04X]", L"DL_SEE_CONE", id);            break;
	case DL_SPOT_RECTANGULAR    : swprintf_s(str, count, L"%s [0x%04X]", L"DL_SPOT_RECTANGULAR", id);    break;
	case DL_SPOT_OVERSHOOT      : swprintf_s(str, count, L"%s [0x%04X]", L"DL_SPOT_OVERSHOOT", id);      break;
	case DL_SPOT_PROJECTOR      : swprintf_s(str, count, L"%s [0x%04X]", L"DL_SPOT_PROJECTOR", id);      break;
	case DL_EXCLUDE             : swprintf_s(str, count, L"%s [0x%04X]", L"DL_EXCLUDE", id);             break;
	case DL_RANGE               : swprintf_s(str, count, L"%s [0x%04X]", L"DL_RANGE", id);               break;
	case DL_SPOT_ROLL           : swprintf_s(str, count, L"%s [0x%04X]", L"DL_SPOT_ROLL", id);           break;
	case DL_SPOT_ASPECT         : swprintf_s(str, count, L"%s [0x%04X]", L"DL_SPOT_ASPECT", id);         break;
	case DL_RAY_BIAS            : swprintf_s(str, count, L"%s [0x%04X]", L"DL_RAY_BIAS", id);            break;
	case DL_INNER_RANGE         : swprintf_s(str, count, L"%s [0x%04X]", L"DL_INNER_RANGE", id);         break;
	case DL_OUTER_RANGE         : swprintf_s(str, count, L"%s [0x%04X]", L"DL_OUTER_RANGE", id);         break;
	case DL_MULTIPLIER          : swprintf_s(str, count, L"%s [0x%04X]", L"DL_MULTIPLIER", id);          break;
	case N_AMBIENT_LIGHT        : swprintf_s(str, count, L"%s [0x%04X]", L"N_AMBIENT_LIGHT", id);        break;
	case N_CAMERA               : swprintf_s(str, count, L"%s [0x%04X]", L"N_CAMERA", id);               break;
	case CAM_SEE_CONE           : swprintf_s(str, count, L"%s [0x%04X]", L"CAM_SEE_CONE", id);           break;
	case CAM_RANGES             : swprintf_s(str, count, L"%s [0x%04X]", L"CAM_RANGES", id);             break;
	case HIERARCHY              : swprintf_s(str, count, L"%s [0x%04X]", L"HIERARCHY", id);              break;
	case PARENT_OBJECT          : swprintf_s(str, count, L"%s [0x%04X]", L"PARENT_OBJECT", id);          break;
	case PIVOT_OBJECT           : swprintf_s(str, count, L"%s [0x%04X]", L"PIVOT_OBJECT", id);           break;
	case PIVOT_LIMITS           : swprintf_s(str, count, L"%s [0x%04X]", L"PIVOT_LIMITS", id);           break;
	case PIVOT_ORDER            : swprintf_s(str, count, L"%s [0x%04X]", L"PIVOT_ORDER", id);            break;
	case XLATE_RANGE            : swprintf_s(str, count, L"%s [0x%04X]", L"XLATE_RANGE", id);            break;
	case POLY_2D                : swprintf_s(str, count, L"%s [0x%04X]", L"POLY_2D", id);                break;
	case SHAPE_OK               : swprintf_s(str, count, L"%s [0x%04X]", L"SHAPE_OK", id);               break;
	case SHAPE_NOT_OK           : swprintf_s(str, count, L"%s [0x%04X]", L"SHAPE_NOT_OK", id);           break;
	case SHAPE_HOOK             : swprintf_s(str, count, L"%s [0x%04X]", L"SHAPE_HOOK", id);             break;
	case PATH_3D                : swprintf_s(str, count, L"%s [0x%04X]", L"PATH_3D", id);                break;
	case PATH_MATRIX            : swprintf_s(str, count, L"%s [0x%04X]", L"PATH_MATRIX", id);            break;
	case SHAPE_2D               : swprintf_s(str, count, L"%s [0x%04X]", L"SHAPE_2D", id);               break;
	case M_SCALE                : swprintf_s(str, count, L"%s [0x%04X]", L"M_SCALE", id);                break;
	case M_TWIST                : swprintf_s(str, count, L"%s [0x%04X]", L"M_TWIST", id);                break;
	case M_TEETER               : swprintf_s(str, count, L"%s [0x%04X]", L"M_TEETER", id);               break;
	case M_FIT                  : swprintf_s(str, count, L"%s [0x%04X]", L"M_FIT", id);                  break;
	case M_BEVEL                : swprintf_s(str, count, L"%s [0x%04X]", L"M_BEVEL", id);                break;
	case XZ_CURVE               : swprintf_s(str, count, L"%s [0x%04X]", L"XZ_CURVE", id);               break;
	case YZ_CURVE               : swprintf_s(str, count, L"%s [0x%04X]", L"YZ_CURVE", id);               break;
	case INTERPCT               : swprintf_s(str, count, L"%s [0x%04X]", L"INTERPCT", id);               break;
	case DEFORM_LIMIT           : swprintf_s(str, count, L"%s [0x%04X]", L"DEFORM_LIMIT", id);           break;
	case USE_CONTOUR            : swprintf_s(str, count, L"%s [0x%04X]", L"USE_CONTOUR", id);            break;
	case USE_TWEEN              : swprintf_s(str, count, L"%s [0x%04X]", L"USE_TWEEN", id);              break;
	case USE_SCALE              : swprintf_s(str, count, L"%s [0x%04X]", L"USE_SCALE", id);              break;
	case USE_TWIST              : swprintf_s(str, count, L"%s [0x%04X]", L"USE_TWIST", id);              break;
	case USE_TEETER             : swprintf_s(str, count, L"%s [0x%04X]", L"USE_TEETER", id);             break;
	case USE_FIT                : swprintf_s(str, count, L"%s [0x%04X]", L"USE_FIT", id);                break;
	case USE_BEVEL              : swprintf_s(str, count, L"%s [0x%04X]", L"USE_BEVEL", id);              break;
	case VIEWPORT_LAYOUT_OLD    : swprintf_s(str, count, L"%s [0x%04X]", L"VIEWPORT_LAYOUT_OLD", id);    break;
	case VIEWPORT_DATA_OLD      : swprintf_s(str, count, L"%s [0x%04X]", L"VIEWPORT_DATA_OLD", id);      break;
	case VIEWPORT_LAYOUT        : swprintf_s(str, count, L"%s [0x%04X]", L"VIEWPORT_LAYOUT", id);        break;
	case VIEWPORT_DATA          : swprintf_s(str, count, L"%s [0x%04X]", L"VIEWPORT_DATA", id);          break;
	case VIEWPORT_DATA_3        : swprintf_s(str, count, L"%s [0x%04X]", L"VIEWPORT_DATA_3", id);        break;
	case VIEWPORT_SIZE          : swprintf_s(str, count, L"%s [0x%04X]", L"VIEWPORT_SIZE", id);          break;
	case NETWORK_VIEW           : swprintf_s(str, count, L"%s [0x%04X]", L"NETWORK_VIEW", id);           break;
	case XDATA_SECTION          : swprintf_s(str, count, L"%s [0x%04X]", L"XDATA_SECTION", id);          break;
	case XDATA_ENTRY            : swprintf_s(str, count, L"%s [0x%04X]", L"XDATA_ENTRY", id);            break;
	case XDATA_APPNAME          : swprintf_s(str, count, L"%s [0x%04X]", L"XDATA_APPNAME", id);          break;
	case XDATA_STRING           : swprintf_s(str, count, L"%s [0x%04X]", L"XDATA_STRING", id);           break;
	case XDATA_FLOAT            : swprintf_s(str, count, L"%s [0x%04X]", L"XDATA_FLOAT", id);            break;
	case XDATA_DOUBLE           : swprintf_s(str, count, L"%s [0x%04X]", L"XDATA_DOUBLE", id);           break;
	case XDATA_SHORT            : swprintf_s(str, count, L"%s [0x%04X]", L"XDATA_SHORT", id);            break;
	case XDATA_LONG             : swprintf_s(str, count, L"%s [0x%04X]", L"XDATA_LONG", id);             break;
	case XDATA_VOID             : swprintf_s(str, count, L"%s [0x%04X]", L"XDATA_VOID", id);             break;
	case XDATA_GROUP            : swprintf_s(str, count, L"%s [0x%04X]", L"XDATA_GROUP", id);            break;
	case XDATA_RFU6             : swprintf_s(str, count, L"%s [0x%04X]", L"XDATA_RFU6", id);             break;
	case XDATA_RFU5             : swprintf_s(str, count, L"%s [0x%04X]", L"XDATA_RFU5", id);             break;
	case XDATA_RFU4             : swprintf_s(str, count, L"%s [0x%04X]", L"XDATA_RFU4", id);             break;
	case XDATA_RFU3             : swprintf_s(str, count, L"%s [0x%04X]", L"XDATA_RFU3", id);             break;
	case XDATA_RFU2             : swprintf_s(str, count, L"%s [0x%04X]", L"XDATA_RFU2", id);             break;
	case XDATA_RFU1             : swprintf_s(str, count, L"%s [0x%04X]", L"XDATA_RFU1", id);             break;
	case PARENT_NAME            : swprintf_s(str, count, L"%s [0x%04X]", L"PARENT_NAME", id);            break;
	case MAT_ENTRY              : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_ENTRY", id);              break;
	case MAT_NAME               : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_NAME", id);               break;
	case MAT_AMBIENT            : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_AMBIENT", id);            break;
	case MAT_DIFFUSE            : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_DIFFUSE", id);            break;
	case MAT_SPECULAR           : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_SPECULAR", id);           break;
	case MAT_SHININESS          : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_SHININESS", id);          break;
	case MAT_SHIN2PCT           : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_SHIN2PCT", id);           break;
	case MAT_SHIN3PCT           : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_SHIN3PCT", id);           break;
	case MAT_TRANSPARENCY       : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_TRANSPARENCY", id);       break;
	case MAT_XPFALL             : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_XPFALL", id);             break;
	case MAT_REFBLUR            : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_REFBLUR", id);            break;
	case MAT_SELF_ILLUM         : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_SELF_ILLUM", id);         break;
	case MAT_TWO_SIDE           : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_TWO_SIDE", id);           break;
	case MAT_DECAL              : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_DECAL", id);              break;
	case MAT_ADDITIVE           : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_ADDITIVE", id);           break;
	case MAT_SELF_ILPCT         : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_SELF_ILPCT", id);         break;
	case MAT_WIRE               : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_WIRE", id);               break;
	case MAT_SUPERSMP           : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_SUPERSMP", id);           break;
	case MAT_WIRESIZE           : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_WIRESIZE", id);           break;
	case MAT_FACEMAP            : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_FACEMAP", id);            break;
	case MAT_XPFALLIN           : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_XPFALLIN", id);           break;
	case MAT_PHONGSOFT          : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_PHONGSOFT", id);          break;
	case MAT_WIREABS            : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_WIREABS", id);            break;
	case MAT_SHADING            : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_SHADING", id);            break;
	case MAT_TEXMAP             : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_TEXMAP", id);             break;
	case MAT_OPACMAP            : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_OPACMAP", id);            break;
	case MAT_REFLMAP            : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_REFLMAP", id);            break;
	case MAT_BUMPMAP            : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_BUMPMAP", id);            break;
	case MAT_SPECMAP            : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_SPECMAP", id);            break;
	case MAT_USE_XPFALL         : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_USE_XPFALL", id);         break;
	case MAT_USE_REFBLUR        : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_USE_REFBLUR", id);        break;
	case MAT_BUMP_PERCENT       : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_BUMP_PERCENT", id);       break;
	case MAT_MAPNAME            : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_MAPNAME", id);            break;
	case MAT_ACUBIC             : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_ACUBIC", id);             break;
	case MAT_SXP_TEXT_DATA      : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_SXP_TEXT_DATA", id);      break;
	case MAT_SXP_TEXT2_DATA     : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_SXP_TEXT2_DATA", id);     break;
	case MAT_SXP_OPAC_DATA      : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_SXP_OPAC_DATA", id);      break;
	case MAT_SXP_BUMP_DATA      : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_SXP_BUMP_DATA", id);      break;
	case MAT_SXP_SPEC_DATA      : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_SXP_SPEC_DATA", id);      break;
	case MAT_SXP_SHIN_DATA      : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_SXP_SHIN_DATA", id);      break;
	case MAT_SXP_SELFI_DATA     : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_SXP_SELFI_DATA", id);     break;
	case MAT_SXP_TEXT_MASKDATA  : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_SXP_TEXT_MASKDATA", id);  break;
	case MAT_SXP_TEXT2_MASKDATA : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_SXP_TEXT2_MASKDATA", id); break;
	case MAT_SXP_OPAC_MASKDATA  : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_SXP_OPAC_MASKDATA", id);  break;
	case MAT_SXP_BUMP_MASKDATA  : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_SXP_BUMP_MASKDATA", id);  break;
	case MAT_SXP_SPEC_MASKDATA  : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_SXP_SPEC_MASKDATA", id);  break;
	case MAT_SXP_SHIN_MASKDATA  : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_SXP_SHIN_MASKDATA", id);  break;
	case MAT_SXP_SELFI_MASKDATA : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_SXP_SELFI_MASKDATA", id); break;
	case MAT_SXP_REFL_MASKDATA  : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_SXP_REFL_MASKDATA", id);  break;
	case MAT_TEX2MAP            : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_TEX2MAP", id);            break;
	case MAT_SHINMAP            : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_SHINMAP", id);            break;
	case MAT_SELFIMAP           : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_SELFIMAP", id);           break;
	case MAT_TEXMASK            : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_TEXMASK", id);            break;
	case MAT_TEX2MASK           : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_TEX2MASK", id);           break;
	case MAT_OPACMASK           : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_OPACMASK", id);           break;
	case MAT_BUMPMASK           : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_BUMPMASK", id);           break;
	case MAT_SHINMASK           : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_SHINMASK", id);           break;
	case MAT_SPECMASK           : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_SPECMASK", id);           break;
	case MAT_SELFIMASK          : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_SELFIMASK", id);          break;
	case MAT_REFLMASK           : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_REFLMASK", id);           break;
	case MAT_MAP_TILINGOLD      : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_MAP_TILINGOLD", id);      break;
	case MAT_MAP_TILING         : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_MAP_TILING", id);         break;
	case MAT_MAP_TEXBLUR_OLD    : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_MAP_TEXBLUR_OLD", id);    break;
	case MAT_MAP_TEXBLUR        : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_MAP_TEXBLUR", id);        break;
	case MAT_MAP_USCALE         : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_MAP_USCALE", id);         break;
	case MAT_MAP_VSCALE         : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_MAP_VSCALE", id);         break;
	case MAT_MAP_UOFFSET        : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_MAP_UOFFSET", id);        break;
	case MAT_MAP_VOFFSET        : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_MAP_VOFFSET", id);        break;
	case MAT_MAP_ANG            : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_MAP_ANG", id);            break;
	case MAT_MAP_COL1           : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_MAP_COL1", id);           break;
	case MAT_MAP_COL2           : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_MAP_COL2", id);           break;
	case MAT_MAP_RCOL           : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_MAP_RCOL", id);           break;
	case MAT_MAP_GCOL           : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_MAP_GCOL", id);           break;
	case MAT_MAP_BCOL           : swprintf_s(str, count, L"%s [0x%04X]", L"MAT_MAP_BCOL", id);           break;
	case KFDATA                 : swprintf_s(str, count, L"%s [0x%04X]", L"KFDATA", id);                 break;
	case KFHDR                  : swprintf_s(str, count, L"%s [0x%04X]", L"KFHDR", id);                  break;
	case AMBIENT_NODE_TAG       : swprintf_s(str, count, L"%s [0x%04X]", L"AMBIENT_NODE_TAG", id);       break;
	case OBJECT_NODE_TAG        : swprintf_s(str, count, L"%s [0x%04X]", L"OBJECT_NODE_TAG", id);        break;
	case CAMERA_NODE_TAG        : swprintf_s(str, count, L"%s [0x%04X]", L"CAMERA_NODE_TAG", id);        break;
	case TARGET_NODE_TAG        : swprintf_s(str, count, L"%s [0x%04X]", L"TARGET_NODE_TAG", id);        break;
	case LIGHT_NODE_TAG         : swprintf_s(str, count, L"%s [0x%04X]", L"LIGHT_NODE_TAG", id);         break;
	case L_TARGET_NODE_TAG      : swprintf_s(str, count, L"%s [0x%04X]", L"L_TARGET_NODE_TAG", id);      break;
	case SPOTLIGHT_NODE_TAG     : swprintf_s(str, count, L"%s [0x%04X]", L"SPOTLIGHT_NODE_TAG", id);     break;
	case KFSEG                  : swprintf_s(str, count, L"%s [0x%04X]", L"KFSEG", id);                  break;
	case KFCURTIME              : swprintf_s(str, count, L"%s [0x%04X]", L"KFCURTIME", id);              break;
	case NODE_HDR               : swprintf_s(str, count, L"%s [0x%04X]", L"NODE_HDR", id);               break;
	case INSTANCE_NAME          : swprintf_s(str, count, L"%s [0x%04X]", L"INSTANCE_NAME", id);          break;
	case PRESCALE               : swprintf_s(str, count, L"%s [0x%04X]", L"PRESCALE", id);               break;
	case PIVOT                  : swprintf_s(str, count, L"%s [0x%04X]", L"PIVOT", id);                  break;
	case BOUNDBOX               : swprintf_s(str, count, L"%s [0x%04X]", L"BOUNDBOX", id);               break;
	case MORPH_SMOOTH           : swprintf_s(str, count, L"%s [0x%04X]", L"MORPH_SMOOTH", id);           break;
	case POS_TRACK_TAG          : swprintf_s(str, count, L"%s [0x%04X]", L"POS_TRACK_TAG", id);          break;
	case ROT_TRACK_TAG          : swprintf_s(str, count, L"%s [0x%04X]", L"ROT_TRACK_TAG", id);          break;
	case SCL_TRACK_TAG          : swprintf_s(str, count, L"%s [0x%04X]", L"SCL_TRACK_TAG", id);          break;
	case FOV_TRACK_TAG          : swprintf_s(str, count, L"%s [0x%04X]", L"FOV_TRACK_TAG", id);          break;
	case ROLL_TRACK_TAG         : swprintf_s(str, count, L"%s [0x%04X]", L"ROLL_TRACK_TAG", id);         break;
	case COL_TRACK_TAG          : swprintf_s(str, count, L"%s [0x%04X]", L"COL_TRACK_TAG", id);          break;
	case MORPH_TRACK_TAG        : swprintf_s(str, count, L"%s [0x%04X]", L"MORPH_TRACK_TAG", id);        break;
	case HOT_TRACK_TAG          : swprintf_s(str, count, L"%s [0x%04X]", L"HOT_TRACK_TAG", id);          break;
	case FALL_TRACK_TAG         : swprintf_s(str, count, L"%s [0x%04X]", L"FALL_TRACK_TAG", id);         break;
	case HIDE_TRACK_TAG         : swprintf_s(str, count, L"%s [0x%04X]", L"HIDE_TRACK_TAG", id);         break;
	case NODE_ID                : swprintf_s(str, count, L"%s [0x%04X]", L"NODE_ID", id);                break;
	case CMAGIC                 : swprintf_s(str, count, L"%s [0x%04X]", L"CMAGIC", id);                 break;
	case C_MDRAWER              : swprintf_s(str, count, L"%s [0x%04X]", L"C_MDRAWER", id);              break;
	case C_TDRAWER              : swprintf_s(str, count, L"%s [0x%04X]", L"C_TDRAWER", id);              break;
	case C_SHPDRAWER            : swprintf_s(str, count, L"%s [0x%04X]", L"C_SHPDRAWER", id);            break;
	case C_MODDRAWER            : swprintf_s(str, count, L"%s [0x%04X]", L"C_MODDRAWER", id);            break;
	case C_RIPDRAWER            : swprintf_s(str, count, L"%s [0x%04X]", L"C_RIPDRAWER", id);            break;
	case C_TXDRAWER             : swprintf_s(str, count, L"%s [0x%04X]", L"C_TXDRAWER", id);             break;
	case C_PDRAWER              : swprintf_s(str, count, L"%s [0x%04X]", L"C_PDRAWER", id);              break;
	case C_MTLDRAWER            : swprintf_s(str, count, L"%s [0x%04X]", L"C_MTLDRAWER", id);            break;
	case C_FLIDRAWER            : swprintf_s(str, count, L"%s [0x%04X]", L"C_FLIDRAWER", id);            break;
	case C_CUBDRAWER            : swprintf_s(str, count, L"%s [0x%04X]", L"C_CUBDRAWER", id);            break;
	case C_MFILE                : swprintf_s(str, count, L"%s [0x%04X]", L"C_MFILE", id);                break;
	case C_SHPFILE              : swprintf_s(str, count, L"%s [0x%04X]", L"C_SHPFILE", id);              break;
	case C_MODFILE              : swprintf_s(str, count, L"%s [0x%04X]", L"C_MODFILE", id);              break;
	case C_RIPFILE              : swprintf_s(str, count, L"%s [0x%04X]", L"C_RIPFILE", id);              break;
	case C_TXFILE               : swprintf_s(str, count, L"%s [0x%04X]", L"C_TXFILE", id);               break;
	case C_PFILE                : swprintf_s(str, count, L"%s [0x%04X]", L"C_PFILE", id);                break;
	case C_MTLFILE              : swprintf_s(str, count, L"%s [0x%04X]", L"C_MTLFILE", id);              break;
	case C_FLIFILE              : swprintf_s(str, count, L"%s [0x%04X]", L"C_FLIFILE", id);              break;
	case C_PALFILE              : swprintf_s(str, count, L"%s [0x%04X]", L"C_PALFILE", id);              break;
	case C_TX_STRING            : swprintf_s(str, count, L"%s [0x%04X]", L"C_TX_STRING", id);            break;
	case C_CONSTS               : swprintf_s(str, count, L"%s [0x%04X]", L"C_CONSTS", id);               break;
	case C_SNAPS                : swprintf_s(str, count, L"%s [0x%04X]", L"C_SNAPS", id);                break;
	case C_GRIDS                : swprintf_s(str, count, L"%s [0x%04X]", L"C_GRIDS", id);                break;
	case C_ASNAPS               : swprintf_s(str, count, L"%s [0x%04X]", L"C_ASNAPS", id);               break;
	case C_GRID_RANGE           : swprintf_s(str, count, L"%s [0x%04X]", L"C_GRID_RANGE", id);           break;
	case C_RENDTYPE             : swprintf_s(str, count, L"%s [0x%04X]", L"C_RENDTYPE", id);             break;
	case C_PROGMODE             : swprintf_s(str, count, L"%s [0x%04X]", L"C_PROGMODE", id);             break;
	case C_PREVMODE             : swprintf_s(str, count, L"%s [0x%04X]", L"C_PREVMODE", id);             break;
	case C_MODWMODE             : swprintf_s(str, count, L"%s [0x%04X]", L"C_MODWMODE", id);             break;
	case C_MODMODEL             : swprintf_s(str, count, L"%s [0x%04X]", L"C_MODMODEL", id);             break;
	case C_ALL_LINES            : swprintf_s(str, count, L"%s [0x%04X]", L"C_ALL_LINES", id);            break;
	case C_BACK_TYPE            : swprintf_s(str, count, L"%s [0x%04X]", L"C_BACK_TYPE", id);            break;
	case C_MD_CS                : swprintf_s(str, count, L"%s [0x%04X]", L"C_MD_CS", id);                break;
	case C_MD_CE                : swprintf_s(str, count, L"%s [0x%04X]", L"C_MD_CE", id);                break;
	case C_MD_SML               : swprintf_s(str, count, L"%s [0x%04X]", L"C_MD_SML", id);               break;
	case C_MD_SMW               : swprintf_s(str, count, L"%s [0x%04X]", L"C_MD_SMW", id);               break;
	case C_LOFT_WITH_TEXTURE    : swprintf_s(str, count, L"%s [0x%04X]", L"C_LOFT_WITH_TEXTURE", id);    break;
	case C_LOFT_L_REPEAT        : swprintf_s(str, count, L"%s [0x%04X]", L"C_LOFT_L_REPEAT", id);        break;
	case C_LOFT_W_REPEAT        : swprintf_s(str, count, L"%s [0x%04X]", L"C_LOFT_W_REPEAT", id);        break;
	case C_LOFT_UV_NORMALIZE    : swprintf_s(str, count, L"%s [0x%04X]", L"C_LOFT_UV_NORMALIZE", id);    break;
	case C_WELD_LOFT            : swprintf_s(str, count, L"%s [0x%04X]", L"C_WELD_LOFT", id);            break;
	case C_MD_PDET              : swprintf_s(str, count, L"%s [0x%04X]", L"C_MD_PDET", id);              break;
	case C_MD_SDET              : swprintf_s(str, count, L"%s [0x%04X]", L"C_MD_SDET", id);              break;
	case C_RGB_RMODE            : swprintf_s(str, count, L"%s [0x%04X]", L"C_RGB_RMODE", id);            break;
	case C_RGB_HIDE             : swprintf_s(str, count, L"%s [0x%04X]", L"C_RGB_HIDE", id);             break;
	case C_RGB_MAPSW            : swprintf_s(str, count, L"%s [0x%04X]", L"C_RGB_MAPSW", id);            break;
	case C_RGB_TWOSIDE          : swprintf_s(str, count, L"%s [0x%04X]", L"C_RGB_TWOSIDE", id);          break;
	case C_RGB_SHADOW           : swprintf_s(str, count, L"%s [0x%04X]", L"C_RGB_SHADOW", id);           break;
	case C_RGB_AA               : swprintf_s(str, count, L"%s [0x%04X]", L"C_RGB_AA", id);               break;
	case C_RGB_OVW              : swprintf_s(str, count, L"%s [0x%04X]", L"C_RGB_OVW", id);              break;
	case C_RGB_OVH              : swprintf_s(str, count, L"%s [0x%04X]", L"C_RGB_OVH", id);              break;
	case C_RGB_PICTYPE          : swprintf_s(str, count, L"%s [0x%04X]", L"C_RGB_PICTYPE", id);          break;
	case C_RGB_OUTPUT           : swprintf_s(str, count, L"%s [0x%04X]", L"C_RGB_OUTPUT", id);           break;
	case C_RGB_TODISK           : swprintf_s(str, count, L"%s [0x%04X]", L"C_RGB_TODISK", id);           break;
	case C_RGB_COMPRESS         : swprintf_s(str, count, L"%s [0x%04X]", L"C_RGB_COMPRESS", id);         break;
	case C_JPEG_COMPRESSION     : swprintf_s(str, count, L"%s [0x%04X]", L"C_JPEG_COMPRESSION", id);     break;
	case C_RGB_DISPDEV          : swprintf_s(str, count, L"%s [0x%04X]", L"C_RGB_DISPDEV", id);          break;
	case C_RGB_HARDDEV          : swprintf_s(str, count, L"%s [0x%04X]", L"C_RGB_HARDDEV", id);          break;
	case C_RGB_PATH             : swprintf_s(str, count, L"%s [0x%04X]", L"C_RGB_PATH", id);             break;
	case C_BITMAP_DRAWER        : swprintf_s(str, count, L"%s [0x%04X]", L"C_BITMAP_DRAWER", id);        break;
	case C_RGB_FILE             : swprintf_s(str, count, L"%s [0x%04X]", L"C_RGB_FILE", id);             break;
	case C_RGB_OVASPECT         : swprintf_s(str, count, L"%s [0x%04X]", L"C_RGB_OVASPECT", id);         break;
	case C_RGB_ANIMTYPE         : swprintf_s(str, count, L"%s [0x%04X]", L"C_RGB_ANIMTYPE", id);         break;
	case C_RENDER_ALL           : swprintf_s(str, count, L"%s [0x%04X]", L"C_RENDER_ALL", id);           break;
	case C_REND_FROM            : swprintf_s(str, count, L"%s [0x%04X]", L"C_REND_FROM", id);            break;
	case C_REND_TO              : swprintf_s(str, count, L"%s [0x%04X]", L"C_REND_TO", id);              break;
	case C_REND_NTH             : swprintf_s(str, count, L"%s [0x%04X]", L"C_REND_NTH", id);             break;
	case C_PAL_TYPE             : swprintf_s(str, count, L"%s [0x%04X]", L"C_PAL_TYPE", id);             break;
	case C_RND_TURBO            : swprintf_s(str, count, L"%s [0x%04X]", L"C_RND_TURBO", id);            break;
	case C_RND_MIP              : swprintf_s(str, count, L"%s [0x%04X]", L"C_RND_MIP", id);              break;
	case C_BGND_METHOD          : swprintf_s(str, count, L"%s [0x%04X]", L"C_BGND_METHOD", id);          break;
	case C_AUTO_REFLECT         : swprintf_s(str, count, L"%s [0x%04X]", L"C_AUTO_REFLECT", id);         break;
	case C_VP_FROM              : swprintf_s(str, count, L"%s [0x%04X]", L"C_VP_FROM", id);              break;
	case C_VP_TO                : swprintf_s(str, count, L"%s [0x%04X]", L"C_VP_TO", id);                break;
	case C_VP_NTH               : swprintf_s(str, count, L"%s [0x%04X]", L"C_VP_NTH", id);               break;
	case C_REND_TSTEP           : swprintf_s(str, count, L"%s [0x%04X]", L"C_REND_TSTEP", id);           break;
	case C_VP_TSTEP             : swprintf_s(str, count, L"%s [0x%04X]", L"C_VP_TSTEP", id);             break;
	case C_SRDIAM               : swprintf_s(str, count, L"%s [0x%04X]", L"C_SRDIAM", id);               break;
	case C_SRDEG                : swprintf_s(str, count, L"%s [0x%04X]", L"C_SRDEG", id);                break;
	case C_SRSEG                : swprintf_s(str, count, L"%s [0x%04X]", L"C_SRSEG", id);                break;
	case C_SRDIR                : swprintf_s(str, count, L"%s [0x%04X]", L"C_SRDIR", id);                break;
	case C_HETOP                : swprintf_s(str, count, L"%s [0x%04X]", L"C_HETOP", id);                break;
	case C_HEBOT                : swprintf_s(str, count, L"%s [0x%04X]", L"C_HEBOT", id);                break;
	case C_HEHT                 : swprintf_s(str, count, L"%s [0x%04X]", L"C_HEHT", id);                 break;
	case C_HETURNS              : swprintf_s(str, count, L"%s [0x%04X]", L"C_HETURNS", id);              break;
	case C_HEDEG                : swprintf_s(str, count, L"%s [0x%04X]", L"C_HEDEG", id);                break;
	case C_HESEG                : swprintf_s(str, count, L"%s [0x%04X]", L"C_HESEG", id);                break;
	case C_HEDIR                : swprintf_s(str, count, L"%s [0x%04X]", L"C_HEDIR", id);                break;
	case C_QUIKSTUFF            : swprintf_s(str, count, L"%s [0x%04X]", L"C_QUIKSTUFF", id);            break;
	case C_SEE_LIGHTS           : swprintf_s(str, count, L"%s [0x%04X]", L"C_SEE_LIGHTS", id);           break;
	case C_SEE_CAMERAS          : swprintf_s(str, count, L"%s [0x%04X]", L"C_SEE_CAMERAS", id);          break;
	case C_SEE_3D               : swprintf_s(str, count, L"%s [0x%04X]", L"C_SEE_3D", id);               break;
	case C_MESHSEL              : swprintf_s(str, count, L"%s [0x%04X]", L"C_MESHSEL", id);              break;
	case C_MESHUNSEL            : swprintf_s(str, count, L"%s [0x%04X]", L"C_MESHUNSEL", id);            break;
	case C_POLYSEL              : swprintf_s(str, count, L"%s [0x%04X]", L"C_POLYSEL", id);              break;
	case C_POLYUNSEL            : swprintf_s(str, count, L"%s [0x%04X]", L"C_POLYUNSEL", id);            break;
	case C_SHPLOCAL             : swprintf_s(str, count, L"%s [0x%04X]", L"C_SHPLOCAL", id);             break;
	case C_MSHLOCAL             : swprintf_s(str, count, L"%s [0x%04X]", L"C_MSHLOCAL", id);             break;
	case C_NUM_FORMAT           : swprintf_s(str, count, L"%s [0x%04X]", L"C_NUM_FORMAT", id);           break;
	case C_ARCH_DENOM           : swprintf_s(str, count, L"%s [0x%04X]", L"C_ARCH_DENOM", id);           break;
	case C_IN_DEVICE            : swprintf_s(str, count, L"%s [0x%04X]", L"C_IN_DEVICE", id);            break;
	case C_MSCALE               : swprintf_s(str, count, L"%s [0x%04X]", L"C_MSCALE", id);               break;
	case C_COMM_PORT            : swprintf_s(str, count, L"%s [0x%04X]", L"C_COMM_PORT", id);            break;
	case C_TAB_BASES            : swprintf_s(str, count, L"%s [0x%04X]", L"C_TAB_BASES", id);            break;
	case C_TAB_DIVS             : swprintf_s(str, count, L"%s [0x%04X]", L"C_TAB_DIVS", id);             break;
	case C_MASTER_SCALES        : swprintf_s(str, count, L"%s [0x%04X]", L"C_MASTER_SCALES", id);        break;
	case C_SHOW_1STVERT         : swprintf_s(str, count, L"%s [0x%04X]", L"C_SHOW_1STVERT", id);         break;
	case C_SHAPER_OK            : swprintf_s(str, count, L"%s [0x%04X]", L"C_SHAPER_OK", id);            break;
	case C_LOFTER_OK            : swprintf_s(str, count, L"%s [0x%04X]", L"C_LOFTER_OK", id);            break;
	case C_EDITOR_OK            : swprintf_s(str, count, L"%s [0x%04X]", L"C_EDITOR_OK", id);            break;
	case C_KEYFRAMER_OK         : swprintf_s(str, count, L"%s [0x%04X]", L"C_KEYFRAMER_OK", id);         break;
	case C_PICKSIZE             : swprintf_s(str, count, L"%s [0x%04X]", L"C_PICKSIZE", id);             break;
	case C_MAPTYPE              : swprintf_s(str, count, L"%s [0x%04X]", L"C_MAPTYPE", id);              break;
	case C_MAP_DISPLAY          : swprintf_s(str, count, L"%s [0x%04X]", L"C_MAP_DISPLAY", id);          break;
	case C_TILE_XY              : swprintf_s(str, count, L"%s [0x%04X]", L"C_TILE_XY", id);              break;
	case C_MAP_XYZ              : swprintf_s(str, count, L"%s [0x%04X]", L"C_MAP_XYZ", id);              break;
	case C_MAP_SCALE            : swprintf_s(str, count, L"%s [0x%04X]", L"C_MAP_SCALE", id);            break;
	case C_MAP_MATRIX_OLD       : swprintf_s(str, count, L"%s [0x%04X]", L"C_MAP_MATRIX_OLD", id);       break;
	case C_MAP_MATRIX           : swprintf_s(str, count, L"%s [0x%04X]", L"C_MAP_MATRIX", id);           break;
	case C_MAP_WID_HT           : swprintf_s(str, count, L"%s [0x%04X]", L"C_MAP_WID_HT", id);           break;
	case C_OBNAME               : swprintf_s(str, count, L"%s [0x%04X]", L"C_OBNAME", id);               break;
	case C_CAMNAME              : swprintf_s(str, count, L"%s [0x%04X]", L"C_CAMNAME", id);              break;
	case C_LTNAME               : swprintf_s(str, count, L"%s [0x%04X]", L"C_LTNAME", id);               break;
	case C_CUR_MNAME            : swprintf_s(str, count, L"%s [0x%04X]", L"C_CUR_MNAME", id);            break;
	case C_CURMTL_FROM_MESH     : swprintf_s(str, count, L"%s [0x%04X]", L"C_CURMTL_FROM_MESH", id);     break;
	case C_GET_SHAPE_MAKE_FACES : swprintf_s(str, count, L"%s [0x%04X]", L"C_GET_SHAPE_MAKE_FACES", id); break;
	case C_DETAIL               : swprintf_s(str, count, L"%s [0x%04X]", L"C_DETAIL", id);               break;
	case C_VERTMARK             : swprintf_s(str, count, L"%s [0x%04X]", L"C_VERTMARK", id);             break;
	case C_MSHAX                : swprintf_s(str, count, L"%s [0x%04X]", L"C_MSHAX", id);                break;
	case C_MSHCP                : swprintf_s(str, count, L"%s [0x%04X]", L"C_MSHCP", id);                break;
	case C_USERAX               : swprintf_s(str, count, L"%s [0x%04X]", L"C_USERAX", id);               break;
	case C_SHOOK                : swprintf_s(str, count, L"%s [0x%04X]", L"C_SHOOK", id);                break;
	case C_RAX                  : swprintf_s(str, count, L"%s [0x%04X]", L"C_RAX", id);                  break;
	case C_STAPE                : swprintf_s(str, count, L"%s [0x%04X]", L"C_STAPE", id);                break;
	case C_LTAPE                : swprintf_s(str, count, L"%s [0x%04X]", L"C_LTAPE", id);                break;
	case C_ETAPE                : swprintf_s(str, count, L"%s [0x%04X]", L"C_ETAPE", id);                break;
	case C_KTAPE                : swprintf_s(str, count, L"%s [0x%04X]", L"C_KTAPE", id);                break;
	case C_SPHSEGS              : swprintf_s(str, count, L"%s [0x%04X]", L"C_SPHSEGS", id);              break;
	case C_GEOSMOOTH            : swprintf_s(str, count, L"%s [0x%04X]", L"C_GEOSMOOTH", id);            break;
	case C_HEMISEGS             : swprintf_s(str, count, L"%s [0x%04X]", L"C_HEMISEGS", id);             break;
	case C_PRISMSEGS            : swprintf_s(str, count, L"%s [0x%04X]", L"C_PRISMSEGS", id);            break;
	case C_PRISMSIDES           : swprintf_s(str, count, L"%s [0x%04X]", L"C_PRISMSIDES", id);           break;
	case C_TUBESEGS             : swprintf_s(str, count, L"%s [0x%04X]", L"C_TUBESEGS", id);             break;
	case C_TUBESIDES            : swprintf_s(str, count, L"%s [0x%04X]", L"C_TUBESIDES", id);            break;
	case C_TORSEGS              : swprintf_s(str, count, L"%s [0x%04X]", L"C_TORSEGS", id);              break;
	case C_TORSIDES             : swprintf_s(str, count, L"%s [0x%04X]", L"C_TORSIDES", id);             break;
	case C_CONESIDES            : swprintf_s(str, count, L"%s [0x%04X]", L"C_CONESIDES", id);            break;
	case C_CONESEGS             : swprintf_s(str, count, L"%s [0x%04X]", L"C_CONESEGS", id);             break;
	case C_NGPARMS              : swprintf_s(str, count, L"%s [0x%04X]", L"C_NGPARMS", id);              break;
	case C_PTHLEVEL             : swprintf_s(str, count, L"%s [0x%04X]", L"C_PTHLEVEL", id);             break;
	case C_MSCSYM               : swprintf_s(str, count, L"%s [0x%04X]", L"C_MSCSYM", id);               break;
	case C_MFTSYM               : swprintf_s(str, count, L"%s [0x%04X]", L"C_MFTSYM", id);               break;
	case C_MTTSYM               : swprintf_s(str, count, L"%s [0x%04X]", L"C_MTTSYM", id);               break;
	case C_SMOOTHING            : swprintf_s(str, count, L"%s [0x%04X]", L"C_SMOOTHING", id);            break;
	case C_MODICOUNT            : swprintf_s(str, count, L"%s [0x%04X]", L"C_MODICOUNT", id);            break;
	case C_FONTSEL              : swprintf_s(str, count, L"%s [0x%04X]", L"C_FONTSEL", id);              break;
	case C_TESS_TYPE            : swprintf_s(str, count, L"%s [0x%04X]", L"C_TESS_TYPE", id);            break;
	case C_TESS_TENSION         : swprintf_s(str, count, L"%s [0x%04X]", L"C_TESS_TENSION", id);         break;
	case C_SEG_START            : swprintf_s(str, count, L"%s [0x%04X]", L"C_SEG_START", id);            break;
	case C_SEG_END              : swprintf_s(str, count, L"%s [0x%04X]", L"C_SEG_END", id);              break;
	case C_CURTIME              : swprintf_s(str, count, L"%s [0x%04X]", L"C_CURTIME", id);              break;
	case C_ANIMLENGTH           : swprintf_s(str, count, L"%s [0x%04X]", L"C_ANIMLENGTH", id);           break;
	case C_PV_FROM              : swprintf_s(str, count, L"%s [0x%04X]", L"C_PV_FROM", id);              break;
	case C_PV_TO                : swprintf_s(str, count, L"%s [0x%04X]", L"C_PV_TO", id);                break;
	case C_PV_DOFNUM            : swprintf_s(str, count, L"%s [0x%04X]", L"C_PV_DOFNUM", id);            break;
	case C_PV_RNG               : swprintf_s(str, count, L"%s [0x%04X]", L"C_PV_RNG", id);               break;
	case C_PV_NTH               : swprintf_s(str, count, L"%s [0x%04X]", L"C_PV_NTH", id);               break;
	case C_PV_TYPE              : swprintf_s(str, count, L"%s [0x%04X]", L"C_PV_TYPE", id);              break;
	case C_PV_METHOD            : swprintf_s(str, count, L"%s [0x%04X]", L"C_PV_METHOD", id);            break;
	case C_PV_FPS               : swprintf_s(str, count, L"%s [0x%04X]", L"C_PV_FPS", id);               break;
	case C_VTR_FRAMES           : swprintf_s(str, count, L"%s [0x%04X]", L"C_VTR_FRAMES", id);           break;
	case C_VTR_HDTL             : swprintf_s(str, count, L"%s [0x%04X]", L"C_VTR_HDTL", id);             break;
	case C_VTR_HD               : swprintf_s(str, count, L"%s [0x%04X]", L"C_VTR_HD", id);               break;
	case C_VTR_TL               : swprintf_s(str, count, L"%s [0x%04X]", L"C_VTR_TL", id);               break;
	case C_VTR_IN               : swprintf_s(str, count, L"%s [0x%04X]", L"C_VTR_IN", id);               break;
	case C_VTR_PK               : swprintf_s(str, count, L"%s [0x%04X]", L"C_VTR_PK", id);               break;
	case C_VTR_SH               : swprintf_s(str, count, L"%s [0x%04X]", L"C_VTR_SH", id);               break;
	case C_WORK_MTLS            : swprintf_s(str, count, L"%s [0x%04X]", L"C_WORK_MTLS", id);            break;
	case C_WORK_MTLS_2          : swprintf_s(str, count, L"%s [0x%04X]", L"C_WORK_MTLS_2", id);          break;
	case C_WORK_MTLS_3          : swprintf_s(str, count, L"%s [0x%04X]", L"C_WORK_MTLS_3", id);          break;
	case C_WORK_MTLS_4          : swprintf_s(str, count, L"%s [0x%04X]", L"C_WORK_MTLS_4", id);          break;
	case C_WORK_MTLS_5          : swprintf_s(str, count, L"%s [0x%04X]", L"C_WORK_MTLS_5", id);          break;
	case C_WORK_MTLS_6          : swprintf_s(str, count, L"%s [0x%04X]", L"C_WORK_MTLS_6", id);          break;
	case C_WORK_MTLS_7          : swprintf_s(str, count, L"%s [0x%04X]", L"C_WORK_MTLS_7", id);          break;
	case C_WORK_MTLS_8          : swprintf_s(str, count, L"%s [0x%04X]", L"C_WORK_MTLS_8", id);          break;
	case C_WORKMTL              : swprintf_s(str, count, L"%s [0x%04X]", L"C_WORKMTL", id);              break;
	case C_SXP_TEXT_DATA        : swprintf_s(str, count, L"%s [0x%04X]", L"C_SXP_TEXT_DATA", id);        break;
	case C_SXP_TEXT2_DATA       : swprintf_s(str, count, L"%s [0x%04X]", L"C_SXP_TEXT2_DATA", id);       break;
	case C_SXP_OPAC_DATA        : swprintf_s(str, count, L"%s [0x%04X]", L"C_SXP_OPAC_DATA", id);        break;
	case C_SXP_BUMP_DATA        : swprintf_s(str, count, L"%s [0x%04X]", L"C_SXP_BUMP_DATA", id);        break;
	case C_SXP_SPEC_DATA        : swprintf_s(str, count, L"%s [0x%04X]", L"C_SXP_SPEC_DATA", id);        break;
	case C_SXP_SHIN_DATA        : swprintf_s(str, count, L"%s [0x%04X]", L"C_SXP_SHIN_DATA", id);        break;
	case C_SXP_SELFI_DATA       : swprintf_s(str, count, L"%s [0x%04X]", L"C_SXP_SELFI_DATA", id);       break;
	case C_SXP_TEXT_MASKDATA    : swprintf_s(str, count, L"%s [0x%04X]", L"C_SXP_TEXT_MASKDATA", id);    break;
	case C_SXP_TEXT2_MASKDATA   : swprintf_s(str, count, L"%s [0x%04X]", L"C_SXP_TEXT2_MASKDATA", id);   break;
	case C_SXP_OPAC_MASKDATA    : swprintf_s(str, count, L"%s [0x%04X]", L"C_SXP_OPAC_MASKDATA", id);    break;
	case C_SXP_BUMP_MASKDATA    : swprintf_s(str, count, L"%s [0x%04X]", L"C_SXP_BUMP_MASKDATA", id);    break;
	case C_SXP_SPEC_MASKDATA    : swprintf_s(str, count, L"%s [0x%04X]", L"C_SXP_SPEC_MASKDATA", id);    break;
	case C_SXP_SHIN_MASKDATA    : swprintf_s(str, count, L"%s [0x%04X]", L"C_SXP_SHIN_MASKDATA", id);    break;
	case C_SXP_SELFI_MASKDATA   : swprintf_s(str, count, L"%s [0x%04X]", L"C_SXP_SELFI_MASKDATA", id);   break;
	case C_SXP_REFL_MASKDATA    : swprintf_s(str, count, L"%s [0x%04X]", L"C_SXP_REFL_MASKDATA", id);    break;
	case C_BGTYPE               : swprintf_s(str, count, L"%s [0x%04X]", L"C_BGTYPE", id);               break;
	case C_MEDTILE              : swprintf_s(str, count, L"%s [0x%04X]", L"C_MEDTILE", id);              break;
	case C_LO_CONTRAST          : swprintf_s(str, count, L"%s [0x%04X]", L"C_LO_CONTRAST", id);          break;
	case C_HI_CONTRAST          : swprintf_s(str, count, L"%s [0x%04X]", L"C_HI_CONTRAST", id);          break;
	case C_FROZ_DISPLAY         : swprintf_s(str, count, L"%s [0x%04X]", L"C_FROZ_DISPLAY", id);         break;
	case C_BOOLWELD             : swprintf_s(str, count, L"%s [0x%04X]", L"C_BOOLWELD", id);             break;
	case C_BOOLTYPE             : swprintf_s(str, count, L"%s [0x%04X]", L"C_BOOLTYPE", id);             break;
	case C_ANG_THRESH           : swprintf_s(str, count, L"%s [0x%04X]", L"C_ANG_THRESH", id);           break;
	case C_SS_THRESH            : swprintf_s(str, count, L"%s [0x%04X]", L"C_SS_THRESH", id);            break;
	case C_TEXTURE_BLUR_DEFAULT : swprintf_s(str, count, L"%s [0x%04X]", L"C_TEXTURE_BLUR_DEFAULT", id); break;
	case C_MAPDRAWER            : swprintf_s(str, count, L"%s [0x%04X]", L"C_MAPDRAWER", id);            break;
	case C_MAPDRAWER1           : swprintf_s(str, count, L"%s [0x%04X]", L"C_MAPDRAWER1", id);           break;
	case C_MAPDRAWER2           : swprintf_s(str, count, L"%s [0x%04X]", L"C_MAPDRAWER2", id);           break;
	case C_MAPDRAWER3           : swprintf_s(str, count, L"%s [0x%04X]", L"C_MAPDRAWER3", id);           break;
	case C_MAPDRAWER4           : swprintf_s(str, count, L"%s [0x%04X]", L"C_MAPDRAWER4", id);           break;
	case C_MAPDRAWER5           : swprintf_s(str, count, L"%s [0x%04X]", L"C_MAPDRAWER5", id);           break;
	case C_MAPDRAWER6           : swprintf_s(str, count, L"%s [0x%04X]", L"C_MAPDRAWER6", id);           break;
	case C_MAPDRAWER7           : swprintf_s(str, count, L"%s [0x%04X]", L"C_MAPDRAWER7", id);           break;
	case C_MAPDRAWER8           : swprintf_s(str, count, L"%s [0x%04X]", L"C_MAPDRAWER8", id);           break;
	case C_MAPDRAWER9           : swprintf_s(str, count, L"%s [0x%04X]", L"C_MAPDRAWER9", id);           break;
	case C_MAPDRAWER_ENTRY      : swprintf_s(str, count, L"%s [0x%04X]", L"C_MAPDRAWER_ENTRY", id);      break;
	case C_BACKUP_FILE          : swprintf_s(str, count, L"%s [0x%04X]", L"C_BACKUP_FILE", id);          break;
	case C_DITHER_256           : swprintf_s(str, count, L"%s [0x%04X]", L"C_DITHER_256", id);           break;
	case C_SAVE_LAST            : swprintf_s(str, count, L"%s [0x%04X]", L"C_SAVE_LAST", id);            break;
	case C_USE_ALPHA            : swprintf_s(str, count, L"%s [0x%04X]", L"C_USE_ALPHA", id);            break;
	case C_TGA_DEPTH            : swprintf_s(str, count, L"%s [0x%04X]", L"C_TGA_DEPTH", id);            break;
	case C_REND_FIELDS          : swprintf_s(str, count, L"%s [0x%04X]", L"C_REND_FIELDS", id);          break;
	case C_REFLIP               : swprintf_s(str, count, L"%s [0x%04X]", L"C_REFLIP", id);               break;
	case C_SEL_ITEMTOG          : swprintf_s(str, count, L"%s [0x%04X]", L"C_SEL_ITEMTOG", id);          break;
	case C_SEL_RESET            : swprintf_s(str, count, L"%s [0x%04X]", L"C_SEL_RESET", id);            break;
	case C_STICKY_KEYINF        : swprintf_s(str, count, L"%s [0x%04X]", L"C_STICKY_KEYINF", id);        break;
	case C_WELD_THRESHOLD       : swprintf_s(str, count, L"%s [0x%04X]", L"C_WELD_THRESHOLD", id);       break;
	case C_ZCLIP_POINT          : swprintf_s(str, count, L"%s [0x%04X]", L"C_ZCLIP_POINT", id);          break;
	case C_ALPHA_SPLIT          : swprintf_s(str, count, L"%s [0x%04X]", L"C_ALPHA_SPLIT", id);          break;
	case C_KF_SHOW_BACKFACE     : swprintf_s(str, count, L"%s [0x%04X]", L"C_KF_SHOW_BACKFACE", id);     break;
	case C_OPTIMIZE_LOFT        : swprintf_s(str, count, L"%s [0x%04X]", L"C_OPTIMIZE_LOFT", id);        break;
	case C_TENS_DEFAULT         : swprintf_s(str, count, L"%s [0x%04X]", L"C_TENS_DEFAULT", id);         break;
	case C_CONT_DEFAULT         : swprintf_s(str, count, L"%s [0x%04X]", L"C_CONT_DEFAULT", id);         break;
	case C_BIAS_DEFAULT         : swprintf_s(str, count, L"%s [0x%04X]", L"C_BIAS_DEFAULT", id);         break;
	case C_DXFNAME_SRC          : swprintf_s(str, count, L"%s [0x%04X]", L"C_DXFNAME_SRC", id);          break;
	case C_AUTO_WELD            : swprintf_s(str, count, L"%s [0x%04X]", L"C_AUTO_WELD", id);            break;
	case C_AUTO_UNIFY           : swprintf_s(str, count, L"%s [0x%04X]", L"C_AUTO_UNIFY", id);           break;
	case C_AUTO_SMOOTH          : swprintf_s(str, count, L"%s [0x%04X]", L"C_AUTO_SMOOTH", id);          break;
	case C_DXF_SMOOTH_ANG       : swprintf_s(str, count, L"%s [0x%04X]", L"C_DXF_SMOOTH_ANG", id);       break;
	case C_SMOOTH_ANG           : swprintf_s(str, count, L"%s [0x%04X]", L"C_SMOOTH_ANG", id);           break;
	case C_NET_USE_VPOST        : swprintf_s(str, count, L"%s [0x%04X]", L"C_NET_USE_VPOST", id);        break;
	case C_NET_USE_GAMMA        : swprintf_s(str, count, L"%s [0x%04X]", L"C_NET_USE_GAMMA", id);        break;
	case C_NET_FIELD_ORDER      : swprintf_s(str, count, L"%s [0x%04X]", L"C_NET_FIELD_ORDER", id);      break;
	case C_BLUR_FRAMES          : swprintf_s(str, count, L"%s [0x%04X]", L"C_BLUR_FRAMES", id);          break;
	case C_BLUR_SAMPLES         : swprintf_s(str, count, L"%s [0x%04X]", L"C_BLUR_SAMPLES", id);         break;
	case C_BLUR_DUR             : swprintf_s(str, count, L"%s [0x%04X]", L"C_BLUR_DUR", id);             break;
	case C_HOT_METHOD           : swprintf_s(str, count, L"%s [0x%04X]", L"C_HOT_METHOD", id);           break;
	case C_HOT_CHECK            : swprintf_s(str, count, L"%s [0x%04X]", L"C_HOT_CHECK", id);            break;
	case C_PIXEL_SIZE           : swprintf_s(str, count, L"%s [0x%04X]", L"C_PIXEL_SIZE", id);           break;
	case C_DISP_GAMMA           : swprintf_s(str, count, L"%s [0x%04X]", L"C_DISP_GAMMA", id);           break;
	case C_FBUF_GAMMA           : swprintf_s(str, count, L"%s [0x%04X]", L"C_FBUF_GAMMA", id);           break;
	case C_FILE_OUT_GAMMA       : swprintf_s(str, count, L"%s [0x%04X]", L"C_FILE_OUT_GAMMA", id);       break;
	case C_FILE_IN_GAMMA        : swprintf_s(str, count, L"%s [0x%04X]", L"C_FILE_IN_GAMMA", id);        break;
	case C_GAMMA_CORRECT        : swprintf_s(str, count, L"%s [0x%04X]", L"C_GAMMA_CORRECT", id);        break;
	case C_APPLY_DISP_GAMMA     : swprintf_s(str, count, L"%s [0x%04X]", L"C_APPLY_DISP_GAMMA", id);     break;
	case C_APPLY_FBUF_GAMMA     : swprintf_s(str, count, L"%s [0x%04X]", L"C_APPLY_FBUF_GAMMA", id);     break;
	case C_APPLY_FILE_GAMMA     : swprintf_s(str, count, L"%s [0x%04X]", L"C_APPLY_FILE_GAMMA", id);     break;
	case C_FORCE_WIRE           : swprintf_s(str, count, L"%s [0x%04X]", L"C_FORCE_WIRE", id);           break;
	case C_RAY_SHADOWS          : swprintf_s(str, count, L"%s [0x%04X]", L"C_RAY_SHADOWS", id);          break;
	case C_MASTER_AMBIENT       : swprintf_s(str, count, L"%s [0x%04X]", L"C_MASTER_AMBIENT", id);       break;
	case C_SUPER_SAMPLE         : swprintf_s(str, count, L"%s [0x%04X]", L"C_SUPER_SAMPLE", id);         break;
	case C_OBJECT_MBLUR         : swprintf_s(str, count, L"%s [0x%04X]", L"C_OBJECT_MBLUR", id);         break;
	case C_MBLUR_DITHER         : swprintf_s(str, count, L"%s [0x%04X]", L"C_MBLUR_DITHER", id);         break;
	case C_DITHER_24            : swprintf_s(str, count, L"%s [0x%04X]", L"C_DITHER_24", id);            break;
	case C_SUPER_BLACK          : swprintf_s(str, count, L"%s [0x%04X]", L"C_SUPER_BLACK", id);          break;
	case C_SAFE_FRAME           : swprintf_s(str, count, L"%s [0x%04X]", L"C_SAFE_FRAME", id);           break;
	case C_VIEW_PRES_RATIO      : swprintf_s(str, count, L"%s [0x%04X]", L"C_VIEW_PRES_RATIO", id);      break;
	case C_BGND_PRES_RATIO      : swprintf_s(str, count, L"%s [0x%04X]", L"C_BGND_PRES_RATIO", id);      break;
	case C_NTH_SERIAL_NUM       : swprintf_s(str, count, L"%s [0x%04X]", L"C_NTH_SERIAL_NUM", id);       break;
	case VPDATA                 : swprintf_s(str, count, L"%s [0x%04X]", L"VPDATA", id);                 break;
	case P_QUEUE_ENTRY          : swprintf_s(str, count, L"%s [0x%04X]", L"P_QUEUE_ENTRY", id);          break;
	case P_QUEUE_IMAGE          : swprintf_s(str, count, L"%s [0x%04X]", L"P_QUEUE_IMAGE", id);          break;
	case P_QUEUE_USEIGAMMA      : swprintf_s(str, count, L"%s [0x%04X]", L"P_QUEUE_USEIGAMMA", id);      break;
	case P_QUEUE_PROC           : swprintf_s(str, count, L"%s [0x%04X]", L"P_QUEUE_PROC", id);           break;
	case P_QUEUE_SOLID          : swprintf_s(str, count, L"%s [0x%04X]", L"P_QUEUE_SOLID", id);          break;
	case P_QUEUE_GRADIENT       : swprintf_s(str, count, L"%s [0x%04X]", L"P_QUEUE_GRADIENT", id);       break;
	case P_QUEUE_KF             : swprintf_s(str, count, L"%s [0x%04X]", L"P_QUEUE_KF", id);             break;
	case P_QUEUE_MOTBLUR        : swprintf_s(str, count, L"%s [0x%04X]", L"P_QUEUE_MOTBLUR", id);        break;
	case P_QUEUE_MB_REPEAT      : swprintf_s(str, count, L"%s [0x%04X]", L"P_QUEUE_MB_REPEAT", id);      break;
	case P_QUEUE_NONE           : swprintf_s(str, count, L"%s [0x%04X]", L"P_QUEUE_NONE", id);           break;
	case P_QUEUE_RESIZE         : swprintf_s(str, count, L"%s [0x%04X]", L"P_QUEUE_RESIZE", id);         break;
	case P_QUEUE_OFFSET         : swprintf_s(str, count, L"%s [0x%04X]", L"P_QUEUE_OFFSET", id);         break;
	case P_QUEUE_ALIGN          : swprintf_s(str, count, L"%s [0x%04X]", L"P_QUEUE_ALIGN", id);          break;
	case P_CUSTOM_SIZE          : swprintf_s(str, count, L"%s [0x%04X]", L"P_CUSTOM_SIZE", id);          break;
	case P_ALPH_NONE            : swprintf_s(str, count, L"%s [0x%04X]", L"P_ALPH_NONE", id);            break;
	case P_ALPH_PSEUDO          : swprintf_s(str, count, L"%s [0x%04X]", L"P_ALPH_PSEUDO", id);          break;
	case P_ALPH_OP_PSEUDO       : swprintf_s(str, count, L"%s [0x%04X]", L"P_ALPH_OP_PSEUDO", id);       break;
	case P_ALPH_BLUR            : swprintf_s(str, count, L"%s [0x%04X]", L"P_ALPH_BLUR", id);            break;
	case P_ALPH_PCOL            : swprintf_s(str, count, L"%s [0x%04X]", L"P_ALPH_PCOL", id);            break;
	case P_ALPH_C0              : swprintf_s(str, count, L"%s [0x%04X]", L"P_ALPH_C0", id);              break;
	case P_ALPH_OP_KEY          : swprintf_s(str, count, L"%s [0x%04X]", L"P_ALPH_OP_KEY", id);          break;
	case P_ALPH_KCOL            : swprintf_s(str, count, L"%s [0x%04X]", L"P_ALPH_KCOL", id);            break;
	case P_ALPH_OP_NOCONV       : swprintf_s(str, count, L"%s [0x%04X]", L"P_ALPH_OP_NOCONV", id);       break;
	case P_ALPH_IMAGE           : swprintf_s(str, count, L"%s [0x%04X]", L"P_ALPH_IMAGE", id);           break;
	case P_ALPH_ALPHA           : swprintf_s(str, count, L"%s [0x%04X]", L"P_ALPH_ALPHA", id);           break;
	case P_ALPH_QUES            : swprintf_s(str, count, L"%s [0x%04X]", L"P_ALPH_QUES", id);            break;
	case P_ALPH_QUEIMG          : swprintf_s(str, count, L"%s [0x%04X]", L"P_ALPH_QUEIMG", id);          break;
	case P_ALPH_CUTOFF          : swprintf_s(str, count, L"%s [0x%04X]", L"P_ALPH_CUTOFF", id);          break;
	case P_ALPHANEG             : swprintf_s(str, count, L"%s [0x%04X]", L"P_ALPHANEG", id);             break;
	case P_TRAN_NONE            : swprintf_s(str, count, L"%s [0x%04X]", L"P_TRAN_NONE", id);            break;
	case P_TRAN_IMAGE           : swprintf_s(str, count, L"%s [0x%04X]", L"P_TRAN_IMAGE", id);           break;
	case P_TRAN_FRAMES          : swprintf_s(str, count, L"%s [0x%04X]", L"P_TRAN_FRAMES", id);          break;
	case P_TRAN_FADEIN          : swprintf_s(str, count, L"%s [0x%04X]", L"P_TRAN_FADEIN", id);          break;
	case P_TRAN_FADEOUT         : swprintf_s(str, count, L"%s [0x%04X]", L"P_TRAN_FADEOUT", id);         break;
	case P_TRANNEG              : swprintf_s(str, count, L"%s [0x%04X]", L"P_TRANNEG", id);              break;
	case P_RANGES               : swprintf_s(str, count, L"%s [0x%04X]", L"P_RANGES", id);               break;
	case P_PROC_DATA            : swprintf_s(str, count, L"%s [0x%04X]", L"P_PROC_DATA", id);            break;
	default: swprintf_s(str, count, L"%s[0x%04x]", L"DONT_KNOW_", id);
	}

}

// This will convert the chunk's id into a string.
void C3dsFile::GetChunkName(unsigned char* buffer, wchar_t* str, size_t count)
{
	int id;

	if (buffer == NULL) {
		wcscpy_s(str, count, L"");
		return;
	}

	id = *((unsigned short*)&buffer[0]);
	
	switch(id) {
	case NULL_CHUNK             : wcscpy_s(str, count, L"NULL_CHUNK");               break;
	case ChunkType              : wcscpy_s(str, count, L"ChunkType");                break;
	case ChunkUnique            : wcscpy_s(str, count, L"ChunkUnique");              break;
	case NotChunk               : wcscpy_s(str, count, L"NotChunk");                 break;
	case Container              : wcscpy_s(str, count, L"Container");                break;
	case IsChunk                : wcscpy_s(str, count, L"IsChunk");                  break;
	case DUMMY                  : wcscpy_s(str, count, L"DUMMY");                    break;
	case POINT_ARRAY_ENTRY      : wcscpy_s(str, count, L"POINT_ARRAY_ENTRY");        break;
	case POINT_FLAG_ARRAY_ENTRY : wcscpy_s(str, count, L"POINT_FLAG_ARRAY_ENTRY");   break;
	case FACE_ARRAY_ENTRY       : wcscpy_s(str, count, L"FACE_ARRAY_ENTRY");         break;
	case MSH_MAT_GROUP_ENTRY    : wcscpy_s(str, count, L"MSH_MAT_GROUP_ENTRY");      break;
	case TEX_VERTS_ENTRY        : wcscpy_s(str, count, L"TEX_VERTS_ENTRY");          break;
	case SMOOTH_GROUP_ENTRY     : wcscpy_s(str, count, L"SMOOTH_GROUP_ENTRY");       break;
	case POS_TRACK_TAG_KEY      : wcscpy_s(str, count, L"POS_TRACK_TAG_KEY");        break;
	case ROT_TRACK_TAG_KEY      : wcscpy_s(str, count, L"ROT_TRACK_TAG_KEY");        break;
	case SCL_TRACK_TAG_KEY      : wcscpy_s(str, count, L"SCL_TRACK_TAG_KEY");        break;
	case FOV_TRACK_TAG_KEY      : wcscpy_s(str, count, L"FOV_TRACK_TAG_KEY");        break;
	case ROLL_TRACK_TAG_KEY     : wcscpy_s(str, count, L"ROLL_TRACK_TAG_KEY");       break;
	case COL_TRACK_TAG_KEY      : wcscpy_s(str, count, L"COL_TRACK_TAG_KEY");        break;
	case MORPH_TRACK_TAG_KEY    : wcscpy_s(str, count, L"MORPH_TRACK_TAG_KEY");      break;
	case HOT_TRACK_TAG_KEY      : wcscpy_s(str, count, L"HOT_TRACK_TAG_KEY");        break;
	case FALL_TRACK_TAG_KEY     : wcscpy_s(str, count, L"FALL_TRACK_TAG_KEY");       break;
	case M3DMAGIC               : wcscpy_s(str, count, L"M3DMAGIC");                 break;
	case SMAGIC                 : wcscpy_s(str, count, L"SMAGIC");                   break;
	case LMAGIC                 : wcscpy_s(str, count, L"LMAGIC");                   break;
	case MLIBMAGIC              : wcscpy_s(str, count, L"MLIBMAGIC");                break;
	case MATMAGIC               : wcscpy_s(str, count, L"MATMAGIC");                 break;
	case M3D_VERSION            : wcscpy_s(str, count, L"M3D_VERSION");              break;
	case M3D_KFVERSION          : wcscpy_s(str, count, L"M3D_KFVERSION");            break;
	case MDATA                  : wcscpy_s(str, count, L"MDATA");                    break;
	case MESH_VERSION           : wcscpy_s(str, count, L"MESH_VERSION");             break;
	case COLOR_F                : wcscpy_s(str, count, L"COLOR_F");                  break;
	case COLOR_24               : wcscpy_s(str, count, L"COLOR_24");                 break;
	case LIN_COLOR_24           : wcscpy_s(str, count, L"LIN_COLOR_24");             break;
	case LIN_COLOR_F            : wcscpy_s(str, count, L"LIN_COLOR_F");              break;
	case INT_PERCENTAGE         : wcscpy_s(str, count, L"INT_PERCENTAGE");           break;
	case FLOAT_PERCENTAGE       : wcscpy_s(str, count, L"FLOAT_PERCENTAGE");         break;
	case MASTER_SCALE           : wcscpy_s(str, count, L"MASTER_SCALE");             break;
	case BIT_MAP                : wcscpy_s(str, count, L"BIT_MAP");                  break;
	case USE_BIT_MAP            : wcscpy_s(str, count, L"USE_BIT_MAP");              break;
	case SOLID_BGND             : wcscpy_s(str, count, L"SOLID_BGND");               break;
	case USE_SOLID_BGND         : wcscpy_s(str, count, L"USE_SOLID_BGND");           break;
	case V_GRADIENT             : wcscpy_s(str, count, L"V_GRADIENT");               break;
	case USE_V_GRADIENT         : wcscpy_s(str, count, L"USE_V_GRADIENT");           break;
	case LO_SHADOW_BIAS         : wcscpy_s(str, count, L"LO_SHADOW_BIAS");           break;
	case HI_SHADOW_BIAS         : wcscpy_s(str, count, L"HI_SHADOW_BIAS");           break;
	case SHADOW_MAP_SIZE        : wcscpy_s(str, count, L"SHADOW_MAP_SIZE");          break;
	case SHADOW_SAMPLES         : wcscpy_s(str, count, L"SHADOW_SAMPLES");           break;
	case SHADOW_RANGE           : wcscpy_s(str, count, L"SHADOW_RANGE");             break;
	case SHADOW_FILTER          : wcscpy_s(str, count, L"SHADOW_FILTER");            break;
	case RAY_BIAS               : wcscpy_s(str, count, L"RAY_BIAS");                 break;
	case O_CONSTS               : wcscpy_s(str, count, L"O_CONSTS");                 break;
	case AMBIENT_LIGHT          : wcscpy_s(str, count, L"AMBIENT_LIGHT");            break;
	case FOG                    : wcscpy_s(str, count, L"FOG");                      break;
	case USE_FOG                : wcscpy_s(str, count, L"USE_FOG");                  break;
	case FOG_BGND               : wcscpy_s(str, count, L"FOG_BGND");                 break;
	case DISTANCE_CUE           : wcscpy_s(str, count, L"DISTANCE_CUE");             break;
	case USE_DISTANCE_CUE       : wcscpy_s(str, count, L"USE_DISTANCE_CUE");         break;
	case LAYER_FOG              : wcscpy_s(str, count, L"LAYER_FOG");                break;
	case USE_LAYER_FOG          : wcscpy_s(str, count, L"USE_LAYER_FOG");            break;
	case DCUE_BGND              : wcscpy_s(str, count, L"DCUE_BGND");                break;
	case DEFAULT_VIEW           : wcscpy_s(str, count, L"DEFAULT_VIEW");             break;
	case VIEW_TOP               : wcscpy_s(str, count, L"VIEW_TOP");                 break;
	case VIEW_BOTTOM            : wcscpy_s(str, count, L"VIEW_BOTTOM");              break;
	case VIEW_LEFT              : wcscpy_s(str, count, L"VIEW_LEFT");                break;
	case VIEW_RIGHT             : wcscpy_s(str, count, L"VIEW_RIGHT");               break;
	case VIEW_FRONT             : wcscpy_s(str, count, L"VIEW_FRONT");               break;
	case VIEW_BACK              : wcscpy_s(str, count, L"VIEW_BACK");                break;
	case VIEW_USER              : wcscpy_s(str, count, L"VIEW_USER");                break;
	case VIEW_CAMERA            : wcscpy_s(str, count, L"VIEW_CAMERA");              break;
	case VIEW_WINDOW            : wcscpy_s(str, count, L"VIEW_WINDOW");              break;
	case NAMED_OBJECT           : wcscpy_s(str, count, L"NAMED_OBJECT");             break;
	case OBJ_HIDDEN             : wcscpy_s(str, count, L"OBJ_HIDDEN");               break;
	case OBJ_VIS_LOFTER         : wcscpy_s(str, count, L"OBJ_VIS_LOFTER");           break;
	case OBJ_DOESNT_CAST        : wcscpy_s(str, count, L"OBJ_DOESNT_CAST");          break;
	case OBJ_MATTE              : wcscpy_s(str, count, L"OBJ_MATTE");                break;
	case OBJ_FAST               : wcscpy_s(str, count, L"OBJ_FAST");                 break;
	case OBJ_PROCEDURAL         : wcscpy_s(str, count, L"OBJ_PROCEDURAL");           break;
	case OBJ_FROZEN             : wcscpy_s(str, count, L"OBJ_FROZEN");               break;
	case OBJ_DONT_RCVSHADOW     : wcscpy_s(str, count, L"OBJ_DONT_RCVSHADOW");       break;
	case N_TRI_OBJECT           : wcscpy_s(str, count, L"N_TRI_OBJECT");             break;
	case POINT_ARRAY            : wcscpy_s(str, count, L"POINT_ARRAY");              break;
	case POINT_FLAG_ARRAY       : wcscpy_s(str, count, L"POINT_FLAG_ARRAY");         break;
	case FACE_ARRAY             : wcscpy_s(str, count, L"FACE_ARRAY");               break;
	case MSH_MAT_GROUP          : wcscpy_s(str, count, L"MSH_MAT_GROUP");            break;
	case OLD_MAT_GROUP          : wcscpy_s(str, count, L"OLD_MAT_GROUP");            break;
	case TEX_VERTS              : wcscpy_s(str, count, L"TEX_VERTS");                break;
	case SMOOTH_GROUP           : wcscpy_s(str, count, L"SMOOTH_GROUP");             break;
	case MESH_MATRIX            : wcscpy_s(str, count, L"MESH_MATRIX");              break;
	case MESH_COLOR             : wcscpy_s(str, count, L"MESH_COLOR");               break;
	case MESH_TEXTURE_INFO      : wcscpy_s(str, count, L"MESH_TEXTURE_INFO");        break;
	case PROC_NAME              : wcscpy_s(str, count, L"PROC_NAME");                break;
	case PROC_DATA              : wcscpy_s(str, count, L"PROC_DATA");                break;
	case MSH_BOXMAP             : wcscpy_s(str, count, L"MSH_BOXMAP");               break;
	case N_D_L_OLD              : wcscpy_s(str, count, L"N_D_L_OLD");                break;
	case N_CAM_OLD              : wcscpy_s(str, count, L"N_CAM_OLD");                break;
	case N_DIRECT_LIGHT         : wcscpy_s(str, count, L"N_DIRECT_LIGHT");           break;
	case DL_SPOTLIGHT           : wcscpy_s(str, count, L"DL_SPOTLIGHT");             break;
	case DL_OFF                 : wcscpy_s(str, count, L"DL_OFF");                   break;
	case DL_ATTENUATE           : wcscpy_s(str, count, L"DL_ATTENUATE");             break;
	case DL_RAYSHAD             : wcscpy_s(str, count, L"DL_RAYSHAD");               break;
	case DL_SHADOWED            : wcscpy_s(str, count, L"DL_SHADOWED");              break;
	case DL_LOCAL_SHADOW        : wcscpy_s(str, count, L"DL_LOCAL_SHADOW");          break;
	case DL_LOCAL_SHADOW2       : wcscpy_s(str, count, L"DL_LOCAL_SHADOW2");         break;
	case DL_SEE_CONE            : wcscpy_s(str, count, L"DL_SEE_CONE");              break;
	case DL_SPOT_RECTANGULAR    : wcscpy_s(str, count, L"DL_SPOT_RECTANGULAR");      break;
	case DL_SPOT_OVERSHOOT      : wcscpy_s(str, count, L"DL_SPOT_OVERSHOOT");        break;
	case DL_SPOT_PROJECTOR      : wcscpy_s(str, count, L"DL_SPOT_PROJECTOR");        break;
	case DL_EXCLUDE             : wcscpy_s(str, count, L"DL_EXCLUDE");               break;
	case DL_RANGE               : wcscpy_s(str, count, L"DL_RANGE");                 break;
	case DL_SPOT_ROLL           : wcscpy_s(str, count, L"DL_SPOT_ROLL");             break;
	case DL_SPOT_ASPECT         : wcscpy_s(str, count, L"DL_SPOT_ASPECT");           break;
	case DL_RAY_BIAS            : wcscpy_s(str, count, L"DL_RAY_BIAS");              break;
	case DL_INNER_RANGE         : wcscpy_s(str, count, L"DL_INNER_RANGE");           break;
	case DL_OUTER_RANGE         : wcscpy_s(str, count, L"DL_OUTER_RANGE");           break;
	case DL_MULTIPLIER          : wcscpy_s(str, count, L"DL_MULTIPLIER");            break;
	case N_AMBIENT_LIGHT        : wcscpy_s(str, count, L"N_AMBIENT_LIGHT");          break;
	case N_CAMERA               : wcscpy_s(str, count, L"N_CAMERA");                 break;
	case CAM_SEE_CONE           : wcscpy_s(str, count, L"CAM_SEE_CONE");             break;
	case CAM_RANGES             : wcscpy_s(str, count, L"CAM_RANGES");               break;
	case HIERARCHY              : wcscpy_s(str, count, L"HIERARCHY");                break;
	case PARENT_OBJECT          : wcscpy_s(str, count, L"PARENT_OBJECT");            break;
	case PIVOT_OBJECT           : wcscpy_s(str, count, L"PIVOT_OBJECT");             break;
	case PIVOT_LIMITS           : wcscpy_s(str, count, L"PIVOT_LIMITS");             break;
	case PIVOT_ORDER            : wcscpy_s(str, count, L"PIVOT_ORDER");              break;
	case XLATE_RANGE            : wcscpy_s(str, count, L"XLATE_RANGE");              break;
	case POLY_2D                : wcscpy_s(str, count, L"POLY_2D");                  break;
	case SHAPE_OK               : wcscpy_s(str, count, L"SHAPE_OK");                 break;
	case SHAPE_NOT_OK           : wcscpy_s(str, count, L"SHAPE_NOT_OK");             break;
	case SHAPE_HOOK             : wcscpy_s(str, count, L"SHAPE_HOOK");               break;
	case PATH_3D                : wcscpy_s(str, count, L"PATH_3D");                  break;
	case PATH_MATRIX            : wcscpy_s(str, count, L"PATH_MATRIX");              break;
	case SHAPE_2D               : wcscpy_s(str, count, L"SHAPE_2D");                 break;
	case M_SCALE                : wcscpy_s(str, count, L"M_SCALE");                  break;
	case M_TWIST                : wcscpy_s(str, count, L"M_TWIST");                  break;
	case M_TEETER               : wcscpy_s(str, count, L"M_TEETER");                 break;
	case M_FIT                  : wcscpy_s(str, count, L"M_FIT");                    break;
	case M_BEVEL                : wcscpy_s(str, count, L"M_BEVEL");                  break;
	case XZ_CURVE               : wcscpy_s(str, count, L"XZ_CURVE");                 break;
	case YZ_CURVE               : wcscpy_s(str, count, L"YZ_CURVE");                 break;
	case INTERPCT               : wcscpy_s(str, count, L"INTERPCT");                 break;
	case DEFORM_LIMIT           : wcscpy_s(str, count, L"DEFORM_LIMIT");             break;
	case USE_CONTOUR            : wcscpy_s(str, count, L"USE_CONTOUR");              break;
	case USE_TWEEN              : wcscpy_s(str, count, L"USE_TWEEN");                break;
	case USE_SCALE              : wcscpy_s(str, count, L"USE_SCALE");                break;
	case USE_TWIST              : wcscpy_s(str, count, L"USE_TWIST");                break;
	case USE_TEETER             : wcscpy_s(str, count, L"USE_TEETER");               break;
	case USE_FIT                : wcscpy_s(str, count, L"USE_FIT");                  break;
	case USE_BEVEL              : wcscpy_s(str, count, L"USE_BEVEL");                break;
	case VIEWPORT_LAYOUT_OLD    : wcscpy_s(str, count, L"VIEWPORT_LAYOUT_OLD");      break;
	case VIEWPORT_DATA_OLD      : wcscpy_s(str, count, L"VIEWPORT_DATA_OLD");        break;
	case VIEWPORT_LAYOUT        : wcscpy_s(str, count, L"VIEWPORT_LAYOUT");          break;
	case VIEWPORT_DATA          : wcscpy_s(str, count, L"VIEWPORT_DATA");            break;
	case VIEWPORT_DATA_3        : wcscpy_s(str, count, L"VIEWPORT_DATA_3");          break;
	case VIEWPORT_SIZE          : wcscpy_s(str, count, L"VIEWPORT_SIZE");            break;
	case NETWORK_VIEW           : wcscpy_s(str, count, L"NETWORK_VIEW");             break;
	case XDATA_SECTION          : wcscpy_s(str, count, L"XDATA_SECTION");            break;
	case XDATA_ENTRY            : wcscpy_s(str, count, L"XDATA_ENTRY");              break;
	case XDATA_APPNAME          : wcscpy_s(str, count, L"XDATA_APPNAME");            break;
	case XDATA_STRING           : wcscpy_s(str, count, L"XDATA_STRING");             break;
	case XDATA_FLOAT            : wcscpy_s(str, count, L"XDATA_FLOAT");              break;
	case XDATA_DOUBLE           : wcscpy_s(str, count, L"XDATA_DOUBLE");             break;
	case XDATA_SHORT            : wcscpy_s(str, count, L"XDATA_SHORT");              break;
	case XDATA_LONG             : wcscpy_s(str, count, L"XDATA_LONG");               break;
	case XDATA_VOID             : wcscpy_s(str, count, L"XDATA_VOID");               break;
	case XDATA_GROUP            : wcscpy_s(str, count, L"XDATA_GROUP");              break;
	case XDATA_RFU6             : wcscpy_s(str, count, L"XDATA_RFU6");               break;
	case XDATA_RFU5             : wcscpy_s(str, count, L"XDATA_RFU5");               break;
	case XDATA_RFU4             : wcscpy_s(str, count, L"XDATA_RFU4");               break;
	case XDATA_RFU3             : wcscpy_s(str, count, L"XDATA_RFU3");               break;
	case XDATA_RFU2             : wcscpy_s(str, count, L"XDATA_RFU2");               break;
	case XDATA_RFU1             : wcscpy_s(str, count, L"XDATA_RFU1");               break;
	case PARENT_NAME            : wcscpy_s(str, count, L"PARENT_NAME");              break;
	case MAT_ENTRY              : wcscpy_s(str, count, L"MAT_ENTRY");                break;
	case MAT_NAME               : wcscpy_s(str, count, L"MAT_NAME");                 break;
	case MAT_AMBIENT            : wcscpy_s(str, count, L"MAT_AMBIENT");              break;
	case MAT_DIFFUSE            : wcscpy_s(str, count, L"MAT_DIFFUSE");              break;
	case MAT_SPECULAR           : wcscpy_s(str, count, L"MAT_SPECULAR");             break;
	case MAT_SHININESS          : wcscpy_s(str, count, L"MAT_SHININESS");            break;
	case MAT_SHIN2PCT           : wcscpy_s(str, count, L"MAT_SHIN2PCT");             break;
	case MAT_SHIN3PCT           : wcscpy_s(str, count, L"MAT_SHIN3PCT");             break;
	case MAT_TRANSPARENCY       : wcscpy_s(str, count, L"MAT_TRANSPARENCY");         break;
	case MAT_XPFALL             : wcscpy_s(str, count, L"MAT_XPFALL");               break;
	case MAT_REFBLUR            : wcscpy_s(str, count, L"MAT_REFBLUR");              break;
	case MAT_SELF_ILLUM         : wcscpy_s(str, count, L"MAT_SELF_ILLUM");           break;
	case MAT_TWO_SIDE           : wcscpy_s(str, count, L"MAT_TWO_SIDE");             break;
	case MAT_DECAL              : wcscpy_s(str, count, L"MAT_DECAL");                break;
	case MAT_ADDITIVE           : wcscpy_s(str, count, L"MAT_ADDITIVE");             break;
	case MAT_SELF_ILPCT         : wcscpy_s(str, count, L"MAT_SELF_ILPCT");           break;
	case MAT_WIRE               : wcscpy_s(str, count, L"MAT_WIRE");                 break;
	case MAT_SUPERSMP           : wcscpy_s(str, count, L"MAT_SUPERSMP");             break;
	case MAT_WIRESIZE           : wcscpy_s(str, count, L"MAT_WIRESIZE");             break;
	case MAT_FACEMAP            : wcscpy_s(str, count, L"MAT_FACEMAP");              break;
	case MAT_XPFALLIN           : wcscpy_s(str, count, L"MAT_XPFALLIN");             break;
	case MAT_PHONGSOFT          : wcscpy_s(str, count, L"MAT_PHONGSOFT");            break;
	case MAT_WIREABS            : wcscpy_s(str, count, L"MAT_WIREABS");              break;
	case MAT_SHADING            : wcscpy_s(str, count, L"MAT_SHADING");              break;
	case MAT_TEXMAP             : wcscpy_s(str, count, L"MAT_TEXMAP");               break;
	case MAT_OPACMAP            : wcscpy_s(str, count, L"MAT_OPACMAP");              break;
	case MAT_REFLMAP            : wcscpy_s(str, count, L"MAT_REFLMAP");              break;
	case MAT_BUMPMAP            : wcscpy_s(str, count, L"MAT_BUMPMAP");              break;
	case MAT_SPECMAP            : wcscpy_s(str, count, L"MAT_SPECMAP");              break;
	case MAT_USE_XPFALL         : wcscpy_s(str, count, L"MAT_USE_XPFALL");           break;
	case MAT_USE_REFBLUR        : wcscpy_s(str, count, L"MAT_USE_REFBLUR");          break;
	case MAT_BUMP_PERCENT       : wcscpy_s(str, count, L"MAT_BUMP_PERCENT");         break;
	case MAT_MAPNAME            : wcscpy_s(str, count, L"MAT_MAPNAME");              break;
	case MAT_ACUBIC             : wcscpy_s(str, count, L"MAT_ACUBIC");               break;
	case MAT_SXP_TEXT_DATA      : wcscpy_s(str, count, L"MAT_SXP_TEXT_DATA");        break;
	case MAT_SXP_TEXT2_DATA     : wcscpy_s(str, count, L"MAT_SXP_TEXT2_DATA");       break;
	case MAT_SXP_OPAC_DATA      : wcscpy_s(str, count, L"MAT_SXP_OPAC_DATA");        break;
	case MAT_SXP_BUMP_DATA      : wcscpy_s(str, count, L"MAT_SXP_BUMP_DATA");        break;
	case MAT_SXP_SPEC_DATA      : wcscpy_s(str, count, L"MAT_SXP_SPEC_DATA");        break;
	case MAT_SXP_SHIN_DATA      : wcscpy_s(str, count, L"MAT_SXP_SHIN_DATA");        break;
	case MAT_SXP_SELFI_DATA     : wcscpy_s(str, count, L"MAT_SXP_SELFI_DATA");       break;
	case MAT_SXP_TEXT_MASKDATA  : wcscpy_s(str, count, L"MAT_SXP_TEXT_MASKDATA");    break;
	case MAT_SXP_TEXT2_MASKDATA : wcscpy_s(str, count, L"MAT_SXP_TEXT2_MASKDATA");   break;
	case MAT_SXP_OPAC_MASKDATA  : wcscpy_s(str, count, L"MAT_SXP_OPAC_MASKDATA");    break;
	case MAT_SXP_BUMP_MASKDATA  : wcscpy_s(str, count, L"MAT_SXP_BUMP_MASKDATA");    break;
	case MAT_SXP_SPEC_MASKDATA  : wcscpy_s(str, count, L"MAT_SXP_SPEC_MASKDATA");    break;
	case MAT_SXP_SHIN_MASKDATA  : wcscpy_s(str, count, L"MAT_SXP_SHIN_MASKDATA");    break;
	case MAT_SXP_SELFI_MASKDATA : wcscpy_s(str, count, L"MAT_SXP_SELFI_MASKDATA");   break;
	case MAT_SXP_REFL_MASKDATA  : wcscpy_s(str, count, L"MAT_SXP_REFL_MASKDATA");    break;
	case MAT_TEX2MAP            : wcscpy_s(str, count, L"MAT_TEX2MAP");              break;
	case MAT_SHINMAP            : wcscpy_s(str, count, L"MAT_SHINMAP");              break;
	case MAT_SELFIMAP           : wcscpy_s(str, count, L"MAT_SELFIMAP");             break;
	case MAT_TEXMASK            : wcscpy_s(str, count, L"MAT_TEXMASK");              break;
	case MAT_TEX2MASK           : wcscpy_s(str, count, L"MAT_TEX2MASK");             break;
	case MAT_OPACMASK           : wcscpy_s(str, count, L"MAT_OPACMASK");             break;
	case MAT_BUMPMASK           : wcscpy_s(str, count, L"MAT_BUMPMASK");             break;
	case MAT_SHINMASK           : wcscpy_s(str, count, L"MAT_SHINMASK");             break;
	case MAT_SPECMASK           : wcscpy_s(str, count, L"MAT_SPECMASK");             break;
	case MAT_SELFIMASK          : wcscpy_s(str, count, L"MAT_SELFIMASK");            break;
	case MAT_REFLMASK           : wcscpy_s(str, count, L"MAT_REFLMASK");             break;
	case MAT_MAP_TILINGOLD      : wcscpy_s(str, count, L"MAT_MAP_TILINGOLD");        break;
	case MAT_MAP_TILING         : wcscpy_s(str, count, L"MAT_MAP_TILING");           break;
	case MAT_MAP_TEXBLUR_OLD    : wcscpy_s(str, count, L"MAT_MAP_TEXBLUR_OLD");      break;
	case MAT_MAP_TEXBLUR        : wcscpy_s(str, count, L"MAT_MAP_TEXBLUR");          break;
	case MAT_MAP_USCALE         : wcscpy_s(str, count, L"MAT_MAP_USCALE");           break;
	case MAT_MAP_VSCALE         : wcscpy_s(str, count, L"MAT_MAP_VSCALE");           break;
	case MAT_MAP_UOFFSET        : wcscpy_s(str, count, L"MAT_MAP_UOFFSET");          break;
	case MAT_MAP_VOFFSET        : wcscpy_s(str, count, L"MAT_MAP_VOFFSET");          break;
	case MAT_MAP_ANG            : wcscpy_s(str, count, L"MAT_MAP_ANG");              break;
	case MAT_MAP_COL1           : wcscpy_s(str, count, L"MAT_MAP_COL1");             break;
	case MAT_MAP_COL2           : wcscpy_s(str, count, L"MAT_MAP_COL2");             break;
	case MAT_MAP_RCOL           : wcscpy_s(str, count, L"MAT_MAP_RCOL");             break;
	case MAT_MAP_GCOL           : wcscpy_s(str, count, L"MAT_MAP_GCOL");             break;
	case MAT_MAP_BCOL           : wcscpy_s(str, count, L"MAT_MAP_BCOL");             break;
	case KFDATA                 : wcscpy_s(str, count, L"KFDATA");                   break;
	case KFHDR                  : wcscpy_s(str, count, L"KFHDR");                    break;
	case AMBIENT_NODE_TAG       : wcscpy_s(str, count, L"AMBIENT_NODE_TAG");         break;
	case OBJECT_NODE_TAG        : wcscpy_s(str, count, L"OBJECT_NODE_TAG");          break;
	case CAMERA_NODE_TAG        : wcscpy_s(str, count, L"CAMERA_NODE_TAG");          break;
	case TARGET_NODE_TAG        : wcscpy_s(str, count, L"TARGET_NODE_TAG");          break;
	case LIGHT_NODE_TAG         : wcscpy_s(str, count, L"LIGHT_NODE_TAG");           break;
	case L_TARGET_NODE_TAG      : wcscpy_s(str, count, L"L_TARGET_NODE_TAG");        break;
	case SPOTLIGHT_NODE_TAG     : wcscpy_s(str, count, L"SPOTLIGHT_NODE_TAG");       break;
	case KFSEG                  : wcscpy_s(str, count, L"KFSEG");                    break;
	case KFCURTIME              : wcscpy_s(str, count, L"KFCURTIME");                break;
	case NODE_HDR               : wcscpy_s(str, count, L"NODE_HDR");                 break;
	case INSTANCE_NAME          : wcscpy_s(str, count, L"INSTANCE_NAME");            break;
	case PRESCALE               : wcscpy_s(str, count, L"PRESCALE");                 break;
	case PIVOT                  : wcscpy_s(str, count, L"PIVOT");                    break;
	case BOUNDBOX               : wcscpy_s(str, count, L"BOUNDBOX");                 break;
	case MORPH_SMOOTH           : wcscpy_s(str, count, L"MORPH_SMOOTH");             break;
	case POS_TRACK_TAG          : wcscpy_s(str, count, L"POS_TRACK_TAG");            break;
	case ROT_TRACK_TAG          : wcscpy_s(str, count, L"ROT_TRACK_TAG");            break;
	case SCL_TRACK_TAG          : wcscpy_s(str, count, L"SCL_TRACK_TAG");            break;
	case FOV_TRACK_TAG          : wcscpy_s(str, count, L"FOV_TRACK_TAG");            break;
	case ROLL_TRACK_TAG         : wcscpy_s(str, count, L"ROLL_TRACK_TAG");           break;
	case COL_TRACK_TAG          : wcscpy_s(str, count, L"COL_TRACK_TAG");            break;
	case MORPH_TRACK_TAG        : wcscpy_s(str, count, L"MORPH_TRACK_TAG");          break;
	case HOT_TRACK_TAG          : wcscpy_s(str, count, L"HOT_TRACK_TAG");            break;
	case FALL_TRACK_TAG         : wcscpy_s(str, count, L"FALL_TRACK_TAG");           break;
	case HIDE_TRACK_TAG         : wcscpy_s(str, count, L"HIDE_TRACK_TAG");           break;
	case NODE_ID                : wcscpy_s(str, count, L"NODE_ID");                  break;
	case CMAGIC                 : wcscpy_s(str, count, L"CMAGIC");                   break;
	case C_MDRAWER              : wcscpy_s(str, count, L"C_MDRAWER");                break;
	case C_TDRAWER              : wcscpy_s(str, count, L"C_TDRAWER");                break;
	case C_SHPDRAWER            : wcscpy_s(str, count, L"C_SHPDRAWER");              break;
	case C_MODDRAWER            : wcscpy_s(str, count, L"C_MODDRAWER");              break;
	case C_RIPDRAWER            : wcscpy_s(str, count, L"C_RIPDRAWER");              break;
	case C_TXDRAWER             : wcscpy_s(str, count, L"C_TXDRAWER");               break;
	case C_PDRAWER              : wcscpy_s(str, count, L"C_PDRAWER");                break;
	case C_MTLDRAWER            : wcscpy_s(str, count, L"C_MTLDRAWER");              break;
	case C_FLIDRAWER            : wcscpy_s(str, count, L"C_FLIDRAWER");              break;
	case C_CUBDRAWER            : wcscpy_s(str, count, L"C_CUBDRAWER");              break;
	case C_MFILE                : wcscpy_s(str, count, L"C_MFILE");                  break;
	case C_SHPFILE              : wcscpy_s(str, count, L"C_SHPFILE");                break;
	case C_MODFILE              : wcscpy_s(str, count, L"C_MODFILE");                break;
	case C_RIPFILE              : wcscpy_s(str, count, L"C_RIPFILE");                break;
	case C_TXFILE               : wcscpy_s(str, count, L"C_TXFILE");                 break;
	case C_PFILE                : wcscpy_s(str, count, L"C_PFILE");                  break;
	case C_MTLFILE              : wcscpy_s(str, count, L"C_MTLFILE");                break;
	case C_FLIFILE              : wcscpy_s(str, count, L"C_FLIFILE");                break;
	case C_PALFILE              : wcscpy_s(str, count, L"C_PALFILE");                break;
	case C_TX_STRING            : wcscpy_s(str, count, L"C_TX_STRING");              break;
	case C_CONSTS               : wcscpy_s(str, count, L"C_CONSTS");                 break;
	case C_SNAPS                : wcscpy_s(str, count, L"C_SNAPS");                  break;
	case C_GRIDS                : wcscpy_s(str, count, L"C_GRIDS");                  break;
	case C_ASNAPS               : wcscpy_s(str, count, L"C_ASNAPS");                 break;
	case C_GRID_RANGE           : wcscpy_s(str, count, L"C_GRID_RANGE");             break;
	case C_RENDTYPE             : wcscpy_s(str, count, L"C_RENDTYPE");               break;
	case C_PROGMODE             : wcscpy_s(str, count, L"C_PROGMODE");               break;
	case C_PREVMODE             : wcscpy_s(str, count, L"C_PREVMODE");               break;
	case C_MODWMODE             : wcscpy_s(str, count, L"C_MODWMODE");               break;
	case C_MODMODEL             : wcscpy_s(str, count, L"C_MODMODEL");               break;
	case C_ALL_LINES            : wcscpy_s(str, count, L"C_ALL_LINES");              break;
	case C_BACK_TYPE            : wcscpy_s(str, count, L"C_BACK_TYPE");              break;
	case C_MD_CS                : wcscpy_s(str, count, L"C_MD_CS");                  break;
	case C_MD_CE                : wcscpy_s(str, count, L"C_MD_CE");                  break;
	case C_MD_SML               : wcscpy_s(str, count, L"C_MD_SML");                 break;
	case C_MD_SMW               : wcscpy_s(str, count, L"C_MD_SMW");                 break;
	case C_LOFT_WITH_TEXTURE    : wcscpy_s(str, count, L"C_LOFT_WITH_TEXTURE");      break;
	case C_LOFT_L_REPEAT        : wcscpy_s(str, count, L"C_LOFT_L_REPEAT");          break;
	case C_LOFT_W_REPEAT        : wcscpy_s(str, count, L"C_LOFT_W_REPEAT");          break;
	case C_LOFT_UV_NORMALIZE    : wcscpy_s(str, count, L"C_LOFT_UV_NORMALIZE");      break;
	case C_WELD_LOFT            : wcscpy_s(str, count, L"C_WELD_LOFT");              break;
	case C_MD_PDET              : wcscpy_s(str, count, L"C_MD_PDET");                break;
	case C_MD_SDET              : wcscpy_s(str, count, L"C_MD_SDET");                break;
	case C_RGB_RMODE            : wcscpy_s(str, count, L"C_RGB_RMODE");              break;
	case C_RGB_HIDE             : wcscpy_s(str, count, L"C_RGB_HIDE");               break;
	case C_RGB_MAPSW            : wcscpy_s(str, count, L"C_RGB_MAPSW");              break;
	case C_RGB_TWOSIDE          : wcscpy_s(str, count, L"C_RGB_TWOSIDE");            break;
	case C_RGB_SHADOW           : wcscpy_s(str, count, L"C_RGB_SHADOW");             break;
	case C_RGB_AA               : wcscpy_s(str, count, L"C_RGB_AA");                 break;
	case C_RGB_OVW              : wcscpy_s(str, count, L"C_RGB_OVW");                break;
	case C_RGB_OVH              : wcscpy_s(str, count, L"C_RGB_OVH");                break;
	case C_RGB_PICTYPE          : wcscpy_s(str, count, L"C_RGB_PICTYPE");            break;
	case C_RGB_OUTPUT           : wcscpy_s(str, count, L"C_RGB_OUTPUT");             break;
	case C_RGB_TODISK           : wcscpy_s(str, count, L"C_RGB_TODISK");             break;
	case C_RGB_COMPRESS         : wcscpy_s(str, count, L"C_RGB_COMPRESS");           break;
	case C_JPEG_COMPRESSION     : wcscpy_s(str, count, L"C_JPEG_COMPRESSION");       break;
	case C_RGB_DISPDEV          : wcscpy_s(str, count, L"C_RGB_DISPDEV");            break;
	case C_RGB_HARDDEV          : wcscpy_s(str, count, L"C_RGB_HARDDEV");            break;
	case C_RGB_PATH             : wcscpy_s(str, count, L"C_RGB_PATH");               break;
	case C_BITMAP_DRAWER        : wcscpy_s(str, count, L"C_BITMAP_DRAWER");          break;
	case C_RGB_FILE             : wcscpy_s(str, count, L"C_RGB_FILE");               break;
	case C_RGB_OVASPECT         : wcscpy_s(str, count, L"C_RGB_OVASPECT");           break;
	case C_RGB_ANIMTYPE         : wcscpy_s(str, count, L"C_RGB_ANIMTYPE");           break;
	case C_RENDER_ALL           : wcscpy_s(str, count, L"C_RENDER_ALL");             break;
	case C_REND_FROM            : wcscpy_s(str, count, L"C_REND_FROM");              break;
	case C_REND_TO              : wcscpy_s(str, count, L"C_REND_TO");                break;
	case C_REND_NTH             : wcscpy_s(str, count, L"C_REND_NTH");               break;
	case C_PAL_TYPE             : wcscpy_s(str, count, L"C_PAL_TYPE");               break;
	case C_RND_TURBO            : wcscpy_s(str, count, L"C_RND_TURBO");              break;
	case C_RND_MIP              : wcscpy_s(str, count, L"C_RND_MIP");                break;
	case C_BGND_METHOD          : wcscpy_s(str, count, L"C_BGND_METHOD");            break;
	case C_AUTO_REFLECT         : wcscpy_s(str, count, L"C_AUTO_REFLECT");           break;
	case C_VP_FROM              : wcscpy_s(str, count, L"C_VP_FROM");                break;
	case C_VP_TO                : wcscpy_s(str, count, L"C_VP_TO");                  break;
	case C_VP_NTH               : wcscpy_s(str, count, L"C_VP_NTH");                 break;
	case C_REND_TSTEP           : wcscpy_s(str, count, L"C_REND_TSTEP");             break;
	case C_VP_TSTEP             : wcscpy_s(str, count, L"C_VP_TSTEP");               break;
	case C_SRDIAM               : wcscpy_s(str, count, L"C_SRDIAM");                 break;
	case C_SRDEG                : wcscpy_s(str, count, L"C_SRDEG");                  break;
	case C_SRSEG                : wcscpy_s(str, count, L"C_SRSEG");                  break;
	case C_SRDIR                : wcscpy_s(str, count, L"C_SRDIR");                  break;
	case C_HETOP                : wcscpy_s(str, count, L"C_HETOP");                  break;
	case C_HEBOT                : wcscpy_s(str, count, L"C_HEBOT");                  break;
	case C_HEHT                 : wcscpy_s(str, count, L"C_HEHT");                   break;
	case C_HETURNS              : wcscpy_s(str, count, L"C_HETURNS");                break;
	case C_HEDEG                : wcscpy_s(str, count, L"C_HEDEG");                  break;
	case C_HESEG                : wcscpy_s(str, count, L"C_HESEG");                  break;
	case C_HEDIR                : wcscpy_s(str, count, L"C_HEDIR");                  break;
	case C_QUIKSTUFF            : wcscpy_s(str, count, L"C_QUIKSTUFF");              break;
	case C_SEE_LIGHTS           : wcscpy_s(str, count, L"C_SEE_LIGHTS");             break;
	case C_SEE_CAMERAS          : wcscpy_s(str, count, L"C_SEE_CAMERAS");            break;
	case C_SEE_3D               : wcscpy_s(str, count, L"C_SEE_3D");                 break;
	case C_MESHSEL              : wcscpy_s(str, count, L"C_MESHSEL");                break;
	case C_MESHUNSEL            : wcscpy_s(str, count, L"C_MESHUNSEL");              break;
	case C_POLYSEL              : wcscpy_s(str, count, L"C_POLYSEL");                break;
	case C_POLYUNSEL            : wcscpy_s(str, count, L"C_POLYUNSEL");              break;
	case C_SHPLOCAL             : wcscpy_s(str, count, L"C_SHPLOCAL");               break;
	case C_MSHLOCAL             : wcscpy_s(str, count, L"C_MSHLOCAL");               break;
	case C_NUM_FORMAT           : wcscpy_s(str, count, L"C_NUM_FORMAT");             break;
	case C_ARCH_DENOM           : wcscpy_s(str, count, L"C_ARCH_DENOM");             break;
	case C_IN_DEVICE            : wcscpy_s(str, count, L"C_IN_DEVICE");              break;
	case C_MSCALE               : wcscpy_s(str, count, L"C_MSCALE");                 break;
	case C_COMM_PORT            : wcscpy_s(str, count, L"C_COMM_PORT");              break;
	case C_TAB_BASES            : wcscpy_s(str, count, L"C_TAB_BASES");              break;
	case C_TAB_DIVS             : wcscpy_s(str, count, L"C_TAB_DIVS");               break;
	case C_MASTER_SCALES        : wcscpy_s(str, count, L"C_MASTER_SCALES");          break;
	case C_SHOW_1STVERT         : wcscpy_s(str, count, L"C_SHOW_1STVERT");           break;
	case C_SHAPER_OK            : wcscpy_s(str, count, L"C_SHAPER_OK");              break;
	case C_LOFTER_OK            : wcscpy_s(str, count, L"C_LOFTER_OK");              break;
	case C_EDITOR_OK            : wcscpy_s(str, count, L"C_EDITOR_OK");              break;
	case C_KEYFRAMER_OK         : wcscpy_s(str, count, L"C_KEYFRAMER_OK");           break;
	case C_PICKSIZE             : wcscpy_s(str, count, L"C_PICKSIZE");               break;
	case C_MAPTYPE              : wcscpy_s(str, count, L"C_MAPTYPE");                break;
	case C_MAP_DISPLAY          : wcscpy_s(str, count, L"C_MAP_DISPLAY");            break;
	case C_TILE_XY              : wcscpy_s(str, count, L"C_TILE_XY");                break;
	case C_MAP_XYZ              : wcscpy_s(str, count, L"C_MAP_XYZ");                break;
	case C_MAP_SCALE            : wcscpy_s(str, count, L"C_MAP_SCALE");              break;
	case C_MAP_MATRIX_OLD       : wcscpy_s(str, count, L"C_MAP_MATRIX_OLD");         break;
	case C_MAP_MATRIX           : wcscpy_s(str, count, L"C_MAP_MATRIX");             break;
	case C_MAP_WID_HT           : wcscpy_s(str, count, L"C_MAP_WID_HT");             break;
	case C_OBNAME               : wcscpy_s(str, count, L"C_OBNAME");                 break;
	case C_CAMNAME              : wcscpy_s(str, count, L"C_CAMNAME");                break;
	case C_LTNAME               : wcscpy_s(str, count, L"C_LTNAME");                 break;
	case C_CUR_MNAME            : wcscpy_s(str, count, L"C_CUR_MNAME");              break;
	case C_CURMTL_FROM_MESH     : wcscpy_s(str, count, L"C_CURMTL_FROM_MESH");       break;
	case C_GET_SHAPE_MAKE_FACES : wcscpy_s(str, count, L"C_GET_SHAPE_MAKE_FACES");   break;
	case C_DETAIL               : wcscpy_s(str, count, L"C_DETAIL");                 break;
	case C_VERTMARK             : wcscpy_s(str, count, L"C_VERTMARK");               break;
	case C_MSHAX                : wcscpy_s(str, count, L"C_MSHAX");                  break;
	case C_MSHCP                : wcscpy_s(str, count, L"C_MSHCP");                  break;
	case C_USERAX               : wcscpy_s(str, count, L"C_USERAX");                 break;
	case C_SHOOK                : wcscpy_s(str, count, L"C_SHOOK");                  break;
	case C_RAX                  : wcscpy_s(str, count, L"C_RAX");                    break;
	case C_STAPE                : wcscpy_s(str, count, L"C_STAPE");                  break;
	case C_LTAPE                : wcscpy_s(str, count, L"C_LTAPE");                  break;
	case C_ETAPE                : wcscpy_s(str, count, L"C_ETAPE");                  break;
	case C_KTAPE                : wcscpy_s(str, count, L"C_KTAPE");                  break;
	case C_SPHSEGS              : wcscpy_s(str, count, L"C_SPHSEGS");                break;
	case C_GEOSMOOTH            : wcscpy_s(str, count, L"C_GEOSMOOTH");              break;
	case C_HEMISEGS             : wcscpy_s(str, count, L"C_HEMISEGS");               break;
	case C_PRISMSEGS            : wcscpy_s(str, count, L"C_PRISMSEGS");              break;
	case C_PRISMSIDES           : wcscpy_s(str, count, L"C_PRISMSIDES");             break;
	case C_TUBESEGS             : wcscpy_s(str, count, L"C_TUBESEGS");               break;
	case C_TUBESIDES            : wcscpy_s(str, count, L"C_TUBESIDES");              break;
	case C_TORSEGS              : wcscpy_s(str, count, L"C_TORSEGS");                break;
	case C_TORSIDES             : wcscpy_s(str, count, L"C_TORSIDES");               break;
	case C_CONESIDES            : wcscpy_s(str, count, L"C_CONESIDES");              break;
	case C_CONESEGS             : wcscpy_s(str, count, L"C_CONESEGS");               break;
	case C_NGPARMS              : wcscpy_s(str, count, L"C_NGPARMS");                break;
	case C_PTHLEVEL             : wcscpy_s(str, count, L"C_PTHLEVEL");               break;
	case C_MSCSYM               : wcscpy_s(str, count, L"C_MSCSYM");                 break;
	case C_MFTSYM               : wcscpy_s(str, count, L"C_MFTSYM");                 break;
	case C_MTTSYM               : wcscpy_s(str, count, L"C_MTTSYM");                 break;
	case C_SMOOTHING            : wcscpy_s(str, count, L"C_SMOOTHING");              break;
	case C_MODICOUNT            : wcscpy_s(str, count, L"C_MODICOUNT");              break;
	case C_FONTSEL              : wcscpy_s(str, count, L"C_FONTSEL");                break;
	case C_TESS_TYPE            : wcscpy_s(str, count, L"C_TESS_TYPE");              break;
	case C_TESS_TENSION         : wcscpy_s(str, count, L"C_TESS_TENSION");           break;
	case C_SEG_START            : wcscpy_s(str, count, L"C_SEG_START");              break;
	case C_SEG_END              : wcscpy_s(str, count, L"C_SEG_END");                break;
	case C_CURTIME              : wcscpy_s(str, count, L"C_CURTIME");                break;
	case C_ANIMLENGTH           : wcscpy_s(str, count, L"C_ANIMLENGTH");             break;
	case C_PV_FROM              : wcscpy_s(str, count, L"C_PV_FROM");                break;
	case C_PV_TO                : wcscpy_s(str, count, L"C_PV_TO");                  break;
	case C_PV_DOFNUM            : wcscpy_s(str, count, L"C_PV_DOFNUM");              break;
	case C_PV_RNG               : wcscpy_s(str, count, L"C_PV_RNG");                 break;
	case C_PV_NTH               : wcscpy_s(str, count, L"C_PV_NTH");                 break;
	case C_PV_TYPE              : wcscpy_s(str, count, L"C_PV_TYPE");                break;
	case C_PV_METHOD            : wcscpy_s(str, count, L"C_PV_METHOD");              break;
	case C_PV_FPS               : wcscpy_s(str, count, L"C_PV_FPS");                 break;
	case C_VTR_FRAMES           : wcscpy_s(str, count, L"C_VTR_FRAMES");             break;
	case C_VTR_HDTL             : wcscpy_s(str, count, L"C_VTR_HDTL");               break;
	case C_VTR_HD               : wcscpy_s(str, count, L"C_VTR_HD");                 break;
	case C_VTR_TL               : wcscpy_s(str, count, L"C_VTR_TL");                 break;
	case C_VTR_IN               : wcscpy_s(str, count, L"C_VTR_IN");                 break;
	case C_VTR_PK               : wcscpy_s(str, count, L"C_VTR_PK");                 break;
	case C_VTR_SH               : wcscpy_s(str, count, L"C_VTR_SH");                 break;
	case C_WORK_MTLS            : wcscpy_s(str, count, L"C_WORK_MTLS");              break;
	case C_WORK_MTLS_2          : wcscpy_s(str, count, L"C_WORK_MTLS_2");            break;
	case C_WORK_MTLS_3          : wcscpy_s(str, count, L"C_WORK_MTLS_3");            break;
	case C_WORK_MTLS_4          : wcscpy_s(str, count, L"C_WORK_MTLS_4");            break;
	case C_WORK_MTLS_5          : wcscpy_s(str, count, L"C_WORK_MTLS_5");            break;
	case C_WORK_MTLS_6          : wcscpy_s(str, count, L"C_WORK_MTLS_6");            break;
	case C_WORK_MTLS_7          : wcscpy_s(str, count, L"C_WORK_MTLS_7");            break;
	case C_WORK_MTLS_8          : wcscpy_s(str, count, L"C_WORK_MTLS_8");            break;
	case C_WORKMTL              : wcscpy_s(str, count, L"C_WORKMTL");                break;
	case C_SXP_TEXT_DATA        : wcscpy_s(str, count, L"C_SXP_TEXT_DATA");          break;
	case C_SXP_TEXT2_DATA       : wcscpy_s(str, count, L"C_SXP_TEXT2_DATA");         break;
	case C_SXP_OPAC_DATA        : wcscpy_s(str, count, L"C_SXP_OPAC_DATA");          break;
	case C_SXP_BUMP_DATA        : wcscpy_s(str, count, L"C_SXP_BUMP_DATA");          break;
	case C_SXP_SPEC_DATA        : wcscpy_s(str, count, L"C_SXP_SPEC_DATA");          break;
	case C_SXP_SHIN_DATA        : wcscpy_s(str, count, L"C_SXP_SHIN_DATA");          break;
	case C_SXP_SELFI_DATA       : wcscpy_s(str, count, L"C_SXP_SELFI_DATA");         break;
	case C_SXP_TEXT_MASKDATA    : wcscpy_s(str, count, L"C_SXP_TEXT_MASKDATA");      break;
	case C_SXP_TEXT2_MASKDATA   : wcscpy_s(str, count, L"C_SXP_TEXT2_MASKDATA");     break;
	case C_SXP_OPAC_MASKDATA    : wcscpy_s(str, count, L"C_SXP_OPAC_MASKDATA");      break;
	case C_SXP_BUMP_MASKDATA    : wcscpy_s(str, count, L"C_SXP_BUMP_MASKDATA");      break;
	case C_SXP_SPEC_MASKDATA    : wcscpy_s(str, count, L"C_SXP_SPEC_MASKDATA");      break;
	case C_SXP_SHIN_MASKDATA    : wcscpy_s(str, count, L"C_SXP_SHIN_MASKDATA");      break;
	case C_SXP_SELFI_MASKDATA   : wcscpy_s(str, count, L"C_SXP_SELFI_MASKDATA");     break;
	case C_SXP_REFL_MASKDATA    : wcscpy_s(str, count, L"C_SXP_REFL_MASKDATA");      break;
	case C_BGTYPE               : wcscpy_s(str, count, L"C_BGTYPE");                 break;
	case C_MEDTILE              : wcscpy_s(str, count, L"C_MEDTILE");                break;
	case C_LO_CONTRAST          : wcscpy_s(str, count, L"C_LO_CONTRAST");            break;
	case C_HI_CONTRAST          : wcscpy_s(str, count, L"C_HI_CONTRAST");            break;
	case C_FROZ_DISPLAY         : wcscpy_s(str, count, L"C_FROZ_DISPLAY");           break;
	case C_BOOLWELD             : wcscpy_s(str, count, L"C_BOOLWELD");               break;
	case C_BOOLTYPE             : wcscpy_s(str, count, L"C_BOOLTYPE");               break;
	case C_ANG_THRESH           : wcscpy_s(str, count, L"C_ANG_THRESH");             break;
	case C_SS_THRESH            : wcscpy_s(str, count, L"C_SS_THRESH");              break;
	case C_TEXTURE_BLUR_DEFAULT : wcscpy_s(str, count, L"C_TEXTURE_BLUR_DEFAULT");   break;
	case C_MAPDRAWER            : wcscpy_s(str, count, L"C_MAPDRAWER");              break;
	case C_MAPDRAWER1           : wcscpy_s(str, count, L"C_MAPDRAWER1");             break;
	case C_MAPDRAWER2           : wcscpy_s(str, count, L"C_MAPDRAWER2");             break;
	case C_MAPDRAWER3           : wcscpy_s(str, count, L"C_MAPDRAWER3");             break;
	case C_MAPDRAWER4           : wcscpy_s(str, count, L"C_MAPDRAWER4");             break;
	case C_MAPDRAWER5           : wcscpy_s(str, count, L"C_MAPDRAWER5");             break;
	case C_MAPDRAWER6           : wcscpy_s(str, count, L"C_MAPDRAWER6");             break;
	case C_MAPDRAWER7           : wcscpy_s(str, count, L"C_MAPDRAWER7");             break;
	case C_MAPDRAWER8           : wcscpy_s(str, count, L"C_MAPDRAWER8");             break;
	case C_MAPDRAWER9           : wcscpy_s(str, count, L"C_MAPDRAWER9");             break;
	case C_MAPDRAWER_ENTRY      : wcscpy_s(str, count, L"C_MAPDRAWER_ENTRY");        break;
	case C_BACKUP_FILE          : wcscpy_s(str, count, L"C_BACKUP_FILE");            break;
	case C_DITHER_256           : wcscpy_s(str, count, L"C_DITHER_256");             break;
	case C_SAVE_LAST            : wcscpy_s(str, count, L"C_SAVE_LAST");              break;
	case C_USE_ALPHA            : wcscpy_s(str, count, L"C_USE_ALPHA");              break;
	case C_TGA_DEPTH            : wcscpy_s(str, count, L"C_TGA_DEPTH");              break;
	case C_REND_FIELDS          : wcscpy_s(str, count, L"C_REND_FIELDS");            break;
	case C_REFLIP               : wcscpy_s(str, count, L"C_REFLIP");                 break;
	case C_SEL_ITEMTOG          : wcscpy_s(str, count, L"C_SEL_ITEMTOG");            break;
	case C_SEL_RESET            : wcscpy_s(str, count, L"C_SEL_RESET");              break;
	case C_STICKY_KEYINF        : wcscpy_s(str, count, L"C_STICKY_KEYINF");          break;
	case C_WELD_THRESHOLD       : wcscpy_s(str, count, L"C_WELD_THRESHOLD");         break;
	case C_ZCLIP_POINT          : wcscpy_s(str, count, L"C_ZCLIP_POINT");            break;
	case C_ALPHA_SPLIT          : wcscpy_s(str, count, L"C_ALPHA_SPLIT");            break;
	case C_KF_SHOW_BACKFACE     : wcscpy_s(str, count, L"C_KF_SHOW_BACKFACE");       break;
	case C_OPTIMIZE_LOFT        : wcscpy_s(str, count, L"C_OPTIMIZE_LOFT");          break;
	case C_TENS_DEFAULT         : wcscpy_s(str, count, L"C_TENS_DEFAULT");           break;
	case C_CONT_DEFAULT         : wcscpy_s(str, count, L"C_CONT_DEFAULT");           break;
	case C_BIAS_DEFAULT         : wcscpy_s(str, count, L"C_BIAS_DEFAULT");           break;
	case C_DXFNAME_SRC          : wcscpy_s(str, count, L"C_DXFNAME_SRC");            break;
	case C_AUTO_WELD            : wcscpy_s(str, count, L"C_AUTO_WELD");              break;
	case C_AUTO_UNIFY           : wcscpy_s(str, count, L"C_AUTO_UNIFY");             break;
	case C_AUTO_SMOOTH          : wcscpy_s(str, count, L"C_AUTO_SMOOTH");            break;
	case C_DXF_SMOOTH_ANG       : wcscpy_s(str, count, L"C_DXF_SMOOTH_ANG");         break;
	case C_SMOOTH_ANG           : wcscpy_s(str, count, L"C_SMOOTH_ANG");             break;
	case C_NET_USE_VPOST        : wcscpy_s(str, count, L"C_NET_USE_VPOST");          break;
	case C_NET_USE_GAMMA        : wcscpy_s(str, count, L"C_NET_USE_GAMMA");          break;
	case C_NET_FIELD_ORDER      : wcscpy_s(str, count, L"C_NET_FIELD_ORDER");        break;
	case C_BLUR_FRAMES          : wcscpy_s(str, count, L"C_BLUR_FRAMES");            break;
	case C_BLUR_SAMPLES         : wcscpy_s(str, count, L"C_BLUR_SAMPLES");           break;
	case C_BLUR_DUR             : wcscpy_s(str, count, L"C_BLUR_DUR");               break;
	case C_HOT_METHOD           : wcscpy_s(str, count, L"C_HOT_METHOD");             break;
	case C_HOT_CHECK            : wcscpy_s(str, count, L"C_HOT_CHECK");              break;
	case C_PIXEL_SIZE           : wcscpy_s(str, count, L"C_PIXEL_SIZE");             break;
	case C_DISP_GAMMA           : wcscpy_s(str, count, L"C_DISP_GAMMA");             break;
	case C_FBUF_GAMMA           : wcscpy_s(str, count, L"C_FBUF_GAMMA");             break;
	case C_FILE_OUT_GAMMA       : wcscpy_s(str, count, L"C_FILE_OUT_GAMMA");         break;
	case C_FILE_IN_GAMMA        : wcscpy_s(str, count, L"C_FILE_IN_GAMMA");          break;
	case C_GAMMA_CORRECT        : wcscpy_s(str, count, L"C_GAMMA_CORRECT");          break;
	case C_APPLY_DISP_GAMMA     : wcscpy_s(str, count, L"C_APPLY_DISP_GAMMA");       break;
	case C_APPLY_FBUF_GAMMA     : wcscpy_s(str, count, L"C_APPLY_FBUF_GAMMA");       break;
	case C_APPLY_FILE_GAMMA     : wcscpy_s(str, count, L"C_APPLY_FILE_GAMMA");       break;
	case C_FORCE_WIRE           : wcscpy_s(str, count, L"C_FORCE_WIRE");             break;
	case C_RAY_SHADOWS          : wcscpy_s(str, count, L"C_RAY_SHADOWS");            break;
	case C_MASTER_AMBIENT       : wcscpy_s(str, count, L"C_MASTER_AMBIENT");         break;
	case C_SUPER_SAMPLE         : wcscpy_s(str, count, L"C_SUPER_SAMPLE");           break;
	case C_OBJECT_MBLUR         : wcscpy_s(str, count, L"C_OBJECT_MBLUR");           break;
	case C_MBLUR_DITHER         : wcscpy_s(str, count, L"C_MBLUR_DITHER");           break;
	case C_DITHER_24            : wcscpy_s(str, count, L"C_DITHER_24");              break;
	case C_SUPER_BLACK          : wcscpy_s(str, count, L"C_SUPER_BLACK");            break;
	case C_SAFE_FRAME           : wcscpy_s(str, count, L"C_SAFE_FRAME");             break;
	case C_VIEW_PRES_RATIO      : wcscpy_s(str, count, L"C_VIEW_PRES_RATIO");        break;
	case C_BGND_PRES_RATIO      : wcscpy_s(str, count, L"C_BGND_PRES_RATIO");        break;
	case C_NTH_SERIAL_NUM       : wcscpy_s(str, count, L"C_NTH_SERIAL_NUM");         break;
	case VPDATA                 : wcscpy_s(str, count, L"VPDATA");                   break;
	case P_QUEUE_ENTRY          : wcscpy_s(str, count, L"P_QUEUE_ENTRY");            break;
	case P_QUEUE_IMAGE          : wcscpy_s(str, count, L"P_QUEUE_IMAGE");            break;
	case P_QUEUE_USEIGAMMA      : wcscpy_s(str, count, L"P_QUEUE_USEIGAMMA");        break;
	case P_QUEUE_PROC           : wcscpy_s(str, count, L"P_QUEUE_PROC");             break;
	case P_QUEUE_SOLID          : wcscpy_s(str, count, L"P_QUEUE_SOLID");            break;
	case P_QUEUE_GRADIENT       : wcscpy_s(str, count, L"P_QUEUE_GRADIENT");         break;
	case P_QUEUE_KF             : wcscpy_s(str, count, L"P_QUEUE_KF");               break;
	case P_QUEUE_MOTBLUR        : wcscpy_s(str, count, L"P_QUEUE_MOTBLUR");          break;
	case P_QUEUE_MB_REPEAT      : wcscpy_s(str, count, L"P_QUEUE_MB_REPEAT");        break;
	case P_QUEUE_NONE           : wcscpy_s(str, count, L"P_QUEUE_NONE");             break;
	case P_QUEUE_RESIZE         : wcscpy_s(str, count, L"P_QUEUE_RESIZE");           break;
	case P_QUEUE_OFFSET         : wcscpy_s(str, count, L"P_QUEUE_OFFSET");           break;
	case P_QUEUE_ALIGN          : wcscpy_s(str, count, L"P_QUEUE_ALIGN");            break;
	case P_CUSTOM_SIZE          : wcscpy_s(str, count, L"P_CUSTOM_SIZE");            break;
	case P_ALPH_NONE            : wcscpy_s(str, count, L"P_ALPH_NONE");              break;
	case P_ALPH_PSEUDO          : wcscpy_s(str, count, L"P_ALPH_PSEUDO");            break;
	case P_ALPH_OP_PSEUDO       : wcscpy_s(str, count, L"P_ALPH_OP_PSEUDO");         break;
	case P_ALPH_BLUR            : wcscpy_s(str, count, L"P_ALPH_BLUR");              break;
	case P_ALPH_PCOL            : wcscpy_s(str, count, L"P_ALPH_PCOL");              break;
	case P_ALPH_C0              : wcscpy_s(str, count, L"P_ALPH_C0");                break;
	case P_ALPH_OP_KEY          : wcscpy_s(str, count, L"P_ALPH_OP_KEY");            break;
	case P_ALPH_KCOL            : wcscpy_s(str, count, L"P_ALPH_KCOL");              break;
	case P_ALPH_OP_NOCONV       : wcscpy_s(str, count, L"P_ALPH_OP_NOCONV");         break;
	case P_ALPH_IMAGE           : wcscpy_s(str, count, L"P_ALPH_IMAGE");             break;
	case P_ALPH_ALPHA           : wcscpy_s(str, count, L"P_ALPH_ALPHA");             break;
	case P_ALPH_QUES            : wcscpy_s(str, count, L"P_ALPH_QUES");              break;
	case P_ALPH_QUEIMG          : wcscpy_s(str, count, L"P_ALPH_QUEIMG");            break;
	case P_ALPH_CUTOFF          : wcscpy_s(str, count, L"P_ALPH_CUTOFF");            break;
	case P_ALPHANEG             : wcscpy_s(str, count, L"P_ALPHANEG");               break;
	case P_TRAN_NONE            : wcscpy_s(str, count, L"P_TRAN_NONE");              break;
	case P_TRAN_IMAGE           : wcscpy_s(str, count, L"P_TRAN_IMAGE");             break;
	case P_TRAN_FRAMES          : wcscpy_s(str, count, L"P_TRAN_FRAMES");            break;
	case P_TRAN_FADEIN          : wcscpy_s(str, count, L"P_TRAN_FADEIN");            break;
	case P_TRAN_FADEOUT         : wcscpy_s(str, count, L"P_TRAN_FADEOUT");           break;
	case P_TRANNEG              : wcscpy_s(str, count, L"P_TRANNEG");                break;
	case P_RANGES               : wcscpy_s(str, count, L"P_RANGES");                 break;
	case P_PROC_DATA            : wcscpy_s(str, count, L"P_PROC_DATA");              break;
	}
}

// This will read the chunk header from the memmory.
// Remember we store the whole file into a memory.
unsigned char* C3dsFile::ReadMainChunk(unsigned short* id, unsigned int* size)
{
	int i;

	i = 0;
	*id = *((unsigned short*)&data[i]);     i += sizeof(unsigned short);
	*size = *((unsigned int*)&data[i]);

	return data;
}

// This will tell if a chunk contains a sub-chunk or it is an actual data.
// Also this will prevent the tree view control from expanding if it's the actual data.
int C3dsFile::GetSubChunkCount(unsigned short id)
{
	int count;
	
	switch(id) {
	case NULL_CHUNK             : count = 1; break;
	case ChunkType              : count = 1; break;
	case ChunkUnique            : count = 1; break;
	case NotChunk               : count = 1; break;
	case Container              : count = 1; break;
	case IsChunk                : count = 1; break;
	case DUMMY                  : count = 1; break;
	case POINT_ARRAY_ENTRY      : count = 1; break;
	case POINT_FLAG_ARRAY_ENTRY : count = 1; break;
	case FACE_ARRAY_ENTRY       : count = 1; break;
	case MSH_MAT_GROUP_ENTRY    : count = 1; break;
	case TEX_VERTS_ENTRY        : count = 1; break;
	case SMOOTH_GROUP_ENTRY     : count = 1; break;
	case POS_TRACK_TAG_KEY      : count = 1; break;
	case ROT_TRACK_TAG_KEY      : count = 1; break;
	case SCL_TRACK_TAG_KEY      : count = 1; break;
	case FOV_TRACK_TAG_KEY      : count = 1; break;
	case ROLL_TRACK_TAG_KEY     : count = 1; break;
	case COL_TRACK_TAG_KEY      : count = 1; break;
	case MORPH_TRACK_TAG_KEY    : count = 1; break;
	case HOT_TRACK_TAG_KEY      : count = 1; break;
	case FALL_TRACK_TAG_KEY     : count = 1; break;
	case M3DMAGIC               : count = 1; break;
	case SMAGIC                 : count = 1; break;
	case LMAGIC                 : count = 1; break;
	case MLIBMAGIC              : count = 1; break;
	case MATMAGIC               : count = 1; break;
	case M3D_VERSION            : count = 0; break;
	case M3D_KFVERSION          : count = 1; break;
	case MDATA                  : count = 1; break;
	case MESH_VERSION           : count = 0; break;
	case COLOR_F                : count = 1; break;
	case COLOR_24               : count = 0; break;
	case LIN_COLOR_24           : count = 0; break;
	case LIN_COLOR_F            : count = 1; break;
	case INT_PERCENTAGE         : count = 0; break;
	case FLOAT_PERCENTAGE       : count = 1; break;
	case MASTER_SCALE           : count = 0; break;
	case BIT_MAP                : count = 1; break;
	case USE_BIT_MAP            : count = 1; break;
	case SOLID_BGND             : count = 1; break;
	case USE_SOLID_BGND         : count = 1; break;
	case V_GRADIENT             : count = 1; break;
	case USE_V_GRADIENT         : count = 1; break;
	case LO_SHADOW_BIAS         : count = 1; break;
	case HI_SHADOW_BIAS         : count = 1; break;
	case SHADOW_MAP_SIZE        : count = 1; break;
	case SHADOW_SAMPLES         : count = 1; break;
	case SHADOW_RANGE           : count = 1; break;
	case SHADOW_FILTER          : count = 1; break;
	case RAY_BIAS               : count = 1; break;
	case O_CONSTS               : count = 1; break;
	case AMBIENT_LIGHT          : count = 1; break;
	case FOG                    : count = 1; break;
	case USE_FOG                : count = 1; break;
	case FOG_BGND               : count = 1; break;
	case DISTANCE_CUE           : count = 1; break;
	case USE_DISTANCE_CUE       : count = 1; break;
	case LAYER_FOG              : count = 1; break;
	case USE_LAYER_FOG          : count = 1; break;
	case DCUE_BGND              : count = 1; break;
	case DEFAULT_VIEW           : count = 1; break;
	case VIEW_TOP               : count = 1; break;
	case VIEW_BOTTOM            : count = 1; break;
	case VIEW_LEFT              : count = 1; break;
	case VIEW_RIGHT             : count = 1; break;
	case VIEW_FRONT             : count = 1; break;
	case VIEW_BACK              : count = 1; break;
	case VIEW_USER              : count = 1; break;
	case VIEW_CAMERA            : count = 1; break;
	case VIEW_WINDOW            : count = 1; break;
	case NAMED_OBJECT           : count = 1; break;
	case OBJ_HIDDEN             : count = 1; break;
	case OBJ_VIS_LOFTER         : count = 1; break;
	case OBJ_DOESNT_CAST        : count = 1; break;
	case OBJ_MATTE              : count = 1; break;
	case OBJ_FAST               : count = 1; break;
	case OBJ_PROCEDURAL         : count = 1; break;
	case OBJ_FROZEN             : count = 1; break;
	case OBJ_DONT_RCVSHADOW     : count = 1; break;
	case N_TRI_OBJECT           : count = 1; break;
	case POINT_ARRAY            : count = 0; break;
	case POINT_FLAG_ARRAY       : count = 0; break;
	case FACE_ARRAY             : count = 0; break;
	case MSH_MAT_GROUP          : count = 1; break;
	case OLD_MAT_GROUP          : count = 1; break;
	case TEX_VERTS              : count = 0; break;
	case SMOOTH_GROUP           : count = 1; break;
	case MESH_MATRIX            : count = 0; break;
	case MESH_COLOR             : count = 1; break;
	case MESH_TEXTURE_INFO      : count = 1; break;
	case PROC_NAME              : count = 1; break;
	case PROC_DATA              : count = 1; break;
	case MSH_BOXMAP             : count = 1; break;
	case N_D_L_OLD              : count = 1; break;
	case N_CAM_OLD              : count = 1; break;
	case N_DIRECT_LIGHT         : count = 1; break;
	case DL_SPOTLIGHT           : count = 1; break;
	case DL_OFF                 : count = 1; break;
	case DL_ATTENUATE           : count = 1; break;
	case DL_RAYSHAD             : count = 1; break;
	case DL_SHADOWED            : count = 1; break;
	case DL_LOCAL_SHADOW        : count = 1; break;
	case DL_LOCAL_SHADOW2       : count = 1; break;
	case DL_SEE_CONE            : count = 1; break;
	case DL_SPOT_RECTANGULAR    : count = 1; break;
	case DL_SPOT_OVERSHOOT      : count = 1; break;
	case DL_SPOT_PROJECTOR      : count = 1; break;
	case DL_EXCLUDE             : count = 1; break;
	case DL_RANGE               : count = 1; break;
	case DL_SPOT_ROLL           : count = 1; break;
	case DL_SPOT_ASPECT         : count = 1; break;
	case DL_RAY_BIAS            : count = 1; break;
	case DL_INNER_RANGE         : count = 1; break;
	case DL_OUTER_RANGE         : count = 1; break;
	case DL_MULTIPLIER          : count = 1; break;
	case N_AMBIENT_LIGHT        : count = 1; break;
	case N_CAMERA               : count = 1; break;
	case CAM_SEE_CONE           : count = 1; break;
	case CAM_RANGES             : count = 1; break;
	case HIERARCHY              : count = 1; break;
	case PARENT_OBJECT          : count = 1; break;
	case PIVOT_OBJECT           : count = 1; break;
	case PIVOT_LIMITS           : count = 1; break;
	case PIVOT_ORDER            : count = 1; break;
	case XLATE_RANGE            : count = 1; break;
	case POLY_2D                : count = 1; break;
	case SHAPE_OK               : count = 1; break;
	case SHAPE_NOT_OK           : count = 1; break;
	case SHAPE_HOOK             : count = 1; break;
	case PATH_3D                : count = 1; break;
	case PATH_MATRIX            : count = 1; break;
	case SHAPE_2D               : count = 1; break;
	case M_SCALE                : count = 1; break;
	case M_TWIST                : count = 1; break;
	case M_TEETER               : count = 1; break;
	case M_FIT                  : count = 1; break;
	case M_BEVEL                : count = 1; break;
	case XZ_CURVE               : count = 1; break;
	case YZ_CURVE               : count = 1; break;
	case INTERPCT               : count = 1; break;
	case DEFORM_LIMIT           : count = 1; break;
	case USE_CONTOUR            : count = 1; break;
	case USE_TWEEN              : count = 1; break;
	case USE_SCALE              : count = 1; break;
	case USE_TWIST              : count = 1; break;
	case USE_TEETER             : count = 1; break;
	case USE_FIT                : count = 1; break;
	case USE_BEVEL              : count = 1; break;
	case VIEWPORT_LAYOUT_OLD    : count = 1; break;
	case VIEWPORT_DATA_OLD      : count = 1; break;
	case VIEWPORT_LAYOUT        : count = 1; break;
	case VIEWPORT_DATA          : count = 1; break;
	case VIEWPORT_DATA_3        : count = 1; break;
	case VIEWPORT_SIZE          : count = 1; break;
	case NETWORK_VIEW           : count = 1; break;
	case XDATA_SECTION          : count = 1; break;
	case XDATA_ENTRY            : count = 1; break;
	case XDATA_APPNAME          : count = 1; break;
	case XDATA_STRING           : count = 1; break;
	case XDATA_FLOAT            : count = 1; break;
	case XDATA_DOUBLE           : count = 1; break;
	case XDATA_SHORT            : count = 1; break;
	case XDATA_LONG             : count = 1; break;
	case XDATA_VOID             : count = 1; break;
	case XDATA_GROUP            : count = 1; break;
	case XDATA_RFU6             : count = 1; break;
	case XDATA_RFU5             : count = 1; break;
	case XDATA_RFU4             : count = 1; break;
	case XDATA_RFU3             : count = 1; break;
	case XDATA_RFU2             : count = 1; break;
	case XDATA_RFU1             : count = 1; break;
	case PARENT_NAME            : count = 1; break;
	case MAT_ENTRY              : count = 1; break;
	case MAT_NAME               : count = 1; break;
	case MAT_AMBIENT            : count = 1; break;
	case MAT_DIFFUSE            : count = 1; break;
	case MAT_SPECULAR           : count = 1; break;
	case MAT_SHININESS          : count = 1; break;
	case MAT_SHIN2PCT           : count = 1; break;
	case MAT_SHIN3PCT           : count = 1; break;
	case MAT_TRANSPARENCY       : count = 1; break;
	case MAT_XPFALL             : count = 1; break;
	case MAT_REFBLUR            : count = 1; break;
	case MAT_SELF_ILLUM         : count = 1; break;
	case MAT_TWO_SIDE           : count = 1; break;
	case MAT_DECAL              : count = 1; break;
	case MAT_ADDITIVE           : count = 1; break;
	case MAT_SELF_ILPCT         : count = 1; break;
	case MAT_WIRE               : count = 1; break;
	case MAT_SUPERSMP           : count = 1; break;
	case MAT_WIRESIZE           : count = 1; break;
	case MAT_FACEMAP            : count = 1; break;
	case MAT_XPFALLIN           : count = 1; break;
	case MAT_PHONGSOFT          : count = 1; break;
	case MAT_WIREABS            : count = 1; break;
	case MAT_SHADING            : count = 0; break;
	case MAT_TEXMAP             : count = 1; break;
	case MAT_OPACMAP            : count = 1; break;
	case MAT_REFLMAP            : count = 1; break;
	case MAT_BUMPMAP            : count = 1; break;
	case MAT_SPECMAP            : count = 1; break;
	case MAT_USE_XPFALL         : count = 1; break;
	case MAT_USE_REFBLUR        : count = 1; break;
	case MAT_BUMP_PERCENT       : count = 1; break;
	case MAT_MAPNAME            : count = 1; break;
	case MAT_ACUBIC             : count = 1; break;
	case MAT_SXP_TEXT_DATA      : count = 1; break;
	case MAT_SXP_TEXT2_DATA     : count = 1; break;
	case MAT_SXP_OPAC_DATA      : count = 1; break;
	case MAT_SXP_BUMP_DATA      : count = 1; break;
	case MAT_SXP_SPEC_DATA      : count = 1; break;
	case MAT_SXP_SHIN_DATA      : count = 1; break;
	case MAT_SXP_SELFI_DATA     : count = 1; break;
	case MAT_SXP_TEXT_MASKDATA  : count = 1; break;
	case MAT_SXP_TEXT2_MASKDATA : count = 1; break;
	case MAT_SXP_OPAC_MASKDATA  : count = 1; break;
	case MAT_SXP_BUMP_MASKDATA  : count = 1; break;
	case MAT_SXP_SPEC_MASKDATA  : count = 1; break;
	case MAT_SXP_SHIN_MASKDATA  : count = 1; break;
	case MAT_SXP_SELFI_MASKDATA : count = 1; break;
	case MAT_SXP_REFL_MASKDATA  : count = 1; break;
	case MAT_TEX2MAP            : count = 1; break;
	case MAT_SHINMAP            : count = 1; break;
	case MAT_SELFIMAP           : count = 1; break;
	case MAT_TEXMASK            : count = 1; break;
	case MAT_TEX2MASK           : count = 1; break;
	case MAT_OPACMASK           : count = 1; break;
	case MAT_BUMPMASK           : count = 1; break;
	case MAT_SHINMASK           : count = 1; break;
	case MAT_SPECMASK           : count = 1; break;
	case MAT_SELFIMASK          : count = 1; break;
	case MAT_REFLMASK           : count = 1; break;
	case MAT_MAP_TILINGOLD      : count = 1; break;
	case MAT_MAP_TILING         : count = 0; break;
	case MAT_MAP_TEXBLUR_OLD    : count = 1; break;
	case MAT_MAP_TEXBLUR        : count = 1; break;
	case MAT_MAP_USCALE         : count = 0; break;
	case MAT_MAP_VSCALE         : count = 0; break;
	case MAT_MAP_UOFFSET        : count = 1; break;
	case MAT_MAP_VOFFSET        : count = 1; break;
	case MAT_MAP_ANG            : count = 1; break;
	case MAT_MAP_COL1           : count = 1; break;
	case MAT_MAP_COL2           : count = 1; break;
	case MAT_MAP_RCOL           : count = 1; break;
	case MAT_MAP_GCOL           : count = 1; break;
	case MAT_MAP_BCOL           : count = 1; break;
	case KFDATA                 : count = 1; break;
	case KFHDR                  : count = 1; break;
	case AMBIENT_NODE_TAG       : count = 1; break;
	case OBJECT_NODE_TAG        : count = 1; break;
	case CAMERA_NODE_TAG        : count = 1; break;
	case TARGET_NODE_TAG        : count = 1; break;
	case LIGHT_NODE_TAG         : count = 1; break;
	case L_TARGET_NODE_TAG      : count = 1; break;
	case SPOTLIGHT_NODE_TAG     : count = 1; break;
	case KFSEG                  : count = 1; break;
	case KFCURTIME              : count = 1; break;
	case NODE_HDR               : count = 1; break;
	case INSTANCE_NAME          : count = 1; break;
	case PRESCALE               : count = 1; break;
	case PIVOT                  : count = 1; break;
	case BOUNDBOX               : count = 1; break;
	case MORPH_SMOOTH           : count = 1; break;
	case POS_TRACK_TAG          : count = 1; break;
	case ROT_TRACK_TAG          : count = 1; break;
	case SCL_TRACK_TAG          : count = 1; break;
	case FOV_TRACK_TAG          : count = 1; break;
	case ROLL_TRACK_TAG         : count = 1; break;
	case COL_TRACK_TAG          : count = 1; break;
	case MORPH_TRACK_TAG        : count = 1; break;
	case HOT_TRACK_TAG          : count = 1; break;
	case FALL_TRACK_TAG         : count = 1; break;
	case HIDE_TRACK_TAG         : count = 1; break;
	case NODE_ID                : count = 1; break;
	case CMAGIC                 : count = 1; break;
	case C_MDRAWER              : count = 1; break;
	case C_TDRAWER              : count = 1; break;
	case C_SHPDRAWER            : count = 1; break;
	case C_MODDRAWER            : count = 1; break;
	case C_RIPDRAWER            : count = 1; break;
	case C_TXDRAWER             : count = 1; break;
	case C_PDRAWER              : count = 1; break;
	case C_MTLDRAWER            : count = 1; break;
	case C_FLIDRAWER            : count = 1; break;
	case C_CUBDRAWER            : count = 1; break;
	case C_MFILE                : count = 1; break;
	case C_SHPFILE              : count = 1; break;
	case C_MODFILE              : count = 1; break;
	case C_RIPFILE              : count = 1; break;
	case C_TXFILE               : count = 1; break;
	case C_PFILE                : count = 1; break;
	case C_MTLFILE              : count = 1; break;
	case C_FLIFILE              : count = 1; break;
	case C_PALFILE              : count = 1; break;
	case C_TX_STRING            : count = 1; break;
	case C_CONSTS               : count = 1; break;
	case C_SNAPS                : count = 1; break;
	case C_GRIDS                : count = 1; break;
	case C_ASNAPS               : count = 1; break;
	case C_GRID_RANGE           : count = 1; break;
	case C_RENDTYPE             : count = 1; break;
	case C_PROGMODE             : count = 1; break;
	case C_PREVMODE             : count = 1; break;
	case C_MODWMODE             : count = 1; break;
	case C_MODMODEL             : count = 1; break;
	case C_ALL_LINES            : count = 1; break;
	case C_BACK_TYPE            : count = 1; break;
	case C_MD_CS                : count = 1; break;
	case C_MD_CE                : count = 1; break;
	case C_MD_SML               : count = 1; break;
	case C_MD_SMW               : count = 1; break;
	case C_LOFT_WITH_TEXTURE    : count = 1; break;
	case C_LOFT_L_REPEAT        : count = 1; break;
	case C_LOFT_W_REPEAT        : count = 1; break;
	case C_LOFT_UV_NORMALIZE    : count = 1; break;
	case C_WELD_LOFT            : count = 1; break;
	case C_MD_PDET              : count = 1; break;
	case C_MD_SDET              : count = 1; break;
	case C_RGB_RMODE            : count = 1; break;
	case C_RGB_HIDE             : count = 1; break;
	case C_RGB_MAPSW            : count = 1; break;
	case C_RGB_TWOSIDE          : count = 1; break;
	case C_RGB_SHADOW           : count = 1; break;
	case C_RGB_AA               : count = 1; break;
	case C_RGB_OVW              : count = 1; break;
	case C_RGB_OVH              : count = 1; break;
	case C_RGB_PICTYPE          : count = 1; break;
	case C_RGB_OUTPUT           : count = 1; break;
	case C_RGB_TODISK           : count = 1; break;
	case C_RGB_COMPRESS         : count = 1; break;
	case C_JPEG_COMPRESSION     : count = 1; break;
	case C_RGB_DISPDEV          : count = 1; break;
	case C_RGB_HARDDEV          : count = 1; break;
	case C_RGB_PATH             : count = 1; break;
	case C_BITMAP_DRAWER        : count = 1; break;
	case C_RGB_FILE             : count = 1; break;
	case C_RGB_OVASPECT         : count = 1; break;
	case C_RGB_ANIMTYPE         : count = 1; break;
	case C_RENDER_ALL           : count = 1; break;
	case C_REND_FROM            : count = 1; break;
	case C_REND_TO              : count = 1; break;
	case C_REND_NTH             : count = 1; break;
	case C_PAL_TYPE             : count = 1; break;
	case C_RND_TURBO            : count = 1; break;
	case C_RND_MIP              : count = 1; break;
	case C_BGND_METHOD          : count = 1; break;
	case C_AUTO_REFLECT         : count = 1; break;
	case C_VP_FROM              : count = 1; break;
	case C_VP_TO                : count = 1; break;
	case C_VP_NTH               : count = 1; break;
	case C_REND_TSTEP           : count = 1; break;
	case C_VP_TSTEP             : count = 1; break;
	case C_SRDIAM               : count = 1; break;
	case C_SRDEG                : count = 1; break;
	case C_SRSEG                : count = 1; break;
	case C_SRDIR                : count = 1; break;
	case C_HETOP                : count = 1; break;
	case C_HEBOT                : count = 1; break;
	case C_HEHT                 : count = 1; break;
	case C_HETURNS              : count = 1; break;
	case C_HEDEG                : count = 1; break;
	case C_HESEG                : count = 1; break;
	case C_HEDIR                : count = 1; break;
	case C_QUIKSTUFF            : count = 1; break;
	case C_SEE_LIGHTS           : count = 1; break;
	case C_SEE_CAMERAS          : count = 1; break;
	case C_SEE_3D               : count = 1; break;
	case C_MESHSEL              : count = 1; break;
	case C_MESHUNSEL            : count = 1; break;
	case C_POLYSEL              : count = 1; break;
	case C_POLYUNSEL            : count = 1; break;
	case C_SHPLOCAL             : count = 1; break;
	case C_MSHLOCAL             : count = 1; break;
	case C_NUM_FORMAT           : count = 1; break;
	case C_ARCH_DENOM           : count = 1; break;
	case C_IN_DEVICE            : count = 1; break;
	case C_MSCALE               : count = 1; break;
	case C_COMM_PORT            : count = 1; break;
	case C_TAB_BASES            : count = 1; break;
	case C_TAB_DIVS             : count = 1; break;
	case C_MASTER_SCALES        : count = 1; break;
	case C_SHOW_1STVERT         : count = 1; break;
	case C_SHAPER_OK            : count = 1; break;
	case C_LOFTER_OK            : count = 1; break;
	case C_EDITOR_OK            : count = 1; break;
	case C_KEYFRAMER_OK         : count = 1; break;
	case C_PICKSIZE             : count = 1; break;
	case C_MAPTYPE              : count = 1; break;
	case C_MAP_DISPLAY          : count = 1; break;
	case C_TILE_XY              : count = 1; break;
	case C_MAP_XYZ              : count = 1; break;
	case C_MAP_SCALE            : count = 1; break;
	case C_MAP_MATRIX_OLD       : count = 1; break;
	case C_MAP_MATRIX           : count = 1; break;
	case C_MAP_WID_HT           : count = 1; break;
	case C_OBNAME               : count = 1; break;
	case C_CAMNAME              : count = 1; break;
	case C_LTNAME               : count = 1; break;
	case C_CUR_MNAME            : count = 1; break;
	case C_CURMTL_FROM_MESH     : count = 1; break;
	case C_GET_SHAPE_MAKE_FACES : count = 1; break;
	case C_DETAIL               : count = 1; break;
	case C_VERTMARK             : count = 1; break;
	case C_MSHAX                : count = 1; break;
	case C_MSHCP                : count = 1; break;
	case C_USERAX               : count = 1; break;
	case C_SHOOK                : count = 1; break;
	case C_RAX                  : count = 1; break;
	case C_STAPE                : count = 1; break;
	case C_LTAPE                : count = 1; break;
	case C_ETAPE                : count = 1; break;
	case C_KTAPE                : count = 1; break;
	case C_SPHSEGS              : count = 1; break;
	case C_GEOSMOOTH            : count = 1; break;
	case C_HEMISEGS             : count = 1; break;
	case C_PRISMSEGS            : count = 1; break;
	case C_PRISMSIDES           : count = 1; break;
	case C_TUBESEGS             : count = 1; break;
	case C_TUBESIDES            : count = 1; break;
	case C_TORSEGS              : count = 1; break;
	case C_TORSIDES             : count = 1; break;
	case C_CONESIDES            : count = 1; break;
	case C_CONESEGS             : count = 1; break;
	case C_NGPARMS              : count = 1; break;
	case C_PTHLEVEL             : count = 1; break;
	case C_MSCSYM               : count = 1; break;
	case C_MFTSYM               : count = 1; break;
	case C_MTTSYM               : count = 1; break;
	case C_SMOOTHING            : count = 1; break;
	case C_MODICOUNT            : count = 1; break;
	case C_FONTSEL              : count = 1; break;
	case C_TESS_TYPE            : count = 1; break;
	case C_TESS_TENSION         : count = 1; break;
	case C_SEG_START            : count = 1; break;
	case C_SEG_END              : count = 1; break;
	case C_CURTIME              : count = 1; break;
	case C_ANIMLENGTH           : count = 1; break;
	case C_PV_FROM              : count = 1; break;
	case C_PV_TO                : count = 1; break;
	case C_PV_DOFNUM            : count = 1; break;
	case C_PV_RNG               : count = 1; break;
	case C_PV_NTH               : count = 1; break;
	case C_PV_TYPE              : count = 1; break;
	case C_PV_METHOD            : count = 1; break;
	case C_PV_FPS               : count = 1; break;
	case C_VTR_FRAMES           : count = 1; break;
	case C_VTR_HDTL             : count = 1; break;
	case C_VTR_HD               : count = 1; break;
	case C_VTR_TL               : count = 1; break;
	case C_VTR_IN               : count = 1; break;
	case C_VTR_PK               : count = 1; break;
	case C_VTR_SH               : count = 1; break;
	case C_WORK_MTLS            : count = 1; break;
	case C_WORK_MTLS_2          : count = 1; break;
	case C_WORK_MTLS_3          : count = 1; break;
	case C_WORK_MTLS_4          : count = 1; break;
	case C_WORK_MTLS_5          : count = 1; break;
	case C_WORK_MTLS_6          : count = 1; break;
	case C_WORK_MTLS_7          : count = 1; break;
	case C_WORK_MTLS_8          : count = 1; break;
	case C_WORKMTL              : count = 1; break;
	case C_SXP_TEXT_DATA        : count = 1; break;
	case C_SXP_TEXT2_DATA       : count = 1; break;
	case C_SXP_OPAC_DATA        : count = 1; break;
	case C_SXP_BUMP_DATA        : count = 1; break;
	case C_SXP_SPEC_DATA        : count = 1; break;
	case C_SXP_SHIN_DATA        : count = 1; break;
	case C_SXP_SELFI_DATA       : count = 1; break;
	case C_SXP_TEXT_MASKDATA    : count = 1; break;
	case C_SXP_TEXT2_MASKDATA   : count = 1; break;
	case C_SXP_OPAC_MASKDATA    : count = 1; break;
	case C_SXP_BUMP_MASKDATA    : count = 1; break;
	case C_SXP_SPEC_MASKDATA    : count = 1; break;
	case C_SXP_SHIN_MASKDATA    : count = 1; break;
	case C_SXP_SELFI_MASKDATA   : count = 1; break;
	case C_SXP_REFL_MASKDATA    : count = 1; break;
	case C_BGTYPE               : count = 1; break;
	case C_MEDTILE              : count = 1; break;
	case C_LO_CONTRAST          : count = 1; break;
	case C_HI_CONTRAST          : count = 1; break;
	case C_FROZ_DISPLAY         : count = 1; break;
	case C_BOOLWELD             : count = 1; break;
	case C_BOOLTYPE             : count = 1; break;
	case C_ANG_THRESH           : count = 1; break;
	case C_SS_THRESH            : count = 1; break;
	case C_TEXTURE_BLUR_DEFAULT : count = 1; break;
	case C_MAPDRAWER            : count = 1; break;
	case C_MAPDRAWER1           : count = 1; break;
	case C_MAPDRAWER2           : count = 1; break;
	case C_MAPDRAWER3           : count = 1; break;
	case C_MAPDRAWER4           : count = 1; break;
	case C_MAPDRAWER5           : count = 1; break;
	case C_MAPDRAWER6           : count = 1; break;
	case C_MAPDRAWER7           : count = 1; break;
	case C_MAPDRAWER8           : count = 1; break;
	case C_MAPDRAWER9           : count = 1; break;
	case C_MAPDRAWER_ENTRY      : count = 1; break;
	case C_BACKUP_FILE          : count = 1; break;
	case C_DITHER_256           : count = 1; break;
	case C_SAVE_LAST            : count = 1; break;
	case C_USE_ALPHA            : count = 1; break;
	case C_TGA_DEPTH            : count = 1; break;
	case C_REND_FIELDS          : count = 1; break;
	case C_REFLIP               : count = 1; break;
	case C_SEL_ITEMTOG          : count = 1; break;
	case C_SEL_RESET            : count = 1; break;
	case C_STICKY_KEYINF        : count = 1; break;
	case C_WELD_THRESHOLD       : count = 1; break;
	case C_ZCLIP_POINT          : count = 1; break;
	case C_ALPHA_SPLIT          : count = 1; break;
	case C_KF_SHOW_BACKFACE     : count = 1; break;
	case C_OPTIMIZE_LOFT        : count = 1; break;
	case C_TENS_DEFAULT         : count = 1; break;
	case C_CONT_DEFAULT         : count = 1; break;
	case C_BIAS_DEFAULT         : count = 1; break;
	case C_DXFNAME_SRC          : count = 1; break;
	case C_AUTO_WELD            : count = 1; break;
	case C_AUTO_UNIFY           : count = 1; break;
	case C_AUTO_SMOOTH          : count = 1; break;
	case C_DXF_SMOOTH_ANG       : count = 1; break;
	case C_SMOOTH_ANG           : count = 1; break;
	case C_NET_USE_VPOST        : count = 1; break;
	case C_NET_USE_GAMMA        : count = 1; break;
	case C_NET_FIELD_ORDER      : count = 1; break;
	case C_BLUR_FRAMES          : count = 1; break;
	case C_BLUR_SAMPLES         : count = 1; break;
	case C_BLUR_DUR             : count = 1; break;
	case C_HOT_METHOD           : count = 1; break;
	case C_HOT_CHECK            : count = 1; break;
	case C_PIXEL_SIZE           : count = 1; break;
	case C_DISP_GAMMA           : count = 1; break;
	case C_FBUF_GAMMA           : count = 1; break;
	case C_FILE_OUT_GAMMA       : count = 1; break;
	case C_FILE_IN_GAMMA        : count = 1; break;
	case C_GAMMA_CORRECT        : count = 1; break;
	case C_APPLY_DISP_GAMMA     : count = 1; break;
	case C_APPLY_FBUF_GAMMA     : count = 1; break;
	case C_APPLY_FILE_GAMMA     : count = 1; break;
	case C_FORCE_WIRE           : count = 1; break;
	case C_RAY_SHADOWS          : count = 1; break;
	case C_MASTER_AMBIENT       : count = 1; break;
	case C_SUPER_SAMPLE         : count = 1; break;
	case C_OBJECT_MBLUR         : count = 1; break;
	case C_MBLUR_DITHER         : count = 1; break;
	case C_DITHER_24            : count = 1; break;
	case C_SUPER_BLACK          : count = 1; break;
	case C_SAFE_FRAME           : count = 1; break;
	case C_VIEW_PRES_RATIO      : count = 1; break;
	case C_BGND_PRES_RATIO      : count = 1; break;
	case C_NTH_SERIAL_NUM       : count = 1; break;
	case VPDATA                 : count = 1; break;
	case P_QUEUE_ENTRY          : count = 1; break;
	case P_QUEUE_IMAGE          : count = 1; break;
	case P_QUEUE_USEIGAMMA      : count = 1; break;
	case P_QUEUE_PROC           : count = 1; break;
	case P_QUEUE_SOLID          : count = 1; break;
	case P_QUEUE_GRADIENT       : count = 1; break;
	case P_QUEUE_KF             : count = 1; break;
	case P_QUEUE_MOTBLUR        : count = 1; break;
	case P_QUEUE_MB_REPEAT      : count = 1; break;
	case P_QUEUE_NONE           : count = 1; break;
	case P_QUEUE_RESIZE         : count = 1; break;
	case P_QUEUE_OFFSET         : count = 1; break;
	case P_QUEUE_ALIGN          : count = 1; break;
	case P_CUSTOM_SIZE          : count = 1; break;
	case P_ALPH_NONE            : count = 1; break;
	case P_ALPH_PSEUDO          : count = 1; break;
	case P_ALPH_OP_PSEUDO       : count = 1; break;
	case P_ALPH_BLUR            : count = 1; break;
	case P_ALPH_PCOL            : count = 1; break;
	case P_ALPH_C0              : count = 1; break;
	case P_ALPH_OP_KEY          : count = 1; break;
	case P_ALPH_KCOL            : count = 1; break;
	case P_ALPH_OP_NOCONV       : count = 1; break;
	case P_ALPH_IMAGE           : count = 1; break;
	case P_ALPH_ALPHA           : count = 1; break;
	case P_ALPH_QUES            : count = 1; break;
	case P_ALPH_QUEIMG          : count = 1; break;
	case P_ALPH_CUTOFF          : count = 1; break;
	case P_ALPHANEG             : count = 1; break;
	case P_TRAN_NONE            : count = 1; break;
	case P_TRAN_IMAGE           : count = 1; break;
	case P_TRAN_FRAMES          : count = 1; break;
	case P_TRAN_FADEIN          : count = 1; break;
	case P_TRAN_FADEOUT         : count = 1; break;
	case P_TRANNEG              : count = 1; break;
	case P_RANGES               : count = 1; break;
	case P_PROC_DATA            : count = 1; break;
	default: count = 1;
	}

	return count;
}

// This is where we gonna read the actual data. I only do the chunk id 0x4110 which are the vertices
// and chunk id 0x4120 which are the indices because I don't know them all.
// I find this file 3dsinfo.txt for the description of some of the chunks.
void C3dsFile::ReadChunkData(unsigned char* buffer, unsigned int* size, int* column, CQueue* que)
{
	unsigned short id, value_s;
	unsigned long long i;
	unsigned short j, k, count;
	float value_f;
	unsigned short* array1;
	unsigned char* array2;
	float* array3;
	wchar_t str[100];

	i = 0;
	id = *((unsigned short*)&buffer[i]);     i += sizeof(unsigned short);
	*size = *((unsigned int*)&buffer[i]);    i += sizeof(unsigned int);

	switch (id) {
	case NULL_CHUNK:
		break;

	case ChunkType:
		break;

	case ChunkUnique:
		break;

	case NotChunk:
		break;

	case Container:
		break;

	case IsChunk:
		break;

	case DUMMY:
		break;

	case POINT_ARRAY_ENTRY:
		break;

	case POINT_FLAG_ARRAY_ENTRY:
		break;

	case FACE_ARRAY_ENTRY:
		break;

	case MSH_MAT_GROUP_ENTRY:
		break;

	case TEX_VERTS_ENTRY:
		break;

	case SMOOTH_GROUP_ENTRY:
		break;

	case POS_TRACK_TAG_KEY:
		break;

	case ROT_TRACK_TAG_KEY:
		break;

	case SCL_TRACK_TAG_KEY:
		break;

	case FOV_TRACK_TAG_KEY:
		break;

	case ROLL_TRACK_TAG_KEY:
		break;

	case COL_TRACK_TAG_KEY:
		break;

	case MORPH_TRACK_TAG_KEY:
		break;

	case HOT_TRACK_TAG_KEY:
		break;

	case FALL_TRACK_TAG_KEY:
		break;

	case M3DMAGIC:
		break;

	case SMAGIC:
		break;

	case LMAGIC:
		break;

	case MLIBMAGIC:
		break;

	case MATMAGIC:
		break;

	case M3D_VERSION:
	case MESH_VERSION:

		// header
		*column = 2;

		wcscpy_s(str, 100, L"Attribute");
		que->Add(str);

		wcscpy_s(str, 100, L"Value");
		que->Add(str);

		// row data
		wcscpy_s(str, 100, L"Version");
		que->Add(str);

		value_s = *((unsigned short*)&buffer[i]);
		swprintf_s(str, 100, L"%d", value_s);
		que->Add(str);

		break;

	case M3D_KFVERSION:
		break;

	case MDATA:
		break;

	case COLOR_F:
		break;

	case COLOR_24:
	case LIN_COLOR_24:

		// header
		*column = 2;

		wcscpy_s(str, 100, L"Attribute");
		que->Add(str);

		wcscpy_s(str, 100, L"Value");
		que->Add(str);

		// row data
		array2 = (unsigned char*)&buffer[i];

		wcscpy_s(str, 100, L"Red");
		que->Add(str);

		swprintf_s(str, 100, L"%d", array2[0]);
		que->Add(str);

		wcscpy_s(str, 100, L"Green");
		que->Add(str);

		swprintf_s(str, 100, L"%d", array2[1]);
		que->Add(str);

		wcscpy_s(str, 100, L"Blue");
		que->Add(str);

		swprintf_s(str, 100, L"%d", array2[2]);
		que->Add(str);

		break;

	case LIN_COLOR_F:
		break;

	case INT_PERCENTAGE:

		// header
		*column = 2;

		wcscpy_s(str, 100, L"Attribute");
		que->Add(str);

		wcscpy_s(str, 100, L"Value");
		que->Add(str);

		// row data
		wcscpy_s(str, 100, L"Percent");
		que->Add(str);

		value_s = *((unsigned short*)&buffer[i]);
		swprintf_s(str, 100, L"%d", value_s);
		que->Add(str);

		break;

	case FLOAT_PERCENTAGE:
		break;

	case MASTER_SCALE:

		// header
		*column = 2;

		wcscpy_s(str, 100, L"Attribute");
		que->Add(str);

		wcscpy_s(str, 100, L"Value");
		que->Add(str);

		// row data
		wcscpy_s(str, 100, L"Scale");
		que->Add(str);

		value_f = *((float*)&buffer[i]);
		swprintf_s(str, 100, L"%f", value_f);
		que->Add(str);

		break;

	case BIT_MAP:
		break;

	case USE_BIT_MAP:
		break;

	case SOLID_BGND:
		break;

	case USE_SOLID_BGND:
		break;

	case V_GRADIENT:
		break;

	case USE_V_GRADIENT:
		break;

	case LO_SHADOW_BIAS:
		break;

	case HI_SHADOW_BIAS:
		break;

	case SHADOW_MAP_SIZE:
		break;

	case SHADOW_SAMPLES:
		break;

	case SHADOW_RANGE:
		break;

	case SHADOW_FILTER:
		break;

	case RAY_BIAS:
		break;

	case O_CONSTS:
		break;

	case AMBIENT_LIGHT:
		break;

	case FOG:
		break;

	case USE_FOG:
		break;

	case FOG_BGND:
		break;

	case DISTANCE_CUE:
		break;

	case USE_DISTANCE_CUE:
		break;

	case LAYER_FOG:
		break;

	case USE_LAYER_FOG:
		break;

	case DCUE_BGND:
		break;

	case DEFAULT_VIEW:
		break;

	case VIEW_TOP:
		break;

	case VIEW_BOTTOM:
		break;

	case VIEW_LEFT:
		break;

	case VIEW_RIGHT:
		break;

	case VIEW_FRONT:
		break;

	case VIEW_BACK:
		break;

	case VIEW_USER:
		break;

	case VIEW_CAMERA:
		break;

	case VIEW_WINDOW:
		break;

	case NAMED_OBJECT:
		break;

	case OBJ_HIDDEN:
		break;

	case OBJ_VIS_LOFTER:
		break;

	case OBJ_DOESNT_CAST:
		break;

	case OBJ_MATTE:
		break;

	case OBJ_FAST:
		break;

	case OBJ_PROCEDURAL:
		break;

	case OBJ_FROZEN:
		break;

	case OBJ_DONT_RCVSHADOW:
		break;

	case N_TRI_OBJECT:
		break;

	case POINT_ARRAY:

		// header
		*column = 3;

		wcscpy_s(str, 100, L"X");
		que->Add(str);

		wcscpy_s(str, 100, L"Y");
		que->Add(str);

		wcscpy_s(str, 100, L"Z");
		que->Add(str);

		// row data
		count = *((unsigned short*)&buffer[i]); i += sizeof(unsigned short);
		array3 = (float*)&buffer[i];

		k = 0;

		for (j = 0; j < count; j++) {

			swprintf_s(str, 100, L"%f", array3[k++]);
			que->Add(str);

			swprintf_s(str, 100, L"%f", array3[k++]);
			que->Add(str);

			swprintf_s(str, 100, L"%f", array3[k++]);
			que->Add(str);
		}

		break;

	case POINT_FLAG_ARRAY:

		// header
		*column = 1;

		wcscpy_s(str, 100, L"Flag");
		que->Add(str);

		// row data
		array1 = (unsigned short*)&buffer[i];
		count = (unsigned short)((*size - 6) / sizeof(unsigned short));

		for (j = 0; j < count; j++) {
			_itow_s(array1[j], str, 2);
			que->Add(str);
		}

		break;

	case FACE_ARRAY:

		// header
		*column = 4;

		wcscpy_s(str, 100, L"A");
		que->Add(str);

		wcscpy_s(str, 100, L"B");
		que->Add(str);

		wcscpy_s(str, 100, L"C");
		que->Add(str);

		wcscpy_s(str, 100, L"Flag");
		que->Add(str);

		// row data
		count = *((unsigned short*)&buffer[i]); i += sizeof(unsigned short);
		array1 = (unsigned short*)&buffer[i];

		k = 0;

		for (j = 0; j < count; j++) {

			swprintf_s(str, 100, L"%d", array1[k++]);
			que->Add(str);

			swprintf_s(str, 100, L"%d", array1[k++]);
			que->Add(str);

			swprintf_s(str, 100, L"%d", array1[k++]);
			que->Add(str);

			_itow_s(array1[k++], str, 2);
			que->Add(str);
		}

		break;

	case MSH_MAT_GROUP:
		break;

	case OLD_MAT_GROUP:
		break;

	case TEX_VERTS:

		// header
		*column = 2;

		wcscpy_s(str, 100, L"U");
		que->Add(str);

		wcscpy_s(str, 100, L"V");
		que->Add(str);

		// row data
		count = *((unsigned short*)&buffer[i]); i += sizeof(unsigned short);
		array3 = (float*)&buffer[i];

		k = 0;

		for (j = 0; j < count; j++) {

			swprintf_s(str, 100, L"%f", array3[k++]);
			que->Add(str);

			swprintf_s(str, 100, L"%f", array3[k++]);
			que->Add(str);

		}

		break;

	case SMOOTH_GROUP:
		break;

	case MESH_MATRIX:

		// header
		*column = 4;

		wcscpy_s(str, 100, L"Attribute");
		que->Add(str);

		wcscpy_s(str, 100, L"X");
		que->Add(str);

		wcscpy_s(str, 100, L"Y");
		que->Add(str);

		wcscpy_s(str, 100, L"Z");
		que->Add(str);

		// row data
		array3 = (float*)&buffer[i];

		k = 0;

		// X1
		wcscpy_s(str, 100, L"Axis 1");
		que->Add(str);

		swprintf_s(str, 100, L"%f", array3[k++]);
		que->Add(str);

		swprintf_s(str, 100, L"%f", array3[k++]);
		que->Add(str);

		swprintf_s(str, 100, L"%f", array3[k++]);
		que->Add(str);

		// X2
		wcscpy_s(str, 100, L"Axis 2");
		que->Add(str);

		swprintf_s(str, 100, L"%f", array3[k++]);
		que->Add(str);

		swprintf_s(str, 100, L"%f", array3[k++]);
		que->Add(str);

		swprintf_s(str, 100, L"%f", array3[k++]);
		que->Add(str);

		// X3
		wcscpy_s(str, 100, L"Axis 3");
		que->Add(str);

		swprintf_s(str, 100, L"%f", array3[k++]);
		que->Add(str);

		swprintf_s(str, 100, L"%f", array3[k++]);
		que->Add(str);

		swprintf_s(str, 100, L"%f", array3[k++]);
		que->Add(str);

		// O
		wcscpy_s(str, 100, L"Origin");
		que->Add(str);

		swprintf_s(str, 100, L"%f", array3[k++]);
		que->Add(str);

		swprintf_s(str, 100, L"%f", array3[k++]);
		que->Add(str);

		swprintf_s(str, 100, L"%f", array3[k++]);
		que->Add(str);

		break;

	case MESH_COLOR:
		break;

	case MESH_TEXTURE_INFO:
		break;

	case PROC_NAME:
		break;

	case PROC_DATA:
		break;

	case MSH_BOXMAP:
		break;

	case N_D_L_OLD:
		break;

	case N_CAM_OLD:
		break;

	case N_DIRECT_LIGHT:
		break;

	case DL_SPOTLIGHT:
		break;

	case DL_OFF:
		break;

	case DL_ATTENUATE:
		break;

	case DL_RAYSHAD:
		break;

	case DL_SHADOWED:
		break;

	case DL_LOCAL_SHADOW:
		break;

	case DL_LOCAL_SHADOW2:
		break;

	case DL_SEE_CONE:
		break;

	case DL_SPOT_RECTANGULAR:
		break;

	case DL_SPOT_OVERSHOOT:
		break;

	case DL_SPOT_PROJECTOR:
		break;

	case DL_EXCLUDE:
		break;

	case DL_RANGE:
		break;

	case DL_SPOT_ROLL:
		break;

	case DL_SPOT_ASPECT:
		break;

	case DL_RAY_BIAS:
		break;

	case DL_INNER_RANGE:
		break;

	case DL_OUTER_RANGE:
		break;

	case DL_MULTIPLIER:
		break;

	case N_AMBIENT_LIGHT:
		break;

	case N_CAMERA:
		break;

	case CAM_SEE_CONE:
		break;

	case CAM_RANGES:
		break;

	case HIERARCHY:
		break;

	case PARENT_OBJECT:
		break;

	case PIVOT_OBJECT:
		break;

	case PIVOT_LIMITS:
		break;

	case PIVOT_ORDER:
		break;

	case XLATE_RANGE:
		break;

	case POLY_2D:
		break;

	case SHAPE_OK:
		break;

	case SHAPE_NOT_OK:
		break;

	case SHAPE_HOOK:
		break;

	case PATH_3D:
		break;

	case PATH_MATRIX:
		break;

	case SHAPE_2D:
		break;

	case M_SCALE:
		break;

	case M_TWIST:
		break;

	case M_TEETER:
		break;

	case M_FIT:
		break;

	case M_BEVEL:
		break;

	case XZ_CURVE:
		break;

	case YZ_CURVE:
		break;

	case INTERPCT:
		break;

	case DEFORM_LIMIT:
		break;

	case USE_CONTOUR:
		break;

	case USE_TWEEN:
		break;

	case USE_SCALE:
		break;

	case USE_TWIST:
		break;

	case USE_TEETER:
		break;

	case USE_FIT:
		break;

	case USE_BEVEL:
		break;

	case VIEWPORT_LAYOUT_OLD:
		break;

	case VIEWPORT_DATA_OLD:
		break;

	case VIEWPORT_LAYOUT:
		break;

	case VIEWPORT_DATA:
		break;

	case VIEWPORT_DATA_3:
		break;

	case VIEWPORT_SIZE:
		break;

	case NETWORK_VIEW:
		break;

	case XDATA_SECTION:
		break;

	case XDATA_ENTRY:
		break;

	case XDATA_APPNAME:
		break;

	case XDATA_STRING:
		break;

	case XDATA_FLOAT:
		break;

	case XDATA_DOUBLE:
		break;

	case XDATA_SHORT:
		break;

	case XDATA_LONG:
		break;

	case XDATA_VOID:
		break;

	case XDATA_GROUP:
		break;

	case XDATA_RFU6:
		break;

	case XDATA_RFU5:
		break;

	case XDATA_RFU4:
		break;

	case XDATA_RFU3:
		break;

	case XDATA_RFU2:
		break;

	case XDATA_RFU1:
		break;

	case PARENT_NAME:
		break;

	case MAT_ENTRY:
		break;

	case MAT_NAME:
		break;

	case MAT_AMBIENT:
		break;

	case MAT_DIFFUSE:
		break;

	case MAT_SPECULAR:
		break;

	case MAT_SHININESS:
		break;

	case MAT_SHIN2PCT:
		break;

	case MAT_SHIN3PCT:
		break;

	case MAT_TRANSPARENCY:
		break;

	case MAT_XPFALL:
		break;

	case MAT_REFBLUR:
		break;

	case MAT_SELF_ILLUM:
		break;

	case MAT_TWO_SIDE:
		break;

	case MAT_DECAL:
		break;

	case MAT_ADDITIVE:
		break;

	case MAT_SELF_ILPCT:
		break;

	case MAT_WIRE:
		break;

	case MAT_SUPERSMP:
		break;

	case MAT_WIRESIZE:
		break;

	case MAT_FACEMAP:
		break;

	case MAT_XPFALLIN:
		break;

	case MAT_PHONGSOFT:
		break;

	case MAT_WIREABS:
		break;

	case MAT_SHADING:

		// header
		*column = 2;

		wcscpy_s(str, 100, L"Attribute");
		que->Add(str);

		wcscpy_s(str, 100, L"Value");
		que->Add(str);

		// row data
		wcscpy_s(str, 100, L"Shading");
		que->Add(str);

		value_s = *((unsigned short*)&buffer[i]);
		swprintf_s(str, 100, L"%d", value_s);
		que->Add(str);

		break;

	case MAT_TEXMAP:
		break;

	case MAT_OPACMAP:
		break;

	case MAT_REFLMAP:
		break;

	case MAT_BUMPMAP:
		break;

	case MAT_SPECMAP:
		break;

	case MAT_USE_XPFALL:
		break;

	case MAT_USE_REFBLUR:
		break;

	case MAT_BUMP_PERCENT:
		break;

	case MAT_MAPNAME:
		break;

	case MAT_ACUBIC:
		break;

	case MAT_SXP_TEXT_DATA:
		break;

	case MAT_SXP_TEXT2_DATA:
		break;

	case MAT_SXP_OPAC_DATA:
		break;

	case MAT_SXP_BUMP_DATA:
		break;

	case MAT_SXP_SPEC_DATA:
		break;

	case MAT_SXP_SHIN_DATA:
		break;

	case MAT_SXP_SELFI_DATA:
		break;

	case MAT_SXP_TEXT_MASKDATA:
		break;

	case MAT_SXP_TEXT2_MASKDATA:
		break;

	case MAT_SXP_OPAC_MASKDATA:
		break;

	case MAT_SXP_BUMP_MASKDATA:
		break;

	case MAT_SXP_SPEC_MASKDATA:
		break;

	case MAT_SXP_SHIN_MASKDATA:
		break;

	case MAT_SXP_SELFI_MASKDATA:
		break;

	case MAT_SXP_REFL_MASKDATA:
		break;

	case MAT_TEX2MAP:
		break;

	case MAT_SHINMAP:
		break;

	case MAT_SELFIMAP:
		break;

	case MAT_TEXMASK:
		break;

	case MAT_TEX2MASK:
		break;

	case MAT_OPACMASK:
		break;

	case MAT_BUMPMASK:
		break;

	case MAT_SHINMASK:
		break;

	case MAT_SPECMASK:
		break;

	case MAT_SELFIMASK:
		break;

	case MAT_REFLMASK:
		break;

	case MAT_MAP_TILINGOLD:
		break;

	case MAT_MAP_TILING:

		// header
		*column = 2;

		wcscpy_s(str, 100, L"Attribute");
		que->Add(str);

		wcscpy_s(str, 100, L"Value");
		que->Add(str);

		// row data
		wcscpy_s(str, 100, L"Map Tiling");
		que->Add(str);

		value_s = *((unsigned short*)&buffer[i]);
		swprintf_s(str, 100, L"%d", value_s);
		que->Add(str);

		break;

	case MAT_MAP_TEXBLUR_OLD:
		break;

	case MAT_MAP_TEXBLUR:
		break;

	case MAT_MAP_USCALE:

		// header
		*column = 2;

		wcscpy_s(str, 100, L"Attribute");
		que->Add(str);

		wcscpy_s(str, 100, L"Value");
		que->Add(str);

		// row data
		wcscpy_s(str, 100, L"U Scale");
		que->Add(str);

		value_f = *((float*)&buffer[i]);
		swprintf_s(str, 100, L"%f", value_f);
		que->Add(str);

		break;

	case MAT_MAP_VSCALE:

		// header
		*column = 2;

		wcscpy_s(str, 100, L"Attribute");
		que->Add(str);

		wcscpy_s(str, 100, L"Value");
		que->Add(str);

		// row data
		wcscpy_s(str, 100, L"V Scale");
		que->Add(str);

		value_f = *((float*)&buffer[i]);
		swprintf_s(str, 100, L"%f", value_f);
		que->Add(str);

		break;

	case MAT_MAP_UOFFSET:
		break;

	case MAT_MAP_VOFFSET:
		break;

	case MAT_MAP_ANG:
		break;

	case MAT_MAP_COL1:
		break;

	case MAT_MAP_COL2:
		break;

	case MAT_MAP_RCOL:
		break;

	case MAT_MAP_GCOL:
		break;

	case MAT_MAP_BCOL:
		break;

	case KFDATA:
		break;

	case KFHDR:
		break;

	case AMBIENT_NODE_TAG:
		break;

	case OBJECT_NODE_TAG:
		break;

	case CAMERA_NODE_TAG:
		break;

	case TARGET_NODE_TAG:
		break;

	case LIGHT_NODE_TAG:
		break;

	case L_TARGET_NODE_TAG:
		break;

	case SPOTLIGHT_NODE_TAG:
		break;

	case KFSEG:
		break;

	case KFCURTIME:
		break;

	case NODE_HDR:
		break;

	case INSTANCE_NAME:
		break;

	case PRESCALE:
		break;

	case PIVOT:
		break;

	case BOUNDBOX:
		break;

	case MORPH_SMOOTH:
		break;

	case POS_TRACK_TAG:
		break;

	case ROT_TRACK_TAG:
		break;

	case SCL_TRACK_TAG:
		break;

	case FOV_TRACK_TAG:
		break;

	case ROLL_TRACK_TAG:
		break;

	case COL_TRACK_TAG:
		break;

	case MORPH_TRACK_TAG:
		break;

	case HOT_TRACK_TAG:
		break;

	case FALL_TRACK_TAG:
		break;

	case HIDE_TRACK_TAG:
		break;

	case NODE_ID:
		break;

	case CMAGIC:
		break;

	case C_MDRAWER:
		break;

	case C_TDRAWER:
		break;

	case C_SHPDRAWER:
		break;

	case C_MODDRAWER:
		break;

	case C_RIPDRAWER:
		break;

	case C_TXDRAWER:
		break;

	case C_PDRAWER:
		break;

	case C_MTLDRAWER:
		break;

	case C_FLIDRAWER:
		break;

	case C_CUBDRAWER:
		break;

	case C_MFILE:
		break;

	case C_SHPFILE:
		break;

	case C_MODFILE:
		break;

	case C_RIPFILE:
		break;

	case C_TXFILE:
		break;

	case C_PFILE:
		break;

	case C_MTLFILE:
		break;

	case C_FLIFILE:
		break;

	case C_PALFILE:
		break;

	case C_TX_STRING:
		break;

	case C_CONSTS:
		break;

	case C_SNAPS:
		break;

	case C_GRIDS:
		break;

	case C_ASNAPS:
		break;

	case C_GRID_RANGE:
		break;

	case C_RENDTYPE:
		break;

	case C_PROGMODE:
		break;

	case C_PREVMODE:
		break;

	case C_MODWMODE:
		break;

	case C_MODMODEL:
		break;

	case C_ALL_LINES:
		break;

	case C_BACK_TYPE:
		break;

	case C_MD_CS:
		break;

	case C_MD_CE:
		break;

	case C_MD_SML:
		break;

	case C_MD_SMW:
		break;

	case C_LOFT_WITH_TEXTURE:
		break;

	case C_LOFT_L_REPEAT:
		break;

	case C_LOFT_W_REPEAT:
		break;

	case C_LOFT_UV_NORMALIZE:
		break;

	case C_WELD_LOFT:
		break;

	case C_MD_PDET:
		break;

	case C_MD_SDET:
		break;

	case C_RGB_RMODE:
		break;

	case C_RGB_HIDE:
		break;

	case C_RGB_MAPSW:
		break;

	case C_RGB_TWOSIDE:
		break;

	case C_RGB_SHADOW:
		break;

	case C_RGB_AA:
		break;

	case C_RGB_OVW:
		break;

	case C_RGB_OVH:
		break;

	case C_RGB_PICTYPE:
		break;

	case C_RGB_OUTPUT:
		break;

	case C_RGB_TODISK:
		break;

	case C_RGB_COMPRESS:
		break;

	case C_JPEG_COMPRESSION:
		break;

	case C_RGB_DISPDEV:
		break;

	case C_RGB_HARDDEV:
		break;

	case C_RGB_PATH:
		break;

	case C_BITMAP_DRAWER:
		break;

	case C_RGB_FILE:
		break;

	case C_RGB_OVASPECT:
		break;

	case C_RGB_ANIMTYPE:
		break;

	case C_RENDER_ALL:
		break;

	case C_REND_FROM:
		break;

	case C_REND_TO:
		break;

	case C_REND_NTH:
		break;

	case C_PAL_TYPE:
		break;

	case C_RND_TURBO:
		break;

	case C_RND_MIP:
		break;

	case C_BGND_METHOD:
		break;

	case C_AUTO_REFLECT:
		break;

	case C_VP_FROM:
		break;

	case C_VP_TO:
		break;

	case C_VP_NTH:
		break;

	case C_REND_TSTEP:
		break;

	case C_VP_TSTEP:
		break;

	case C_SRDIAM:
		break;

	case C_SRDEG:
		break;

	case C_SRSEG:
		break;

	case C_SRDIR:
		break;

	case C_HETOP:
		break;

	case C_HEBOT:
		break;

	case C_HEHT:
		break;

	case C_HETURNS:
		break;

	case C_HEDEG:
		break;

	case C_HESEG:
		break;

	case C_HEDIR:
		break;

	case C_QUIKSTUFF:
		break;

	case C_SEE_LIGHTS:
		break;

	case C_SEE_CAMERAS:
		break;

	case C_SEE_3D:
		break;

	case C_MESHSEL:
		break;

	case C_MESHUNSEL:
		break;

	case C_POLYSEL:
		break;

	case C_POLYUNSEL:
		break;

	case C_SHPLOCAL:
		break;

	case C_MSHLOCAL:
		break;

	case C_NUM_FORMAT:
		break;

	case C_ARCH_DENOM:
		break;

	case C_IN_DEVICE:
		break;

	case C_MSCALE:
		break;

	case C_COMM_PORT:
		break;

	case C_TAB_BASES:
		break;

	case C_TAB_DIVS:
		break;

	case C_MASTER_SCALES:
		break;

	case C_SHOW_1STVERT:
		break;

	case C_SHAPER_OK:
		break;

	case C_LOFTER_OK:
		break;

	case C_EDITOR_OK:
		break;

	case C_KEYFRAMER_OK:
		break;

	case C_PICKSIZE:
		break;

	case C_MAPTYPE:
		break;

	case C_MAP_DISPLAY:
		break;

	case C_TILE_XY:
		break;

	case C_MAP_XYZ:
		break;

	case C_MAP_SCALE:
		break;

	case C_MAP_MATRIX_OLD:
		break;

	case C_MAP_MATRIX:
		break;

	case C_MAP_WID_HT:
		break;

	case C_OBNAME:
		break;

	case C_CAMNAME:
		break;

	case C_LTNAME:
		break;

	case C_CUR_MNAME:
		break;

	case C_CURMTL_FROM_MESH:
		break;

	case C_GET_SHAPE_MAKE_FACES:
		break;

	case C_DETAIL:
		break;

	case C_VERTMARK:
		break;

	case C_MSHAX:
		break;

	case C_MSHCP:
		break;

	case C_USERAX:
		break;

	case C_SHOOK:
		break;

	case C_RAX:
		break;

	case C_STAPE:
		break;

	case C_LTAPE:
		break;

	case C_ETAPE:
		break;

	case C_KTAPE:
		break;

	case C_SPHSEGS:
		break;

	case C_GEOSMOOTH:
		break;

	case C_HEMISEGS:
		break;

	case C_PRISMSEGS:
		break;

	case C_PRISMSIDES:
		break;

	case C_TUBESEGS:
		break;

	case C_TUBESIDES:
		break;

	case C_TORSEGS:
		break;

	case C_TORSIDES:
		break;

	case C_CONESIDES:
		break;

	case C_CONESEGS:
		break;

	case C_NGPARMS:
		break;

	case C_PTHLEVEL:
		break;

	case C_MSCSYM:
		break;

	case C_MFTSYM:
		break;

	case C_MTTSYM:
		break;

	case C_SMOOTHING:
		break;

	case C_MODICOUNT:
		break;

	case C_FONTSEL:
		break;

	case C_TESS_TYPE:
		break;

	case C_TESS_TENSION:
		break;

	case C_SEG_START:
		break;

	case C_SEG_END:
		break;

	case C_CURTIME:
		break;

	case C_ANIMLENGTH:
		break;

	case C_PV_FROM:
		break;

	case C_PV_TO:
		break;

	case C_PV_DOFNUM:
		break;

	case C_PV_RNG:
		break;

	case C_PV_NTH:
		break;

	case C_PV_TYPE:
		break;

	case C_PV_METHOD:
		break;

	case C_PV_FPS:
		break;

	case C_VTR_FRAMES:
		break;

	case C_VTR_HDTL:
		break;

	case C_VTR_HD:
		break;

	case C_VTR_TL:
		break;

	case C_VTR_IN:
		break;

	case C_VTR_PK:
		break;

	case C_VTR_SH:
		break;

	case C_WORK_MTLS:
		break;

	case C_WORK_MTLS_2:
		break;

	case C_WORK_MTLS_3:
		break;

	case C_WORK_MTLS_4:
		break;

	case C_WORK_MTLS_5:
		break;

	case C_WORK_MTLS_6:
		break;

	case C_WORK_MTLS_7:
		break;

	case C_WORK_MTLS_8:
		break;

	case C_WORKMTL:
		break;

	case C_SXP_TEXT_DATA:
		break;

	case C_SXP_TEXT2_DATA:
		break;

	case C_SXP_OPAC_DATA:
		break;

	case C_SXP_BUMP_DATA:
		break;

	case C_SXP_SPEC_DATA:
		break;

	case C_SXP_SHIN_DATA:
		break;

	case C_SXP_SELFI_DATA:
		break;

	case C_SXP_TEXT_MASKDATA:
		break;

	case C_SXP_TEXT2_MASKDATA:
		break;

	case C_SXP_OPAC_MASKDATA:
		break;

	case C_SXP_BUMP_MASKDATA:
		break;

	case C_SXP_SPEC_MASKDATA:
		break;

	case C_SXP_SHIN_MASKDATA:
		break;

	case C_SXP_SELFI_MASKDATA:
		break;

	case C_SXP_REFL_MASKDATA:
		break;

	case C_BGTYPE:
		break;

	case C_MEDTILE:
		break;

	case C_LO_CONTRAST:
		break;

	case C_HI_CONTRAST:
		break;

	case C_FROZ_DISPLAY:
		break;

	case C_BOOLWELD:
		break;

	case C_BOOLTYPE:
		break;

	case C_ANG_THRESH:
		break;

	case C_SS_THRESH:
		break;

	case C_TEXTURE_BLUR_DEFAULT:
		break;

	case C_MAPDRAWER:
		break;

	case C_MAPDRAWER1:
		break;

	case C_MAPDRAWER2:
		break;

	case C_MAPDRAWER3:
		break;

	case C_MAPDRAWER4:
		break;

	case C_MAPDRAWER5:
		break;

	case C_MAPDRAWER6:
		break;

	case C_MAPDRAWER7:
		break;

	case C_MAPDRAWER8:
		break;

	case C_MAPDRAWER9:
		break;

	case C_MAPDRAWER_ENTRY:
		break;

	case C_BACKUP_FILE:
		break;

	case C_DITHER_256:
		break;

	case C_SAVE_LAST:
		break;

	case C_USE_ALPHA:
		break;

	case C_TGA_DEPTH:
		break;

	case C_REND_FIELDS:
		break;

	case C_REFLIP:
		break;

	case C_SEL_ITEMTOG:
		break;

	case C_SEL_RESET:
		break;

	case C_STICKY_KEYINF:
		break;

	case C_WELD_THRESHOLD:
		break;

	case C_ZCLIP_POINT:
		break;

	case C_ALPHA_SPLIT:
		break;

	case C_KF_SHOW_BACKFACE:
		break;

	case C_OPTIMIZE_LOFT:
		break;

	case C_TENS_DEFAULT:
		break;

	case C_CONT_DEFAULT:
		break;

	case C_BIAS_DEFAULT:
		break;

	case C_DXFNAME_SRC:
		break;

	case C_AUTO_WELD:
		break;

	case C_AUTO_UNIFY:
		break;

	case C_AUTO_SMOOTH:
		break;

	case C_DXF_SMOOTH_ANG:
		break;

	case C_SMOOTH_ANG:
		break;

	case C_NET_USE_VPOST:
		break;

	case C_NET_USE_GAMMA:
		break;

	case C_NET_FIELD_ORDER:
		break;

	case C_BLUR_FRAMES:
		break;

	case C_BLUR_SAMPLES:
		break;

	case C_BLUR_DUR:
		break;

	case C_HOT_METHOD:
		break;

	case C_HOT_CHECK:
		break;

	case C_PIXEL_SIZE:
		break;

	case C_DISP_GAMMA:
		break;

	case C_FBUF_GAMMA:
		break;

	case C_FILE_OUT_GAMMA:
		break;

	case C_FILE_IN_GAMMA:
		break;

	case C_GAMMA_CORRECT:
		break;

	case C_APPLY_DISP_GAMMA:
		break;

	case C_APPLY_FBUF_GAMMA:
		break;

	case C_APPLY_FILE_GAMMA:
		break;

	case C_FORCE_WIRE:
		break;

	case C_RAY_SHADOWS:
		break;

	case C_MASTER_AMBIENT:
		break;

	case C_SUPER_SAMPLE:
		break;

	case C_OBJECT_MBLUR:
		break;

	case C_MBLUR_DITHER:
		break;

	case C_DITHER_24:
		break;

	case C_SUPER_BLACK:
		break;

	case C_SAFE_FRAME:
		break;

	case C_VIEW_PRES_RATIO:
		break;

	case C_BGND_PRES_RATIO:
		break;

	case C_NTH_SERIAL_NUM:
		break;

	case VPDATA:
		break;

	case P_QUEUE_ENTRY:
		break;

	case P_QUEUE_IMAGE:
		break;

	case P_QUEUE_USEIGAMMA:
		break;

	case P_QUEUE_PROC:
		break;

	case P_QUEUE_SOLID:
		break;

	case P_QUEUE_GRADIENT:
		break;

	case P_QUEUE_KF:
		break;

	case P_QUEUE_MOTBLUR:
		break;

	case P_QUEUE_MB_REPEAT:
		break;

	case P_QUEUE_NONE:
		break;

	case P_QUEUE_RESIZE:
		break;

	case P_QUEUE_OFFSET:
		break;

	case P_QUEUE_ALIGN:
		break;

	case P_CUSTOM_SIZE:
		break;

	case P_ALPH_NONE:
		break;

	case P_ALPH_PSEUDO:
		break;

	case P_ALPH_OP_PSEUDO:
		break;

	case P_ALPH_BLUR:
		break;

	case P_ALPH_PCOL:
		break;

	case P_ALPH_C0:
		break;

	case P_ALPH_OP_KEY:
		break;

	case P_ALPH_KCOL:
		break;

	case P_ALPH_OP_NOCONV:
		break;

	case P_ALPH_IMAGE:
		break;

	case P_ALPH_ALPHA:
		break;

	case P_ALPH_QUES:
		break;

	case P_ALPH_QUEIMG:
		break;

	case P_ALPH_CUTOFF:
		break;

	case P_ALPHANEG:
		break;

	case P_TRAN_NONE:
		break;

	case P_TRAN_IMAGE:
		break;

	case P_TRAN_FRAMES:
		break;

	case P_TRAN_FADEIN:
		break;

	case P_TRAN_FADEOUT:
		break;

	case P_TRANNEG:
		break;

	case P_RANGES:
		break;

	case P_PROC_DATA:
		break;

	default: count = 1;
	}

}

// Determine if header is followed by string.
bool C3dsFile::HasStringData(unsigned short id)
{
	bool result;

	switch (id)
	{
	case NAMED_OBJECT:
	case MAT_NAME:
	case MAT_MAPNAME:
		result = true;
		break;
	default:
		result = false;
	}

	return result;
}

// Determine if all chunks are read.
bool C3dsFile::IsEnd()
{
	return (index >= size);
}

// Go to the first chunk;
void C3dsFile::Reset(unsigned char* buffer, wchar_t** str)
{
	int id, n;
	char* ch;

	this->buffer = buffer;
	                                         index = 0;
	id = *((unsigned short*)&buffer[index]); index += sizeof(unsigned short);
	size = *((unsigned int*)&buffer[index]); index += sizeof(unsigned int);

	*str = NULL;

	if (HasStringData(id)) {

		ch = (char*)&buffer[index];
		n = (int)strlen(ch) + 1;
		index += n;

		*str = new wchar_t[n];
		MultiByteToWideChar(CP_UTF8, 0, ch, -1, *str, n);
	}
}

// Read the current chunk and move the index to the next chunk.
// Next chunk is determined by it size.
unsigned char* C3dsFile::Read(unsigned short* id)
{
	unsigned int i, size;

	i = index;
	*id  = *((unsigned short*)&buffer[i]);
	size = *((unsigned int*)&buffer[i + sizeof(unsigned short)]);

	index += size;

	return (unsigned char*)&buffer[i];
}
