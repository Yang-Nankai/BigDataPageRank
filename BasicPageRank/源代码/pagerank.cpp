#pragma warning(disable:4996)
#include<cstdio>
#include<iostream>
#include<fstream>
#include<algorithm>
#include<vector>
#include<iomanip>
#include<map>
#include<cmath>
using namespace std;

const double Epsilon = 1e-10;//迭代终止条件的精度
vector<int>in_dgree;//入度
vector<int>out_dgree;//出度
map<int, vector<int>> E_in;//入度邻接表
map<int, vector<int>> E_out;//出度邻接表
vector<double> Rn;//Rank_new
vector<double> Ro;//Rank_old


int max1(unsigned short a, unsigned short b)
{
	return a > b ? a : b;
}
bool cmp0(const pair<int, double>& x, const pair<int, double>& y)
{
	return x.second > y.second;
}

int main()
{
	freopen("Data.txt", "r", stdin);
	freopen("result.txt", "w", stdout);
	//cin 
	int max0 = 0;
	int from, to;
	while (cin >> from >> to)
	{
		E_in[to].push_back(from);
		E_out[from].push_back(to);
		max0 = max1(max0, max1(from, to));
	}
	in_dgree.resize(max0 + 1);
	out_dgree.resize(max0 + 1);
	for (int i = 0; i < max0 + 1; i++)
	{
		in_dgree[i] = E_in[i].size();
		out_dgree[i] = E_out[i].size();
	}
	int n = max0 + 1;
	for (int i = 0; i < max0 + 1; i++)
		if (in_dgree[i] == 0 && out_dgree[i] == 0)
			n--;

	//进行PageRank的计算
	double teleport = 0.85;//teleport参数
	Rn.resize(max0 + 1, 0);
	Ro.resize(max0 + 1, 1.0 / n);
	double loss;
	do
	{
		loss = 0;
		for (auto i : E_in)
		{
			Rn[i.first] = 0;
			for (int j = 0; j < i.second.size(); j++)
				if (out_dgree[i.second[j]] != 0)
					Rn[i.first] += teleport * (Ro[i.second[j]] / out_dgree[i.second[j]]);
				else
					Rn[i.first] += 0;
		}
		double sum0 = 0;
		for (auto i : Rn)
			sum0 += i;
		for (int i = 0; i < Rn.size(); i++)
		{
			Rn[i] += (1 - sum0) / n;
			loss += (Rn[i] > Ro[i]) ? (Rn[i] - Ro[i]) : (Ro[i] - Rn[i]);
			Ro[i] = Rn[i];
		}			
	}
	while (loss > Epsilon);

	//ComputeRank(n, teleport);
	//cout Top 100
	map<int, double> node_score;
	for (int i = 0; i < Rn.size(); i++)
		node_score[i] = Rn[i];
	vector<pair<int, double>>score_vec(node_score.begin(), node_score.end());
	sort(score_vec.begin(), score_vec.end(), cmp0);
	for (int i = 0; i < 100; i++)
		cout << score_vec[i].first << " " << score_vec[i].second << endl;
	return 0;
}