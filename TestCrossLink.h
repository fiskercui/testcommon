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
/*ʮ�������Ľṹ���Ͷ������£�*/
typedef struct OLNode
{
	int row,col; /*����Ԫ�ص��к����±�*/
	int value;
	struct OLNode *right; /*����Ԫ�������б��б��ĺ�������*/
	struct OLNode *down;
}OLNode, *OLink;
typedef struct
{
	OLink *row_head; /*�С���������ͷָ������*/
	OLink *col_head;
	int m,n,len; /*ϡ������������������������Ԫ�صĸ���*/
}CrossList;
/*����ϡ��������ʮ���������㷨*/
void CreateCrossList(CrossList *M)
{
	int m, n, t, i, j, e;
	OLNode* p;
	OLNode* q;
	/*����ʮ�������洢�ṹ������ϡ������M*/
	printf("input the rows, cols, nozeros");
	scanf("%d%d%d", &m,&n,&t); /*����M������,�����ͷ���Ԫ�صĸ���*/
	M->m=m;
	M->n=n;
	M->len=t;
	if(!(M->row_head=(OLink *)malloc(m*sizeof(OLink))))
		exit(OVERFLOW);
	if(!(M->col_head=(OLink * )malloc(n*sizeof(OLink))))
		exit(OVERFLOW);
	/*��ʼ���С���ͷָ�����������С�������Ϊ�յ�����*/
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
		p->value=e; /*���ɽ���*/
		if(M->row_head[i]==NULL)
		{
			M->row_head[i]=p;
			p->right=NULL;
		}
		else
		{
			/*Ѱ���б��еĲ���λ��*/
			for(q=M->row_head[i];q->right&&q->right->col<j;q=q->right); /*��ѭ����*/
			p->right=q->right;
			q->right=p; /*���ɲ���*/
		}
		if(M->col_head[j]==NULL)
		{
			M->col_head[j]=p;
			p->down=NULL;
		}
		else
		{
			/*Ѱ���б��еĲ���λ��*/
			for(q=M->col_head[j];q->down&&q->down->row<i;q=q->down); /*��ѭ����*/
			p->down=q->down;
			q->down=p; /*���ɲ���*/
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


#include <time.h> //ͷ�ļ� 
#include <windows.h>
void printTime()
{
	//time_t now; // �������� 
	//time(&now); // ȡ�����ڵ�����ʱ�� 
	//printf("%s",ctime(&now)); // ��ӡ���ڵ�ʱ�� 

	DWORD take = GetTickCount();   
	//�ڴ˴���Ӵ��� 
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


