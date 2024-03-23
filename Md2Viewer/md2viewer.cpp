// 
//   This program will load an md2 file and perform animation.
//   To start the animation select menu Tools -> Control.
//   On the dialog box, slide the scroll bar to do animation.
// 
//   Up Arrow Key     - move forward
//   Down Arrow Key   - move backward
//   Right Arrow Key  - rotate right
//   Left Arrow Key   - rotate left
//   S                - strafe left
//   D                - strafe right

#include "framework.h"
#include "md2viewer.h"
#include "camera.h"
#include "terrain.h"
#include "md2file.h"
#include "pngfile.h"
#include "messagedialog.h"
#include "framedialog.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

CCamera camera;
CTerrain terrain;
CMd2File file1;
CPngFile file2;
CMessageDialog dlg1;
CFrameDialog dlg2;
GLuint textures;

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK DlgProc1(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK DlgProc2(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

void OnFrameIndex(HWND hWnd, WPARAM wParam, LPARAM lParam);

void MoveCamera(double t);

void DrawAxis();

void OnPaint(HDC hDC);
void OnCreate(HWND hWnd, HDC* hDC);
void OnDestroy(HWND hWnd, HDC hDC);
void OnSize(HWND hWnd, int cx, int cy);

void OnFileOpen(HWND hWnd);
void OnFileExit(HWND hWnd);

void OnViewPoint(HWND hWnd);
void OnViewWireframe(HWND hWnd);
void OnViewSolid(HWND hWnd);

void OnToolsControl(HWND hWnd);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_MD2VIEWER, szWindowClass, MAX_LOADSTRING);

	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MD2VIEWER));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MD2VIEWER);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	RegisterClassExW(&wcex);

	// save the instance handle in a global variable
	hInst = hInstance;

	// create the main program window
	int X, Y, nWidth, nHeight, Cx, Cy;

	// 1080p: 1920 x 1080
	//  720p: 1280 x  720
	//  480p:  854 x  480

	Cx = 1280;
	Cy = 720;

	nWidth = Cx + 16;
	nHeight = Cy + 58;

	X = (GetSystemMetrics(SM_CXSCREEN) - nWidth) / 2;
	Y = (GetSystemMetrics(SM_CYSCREEN) - nHeight) / 4;

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		X, Y,
		nWidth, nHeight,
		nullptr, nullptr, hInstance, nullptr);

	if (!hWnd) return FALSE;

	// display the main program window
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MD2VIEWER));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

