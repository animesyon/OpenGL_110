
// Last In First Out Data Structure

#pragma once

typedef struct NODE_S NODE_S;

struct NODE_S
{
	wchar_t* str;
	NODE_S* Next;
};

class CStack
{
private:
	NODE_S* First;
	int Count;

public:
	CStack();
	~CStack();

	int GetCount();
	bool IsEmpty();
	void Push(wchar_t* str);
	void Pop(wchar_t* str, size_t n);

};
