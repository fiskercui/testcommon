// Test2010.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <map>
#include <vector>

using namespace std;

struct stTask
{
	int m_a;
	int m_b;
};


#include <malloc.h>
#include <stdio.h>
/*Ê®ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ä½á¹¹ï¿½ï¿½ï¿½Í¶ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Â£ï¿½*/
typedef struct OLNode
{
	int row,col; /*ï¿½ï¿½ï¿½ï¿½Ôªï¿½Øµï¿½ï¿½Ðºï¿½ï¿½ï¿½ï¿½Â±ï¿½*/
	int value;
	struct OLNode *right; /*ï¿½ï¿½ï¿½ï¿½Ôªï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ð±í¡¢ï¿½Ð±ï¿½ï¿½Äºï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½*/
	struct OLNode *down;
}OLNode, *OLink;
typedef struct
{
	OLink *row_head; /*ï¿½Ð¡ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Í·Ö¸ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½*/
	OLink *col_head;
	int m,n,len; /*Ï¡ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ôªï¿½ØµÄ¸ï¿½ï¿½ï¿½*/
}CrossList;
/*ï¿½ï¿½ï¿½ï¿½Ï¡ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ê®ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ã·¨*/
void CreateCrossList(CrossList *M)
{
	int m, n, t, i, j, e;
	OLNode* p;
	OLNode* q;
	/*ï¿½ï¿½ï¿½ï¿½Ê®ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½æ´¢ï¿½á¹¹ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ï¡ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½M*/
	printf("input the rows, cols, nozeros");
	scanf("%d%d%d", &m,&n,&t); /*ï¿½ï¿½ï¿½ï¿½Mï¿½ï¿½ï¿½ï¿½ï¿½ï¿½,ï¿½ï¿½ï¿½ï¿½ï¿½Í·ï¿½ï¿½ï¿½Ôªï¿½ØµÄ¸ï¿½ï¿½ï¿½*/
	M->m=m;
	M->n=n;
	M->len=t;
	if(!(M->row_head=(OLink *)malloc(m*sizeof(OLink))))
		exit(OVERFLOW);
	if(!(M->col_head=(OLink * )malloc(n*sizeof(OLink))))
		exit(OVERFLOW);
	/*ï¿½ï¿½Ê¼ï¿½ï¿½ï¿½Ð¡ï¿½ï¿½ï¿½Í·Ö¸ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ð¡ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Îªï¿½Õµï¿½ï¿½ï¿½ï¿½ï¿½*/
	for(int h=0; h<m+1; h++)
	{
		M->row_head[h] = NULL;
	}
	for(int t=0; t<n+1; t++)
	{
		M->col_head[t] = NULL;
	}
	printf("input the row, col, 1");
	for(scanf("%d%d%d", &i,&j,&e);e!=0;scanf("%d%d%d", &i,&j,&e))
	{
		if(!(p=(OLNode *)malloc(sizeof(OLNode))))
			exit(OVERFLOW);
		p->row=i;
		p->col=j;
		p->value=e; /*ï¿½ï¿½ï¿½É½ï¿½ï¿½ï¿½*/
		if(M->row_head[i]==NULL)
		{
			M->row_head[i]=p;
			p->right=NULL;
		}
		else
		{
			/*Ñ°ï¿½ï¿½ï¿½Ð±ï¿½ï¿½ÐµÄ²ï¿½ï¿½ï¿½Î»ï¿½ï¿½*/
			for(q=M->row_head[i];q->right&&q->right->col<j;q=q->right); /*ï¿½ï¿½Ñ­ï¿½ï¿½ï¿½ï¿½*/
			p->right=q->right;
			q->right=p; /*ï¿½ï¿½ï¿½É²ï¿½ï¿½ï¿½*/
		}
		if(M->col_head[j]==NULL)
		{
			M->col_head[j]=p;
			p->down=NULL;
		}
		else
		{
			/*Ñ°ï¿½ï¿½ï¿½Ð±ï¿½ï¿½ÐµÄ²ï¿½ï¿½ï¿½Î»ï¿½ï¿½*/
			for(q=M->col_head[j];q->down&&q->down->row<i;q=q->down); /*ï¿½ï¿½Ñ­ï¿½ï¿½ï¿½ï¿½*/
			p->down=q->down;
			q->down=p; /*ï¿½ï¿½ï¿½É²ï¿½ï¿½ï¿½*/
		}
	}
}


void testCrossLink()
{
	CrossList* pCrossList = (CrossList* )malloc(sizeof(CrossList));
	CreateCrossList(pCrossList);
	std::map<int, stTask> mpTask;
	mpTask.insert(pair<int, stTask>(1, stTask()));

	std::map<int, stTask>::iterator it;
	it = mpTask.find(1);
	if(it != mpTask.end())
	{
		stTask& st = it->second;
		st.m_b = 1;
		it->second.m_a = 1;
	}
}



class cBase
{
public:
	cBase(){}
	virtual ~cBase() {}
	void TestPrint()
	{
		printf("cBase TestPrintf\n");
	}
private:
	int mum;
};


class cDevided: public cBase
{
public :
	cDevided(){}

	~cDevided() {}
	void TestPrint()
	{
		printf("cDevided TestPrint\n");
	}
};


#include <time.h> //Í·ÎÄ¼þ 
#include <windows.h>
void printTime()
{
	//time_t now; // ±äÁ¿ÉùÃ÷ 
	//time(&now); // È¡µÃÏÖÔÚµÄÈÕÆÚÊ±¼ä 
	//printf("%s",ctime(&now)); // ´òÓ¡ÏÖÔÚµÄÊ±¼ä 

	DWORD take = GetTickCount();   
	//ÔÚ´Ë´¦Ìí¼Ó´úÂë 
	printf("\n%ld\n", GetTickCount() - take);
	//exit(0);
}


void printTimePass(DWORD take)
{ 
	printf("\n%ld\n", GetTickCount() - take);
}

void socketClient( void );
int testMina();

int _tmain(int argc, _TCHAR* argv[])
{
	//cDevided* devided= new cDevided();
	//devided->TestPrint();
	//cBase* base = devided;
	//cDevided* devided2 = dynamic_cast<cDevided*>(base);

	//printf("cDevided2 Address:%x\n", devided2);
	//printf("cDevided Address:%x\n", devided);
	//printf("cBase Address:%x\n", base);
	printTime();
	DWORD take = GetTickCount();   
	//testMina();
	socketClient();
	printTimePass(take);
	printTime();
	return 0;
}


