#include<iostream>
#include<vector>
#include<cmath>
//#include <random>
using namespace std;

long double Function_dt(long double r, int lambda) {
	return -log(r) / lambda;
}

long double Function_t_obs(long double q, int mu) {
	return -log(q) / mu;
}

double MO(vector<double> x) {
	double MO;
	for (int i = 0; i < x.size(); ++i) {
		MO += x[i];
	}
	return MO / x.size();
}

double SKO(vector<double> x, double MO) {
	double SKO = 0;
	for (int i = 0; i < x.size(); ++i) {
		SKO += pow(MO - x[i], 2);
	}
	return sqrt(SKO / x.size());
}

int main() {
	int n = 8, m = 5, lambda = 82, mu = 6, j_k = 400, k_j = 7;
	FILE* file;
	fopen_s(&file, "table.txt", "w");
	vector<int> count_obs(100);
	vector<double> Q(100), A(100), Mt_obs(100), Mt_m(100), Mn_busy(100), Mm_busy(100);
	for (int j = 0; j < 100; ++j) {
		double dt = 0, r = 0, q = 0, t_pr = 0, t_obs = 0, count_N_busy = 0, count_M_busy = 0, t_no = 0, index = 1000, t_m = 0;
		vector<pair<bool, double>> n_busy, m_busy;
		vector<int> m_busy_n; // хранит номер канала для очереди
		n = 8;
		n_busy.assign(n, make_pair(false, 0));
		m_busy.assign(m, make_pair(false, 0));
		m_busy_n.assign(m, 1000);
		fprintf_s(file, "%2s | %5s | %5s | %5s | %5s | %3s | %3s | %3s | %5s | %7s | %7s | \n", "i", "r", "q", "dt", "t_pr", "n", "m", "N", "t_no", "t_obs", "t_ko");
		for (int i = 0; i < 1000; ++i) {
			double min = 100000000;
			int min_index = 1000;
			double min_m = 100000000;
			int min_m_index = 1000;
			bool exp = false;
			if (i == j_k) {
				if (count_N_busy == n)count_N_busy--;
				for (int k = 0; k < n - k_j; ++k) {
					n_busy[n_busy.size() - 1 - k].first = false;
					n_busy[n_busy.size() - 1 - k].second = 10000;
				}
				n = k_j;
			}
			if (i != 0) {
				r = abs(2.0 * rand() / RAND_MAX - 1.0);
				dt = Function_dt(r, lambda);
				t_pr = t_pr + dt;
				for (int k = 0; k < n; ++k) {
					if (t_pr >= n_busy[k].second && n_busy[k].first) {
						n_busy[k].first = false;
						count_N_busy--;
					}
					if (min > n_busy[k].second && n_busy[k].first) {
						min = n_busy[k].second;
						min_index = k;
					}
				}
				for (int k = 0; k < m; ++k) {
					if (t_pr >= m_busy[k].second && m_busy[k].first) {
						m_busy[k].first = false;
						count_M_busy--;
					}
					if (min > m_busy[k].second && m_busy[k].first) {
						min_m = n_busy[k].second;
						min_m_index = k;
					}
				}
			}
			q = abs(2.0 * rand() / RAND_MAX - 1.0);
			t_obs = Function_t_obs(q, mu);
			if (i >= 10) {
				fprintf_s(file, "%i | %3.3f | %3.3f | %3.3f | %3.3f | %3.0f | %3.0f |", i, r, q, dt, t_pr, count_N_busy, count_M_busy);
			}
			else {
				fprintf_s(file, "%2i | %3.3f | %3.3f | %3.3f | %3.3f | %3.0f | %3.0f |", i, r, q, dt, t_pr, count_N_busy, count_M_busy);
			}
			if (count_N_busy < n) {
				for (int k = 0; k < n; ++k) {
					if (!n_busy[k].first) {
						t_no = t_pr;
						n_busy[k].second = t_no + t_obs;
						n_busy[k].first = true;
						index = k;
						break;
					}
				}
				count_N_busy++;
			}
			else if (count_N_busy == n && count_M_busy <= m) {
				if (min_index < 1000 && n_busy[min_index].second <= t_pr) {
					t_no = n_busy[min_index].second;
					n_busy[min_index].second = t_no + t_obs;
					index = min_index;
				}
				else {
					if (count_M_busy < m) {
						for (int k = 0; k < m; ++k) {
							if (!m_busy[k].first) {
								m_busy[k].second = n_busy[min_index].second;
								m_busy[k].first = true;
								t_no = n_busy[min_index].second;
								m_busy_n[k] = min_index;
								n_busy[min_index].second = t_no + t_obs;
								index = min_index;
								count_M_busy++;

								if (i < 999) {
									t_m += m_busy[k].second - t_pr;
									Mt_m[j]++;
								}
								break;
							}
						}
					}
					else {
						if (min_m_index < 1000 && m_busy[min_m_index].second <= t_pr) {
							m_busy[min_m_index].second = t_pr;
							t_no = n_busy[m_busy_n[min_m_index]].second;
							n_busy[min_index].second = t_no + t_obs;
							index = min_index;
							if (i < 999){
								t_m += m_busy[min_m_index].second - t_pr;
								Mt_m[j]++;
							}
						}
						else {
							exp = true;
						}
					}
				}
			}
			if (!exp) { 
				count_obs[j]++; 
				Mn_busy[j] += t_obs;
				fprintf_s(file, " %3.0f | %3.3f | %3.5f | %3.5f | \n", index + 1, t_no, t_obs, n_busy[index].second);
			}
			else {
				fprintf_s(file, " %3s | %3s | %3s | %3s | \n", "__", "__", "__", "__");
			}
			if (i == 999) {
				Q[j] = count_obs[j] / 1000;
				A[j] = count_obs[j] / t_pr;
				Mt_obs[j] = (Mn_busy[j] - t_obs) / count_obs[j];
				Mt_m[j] = t_m / Mt_m[j];
				Mn_busy[j] = (Mn_busy[j] - t_obs) / t_pr;
				Mm_busy[j] = t_m / t_pr;
			}
		}
		fclose(file);
	}
	fopen_s(&file, "output.txt", "w");
	fprintf_s(file, " %3.5f | %3.5f | \n", MO(Q), SKO(Q, MO(Q)));
	fprintf_s(file, " %3.5f | %3.5f | \n", MO(A), SKO(A, MO(A)));
	fprintf_s(file, " %3.5f | %3.5f | \n", MO(Mt_obs), SKO(Mt_obs, MO(Mt_obs)));
	fprintf_s(file, " %3.5f | %3.5f | \n", MO(Mt_m), SKO(Mt_m, MO(Mt_m)));
	fprintf_s(file, " %3.5f | %3.5f | \n", MO(Mn_busy), SKO(Mn_busy, MO(Mn_busy)));
	fprintf_s(file, " %3.5f | %3.5f | \n", MO(Mm_busy), SKO(Mm_busy, MO(Mm_busy)));
}
