// header.h : include file for standard system include files,
// or project specific include files
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define _USE_MATH_DEFINES               // for M_PI

#define WM_FRAME_INDEX     WM_USER + 5


// Windows Header Files
#include <windows.h>
#include <commdlg.h>

// C RunTime Header Files
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <math.h>

#include <gl/gl.h>               // Standard opengl include.
#include <gl/glu.h>              // Opengl utilities.

// png headers
#include <png.h>

/*
LINK THESE TO PROJECT

opengl32.lib
glu32.lib
libpngd.lib
libpng.lib
zlibstatd.lib
zlibstat.lib
*/
