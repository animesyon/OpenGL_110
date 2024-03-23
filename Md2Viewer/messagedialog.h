/*
   Class Name:

	  CMessageDialog

   Description:

	  display information
*/

#pragma once

#include "resource.h"

class CMessageDialog
{
private:
	wchar_t str[100];

	void OnInitDialog(HWND hWnd);
	void OnOk(HWND hWnd, WPARAM wParam);

public:
	CMessageDialog();
	~CMessageDialog();

	void Show(HWND hWnd, HINSTANCE hInstance, DLGPROC pProc, const wchar_t* str);
	INT_PTR Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};
