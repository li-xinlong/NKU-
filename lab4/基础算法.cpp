//#include <iostream>
//#include <string>
//#include <fstream>
//#include<sstream>
//#include<vector>
//#include <algorithm> 
//#include <bitset>
//#include <Windows.h>
//#include <emmintrin.h>  //SSE2
//#define idlength 25205248
//using namespace std;
//int main()
//{
//	cout << "ExpIndex������ģΪ1757" << endl;
//	cout << "ExpIndex������ĵ�IDΪ25205174,��С�ĵ�IDΪ0" << endl;
//	cout << "ExpQuery����ѯ��ģΪ1000" << endl;
//	int QueryNum;
//	cout << "�����������ģ(0-1000�ڵ�����)��" << endl;
//
//	cin >> QueryNum;
//	LARGE_INTEGER frequency;
//
//	LARGE_INTEGER start;
//
//	LARGE_INTEGER end;
//	LARGE_INTEGER end_f;
//	double interval;
//	// ��ȡ���ܼ�����Ƶ��
//	QueryPerformanceFrequency(&frequency);
//	// ��ʼ��ʱ
//	QueryPerformanceCounter(&start);
//	ifstream readIndex("ExpIndex", ios::binary | ios::in);
//	if (!readIndex) {
//		cerr << "�޷���ExpIndex�ļ�" << endl;
//		return 1;
//	}
//	vector<vector<unsigned int>>index;
//	for (unsigned int i = 0; !readIndex.eof(); ++i) {
//		unsigned int n; // ����ĳ���
//		readIndex.read(reinterpret_cast<char*>(&n), sizeof(n)); // ��ȡ���鳤��
//		vector<unsigned int> array; // ����һ���������洢����Ԫ��
//		unsigned int a;
//		for (unsigned int j = 0; j < n; j++)
//		{
//			readIndex.read(reinterpret_cast<char*>(&a), sizeof(a));
//			array.push_back(a);
//		}
//		if (array.size() != n)
//		{
//			cerr << "��ȡExpIndex��" << i << "����ʧ��" << endl;
//		}
//		sort(array.begin(), array.end());
//		index.push_back(array);
//	}
//	// �ر��ļ���
//	readIndex.close();
//	ifstream readQuery("ExpQuery", ios::in);
//	if (!readQuery) {
//		cerr << "�޷���ExpQuery�ļ�" << endl;
//		return 1;
//	}
//	vector<vector<int>>query;
//	string line;
//	while (getline(readQuery, line))
//	{
//		vector<int> array; // ����һ���������洢����Ԫ��
//		stringstream readline(line);
//		int a;
//		while (readline >> a)
//		{
//			array.push_back(a);
//		}
//		sort(array.begin(), array.end());
//		query.push_back(array);
//	}
//	// �ر��ļ���
//	readQuery.close();
//	QueryPerformanceCounter(&end_f);
//	interval = static_cast<double>(end_f.QuadPart - start.QuadPart) / frequency.QuadPart;
//
//	std::cout << "��ȡ�ļ�ʱ��: " << interval << " ��" << std::endl;
//	cout << "��ȡ�ļ��������" << endl;
//
//	//cout << "��������ܳ���" << index.size() << endl;
//	vector<vector<unsigned int>>results;
//	for (int i = 0; i < QueryNum; i++)
//	{
//		int num = query[i].size();
//		vector<unsigned int>result;
//		bitset<idlength>* bits = new bitset<idlength>[num];
//		for (int j = 0; j < num; j++)
//		{
//			for (int k = 0; k < index[query[i][j]].size(); k++)
//			{
//				bits[j].set(index[query[i][j]][k]);
//			}
//		}
//		for (int j = 1; j < num; j++)
//		{
//			for (int k = 0; k < idlength / 128; k++)
//			{
//				int* data1 = (int*)&bits[0];
//				int* add1 = (int*)(data1 + 4 * k);
//				__m128i xmm_data1 = _mm_loadu_si128((__m128i*)add1);
//				int* data2 = (int*)&bits[j];
//				int* add2 = (int*)(data2 + 4 * k);
//				__m128i xmm_data2 = _mm_loadu_si128((__m128i*)add2);
//				xmm_data1 = _mm_and_si128(xmm_data1, xmm_data2);
//				_mm_storeu_si128((__m128i*)add1, xmm_data1);
//			}
//		}
//		int* address = (int*)&bits[0];
//		for (int j = 0; j < idlength / 128; j++)
//		{
//			bitset<128>* temp = (bitset<128>*)(address + j * 4);
//			if (*temp != 0)
//			{
//				for (int k = j * 128; k < 128 * (j + 1); k++)
//				{
//					if (bits[0][k])
//					{
//						result.push_back(k);
//					}
//				}
//			}
//		}
//		results.push_back(result);
//	}
//	//������ʱ
//	QueryPerformanceCounter(&end);
//	// ����ʱ����
//	interval = static_cast<double>(end.QuadPart - start.QuadPart) / frequency.QuadPart;
//	std::cout << "����ִ��ʱ��: " << interval << " ��" << std::endl;
//	return 0;
//}

#include<iostream>
#include <string>
#include <fstream>
#include<sstream>
#include<vector>
#include <algorithm> 
#include <Windows.h>
using namespace std;
bool Find(vector<unsigned int>A, unsigned int  a)
{
	for (int i = 0; i < A.size() && A[i] <= a; i++)
	{
		if (A[i] == a)
		{
			return true;
		}
	}
	return false;
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
	unsigned int n; // ����ĳ���
	int i = 0;
	while (readIndex.read(reinterpret_cast<char*>(&n), sizeof(n))) {
		vector<unsigned int> array(n); // ����һ���������洢����Ԫ��
		if (!readIndex.read(reinterpret_cast<char*>(array.data()), n * sizeof(unsigned int)))
		{
			cerr << "��ȡExpIndex��" << i << "����ʧ��" << endl;
		}
		sort(array.begin(), array.end());
		index.push_back(array);
		i++;
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
		for (int j = 0; j < index[query[i][0]].size(); j++)
		{
			bool bo = true;
			unsigned int a = index[query[i][0]][j];
			for (int k = 1; k < num; k++)
			{
				if (!Find(index[query[i][k]], a))
				{
					bo = false;
					break;
				}
			}
			if (bo)
			{
				result.push_back(a);
			}
		}

		results.push_back(result);
	}
	// ������ʱ
	QueryPerformanceCounter(&end);
	// ����ʱ����
	interval = static_cast<double>(end.QuadPart - start.QuadPart) / frequency.QuadPart;

	std::cout << "����ִ��ʱ��: " << interval << " ��" << std::endl;
	return 0;
}