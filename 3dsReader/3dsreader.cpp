
// A 3ds file is a collection of chunks arrange in hierarchical order.
// This program is to open a 3ds file.
// The chunk data are display similar to a window explorer.

#include "framework.h"
#include "3dsreader.h"
#include "3dsfile.h"
#include "queue.h"
#include "stack.h"

const int MAX_LOADSTRING = 100;
const int PADDING = 2;
const int SPLITTER = 6;
const int FONT_HEIGHT = 23;

HINSTANCE hInst;
WCHAR szTitle[MAX_LOADSTRING];
WCHAR szWindowClass[MAX_LOADSTRING];

RECT rect;
bool dragging;

HIMAGELIST himl;
HWND hEdit1, hEdit2, hTree1, hList1;

C3dsFile file;

LRESULT CALLBACK    WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

HTREEITEM TreeView_GetPathName(HWND hTree, HTREEITEM hItem, CStack* stack);

void OnItemExpanding(HWND hWnd, LPARAM lParam);
void OnItemExpanded(LPARAM lParam);
void OnSelChanged(HWND hWnd, LPARAM lParam);

void OnLButtonDown(HWND hWnd, int x, int y);
void OnLButtonUp(HWND hWnd, int x, int y);
void OnMouseMove(HWND hWnd, int x, int y);
void OnSize(HWND hWnd, int width, int height);

void OnPaint(HWND hWnd);
void OnCreate(HWND hWnd);
void OnDestroy(HWND hWnd);

void OnFileOpen(HWND hWnd);
void OnFileExit(HWND hWnd);

// main program
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY3DSREADER, szWindowClass, MAX_LOADSTRING);

    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY3DSREADER));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MY3DSREADER);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    RegisterClassExW(&wcex);

    hInst = hInstance;

    int X, Y, nWidth, nHeight, Cx, Cy;

    Cx = 1280;
    Cy =  720;

    nWidth = Cx + 16;
    nHeight = Cy + 59;

    X = (GetSystemMetrics(SM_CXSCREEN) - nWidth) / 2;
    Y = (GetSystemMetrics(SM_CYSCREEN) - nHeight) / 3;

    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
        X, Y,
        nWidth, nHeight,
        nullptr, nullptr, hInstance, nullptr);

    if (!hWnd)
        return FALSE;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY3DSREADER));

    // message loop
    MSG msg;

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

