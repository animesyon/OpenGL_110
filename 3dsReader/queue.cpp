
// First In First Out Data Structure

#include "framework.h"
#include "queue.h"

//
CQueue::CQueue()
{
	First = Last = NULL;
	Count = 0;
}

//
CQueue::~CQueue()
{
	wchar_t str[300];

	while (!IsEmpty())
		Remove(str, 300);
}

//
int CQueue::GetCount()
{
	return Count;
}

//
bool CQueue::IsEmpty()
{
	return (Count == 0);
}

//                                                                       add
//                                                               <---------------
//      First                                             Last                     Node
//     +----+    +----+    +----+    +----+    +----+    +----+                   +----+
//     |    |--->|    |--->|    |--->|    |--->|    |--->|    |---> NULL          |    |---> NULL
//     +----+    +----+    +----+    +----+    +----+    +----+                   +----+
//
void CQueue::Add(wchar_t* str)
{
	NODE* Node;
	size_t n;

	Node = new NODE;

	n = wcslen(str) + 1;
	Node->str = new wchar_t[n];
	wcscpy_s(Node->str, n, str);

	Node->Next = NULL;

	if (IsEmpty())
	{
		First = Last = Node;
	}
	else
	{
		Last->Next = Node;
		Last = Node;
	}

	Count++;
}

//               remove
//         <-----------------
//      First                                                                 Last
//     +----+              +----+    +----+    +----+    +----+    +----+    +----+
//     |    |--->          |    |--->|    |--->|    |--->|    |--->|    |--->|    |---> NULL
//     +----+              +----+    +----+    +----+    +----+    +----+    +----+
//
void CQueue::Remove(wchar_t* str, size_t n)
{
	NODE* Node;

	Node = First;

	wcscpy_s(str, n, Node->str);

	First = First->Next;
	Count--;

	if (IsEmpty())
		Last = First;

	delete[] Node->str;
	delete Node;
}