//  Processes messages for the main window.
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HDC hDC;

	switch (message)
	{
	case WM_FRAME_INDEX: OnFrameIndex(hWnd, wParam, lParam); break;

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDM_OPEN:      OnFileOpen(hWnd);		break;
		case IDM_EXIT:      OnFileExit(hWnd);		break;
		case IDM_POINT:		OnViewPoint(hWnd);		break;
		case IDM_WIREFRAME:	OnViewWireframe(hWnd);	break;
		case IDM_SOLID:		OnViewSolid(hWnd);		break;
		case IDM_CONTROL:	OnToolsControl(hWnd);   break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:   OnPaint(hDC);									break;
	case WM_CREATE:  OnCreate(hWnd, &hDC);							break;
	case WM_DESTROY: OnDestroy(hWnd, hDC);							break;
	case WM_SIZE:    OnSize(hWnd, LOWORD(lParam), HIWORD(lParam)); break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// callback function for dialog
INT_PTR CALLBACK DlgProc1(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return dlg1.Proc(hWnd, message, wParam, lParam);
}

// callback function for dialog
INT_PTR CALLBACK DlgProc2(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return dlg2.Proc(hWnd, message, wParam, lParam);
}

// set the key frame for animation
void OnFrameIndex(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	int index = (int)lParam;
	file1.SetFrame(index);
}

// Move the camera based on time rather than frame rate.
// Low or high frame rate have no effect on camera movement.
void MoveCamera(double t)
{
	double a, b;

	a = 10.0;  // unit per second
	b = 60.0;  // degree per second

	if (GetKeyState(VK_UP) & 0x80)    camera.MoveForward(a * t);
	if (GetKeyState(VK_DOWN) & 0x80)  camera.MoveBackward(a * t);
	if (GetKeyState(VK_LEFT) & 0x80)  camera.RotateLeft(b * t);
	if (GetKeyState(VK_RIGHT) & 0x80) camera.RotateRight(b * t);
	if (GetKeyState('S') & 0x80)      camera.StrafeLeft(a * t);
	if (GetKeyState('D') & 0x80)      camera.StrafeRight(a * t);
}

// draw x, y and z axis
void DrawAxis()
{
	glBegin(GL_LINES);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3d(0.0, 0.05, 0.0);
	glVertex3d(500.0, 0.05, 0.0);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3d(0.0, 0.05, 0.0);
	glVertex3d(0.0, 500.0, 0.0);

	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3d(0.0, 0.05, 0.0);
	glVertex3d(0.0, 0.05, 500.0);

	glEnd();
}

//
void OnPaint(HDC hDC)
{
	static DWORD t1 = GetTickCount();
	DWORD t2;
	double t;
	int i, params[4];

	// move camera based on time
	t2 = GetTickCount();
	t = (t2 - t1) / 1000.0;
	t1 = t2;

	MoveCamera(t);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	gluLookAt(camera.eyex, camera.eyey, camera.eyez, camera.centerx, camera.centery, camera.centerz, camera.upx, camera.upy, camera.upz);

	// save
	glGetIntegerv(GL_POLYGON_MODE, params);
	glDisable(GL_TEXTURE_2D);

	// draw terrain
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glColor3f(0.8f, 0.8f, 0.8f);
	terrain.Draw();

	// draw axis
	DrawAxis();

	// restore
	glPolygonMode(GL_FRONT_AND_BACK, params[0]);
	glEnable(GL_TEXTURE_2D);

	// draw model
	glBegin(GL_TRIANGLES);
	glColor3f(1.0f, 1.0f, 1.0f);

	for (i = 0; i < file1.GetFaceCount(); ++i) {

		glNormal3f(file1[i].nx, file1[i].ny, file1[i].nz);

		glTexCoord2f(file1[i].s1, file1[i].t1);
		glVertex3f(file1[i].x1, file1[i].y1, file1[i].z1);

		glTexCoord2f(file1[i].s2, file1[i].t2);;
		glVertex3f(file1[i].x2, file1[i].y2, file1[i].z2);

		glTexCoord2f(file1[i].s3, file1[i].t3);
		glVertex3f(file1[i].x3, file1[i].y3, file1[i].z3);
	}
	glEnd();

	SwapBuffers(hDC);
}

//
void OnCreate(HWND hWnd, HDC* hDC)
{
	int iPixelFormat;
	HGLRC hglRC;                // rendering context

	// create a pixel format
	static PIXELFORMATDESCRIPTOR pfd = {
		 sizeof(PIXELFORMATDESCRIPTOR),   // size of structure.
		 1,                               // version number
		 PFD_DRAW_TO_WINDOW |             // support window
		 PFD_SUPPORT_OPENGL |             // support OpenGl
		 PFD_DOUBLEBUFFER,                // double buffered
		 PFD_TYPE_RGBA,                   // RGBA type
		 24,                              // 24-bit color depth
		 0, 0, 0, 0, 0, 0,                // color bits ignored
		 0,                               // no alpha buffer
		 0,                               // shift bit ignored
		 0,                               // no accumulation buffer
		 0, 0, 0, 0,                      // accum bits ignored
		 32,                              // 32-bit z-buffer
		 0,                               // no stencil buffer
		 0,                               // no auxiliary buffer
		 PFD_MAIN_PLANE,                  // main layer
		 0,                               // reserved
		 0, 0, 0 };                       // layer masks ignored.

	*hDC = GetDC(hWnd);                                 // get the device context for our window
	iPixelFormat = ChoosePixelFormat(*hDC, &pfd);   // get the best available match of pixel format for the device context
	SetPixelFormat(*hDC, iPixelFormat, &pfd);       // make that the pixel format of the device context
	hglRC = wglCreateContext(*hDC);                     // create an OpenGL rendering context
	wglMakeCurrent(*hDC, hglRC);                        // make it the current rendering context

	// set camera inital position
	camera.SetPosition(108.19099, 1.6, 99.08579, 107.52732, 1.6, 98.33775, 0.0, 1.0, 0.0);

	// create terrain
	terrain.Create(500.0f, 500);

	// clear window
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);

	// set blending parameter
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// set up texture parameter
	glGenTextures(1, &textures);
	glBindTexture(GL_TEXTURE_2D, textures);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	char str[100];
	OutputDebugStringA("-----------------------------------------------------------------------------\n");
	sprintf_s(str, 100, "OpenGL Version :%s\n", glGetString(GL_VERSION));   OutputDebugStringA(str);
	//sprintf_s(str, 100, "GLES Version   :%s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));  OutputDebugStringA(str);
	sprintf_s(str, 100, "GLU Version    :%s\n", glGetString(GLU_VERSION));  OutputDebugStringA(str);
	sprintf_s(str, 100, "Vendor         :%s\n", glGetString(GL_VENDOR));    OutputDebugStringA(str);
	sprintf_s(str, 100, "Renderer       :%s\n", glGetString(GL_RENDERER));  OutputDebugStringA(str);
	OutputDebugStringA("-----------------------------------------------------------------------------");

}