// window callback function
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_NOTIFY:
        switch (((LPNMHDR)lParam)->code)
        {
        case TVN_ITEMEXPANDING: OnItemExpanding(hWnd, lParam);  break;
        case TVN_ITEMEXPANDED:  OnItemExpanded(lParam);         break;
        case TVN_SELCHANGED:    OnSelChanged(hWnd, lParam);	    break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;
    case WM_LBUTTONDOWN:OnLButtonDown(hWnd, LOWORD(lParam), HIWORD(lParam));	break;
    case WM_LBUTTONUP:	OnLButtonUp(hWnd, LOWORD(lParam), HIWORD(lParam));		break;
    case WM_MOUSEMOVE:	OnMouseMove(hWnd, LOWORD(lParam), HIWORD(lParam));		break;
    case WM_SIZE:       OnSize(hWnd, LOWORD(lParam), HIWORD(lParam));           break;

    case WM_COMMAND:

        switch (LOWORD(wParam))
        {
        case IDM_OPEN:		        OnFileOpen(hWnd);				break;
        case IDM_EXIT:		        OnFileExit(hWnd);				break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;

    case WM_PAINT:   OnPaint(hWnd);									break;
    case WM_CREATE:  OnCreate(hWnd);								break;
    case WM_DESTROY: OnDestroy(hWnd);								break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

//
HTREEITEM TreeView_GetPathName(HWND hTree, HTREEITEM hItem, CStack* stack)
{
    HTREEITEM hPrevItem;
    TV_ITEM tvi;
    unsigned char* buffer;
    wchar_t str[300];

    hPrevItem = hItem;

    while (hPrevItem != NULL) {

        tvi.mask = TVIF_HANDLE | TVIF_PARAM;
        tvi.hItem = hPrevItem;

        TreeView_GetItem(hTree, &tvi);

        buffer = (unsigned char*)tvi.lParam;

        file.GetChunkName(buffer, str, 300);

        stack->Push(str);

        hPrevItem = TreeView_GetParent(hTree, hPrevItem);
    }

    return hItem;
}

//
void OnItemExpanding(HWND hWnd, LPARAM lParam)
{
    HWND hTree = ((LPNMHDR)lParam)->hwndFrom;
    LPNM_TREEVIEW lpnmtv = (LPNM_TREEVIEW)lParam;
    HTREEITEM hItem = lpnmtv->itemNew.hItem;
    TVINSERTSTRUCT tvis;
    TV_ITEM tvi;
    unsigned char* buffer;
    unsigned short id;
    int count;
    unsigned char* lparam;
    wchar_t str[200];
    wchar_t* name;

    if (lpnmtv->action == TVE_EXPAND) {

        // Get the selected chunk
        // Variable tvi.lParam point to this chunk
        tvi.mask = TVIF_HANDLE | TVIF_PARAM;
        tvi.hItem = hItem;

        TreeView_GetItem(hTree, &tvi);

        buffer = (unsigned char*)tvi.lParam;

        tvis.hParent = hItem;
        tvis.hInsertAfter = TVI_LAST;
        tvis.itemex.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM | TVIF_CHILDREN;
        tvis.item.hItem = NULL;
        tvis.item.state = 0;
        tvis.item.stateMask = 0;

        file.Reset(buffer, &name);

        // If it has string data, add it with no sub-chunk.
        if (name != NULL) {

            tvis.item.iImage = 3;
            tvis.item.iSelectedImage = 3;

            tvis.item.pszText = name;
            tvis.item.cchTextMax = (int)wcslen(name);
            tvis.item.lParam = (LPARAM)NULL;
            tvis.item.cChildren = 0;

            TreeView_InsertItem(hTree, &tvis);

            delete[] name;
        }

        // Add all remaining chunk into parent chunk
        tvis.item.iImage = 1;
        tvis.item.iSelectedImage = 2;

        while (!file.IsEnd()) {

            lparam = file.Read(&id);
            file.GetChunkName(id, str, 200);
            count = file.GetSubChunkCount(id);

            tvis.item.pszText = str;
            tvis.item.cchTextMax = (int)wcslen(str);
            tvis.item.lParam = (LPARAM)lparam;
            tvis.item.cChildren = count;

            TreeView_InsertItem(hTree, &tvis);
        }
    }
}

//
void OnItemExpanded(LPARAM lParam)
{
    HWND hTree = ((LPNMHDR)lParam)->hwndFrom;
    LPNM_TREEVIEW lpnmtv = (LPNM_TREEVIEW)lParam;
    TV_ITEM tvi = lpnmtv->itemNew;

    if (lpnmtv->action == TVE_COLLAPSE)
        TreeView_Expand(hTree, tvi.hItem, TVE_COLLAPSE | TVE_COLLAPSERESET);
}

//
void OnSelChanged(HWND hWnd, LPARAM lParam)
{
    const int N = 100;
    const int N1 = 300;

    HWND hTree = ((LPNMHDR)lParam)->hwndFrom;
    LPNM_TREEVIEW lpnmtv = (LPNM_TREEVIEW)lParam;
    HTREEITEM hItem = lpnmtv->itemNew.hItem;
    LVCOLUMN lvc;
    TV_ITEM tvi;
    LV_ITEM lvi;
    CStack stack;
    CQueue que;
    int i, j, m, n;
    unsigned int size;
    unsigned char* buffer;
    wchar_t str[N], str1[N1];

    // Get the path.
    TreeView_GetPathName(hTree, hItem, &stack);

    stack.Pop(str, N);
    wcscpy_s(str1, N1, str);

    while (!stack.IsEmpty()) {

        stack.Pop(str, 100);

        wcscat_s(str1, N1, L" \u25ba ");
        wcscat_s(str1, N1, str);
    }

    // Display the path.
    SetWindowText(hEdit1, str1);

    // Clear all.
    lvc.mask = LVCF_TEXT;
    lvc.pszText = (LPWSTR)L"";

    ListView_SetColumn(hList1, 0, &lvc);
    ListView_SetColumn(hList1, 1, &lvc);
    ListView_SetColumn(hList1, 2, &lvc);
    ListView_SetColumn(hList1, 3, &lvc);

    ListView_DeleteAllItems(hList1);
    SetWindowText(hEdit2, L"");

    // Read the chunk data.
    // Chunk data is in the tvi.lParam of the selected item.
    tvi.mask = TVIF_HANDLE | TVIF_PARAM;
    tvi.hItem = hItem;

    TreeView_GetItem(hTree, &tvi);
    buffer = (unsigned char*)tvi.lParam;

    if (buffer == NULL) return;

    file.ReadChunkData(buffer, &size, &m, &que);
    n = que.GetCount() / m - 1;

    // Display data size and data count.
    swprintf_s(str, 100, L"%d bytes %d count", size, n);
    SetWindowText(hEdit2, str);

    if (que.IsEmpty()) return;

    // Set the column of a list view control
    for (j = 0; j < m; j++) {

        que.Remove(str, N);
        lvc.pszText = str;
        ListView_SetColumn(hList1, j+1, &lvc);
    }

    // Fill row with data
    if (que.IsEmpty()) return;

    lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;
    lvi.state = 0;
    lvi.stateMask = 0;
    lvi.iImage = 3;

    for (i = 0; i < n; i++) {

        lvi.iItem = i;
        lvi.iSubItem = 0;
        lvi.pszText = (LPWSTR)L"";
        ListView_InsertItem(hList1, &lvi);

        for (j = 0; j < m; j++) {

            que.Remove(str, N);
            ListView_SetItemText(hList1, i, j+1, str);

        }
    }
}

// Begin dragging splitter.
void OnLButtonDown(HWND hWnd, int x, int y)
{
    if (rect.left < x && x < rect.right && rect.top < y && y < rect.bottom) {
        dragging = true;
        SetCursor(LoadCursor(NULL, IDC_SIZEWE));
        SetCapture(hWnd);
    }
}

// End dragging splitter.
void OnLButtonUp(HWND hWnd, int x, int y)
{
    dragging = false;
    ReleaseCapture();
}

// Dragging splitter in progress.
void OnMouseMove(HWND hWnd, int x, int y)
{
    RECT cr;
    int x1, x2, y2, width2, width3, height2;

    if (dragging) {

        GetClientRect(hWnd, &cr);
        if (x < 150 || (cr.right - 150) < x) return;

        rect.left = x;
        rect.right = rect.left + SPLITTER;

        x1 = PADDING;
        x2 = rect.right;

        y2 = rect.top;

        width2 = rect.left - PADDING;
        width3 = cr.right - rect.right - PADDING;

        height2 = rect.bottom - rect.top;

        MoveWindow(hTree1, x1, y2, width2, height2, TRUE);
        MoveWindow(hList1, x2, y2, width3, height2, TRUE);
    }
    else {
        if (rect.left < x && x < rect.right && rect.top < y && y < rect.bottom) SetCursor(LoadCursor(NULL, IDC_SIZEWE));
    }
}

// Position UI
// 
//       y1 +--------------width1-----------------+              ---
//          |                                     |               ^
//          |               EDIT 1                | height1       |
//          |                L R                  |               |
//       y2 +-----width2----T+-+------width3------+               |
//          |                | |                  |               |
//          |                | |                  |               |
//          |     TREE       | |      LIST        | height2     height
//          |                | |                  |               |
//          |                | |                  |               |
//       y3 +---------------B+-+------------------+               |
//          |                                     |               ^
//          |              EDIT 2                 | height1       |
//          |                                     |               v
//          +-------------------------------------+              ---
//          x1                 x2
// 
//          |<----------------width-------------->|

void OnSize(HWND hWnd, int width, int height)
{
    rect.bottom = height - rect.top;

    int x1, x2, y1, y2, y3, width1, width2, width3, height1, height2;

    x1 = PADDING;
    x2 = rect.right;

    y1 = PADDING;
    y2 = rect.top;
    y3 = rect.bottom + PADDING;

    width1 = width - 2 * PADDING;
    width2 = rect.left - PADDING;
    width3 = width - rect.right - PADDING;

    height1 = FONT_HEIGHT;
    height2 = rect.bottom - rect.top;

    MoveWindow(hEdit1, x1, y1, width1, height1, FALSE);
    MoveWindow(hEdit2, x1, y3, width1, height1, FALSE);

    MoveWindow(hTree1, x1, y2, width2, height2, FALSE);
    MoveWindow(hList1, x2, y2, width3, height2, FALSE);
}

//
void OnPaint(HWND hWnd)
{
    PAINTSTRUCT ps;
    HDC hDC;

    hDC = BeginPaint(hWnd, &ps);
    EndPaint(hWnd, &ps);
}

//
void OnCreate(HWND hWnd)
{
    rect.top = 2 * PADDING + FONT_HEIGHT;
    rect.left = 300;
    rect.right = rect.left + SPLITTER;

    HICON hicon;

    // Create icons to be used in tree view control and list view control
    himl = ImageList_Create(16, 16, ILC_MASK, 5, 0);

    if (himl == NULL)
        return;

    hicon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON_1));
    ImageList_AddIcon(himl, hicon);

    hicon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON_2));
    ImageList_AddIcon(himl, hicon);

    hicon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON_3));
    ImageList_AddIcon(himl, hicon);

    hicon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_ICON_4));
    ImageList_AddIcon(himl, hicon);

    // Create edit view control
    HFONT hFont = CreateFont(16, 0, 0, 0, FW_REGULAR, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_MODERN, L"Consolas");

    hEdit1 = CreateWindowEx(0, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_LEFT | ES_READONLY | WS_BORDER,
        0, 0, 1, 1, hWnd, (HMENU)IDC_EDIT1, hInst, NULL);

    SendMessage(hEdit1, WM_SETFONT, (WPARAM)hFont, 0);

    hEdit2 = CreateWindowEx(0, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_RIGHT | ES_READONLY | WS_BORDER,
        0, 0, 1, 1, hWnd, (HMENU)IDC_EDIT2, hInst, NULL);

    SendMessage(hEdit2, WM_SETFONT, (WPARAM)hFont, 0);

    // Create tree view control
    hTree1 = CreateWindowEx(0L, WC_TREEVIEW, L"", WS_VISIBLE | WS_CHILD | WS_BORDER | TVS_HASLINES | TVS_HASBUTTONS | TVS_LINESATROOT,
        0, 0, 1, 1, hWnd, (HMENU)IDC_TREE1, hInst, NULL);

    TreeView_SetImageList(hTree1, himl, TVSIL_NORMAL);

    // Create list view control
    hList1 = CreateWindowEx(0L, WC_LISTVIEW, L"", WS_VISIBLE | LVS_SHOWSELALWAYS | WS_CHILD | WS_BORDER | LVS_REPORT | LVS_NOSORTHEADER | LVS_SINGLESEL,
        0, 0, 1, 1, hWnd, (HMENU)IDC_LIST1, hInst, NULL);

    ListView_SetImageList(hList1, himl, LVSIL_SMALL);

    LV_COLUMN lvc;
    wchar_t str[100];

    wcscpy_s(str, 100, L"");

    lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
    lvc.pszText = str;
    lvc.fmt = LVCFMT_RIGHT;

    lvc.cx = 32;
    ListView_InsertColumn(hList1, 0, &lvc);

    lvc.cx = 128;
    ListView_InsertColumn(hList1, 1, &lvc);
    ListView_InsertColumn(hList1, 2, &lvc);
    ListView_InsertColumn(hList1, 3, &lvc);
    ListView_InsertColumn(hList1, 4, &lvc);

}

