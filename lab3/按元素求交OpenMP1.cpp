#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <omp.h> // ���� OpenMP ͷ�ļ�
#include <Windows.h>
using namespace std;

struct ThreadData {
	vector<vector<unsigned int>>* index;
	vector<vector<int>>* query;
	vector<vector<unsigned int>>* results;
	int query_index;
};

bool Find(vector<unsigned int>& A, unsigned int a) {
	for (size_t i = 0; i < A.size() && A[i] <= a; i++) {
		if (A[i] == a) {
			return true;
		}
	}
	return false;
}

void processQuery(ThreadData* data) {
	int query_index = data->query_index;
	vector<vector<unsigned int>>& index = *(data->index);
	vector<vector<int>>& query = *(data->query);
	vector<vector<unsigned int>>& results = *(data->results);

	int num = query[query_index].size();
	vector<unsigned int> result;
	for (size_t j = 0; j < index[query[query_index][0]].size(); j++) {
		bool bo = true;
		unsigned int a = index[query[query_index][0]][j];
		for (int k = 1; k < num; k++) {
			if (!Find(index[query[query_index][k]], a)) {
				bo = false;
				break;
			}
		}
		if (bo) {
			result.push_back(a);
		}
	}

	results[query_index] = result;
}

int main() {
	cout << "ExpIndex������ģΪ1757" << endl;
	cout << "ExpIndex������ĵ�IDΪ25205174,��С�ĵ�IDΪ0" << endl;
	cout << "ExpQuery����ѯ��ģΪ1000" << endl;
	int QueryNum;
	cout << "�����������ģ(0-1000�ڵ�����)��" << endl;

	cin >> QueryNum;
	LARGE_INTEGER frequency;
	LARGE_INTEGER start, end;

	// ��ȡ���ܼ�����Ƶ��
	QueryPerformanceFrequency(&frequency);
	// ��ʼ��ʱ
	QueryPerformanceCounter(&start);
	ifstream readIndex("ExpIndex", ios::binary | ios::in);
	if (!readIndex) {
		cerr << "�޷���ExpIndex�ļ�" << endl;
		return 1;
	}
	vector<vector<unsigned int>> index;
	unsigned int n;
	while (readIndex.read(reinterpret_cast<char*>(&n), sizeof(n))) {
		vector<unsigned int> array(n);
		if (!readIndex.read(reinterpret_cast<char*>(array.data()), n * sizeof(unsigned int))) {
			cerr << "��ȡExpIndex����ʧ��" << endl;
		}
		sort(array.begin(), array.end());
		index.push_back(array);
	}
	readIndex.close();

	ifstream readQuery("ExpQuery", ios::in);
	if (!readQuery) {
		cerr << "�޷���ExpQuery�ļ�" << endl;
		return 1;
	}
	vector<vector<int>> query;
	string line;
	while (getline(readQuery, line)) {
		vector<int> array;
		stringstream readline(line);
		int a;
		while (readline >> a) {
			array.push_back(a);
		}
		sort(array.begin(), array.end());
		query.push_back(array);
	}
	readQuery.close();
	// ������ʱ
	QueryPerformanceCounter(&end);
	double readTime = static_cast<double>(end.QuadPart - start.QuadPart) / frequency.QuadPart;
	cout << "��ȡ�ļ�ʱ��: " << readTime << " ��" << endl;
	cout << "��ȡ�ļ��������" << endl;
	// ��ʼ���������
	vector<vector<unsigned int>> results(QueryNum);

	// ʹ�� OpenMP ���л������ѯ
#pragma omp parallel for
	for (int i = 0; i < QueryNum; ++i) {
		ThreadData thread_data;
		thread_data.index = &index;
		thread_data.query = &query;
		thread_data.results = &results;
		thread_data.query_index = i;
		processQuery(&thread_data);
	}

	// ������
	// ����ִ��ʱ��
	QueryPerformanceCounter(&end);
	double execTime = static_cast<double>(end.QuadPart - start.QuadPart) / frequency.QuadPart;
	cout << "����ִ��ʱ��: " << execTime << " ��" << endl;

	return 0;
}
