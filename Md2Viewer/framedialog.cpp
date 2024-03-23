/*
   Class Name:

	  CFrameDialog

   Description:

	  select key frame
*/

#include "framework.h"
#include "framedialog.h"

CFrameDialog::CFrameDialog()
{
	hDialog = NULL;
	index = count = 0;
}

CFrameDialog::~CFrameDialog()
{

}

void CFrameDialog::OnInitDialog(HWND hWnd)
{
	HWND hwnd;
	RECT rect1, rect2;
	int x, y;

	// center dialog
	hwnd = GetParent(hWnd);
	GetWindowRect(hwnd, &rect1);
	GetWindowRect(hWnd, &rect2);

	x = rect1.left + ((rect1.right - rect1.left) - (rect2.right - rect2.left)) / 2;
	y = rect1.top + ((rect1.bottom - rect1.top) - (rect2.bottom - rect2.top)) / 2;

	SetWindowPos(hWnd, HWND_TOP, x, y, 0, 0, SWP_NOSIZE);

	// set scrollbar parameter
	SCROLLINFO si;
	HWND hScrollBar;
	int page;
	wchar_t str[100];

	page = 1;

	hScrollBar = GetDlgItem(hWnd, IDC_SCROLLBAR1);
	si.cbSize = sizeof(si);
	si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
	si.nMin = 0;
	si.nMax = count - 1;
	si.nPage = page;
	si.nPos = index;

	SetScrollInfo(hScrollBar, SB_CTL, &si, TRUE);

	swprintf_s(str, 100, L"%d / %d", index, count);
	SetDlgItemText(hWnd, IDC_TEXT1, str);

}

void CFrameDialog::OnHScroll(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	HWND hScrollBar = (HWND)lParam;
	SCROLLINFO si;
	wchar_t str[100];
	int index = 0;

	si.cbSize = sizeof(si);
	si.fMask = SIF_ALL;
	GetScrollInfo(hScrollBar, SB_CTL, &si);

	switch (LOWORD(wParam)) {
	case SB_LINELEFT:     si.nPos -= 1;            break;
	case SB_LINERIGHT:    si.nPos += 1;            break;
	case SB_PAGELEFT:     si.nPos -= si.nPage;     break;
	case SB_PAGERIGHT:    si.nPos += si.nPage;     break;
	case SB_THUMBTRACK:   si.nPos = si.nTrackPos;  break;
	default:
		break;
	}

	si.fMask = SIF_POS;
	SetScrollInfo(hScrollBar, SB_CTL, &si, TRUE);

	if (GetDlgCtrlID(hScrollBar) == IDC_SCROLLBAR1) {
		index = si.nPos;
		if (index < 0) index = 0;
		if (index > (count - 1)) index = count - 1;
	}

	swprintf_s(str, 100, L"%d / %d", index, count);
	SetDlgItemText(hWnd, IDC_TEXT1, str);

	HWND hwnd = GetParent(hWnd);
	SendMessage(hwnd, WM_FRAME_INDEX, (WPARAM)0, (LPARAM)index);
}

void CFrameDialog::OnClose(HWND hWnd)
{
	DestroyWindow(hWnd);
	hDialog = NULL;
}

void CFrameDialog::Show(HWND hWnd, HINSTANCE hInstance, DLGPROC pProc, int index, int count)
{
	if (IsWindow(hDialog)) return;

	this->index = index;
	this->count = count;

	hDialog = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG2), hWnd, pProc);
	ShowWindow(hDialog, SW_SHOW);
}

INT_PTR CFrameDialog::Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	HWND hwnd = GetParent(hWnd);

	switch (message)
	{
	case WM_INITDIALOG: OnInitDialog(hWnd); return (INT_PTR)TRUE;
	case WM_HSCROLL: OnHScroll(hWnd, wParam, lParam); break;
	case WM_CLOSE: OnClose(hWnd);
	}

	return (INT_PTR)FALSE;
}
