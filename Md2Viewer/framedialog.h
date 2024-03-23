/*
   Class Name:

	  CFrameDialog

   Description:

	  select key frame
*/

#pragma once

#include "resource.h"

class CFrameDialog
{
private:
	HWND hDialog;
	int index, count;

	void OnInitDialog(HWND hWnd);
	void OnHScroll(HWND hWnd, WPARAM wParam, LPARAM lParam);
	void OnClose(HWND hWnd);

public:
	CFrameDialog();
	~CFrameDialog();

	void Show(HWND hWnd, HINSTANCE hInstance, DLGPROC pProc, int index, int count);
	INT_PTR Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

};
