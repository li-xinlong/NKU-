
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