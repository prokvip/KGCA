#include <exception>
#include <iostream>
#include <string>
using namespace std;
class Exception {
	int  iLine;
	char filepath[1024] = { 0, };
	char msg[1024] = { 0, };
public:
	Exception() {
	};
	Exception(const char* const& message) {
		string data ;
		data += __FILE__;		
		data += std::to_string(__LINE__);
		data += message;
		strcpy_s(msg, data.c_str());
	};
	virtual ~Exception() {};
	virtual const char* what() const { return msg; }
};
class Temp { public: int kkkk = 23; };
class parent {
public :
	int* pParentData = nullptr;
	void   Work() throw(int);
	virtual const int* get() {
		return nullptr;
	};
	parent()
	{
		pParentData = new int;
		*pParentData = 10;
	}
	~parent()
	{
		delete pParentData;
		pParentData = nullptr;
	}
};
void   parent::Work() throw(float)
{
	string data;
	data += __FILE__;
	data += std::to_string(__LINE__);
	data += "Work";
	Exception exc(data.c_str());
	throw exc;
}
class child : public parent{
public:
	const int* get()
	{
		return nullptr;
	};
	int* pChildData = nullptr;
	child()
	{
		pChildData = new int;
		*pChildData = 100;
	}
	~child()
	{
		Release();
	}
	void Release()
	{
		if( pChildData != nullptr)
			delete pChildData;
		pChildData = nullptr;
	}
};
child* aFun(child* b)
{
	child* c = b;
	return c;
}
void main()
{	
	//: error C2440: '초기화 중': 'parent *'에서 'child *'(으)로 변환할 수 없습니다.
	parent* pParent = static_cast<parent*>(new child);
	delete pParent;
	pParent = nullptr;
}