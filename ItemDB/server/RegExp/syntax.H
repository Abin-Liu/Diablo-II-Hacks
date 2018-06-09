#include <stdio.h>
#include <stdlib.h> 
#include <windows.h>

#pragma warning( disable : 4786 )
#include <stack>
#include <map>
#include <string>

using namespace std;

#define TOKEN_TYPE_BAD -1
#define TOKEN_TYPE_END 0
#define TOKEN_TYPE_OP 1
#define TOKEN_TYPE_NUM 2

struct SINGLE_TOKEN
{
	int type;
	union
	{
	int num;
	char op;
	};
};


class CSyntax
{
	
public:
	BOOL Parse(char *src, int& rResult);

private:
	static std::map<std::string,DWORD> _mod_def;
	int Operate(int l,int r,char op);
	char* GetAToken(char *p, SINGLE_TOKEN & token); 
	int GetOpPriority(char op);
	void RemoveBlanks(char *str);
	BOOL m_bError;
};

