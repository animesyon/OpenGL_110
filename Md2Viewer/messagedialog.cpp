/*
   Class Name:

	  CMessageDialog

   Description:

	  display information
*/

#include "framework.h"
#include "messagedialog.h"

// constructor
CMessageDialog::CMessageDialog()
{
}

// destructor
CMessageDialog::~CMessageDialog()
{

}


// center the dialog
//
//          +----------------------------+
//          |                            |
//          |           rect1            |
//          |                            |
//          |   +--------------------+   |
//          |   |                    |   |
//          |   |       rect2        |   |
//          |   |                    |   |
//          |   +--------------------+   |
//          |                            |
//          |                            |
//          +----------------------------+
//
void CMessageDialog::OnInitDialog(HWND hWnd)
{
	HWND hwnd;
	RECT rect1, rect2;
	int x, y;

	hwnd = GetParent(hWnd);
	GetWindowRect(hwnd, &rect1);
	GetWindowRect(hWnd, &rect2);

	x = rect1.left + ((rect1.right - rect1.left) - (rect2.right - rect2.left)) / 2;
	y = rect1.top + ((rect1.bottom - rect1.top) - (rect2.bottom - rect2.top)) / 2;

	SetWindowPos(hWnd, HWND_TOP, x, y, 0, 0, SWP_NOSIZE);

	SetDlgItemText(hWnd, IDC_STATIC1, str);
}

//
void CMessageDialog::OnOk(HWND hWnd, WPARAM wParam)
{
	EndDialog(hWnd, LOWORD(wParam));
}

//
void CMessageDialog::Show(HWND hWnd, HINSTANCE hInstance, DLGPROC pProc, const wchar_t* str)
{
	wcscpy_s(this->str, 100, str);
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, pProc);
}

//
INT_PTR CMessageDialog::Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
	case WM_INITDIALOG:
		OnInitDialog(hWnd);
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			OnOk(hWnd, wParam);
			return (INT_PTR)TRUE;
		}
		break;
	}

	return (INT_PTR)FALSE;
}
