#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <windows.h>

using namespace std;

vector<unsigned int> divideAndConquerIntersection(const vector<vector<unsigned int>>& arrays, int start, int end) {
	if (start == end) {
		return arrays[start];
	}
	int mid = (start + end) / 2;
	vector<unsigned int> left = divideAndConquerIntersection(arrays, start, mid);
	vector<unsigned int> right = divideAndConquerIntersection(arrays, mid + 1, end);
	vector<unsigned int> result;
	size_t i = 0, j = 0;
	while (i < left.size() && j < right.size()) {
		if (left[i] < right[j]) {
			++i;
		}
		else if (left[i] > right[j]) {
			++j;
		}
		else {
			result.push_back(left[i]);
			++i;
			++j;
		}
	}
	return result;
}

int main() {
	cout << "ExpIndex������ģΪ1757" << endl;
	cout << "ExpIndex������ĵ�IDΪ25205174,��С�ĵ�IDΪ0" << endl;
	cout << "ExpQuery����ѯ��ģΪ1000" << endl;
	int QueryNum;
	cout << "�����������ģ(0-1000�ڵ�����)��" << endl;
	cin >> QueryNum;

	LARGE_INTEGER frequency;
	LARGE_INTEGER start, end, end_f;
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
		query.push_back(array);
	}
	readQuery.close();

	QueryPerformanceCounter(&end_f);
	interval = static_cast<double>(end_f.QuadPart - start.QuadPart) / frequency.QuadPart;
	cout << "��ȡ�ļ�ʱ��: " << interval << " ��" << endl;
	cout << "��ȡ�ļ��������" << endl;

	vector<vector<unsigned int>> results;
	for (int i = 0; i < QueryNum; i++) {
		if (query[i].empty()) {
			results.push_back({});
			continue;
		}

		vector<vector<unsigned int>> subsets;
		for (size_t j = 0; j < query[i].size(); j++) {
			subsets.push_back(index[query[i][j]]);
		}
		vector<unsigned int> result = divideAndConquerIntersection(subsets, 0, subsets.size() - 1);
		results.push_back(result);
	}

	QueryPerformanceCounter(&end);
	interval = static_cast<double>(end.QuadPart - end_f.QuadPart) / frequency.QuadPart;
	cout << "����ִ��ʱ��: " << interval << " ��" << endl;
	for (int i = 0; i < results.size(); i++)
	{
		cout << results[i].size() << endl;
	}
	return 0;
}
