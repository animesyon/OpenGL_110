
// First In First Out Data Structure

#pragma once

typedef struct NODE NODE;

struct NODE
{
	wchar_t* str;
	NODE* Next;
};

class CQueue
{
private:
	NODE* First, * Last, * Cursor;
	int Count;

public:
	CQueue();
	~CQueue();

	int GetCount();
	bool IsEmpty();
	void Add(wchar_t* str);
	void Remove(wchar_t* str, size_t n);
};
