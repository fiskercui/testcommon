#include <iostream>


void Swap(int &a , int &b)
{
	int c=a;
	a = b;
	b = c;
}

//冒泡排序
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


//直接插入排序（稳定）
void InsertSort(int* a, const int length)
{
	for(int i=1; i< length ; i++)	//假设0为有序区
	{
		const int key = a[i];
		int j= i-1;
		for(; j>=0&& a[j]>key; j--)//从后向前遍历是为了保证算法稳定
		{
			a[j+1] = a[j];
		}
		a[j+1] = key;
	}
}
//希尔排序（分组插入排序）上面希尔排序的步长选择都是从n/2开始
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

//直接选择排序  每次遍历找出最小， 第二小
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
