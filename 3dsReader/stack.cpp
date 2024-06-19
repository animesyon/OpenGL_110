
// Last In First Out Data Structure

#include "framework.h"
#include "stack.h"

// constructor
CStack::CStack()
{
	First = NULL;
	Count = 0;
}

// destructor
CStack::~CStack()
{
	wchar_t str[MAX_PATH];

	while (!IsEmpty())
		Pop(str, MAX_PATH);
}

//
int CStack::GetCount()
{
	return Count;
}

//
bool CStack::IsEmpty()
{
	return (Count == 0);
}

// 
//     +-----+
//     |     |  Node     |
//     +-----+           |
//        |              |
//        V              | push
//                       |
//                       |
//     +-----+           |
//     |     | First     V
//     +-----+
//        |
//        V
//     +-----+
//     |     |
//     +-----+
//        |
//        V
//     +-----+
//     |     |
//     +-----+
//        |
//        V
//       NULL
//
void CStack::Push(wchar_t* str)
{
	NODE_S* Node;
	size_t n;

	Node = new NODE_S;

	n = wcslen(str) + 1;
	Node->str = new wchar_t[n];
	wcscpy_s(Node->str, n, str);

	Node->Next = NULL;

	if (IsEmpty()) {
		First = Node;
	}
	else {
		Node->Next = First;
		First = Node;
	}

	Count++;
}

// 
//     +-----+
//     |     |  First     ^
//     +-----+            |
//        |               |
//        V               |
//                        | pop
//                        |
//     +-----+            |
//     |     |            |
//     +-----+
//        |
//        V
//     +-----+
//     |     |
//     +-----+
//        |
//        V
//     +-----+
//     |     |
//     +-----+
//        |
//        V
//       NULL
//
void CStack::Pop(wchar_t* str, size_t n)
{
	NODE_S* Node;

	Node = First;

	wcscpy_s(str, n, Node->str);

	First = First->Next;
	Count--;

	delete[] Node->str;
	delete Node;
}
