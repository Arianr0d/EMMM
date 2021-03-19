#include<iostream>
#include<vector>
#include<cmath>
//#include <random>
using namespace std;

long double Function_dt(long double r,int lambda) {
	return -log(r) / lambda;
}

long double Function_t_obs(long double q, int mu) {
	return -log(q) / mu;
}

int main() {

	//exponential_distribution<long double> d(1);
	int n=8, m=5, lambda=82, mu=6;
	vector<bool> work;
	vector<long double> r, q, dt, t_pr, n_busy, m_busy, N, t_no, t_obs, t_ko;

	for (int j = 0; j < 100; j++) {
		for (int i = 0; i < 1000; i++) {
			r.push_back(abs(2.0 * rand() / RAND_MAX - 1.0));
			q.push_back(abs(2.0 * rand() / RAND_MAX - 1.0));

			dt[i] = Function_dt(r[i], lambda);
			t_obs[i] = Function_t_obs(q[i], mu);

			if (i == 0) {
				t_pr[i] = 0.;
				t_no[i] = 0.;

				n_busy[i] = 0.;
				m_busy[i] = 0.;
				N[i] = 1.;

				t_ko[i] = t_no[i] + t_obs[i];
				continue;
			}

			t_pr[i] = t_pr[i - 1] + dt[i];
			work[i] = true;

			long double min = t_ko[i];
			int count_out_work = 0;

			for (int k = 0; k < i; k++) {
				if (min >= t_ko[k] && work[k] == true) {
					min = k;
				}
				if (t_pr[i] > t_ko[k]) {
					work[k] = false;
					count_out_work++;
				}
				if (t_pr[i] > t_ko[k] && work[k] == true) {
					if (n_busy[i - 1] >= n && work[k] == true) {
						n_busy[i] = n_busy[i - 1] - count_out_work;
						m_busy[i] = 0.;
						N[i] = N[k];

						work[k] = false;
						t_no[i] = t_pr[i];
						break;
					}
					else if (n_busy[i - 1] == n && m_busy[i - 1] > 0 && work[k] == true) {
						if (m_busy[i - 1] - count_out_work < 0) {
							n_busy[i] = n_busy[i - 1] + (m_busy[i - 1] - count_out_work);
							m_busy[i] = 0.;
						}
						else {
							n_busy[i] = n_busy[i - 1];
							m_busy[i] = m_busy[i - 1] - count_out_work;
						}
						
						N[i] = N[k];
						work[k] = false;
						t_no[i] = t_pr[i];
						break;
					}
				}
				if (k == (j - 1) && t_pr[i] <= t_ko[k]) {
					if (n_busy[i - 1] < n) {
						n_busy[i] = n_busy[i - 1] + 1;
						m_busy[i] = 0.;
						
						N[i] = N[k] + 1;
					}
					else if (n_busy[i - 1] == n && m_busy[i - 1] != m) {
						n_busy[i] = n_busy[i - 1];
						m_busy[i] = m_busy[i - 1] + 1;

						N[i] = N[min];
					}
					else if (n_busy[i - 1] == n && (m_busy[i - 1] == m - 1 || m_busy[i - 1] == m)) {
						n_busy[i] = n;
						m_busy[i] = m;

						work[i] = false;
						N[i] = 0.;
						t_no[i] = 0.;
						t_obs[i] = 0.;
						t_ko[i] = 0.;
					}
					t_no[i] = t_no[min];
				}
			}

			t_ko[i] = t_no[i] + t_obs[i];
		}
	}
}