//
void OnDestroy(HWND hWnd, HDC hDC)
{
	glDeleteTextures(1, &textures);

	HGLRC hglRC;					// rendering context

	hglRC = wglGetCurrentContext(); // get current OpenGL rendering context
	wglMakeCurrent(hDC, NULL);      // make the rendering context not current
	wglDeleteContext(hglRC);        // delete the rendering context
	ReleaseDC(hWnd, hDC);           // releases a device context

	PostQuitMessage(0);		// close the program.
}

//
void OnSize(HWND hWnd, int cx, int cy)
{
	double fovy, aspect, zNear, zFar;

	fovy = 45.0;
	aspect = (double)cx / (double)cy;
	zNear = 0.1;
	zFar = 1000.0;

	glViewport(0, 0, cx, cy);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, aspect, zNear, zFar);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

//
void OnFileOpen(HWND hWnd)
{
	OPENFILENAME fn;
	TCHAR szFile1[MAX_PATH] = L"", szFile2[MAX_PATH], str[MAX_PATH];
	char name[100];

	ZeroMemory(&fn, sizeof(OPENFILENAME));

	fn.lStructSize = sizeof(OPENFILENAME);
	fn.hwndOwner = hWnd;
	fn.hInstance = hInst;
	fn.lpstrFilter = _T("MD2 Files\0*.md2\0All Files\0*.*\0");
	fn.nFilterIndex = 0;
	fn.lpstrFile = szFile1;
	fn.nMaxFile = MAX_PATH;
	fn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (!GetOpenFileName(&fn)) return;

	if (!file1.Open(szFile1)) {
		dlg1.Show(hWnd, hInst, DlgProc1, L"Cannot open file: Not md2 file.");
		return;
	}

	// get texture filename
	file1.GetTextureName(name, 100);
	MultiByteToWideChar(CP_UTF8, 0, name, -1, szFile2, MAX_PATH);

	if (!file2.Open(szFile2)) {
		dlg1.Show(hWnd, hInst, DlgProc1, L"Cannot open file: Not png file.");
		return;
	}

	// set up texture image
	switch (file2.color_type)
	{
	case PNG_COLOR_TYPE_RGB:       gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, file2.width, file2.height, GL_RGB, GL_UNSIGNED_BYTE, file2.buffer); break;
	case PNG_COLOR_TYPE_RGB_ALPHA: gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, file2.width, file2.height, GL_RGBA, GL_UNSIGNED_BYTE, file2.buffer); break;
	}

	// set window title to include filename
	swprintf_s(str, MAX_PATH, L"%s - %s", szTitle, szFile1);
	SetWindowText(hWnd, str);
}

//
void OnFileExit(HWND hWnd)
{
	DestroyWindow(hWnd);
}

//
void OnViewPoint(HWND hWnd)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
}

//
void OnViewWireframe(HWND hWnd)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

//
void OnViewSolid(HWND hWnd)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

//
void OnToolsControl(HWND hWnd)
{
	dlg2.Show(hWnd, hInst, DlgProc2, 0, file1.GetFrameCount());
}
