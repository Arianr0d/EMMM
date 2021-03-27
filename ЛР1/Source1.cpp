#include<iostream>
#include<vector>
#include<cmath>
//#include <random>
using namespace std;

long double Function_dt(long double r, int lambda) {
	return log(r) / lambda;
}

long double Function_t_obs(long double q, int mu) {
	return -log(q) / mu;
}

int main() {
	int n = 8, m = 5, lambda = 82, mu = 6;
	for (int j = 0; j < 100; ++j) {
		cout << "I " << "r " << "q " << "dt " << "t_pr " << "t_no " << "конец" << endl;
		double dt = 1000, r = 0, q = 0, t_pr = 0, t_obs = 0, count_N_busy = 0, count_M_busy = 0, t_no = 0, index = 1000;
		vector<pair<bool, long double>> n_busy, m_busy;
		vector<int> m_busy_n; // хранит номер канала для очереди
		n_busy.assign(n, make_pair(false, 0));
		m_busy.assign(m, make_pair(false, 0));
		m_busy_n.assign(m, 1000);
		FILE* file;
		fopen_s(&file, "output.txt", "w");
		fprintf_s(file, "%1s | %3s %6s %6s %6s %6s %6s %6s %6s \n", "i", "r", "q", "dt", "t_pr", "N", "M", "t_no", "t_ko");
		for (int i = 0; i < 1000; ++i) {
			if (i != 0) {
				r = abs(2.0 * rand() / RAND_MAX - 1.0);
				dt = Function_dt(r, lambda);
				t_pr = t_pr - dt;
				for (int k = 0; k < n; ++k) {
					if (t_pr >= n_busy[k].second && n_busy[k].first) {
						n_busy[k].first = false;
						count_N_busy--;
					}
				}
				for (int k = 0; k < m; ++k) {
					if (t_pr >= m_busy[k].second && m_busy[k].first) {
						m_busy[k].first = false;
						count_M_busy--;
					}
				}
			}
			q = abs(2.0 * rand() / RAND_MAX - 1.0);
			t_obs = Function_t_obs(q, mu);
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
				double min = 100000000;
				int min_index = 1000;
				for (int k = 0; k < n; ++k) {
					if (min > n_busy[k].second) {
						min = n_busy[k].second;
						min_index = k;
					}
				}
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
								break;
							}
						}
					}
					else {
						double min_m = 100000000;
						int min_m_index = 1000;
						for (int k = 0; k < m; ++k) {
							if (min_m > m_busy[k].second) {
								min_m = m_busy[k].second;
								min_m_index = k;
							}
						}
						if (min_m_index < 1000 && m_busy[min_m_index].second <= t_pr) {
							m_busy[min_m_index].second = t_pr;
							t_no = n_busy[m_busy_n[min_m_index]].second;
							n_busy[min_index].second = t_no + t_obs;
							index = min_index;
						}
					}
				}
			}
			fprintf_s(file, "%1i | %3.3f %3.3f %3.3f %3.3f %3.3f %3.3f %3.3f %3f \n", i, r, q, dt, t_pr, count_N_busy, count_M_busy, t_no, n_busy[index].second);
		}
	}
}