//
void OnDestroy(HWND hWnd)
{
    PostQuitMessage(0);
}

//
void OnFileOpen(HWND hWnd)
{
    OPENFILENAME ofn;
    wchar_t szFile[MAX_PATH] = L"";
    wchar_t str[MAX_PATH];
    unsigned short id;
    unsigned int size;
    unsigned char* lParam;

    ZeroMemory(&ofn, sizeof(OPENFILENAME));

    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = hWnd;
    ofn.hInstance = hInst;
    ofn.lpstrFilter = L"3D Sudio\0*.3ds\0All Files\0*.*\0";
    ofn.nFilterIndex = 0;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (!GetOpenFileName(&ofn)) return;

    if (!file.Open(szFile))
        return;

    swprintf_s(str, MAX_PATH, L"%s - %s", szTitle, szFile);
    SetWindowText(hWnd, str);

    // Read the main chunk.
    // Variable lParam point to this data.
    lParam = file.ReadMainChunk(&id, &size);
    file.GetChunkName(id, str, MAX_PATH);

    TVINSERTSTRUCT tvinsert;

    // Clear all.
    TreeView_DeleteAllItems(hTree1);
    ListView_DeleteAllItems(hList1);
    SetWindowText(hEdit1, L"");
    SetWindowText(hEdit2, L"");

    // Add the root item in the tree view control
    // Assign lParam got from above to tvinsert.item.lParam
    // We gonna use it to get chunk data when expanding and selecting.
    tvinsert.hParent = NULL;
    tvinsert.hInsertAfter = TVI_ROOT;
    tvinsert.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT | TVIF_PARAM | TVIF_CHILDREN;
    tvinsert.item.hItem = NULL;
    tvinsert.item.state = 0;
    tvinsert.item.stateMask = 0;
    tvinsert.item.iImage = 0;
    tvinsert.item.iSelectedImage = 0;
    tvinsert.item.pszText = str;
    tvinsert.item.cchTextMax = (int)wcslen(str);
    tvinsert.item.lParam = (LPARAM)lParam;
    tvinsert.item.cChildren = 1;

    TreeView_InsertItem(hTree1, &tvinsert);
}

//
void OnFileExit(HWND hWnd)
{
    DestroyWindow(hWnd);
}
