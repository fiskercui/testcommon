#include <iostream>


void Swap(int &a , int &b)
{
	int c=a;
	a = b;
	b = c;
}

//ð������
void BubbleSort(int* a, const int length)
{
	for(int i=0; i<length; i++)
	{
		for(int j = 1; j < length-i; j++)
		{
			if(a[j-1]>a[j])
				Swap(a[j-1], a[j]);
		}
	}
}


//ֱ�Ӳ��������ȶ���
void InsertSort(int* a, const int length)
{
	for(int i=1; i< length ; i++)	//����0Ϊ������
	{
		const int key = a[i];
		int j= i-1;
		for(; j>=0&& a[j]>key; j--)//�Ӻ���ǰ������Ϊ�˱�֤�㷨�ȶ�
		{
			a[j+1] = a[j];
		}
		a[j+1] = key;
	}
}
//ϣ�����򣨷��������������ϣ������Ĳ���ѡ���Ǵ�n/2��ʼ
void ShellSort(int * a, const int length)
{
	for(int gap = length/2; gap >0  ; gap = gap/2 )
	{
		for(int j=gap; j <length; j++)
		{
			const int key = a[j];
			int k = j-gap;
			for(; k>=0 &&a[k]>key ; k-=gap)
			{
				a[k+gap] = a[k];
			}
			a[k+gap] = key;
		}
	}
}

//ֱ��ѡ������  ÿ�α����ҳ���С�� �ڶ�С
void SelectSort(int* a, const int length)
{
	for (int i=0; i <length; i++)//n
	{
		for(int j=i+1; j<length; j++ )//n-1
		{
			if(a[j]<a[i])//
			{
				int temp = a[i];
				a[i] = a[j];
				a[j] = temp;
			}
		}
	}
}
//;
void PrintArray(const int *a, const int length)
{
	std::cout << "Array:";
	for(int i=0; i<length; i++)
	{
		std::cout << a[i] << " ";
	}
	std::cout << std::endl;
}
