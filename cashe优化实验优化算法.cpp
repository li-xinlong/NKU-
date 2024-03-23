
#include <iostream>
#include<windows.h>
using namespace std;

int main()
{
	int n = 0;
	cout << "�����������ģ��";
	cin >> n;
	int** Arr = new int* [n];
	for (int i = 0; i < n; i++)
	{
		Arr[i] = new int[n];
	}
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			Arr[i][j] = i + j;
		}
	}
	int* test = new int[n];
	int* sum = new int[n];
	for (int i = 0; i < n; i++)
	{
		test[i] = i;
		sum[i] = 0;
	}
	//���з��ʾ���Ԫ�أ�һ�����ѭ�����ڴ�ѭ��һ������ִ�У������һ���ڻ����
	LARGE_INTEGER frequency; // ���ڻ�ȡ��ʱ����Ƶ��
	LARGE_INTEGER start_time; // ���ڴ洢��ʼʱ��
	LARGE_INTEGER end_time;   // ���ڴ洢����ʱ��
	double timesum = 0;
	for (int k = 0; k < 10; k++)
	{
		int count = 1000;
		QueryPerformanceFrequency(&frequency);
		QueryPerformanceCounter(&start_time);
		while (count)
		{
			for (int j = 0; j < n; j++)
			{
				for (int i = 0; i < n; i++)
				{
					sum[i] += Arr[j][i] * test[j];
				}
			}
			count--;
		}
		QueryPerformanceCounter(&end_time);
		double time = static_cast<double>(end_time.QuadPart - start_time.QuadPart) / frequency.QuadPart;
		timesum += time;
	}
	cout << "����ʱ�䣺" << timesum / 10;
	return 0;
}

