#include <iostream>
#include <string>
#include <fstream>
#include<sstream>
#include<vector>
#include <algorithm> 
#include <bitset>
#include <Windows.h>
#include <emmintrin.h>  //SSE2

#pragma comment(lib, "pthreadVC2.lib")
using namespace std;
#define idlength 25214976
struct package {
	bitset<idlength> first; //һ������
	bitset<idlength / 128>second;
	vector<unsigned int>index;
};

void* tobits(void* parm)
{
	package* pack = (package*)parm;
	for (int i = 0; i < pack->index.size(); i++)
	{
		pack->first.set(pack->index[i]);//ת��Ϊһ������
	}
	long* temp = (long*)&pack->first;
	for (int i = 0; i < idlength / 128; i++)
	{
		bitset<128>* bitset_temp = (bitset<128>*)(temp + i * 2);
		if (*bitset_temp == 1)
		{
			pack->second.set(i);
		}
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
#pragma omp parallel for
		for (int j = 0; j < num; j++)
		{
			packages[j].index = index[query[i][j]];
		}
#pragma omp parallel for
		for (int j = 0; j < num; j++) {
			tobits(&packages[j]);
		}
		for (int j = 1; j < num; j++)
		{
			for (int k = 0; k < idlength / 128 / 128; k++)
			{
				int* data_second1 = (int*)&packages[0].second;//ÿ������32λ��128λ8������
				// ʹ�� sse2 �������ݵ��Ĵ���
				int* add_second1 = (int*)(data_second1 + 4 * k);
				__m128i xmm_data_second1 = _mm_loadu_si128((__m128i*)add_second1);
				// ����һ���������ڴ洢�ڶ���λ��������
				int* data_second2 = (int*)&packages[j].second;//ÿ������32λ��128λ8������
				int* add_second2 = (int*)(data_second2 + 4 * k);
				// ʹ�� sse2 �������ݵ��ڶ����Ĵ���
				__m128i xmm_data_second2 = _mm_loadu_si128((__m128i*)add_second2);
				xmm_data_second1 = _mm_and_si128(xmm_data_second1, xmm_data_second2);
				_mm_storeu_si128((__m128i*)add_second1, xmm_data_second1);
				for (int m = k * 128; m < 128 * (k + 1); m++) {
					if (packages[j].second[m])
					{
						int* data1 = (int*)&packages[0].first;//ÿ������32λ��128λ8������
						// ʹ�� sse2 �������ݵ��Ĵ���
						int* add1 = (int*)(data1 + 4 * m);
						__m128i xmm_data1 = _mm_loadu_si128((__m128i*)add1);
						// ����һ���������ڴ洢�ڶ���λ��������
						int* data2 = (int*)&packages[j].first;//ÿ������32λ��128λ8������
						int* add2 = (int*)(data2 + 4 * m);
						// ʹ�� sse2 �������ݵ��ڶ����Ĵ���
						__m128i xmm_data2 = _mm_loadu_si128((__m128i*)add2);
						xmm_data1 = _mm_and_si128(xmm_data1, xmm_data2);
						_mm_storeu_si128((__m128i*)add1, xmm_data1);
					}
					else
					{
						int* data1 = (int*)&packages[0].first;//ÿ������32λ��128λ8������
						int* add1 = (int*)(data1 + 4 * m);
						__m128i zero_vector = _mm_setzero_si128();
						_mm_storeu_si128((__m128i*)add1, zero_vector);
					}

				}
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