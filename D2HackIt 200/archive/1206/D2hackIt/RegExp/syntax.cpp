#include "syntax.h"

#ifdef USE_IN_DIABLO
	#include "D2HackIt.h"
	#ifdef SHOW_DEBUG_MSG
		#define __PRINTF	GameErrorf
	#else
		#define __PRINTF	Dummy
	#endif
#else
	#ifdef SHOW_DEBUG_MSG
		#define __PRINTF	printf
	#else
		#define __PRINTF	Dummy
	#endif
#endif

int Dummy(LPCSTR lpszFmt, ...)
{
	return 0;
}

char* CSyntax::GetAToken(char *p, SINGLE_TOKEN & token)
{
	token.type=TOKEN_TYPE_OP;

	switch(*p)
	{
	case '>': 
		if(p[1]=='=')
		{
			token.op=']';
			return p+2;
		}
		token.op='>';
		return p+1;
	case '<':
		if(p[1]=='=')
		{
			token.op='[';
			return p+2;
		}
		token.op='<';
		return p+1;
	case '&': 
		if(p[1]=='&')
		{
			token.op=',';
			return p+2;
		}
		break;
	case '!':
		if(p[1]=='=')
		{
			token.op='#';
			return p+2;
		}
		break;
	case '|':
		token.op='|';
		if(p[1]=='|')
		{
			return p+2;
		}
		break;
	case '=': 
		token.op='=';
		if(p[1]=='=')
		{
			return p+2;
		}
		break;
	case ')': 
	case '(': 
	case '*': 
	case '/': 
	case '+': 
	case '-': 
		token.op=*p;
		return p+1;
	}

	if(*p>='0'&&*p<='9')
	{
		token.type=TOKEN_TYPE_NUM;
		token.num=atoi(p);

		while(*p>='0'&&*p<='9')p++;
		return p;
	}
	else if (*p=='\0')
	{
		token.type=TOKEN_TYPE_END;
		return NULL;
	}
	else 
	{
		__PRINTF("bad character '%c'\n",*p);
		token.type=TOKEN_TYPE_BAD;
		m_bError = TRUE;
		return p;
	}
}



int CSyntax::GetOpPriority(char op)
{
	switch(op)
	{
	case '*': return 5;
	case '/': return 5;
	case '+': return 4;
	case '-': return 4;
	case '=': return 3;
	case '#': return 3;
	case ']': return 3;
	case '[': return 3;
	case '>': return 3;
	case '<': return 3;
	case ',': return 2;
	case '|': return 1;
	case '(': return 0;
	case ')': return 0;
	default: return -1;
	}
}



int CSyntax::Operate(int l, int r, char op)
{
	int res;
	switch(op)
	{
	case '*': 
		res= l*r;
		break;
	case '/':
		if(r)
		{
			res= l/r;
		}
		else
		{
			__PRINTF("ÿc1Error: Divided by zero.\n");
			m_bError = TRUE;
			res= 0;
		}
		break;
	case '+': 
		res= l+r;
		break;
	case '-': 
		res= l-r;
		break;
	case ']':
		res= (l>=r);
		//__PRINTF("%d>=%d =%d\n",l,r,res);
		return res;
	case '[': 
		res= (l<=r);
		//__PRINTF("%d<=%d =%d\n",l,r,res);
		return res;
	case '=': 
		res= (l==r);
		//__PRINTF("%d==%d =%d\n",l,r,res);
		return res;
	case '>': 
		res= (l>r);
		break;
	case '<': 
		res= (l<r);
		break;
	case '#':
		res= (l!=r);
		//__PRINTF("%d!=%d =%d\n",l,r,res);
		return res;
	case ',': 
		res= l&&r;
		//__PRINTF("%d&&%d =%d\n",l,r,res);
		return res;
	case '|': 
		res= l||r;
		//__PRINTF("%d||%d =%d\n",l,r,res);
		return res;
	default: 
		res= 0;
		break;
	}

	//__PRINTF("%d%c%d =%d\n",l,op,r,res);
	return res;
}



BOOL CSyntax::Parse(char *src, int& rResult)
{
	m_bError = FALSE;
	rResult = 0;
	
	RemoveBlanks(src);

	if(src[0]=='\0')
	{
		__PRINTF("empty expression\n");
		return FALSE;	
	}

	stack<char> op_stack;
	stack<int> num_stack;

	SINGLE_TOKEN token;
	char *p;
	while(!m_bError && (p=GetAToken(src,token))!=NULL)
	{
		switch(token.type)
		{
		case TOKEN_TYPE_BAD:
			return FALSE;
		case TOKEN_TYPE_END:
			break;
		case TOKEN_TYPE_OP:
			if(op_stack.empty())
			{
				op_stack.push(token.op);
				src=p;
			}
			else if(token.op=='(' || GetOpPriority(token.op)>GetOpPriority(op_stack.top()))
			{
				op_stack.push(token.op);
				src=p;
			}
			else
			{
				if(op_stack.empty()) return FALSE;

				if(op_stack.top()=='(')
				{
					op_stack.pop();
					if(token.op==')') src=p;
					break;
				}

				if(num_stack.empty()) return FALSE;

				int r=num_stack.top();
				num_stack.pop();
				if(num_stack.empty()) return FALSE;

				int l=num_stack.top();
				num_stack.pop();
				num_stack.push(Operate(l,r,op_stack.top()));
				op_stack.pop();
			}
			break;
		case TOKEN_TYPE_NUM:
			num_stack.push(token.num);
			src=p;
			break;
		}
	}

	while(!m_bError && !op_stack.empty())
	{
		if(op_stack.top()=='(')
		{
			op_stack.pop();
			continue;
		}

		if(num_stack.empty()) return FALSE;

		int r=num_stack.top();
		num_stack.pop();
		if(num_stack.empty()) return FALSE;

		int l=num_stack.top();
		num_stack.pop();
		num_stack.push(Operate(l,r,op_stack.top()));
		op_stack.pop();
	}

	if(num_stack.size()>1)	return FALSE;

	if (m_bError)
		return FALSE;

	rResult = num_stack.top();
	return TRUE;
}


void CSyntax::RemoveBlanks(char *str)
{
	char* pBlank = NULL;
	int i = 0;

	while(str[i]!='\0')
	{
		if ( pBlank && (str[i]!=' ' && str[i]!='\t') )
		{
			*pBlank = str[i];
			str[i] = ' ';
			pBlank = pBlank + sizeof(char);
		}
		else if (!pBlank && (str[i]==' ' || str[i]=='\t') )
		{
			pBlank = &str[i];
		}

		i++;
	}

	if(pBlank) *pBlank = '\0';
}