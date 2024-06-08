#include <mpi.h>
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
int main(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);

	int world_size;
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
	int QueryNum = 48;
	int world_rank;
	int part = QueryNum / world_size;
	MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

	LARGE_INTEGER frequency;

	LARGE_INTEGER start;
	LARGE_INTEGER end;
	LARGE_INTEGER end_f;
	double interval;
	// ��ȡ���ܼ�����Ƶ��
	vector<vector<unsigned int>> index;
	vector<vector<int>> query;
	QueryPerformanceFrequency(&frequency);
	// ��ʼ��ʱ
	QueryPerformanceCounter(&start);
	if (world_rank == 0) {
		ifstream readIndex("ExpIndex", ios::binary | ios::in);
		if (!readIndex) {
			cerr << "�޷���ExpIndex�ļ�" << endl;
			MPI_Abort(MPI_COMM_WORLD, 1);
			MPI_Finalize();
			return 1;
		}

		for (unsigned int i = 0; !readIndex.eof(); ++i) {
			unsigned int n; // ����ĳ���
			readIndex.read(reinterpret_cast<char*>(&n), sizeof(n)); // ��ȡ���鳤��
			vector<unsigned int> array; // ����һ���������洢����Ԫ��
			unsigned int a;
			for (unsigned int j = 0; j < n; j++) {
				readIndex.read(reinterpret_cast<char*>(&a), sizeof(a));
				array.push_back(a);
			}
			if (array.size() != n) {
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
			MPI_Abort(MPI_COMM_WORLD, 1);
			MPI_Finalize();
			return 1;
		}

		string line;
		while (getline(readQuery, line)) {
			vector<int> array; // ����һ���������洢����Ԫ��
			stringstream readline(line);
			int a;
			while (readline >> a) {
				array.push_back(a);
			}
			sort(array.begin(), array.end());
			query.push_back(array);
		}
		// �ر��ļ���
		readQuery.close();
		if (world_rank == 0) {
			cout << "ExpIndex������ģΪ1757" << endl;
			cout << "ExpIndex������ĵ�IDΪ25205174,��С�ĵ�IDΪ0" << endl;
			cout << "ExpQuery����ѯ��ģΪ1000" << endl;
			QueryPerformanceCounter(&end_f);
			interval = static_cast<double>(end_f.QuadPart - start.QuadPart) / frequency.QuadPart;
			cout << "��ȡ�ļ�ʱ��: " << interval << " ��" << endl;
			cout << "��ȡ�ļ��������" << endl;
		}
	}

	int index_size = index.size();
	MPI_Bcast(&index_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
	if (world_rank != 0) {
		index.resize(index_size);
	}
	for (int i = 0; i < index_size; ++i) {
		int array_size = (world_rank == 0) ? index[i].size() : 0;
		MPI_Bcast(&array_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
		if (world_rank != 0) {
			index[i].resize(array_size);
		}
		MPI_Bcast(index[i].data(), array_size, MPI_UNSIGNED, 0, MPI_COMM_WORLD);
	}

	int query_size = query.size();
	MPI_Bcast(&query_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
	if (world_rank != 0) {
		query.resize(query_size);
	}
	for (int i = 0; i < query_size; ++i) {
		int array_size = (world_rank == 0) ? query[i].size() : 0;
		MPI_Bcast(&array_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
		if (world_rank != 0) {
			query[i].resize(array_size);
		}
		MPI_Bcast(query[i].data(), array_size, MPI_INT, 0, MPI_COMM_WORLD);
	}
	vector<vector<unsigned int>>local_results;
	int begin = world_rank;
	for (int i = begin; i < QueryNum; i += world_size)
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

		local_results.push_back(result);
	}
	vector<vector<unsigned int>> global_results;
	if (world_rank == 0) {
		global_results.resize(QueryNum);
	}

	for (int i = begin; i < QueryNum; i += world_size) {
		vector<unsigned int>& local_res = local_results[(i - begin) / world_size];
		int local_size = local_res.size();

		// Gather the size of the local result to the root process
		vector<int> all_sizes(world_size);
		MPI_Gather(&local_size, 1, MPI_INT, all_sizes.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);

		// Gather the local result data to the root process
		vector<int> displs(world_size, 0);
		vector<unsigned int> recvbuf;
		if (world_rank == 0) {
			int total_size = 0;
			for (int j = 0; j < world_size; ++j) {
				displs[j] = total_size;
				total_size += all_sizes[j];
			}
			recvbuf.resize(total_size);
		}
		MPI_Gatherv(local_res.data(), local_size, MPI_UNSIGNED, recvbuf.data(), all_sizes.data(), displs.data(), MPI_UNSIGNED, 0, MPI_COMM_WORLD);

		if (world_rank == 0) {
			for (int j = 0; j < world_size; ++j) {
				global_results[i].insert(global_results[i].end(), recvbuf.begin() + displs[j], recvbuf.begin() + displs[j] + all_sizes[j]);
			}
			sort(global_results[i].begin(), global_results[i].end());
			global_results[i].erase(unique(global_results[i].begin(), global_results[i].end()), global_results[i].end());
		}
	}

	if (world_rank == 0) {
		// ������ʱ
		QueryPerformanceCounter(&end);
		// ����ʱ����
		interval = static_cast<double>(end.QuadPart - start.QuadPart) / frequency.QuadPart;
		cout << "����ִ��ʱ��: " << interval << " ��" << endl;
	}

	MPI_Finalize();
	return 0;
}