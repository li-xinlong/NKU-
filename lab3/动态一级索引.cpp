#include <iostream>
#include <string>
#include <fstream>
#include<sstream>
#include<vector>
#include <algorithm> 
#include <bitset>
#include <Windows.h>
#include <emmintrin.h>  //SSE2
#include <pthread.h>

#pragma comment(lib, "pthreadVC2.lib")
using namespace std;
#define idlength 25205248
struct package {
	bitset<idlength> first; //һ������
	vector<unsigned int>index;
};
void* tobits(void* parm)
{
	package* pack = (package*)parm;
	for (int i = 0; i < pack->index.size(); i++)
	{
		pack->first.set(pack->index[i]);//ת��Ϊһ������
	}
	return NULL;
}
int main()
{
	cout << "ExpIndex������ģΪ1757" << endl;
	cout << "ExpIndex������ĵ�IDΪ25205174,��С�ĵ�IDΪ0" << endl;
	cout << "ExpQuery����ѯ��ģΪ1000" << endl;
	int QueryNum;
	cout << "�����������ģ(0-1000�ڵ�����)��" << endl;

	cin >> QueryNum;
	LARGE_INTEGER frequency;

	LARGE_INTEGER start;

	LARGE_INTEGER end;
	LARGE_INTEGER end_f;
	double interval;
	// ��ȡ���ܼ�����Ƶ��
	QueryPerformanceFrequency(&frequency);
	// ��ʼ��ʱ
	QueryPerformanceCounter(&start);
	ifstream readIndex("ExpIndex", ios::binary | ios::in);
	if (!readIndex) {
		cerr << "�޷���ExpIndex�ļ�" << endl;
		return 1;
	}
	vector<vector<unsigned int>>index;
	for (unsigned int i = 0; !readIndex.eof(); ++i) {
		unsigned int n; // ����ĳ���
		readIndex.read(reinterpret_cast<char*>(&n), sizeof(n)); // ��ȡ���鳤��
		vector<unsigned int> array; // ����һ���������洢����Ԫ��
		unsigned int a;
		for (unsigned int j = 0; j < n; j++)
		{
			readIndex.read(reinterpret_cast<char*>(&a), sizeof(a));
			array.push_back(a);
		}
		if (array.size() != n)
		{
			cerr << "��ȡExpIndex��" << i << "����ʧ��" << endl;
		}
		sort(array.begin(), array.end());
		index.push_back(array);
	}
	// �ر��ļ���
	readIndex.close();
	ifstream readQuery("ExpQuery", ios::in);
	if (!readQuery) {
		cerr << "�޷���ExpQuery�ļ�" << endl;
		return 1;
	}
	vector<vector<int>>query;
	string line;
	while (getline(readQuery, line))
	{
		vector<int> array; // ����һ���������洢����Ԫ��
		stringstream readline(line);
		int a;
		while (readline >> a)
		{
			array.push_back(a);
		}
		sort(array.begin(), array.end());
		query.push_back(array);
	}
	// �ر��ļ���
	readQuery.close();
	QueryPerformanceCounter(&end_f);
	interval = static_cast<double>(end_f.QuadPart - start.QuadPart) / frequency.QuadPart;

	std::cout << "��ȡ�ļ�ʱ��: " << interval << " ��" << std::endl;
	cout << "��ȡ�ļ��������" << endl;
	vector<vector<unsigned int>>results;
	for (int i = 0; i < QueryNum; i++)
	{
		int num = query[i].size();
		vector<unsigned int>result;
		package* packages = new package[num];
		for (int j = 0; j < num; j++)
		{
			packages[j].index = index[query[i][j]];
		}
		pthread_t* threads = new pthread_t[num];
		for (int j = 0; j < num; j++)
		{
			pthread_create(&threads[j], NULL, tobits, (void*)&packages[j]);
		}
		for (int j = 0; j < num; j++)
		{
			pthread_join(threads[j], NULL);
		}
		for (int j = 1; j < num; j++)
		{
			for (int k = 0; k < idlength / 128; k++)
			{
				int* data1 = (int*)&packages[0].first;
				int* add1 = (int*)(data1 + 4 * k);
				__m128i xmm_data1 = _mm_loadu_si128((__m128i*)add1);
				int* data2 = (int*)&packages[j].first;
				int* add2 = (int*)(data2 + 4 * k);
				__m128i xmm_data2 = _mm_loadu_si128((__m128i*)add2);
				xmm_data1 = _mm_and_si128(xmm_data1, xmm_data2);
				_mm_storeu_si128((__m128i*)add1, xmm_data1);
			}

		}
		int* address = (int*)&packages[0].first;
		for (int j = 0; j < idlength / 128; j++)
		{
			bitset<128>* temp = (bitset<128>*)(address + j * 4);
			if (*temp != 0)
			{
				for (int k = j * 128; k < 128 * (j + 1); k++)
				{
					if (packages[0].first[k])
					{
						result.push_back(k);
					}
				}
			}
		}
		results.push_back(result);
	}
	//������ʱ
	QueryPerformanceCounter(&end);
	// ����ʱ����
	interval = static_cast<double>(end.QuadPart - start.QuadPart) / frequency.QuadPart;
	std::cout << "����ִ��ʱ��: " << interval << " ��" << std::endl;
	return 0;
}