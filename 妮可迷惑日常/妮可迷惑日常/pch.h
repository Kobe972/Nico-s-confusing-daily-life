#pragma once

/*
	这里放头文件和全局变量
*/

#include <cassert>
#include <cmath>
#include <algorithm>
#include <deque>
#include <exception>
#include <fstream>
#include <functional>
#include <ios>
#include <iostream>
#include <istream>
#include <iterator>
#include <limits>
#include <list>
#include <locale>
#include <map>
#include <memory>
#include <numeric>
#include <ostream>
#include <queue>
#include <random>
#include <regex>
#include <set>
#include <stack>
#include <string>
#include <strstream>
#include <tuple>
#include <vector>


#include <ddraw.h>    // directX includes
#pragma comment(lib, "ddraw.lib")
#pragma comment(lib, "dxguid.lib")

/*
#define EXTERN_BOB_OBJECTS()
LPDIRECTDRAW7         lpdd = NULL;   // dd object																	\
LPDIRECTDRAWSURFACE7  lpddsprimary = NULL;   // dd primary surface
LPDIRECTDRAWSURFACE7  lpddsback = NULL;   // dd back surface
LPDIRECTDRAWSURFACE7  lpddsoffscreen = NULL;   // dd off screen surface
LPDIRECTDRAWPALETTE   lpddpal = NULL;   // a pointer to the created dd palette
LPDIRECTDRAWCLIPPER   lpddclipper = NULL;   // dd clipper
PALETTEENTRY          palette[256];          // color palette
PALETTEENTRY          save_palette[256];     // used to save palettes
DDSURFACEDESC2        ddsd;                  // a direct draw surface description struct
DDBLTFX               ddbltfx;               // used to fill
DDSCAPS2              ddscaps; // a direct draw surface capabilities struct



extern LPDIRECTDRAW7        lpdd;  // dd object
extern LPDIRECTDRAWSURFACE7 lpddsprimary;  // dd primary surface
extern LPDIRECTDRAWSURFACE7 lpddsback;  // dd back surface
extern LPDIRECTDRAWCLIPPER  lpddclipper;   // dd clipper for back surface
extern LPDIRECTDRAWCLIPPER  lpddclipperwin; // dd clipper for window

extern DDSURFACEDESC2       ddsd;                 // a direct draw surface description struct
extern DDBLTFX              ddbltfx;              // used to fill
extern DDSCAPS2             ddscaps;              // a direct draw surface capabilities struct
extern HRESULT              ddrval;               // result back from dd calls
extern UINT* primary_buffer; // primary video buffer
extern UINT* back_buffer; // secondary back buffer

extern HWND main_window_handle; // save the window handle
extern HINSTANCE main_instance; // save the instance

extern int screen_width,            // width of screen
screen_height;           // height of screen

extern int min_clip_x,                             // clipping rectangle
max_clip_x,
min_clip_y,
max_clip_y;

extern int window_client_x0;   // used to track the starting (x,y) client area for
extern int window_client_y0;   // for windowed mode directdraw operations
*/

typedef unsigned short USHORT;
typedef unsigned short WORD;
typedef unsigned char  BYTE;
typedef unsigned int   QUAD;
typedef unsigned int   UINT;

#define EXTERN_BOB_OBJECTS()																\
extern LPDIRECTDRAW7        lpdd;														\
extern LPDIRECTDRAWSURFACE7 lpddsprimary;  												\
extern LPDIRECTDRAWSURFACE7 lpddsback;  												\
extern LPDIRECTDRAWCLIPPER  lpddclipper; 												\
extern LPDIRECTDRAWCLIPPER  lpddclipperwin; 											\
extern DDSURFACEDESC2       ddsd;               										\
extern DDBLTFX              ddbltfx;          								 			\
extern DDSCAPS2             ddscaps;        											\
extern HRESULT              ddrval;             										\
extern UINT* primary_buffer;															\
extern UINT* back_buffer; 																\
extern HWND main_window_handle; 														\
extern HINSTANCE main_instance; 														\
extern int screen_width, screen_height;													\
extern int min_clip_x,  max_clip_x, min_clip_y, max_clip_y;  							\
extern int window_client_x0;   															\
extern int window_client_y0;   															\


#define CREATE_BOB_OBJECTS()													\
LPDIRECTDRAW7        lpdd;														\
LPDIRECTDRAWSURFACE7 lpddsprimary;  											\
LPDIRECTDRAWSURFACE7 lpddsback;  												\
LPDIRECTDRAWCLIPPER  lpddclipper; 												\
LPDIRECTDRAWCLIPPER  lpddclipperwin; 											\
DDSURFACEDESC2       ddsd;               										\
DDBLTFX              ddbltfx;          								 			\
DDSCAPS2             ddscaps;        											\
HRESULT              ddrval;             										\
UINT* primary_buffer;															\
UINT* back_buffer; 																\
HWND main_window_handle; 														\
HINSTANCE main_instance; 														\
int screen_width, screen_height;												\
int min_clip_x,  max_clip_x, min_clip_y, max_clip_y;  							\
int window_client_x0;   														\
int window_client_y0;   														\

// these read the keyboard asynchronously
#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

#define SCREEN_WIDTH        640  // size of screen
#define SCREEN_HEIGHT       480

#define DDRAW_INIT_STRUCT(ddstruct) { memset(&ddstruct,0,sizeof(ddstruct)); ddstruct.dwSize=sizeof(ddstruct); }

// bit manipulation macros
#define SET_BIT(word,bit_flag)   ((word)=((word) | (bit_flag)))
#define RESET_BIT(word,bit_flag) ((word)=((word) & (~bit_flag)))
