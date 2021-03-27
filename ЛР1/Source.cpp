//#include<iostream>
//#include<vector>
//#include<cmath>
////#include <random>
//using namespace std;
//
//long double Function_dt(long double r,int lambda) {
//	return -log(r) / lambda;
//}
//
//long double Function_t_obs(long double q, int mu) {
//	return -log(q) / mu;
//}
//
//int main() {
//
//	//exponential_distribution<long double> d(1);
//	int n=8, m=5, lambda=82, mu=6;
//	vector<bool> work;
//	vector<long double> r, q, dt, t_pr, n_busy, m_busy, N, t_no, t_obs, t_ko;
//
//	FILE* file;
//	fopen_s(&file, "output.txt", "w");
//	fprintf_s(file, "s1 | s3 s3 s3 s3 s3 s3 s3 s3 s3 s3", "i", "r", "q", "dt", "t_pr", "n_busy", "m_busy", "N", "t_no", "t_obs", "t_ko");
//
//	for (int j = 0; j < 100; j++) {
//
//		for (int i = 0; i < 1000; i++) {
//			r.push_back(abs(2.0 * rand() / RAND_MAX - 1.0));
//			q.push_back(abs(2.0 * rand() / RAND_MAX - 1.0));
//
//			dt.push_back(Function_dt(r[i], lambda));
//			t_obs.push_back(Function_t_obs(q[i], mu));
//
//			if (i == 0) {
//				t_pr.push_back(0.);
//				t_no.push_back(0.);
//
//				n_busy.push_back(0.);
//				m_busy.push_back(0.);
//				N.push_back(1.);
//
//				t_ko.push_back(t_no[i] + t_obs[i]);
//
//				if (j == 0) {
//					fprintf_s(file, "%1i | %3.3f %3.3f %3.3f %3.3f %3i %3i %3i %3.3f %3.3f %3.3f", 
//						i, r[i], q[i], dt[i], t_pr[i], n_busy[i], m_busy[i], N[i], t_no[i], t_obs[i], t_ko[i]);
//				}
//				continue;
//			}
//
//			t_pr.push_back(t_pr[i - 1] + dt[i]);
//			work.push_back(true);
//
//			long double min = t_ko[i-1];
//			int count_out_work = 0;
//
//			for (int k = 0; k < i; k++) {
//				if (min >= t_ko[k] && work[k] == true) {
//					min = k;
//				}
//				if (t_pr[i] > t_ko[k]) {
//					work[k] = false;
//					count_out_work++;
//				}
//				if (t_pr[i] > t_ko[k] && work[k] == true) {
//					if (n_busy[i - 1] <= n && work[k] == true) {
//						n_busy.push_back(n_busy[i - 1] - count_out_work);
//						m_busy.push_back(0.);
//						N.push_back(N[k]);
//
//						work[k] = false;
//						t_no.push_back(t_pr[i]);
//						break;
//					}
//					else if (n_busy[i - 1] == n && m_busy[i - 1] > 0 && work[k] == true) {
//						if (m_busy[i - 1] - count_out_work < 0) {
//							n_busy.push_back(n_busy[i - 1] + (m_busy[i - 1] - count_out_work));
//							m_busy.push_back(0.);
//						}
//						else {
//							n_busy.push_back(n_busy[i - 1]);
//							m_busy.push_back(m_busy[i - 1] - count_out_work);
//						}
//						
//						N.push_back(N[k]);
//						work[k] = false;
//						t_no.push_back(t_pr[i]);
//						break;
//					}
//				}
//				if (k == (i - 1) && t_pr[i] <= t_ko[k]) {
//					if (n_busy[i - 1] < n) {
//						n_busy.push_back(n_busy[i - 1] + 1);
//						m_busy.push_back(0.);
//						
//						N.push_back(N[k]);
//					}
//					else if (n_busy[i - 1] == n && m_busy[i - 1] != m) {
//						n_busy.push_back(n_busy[i - 1]);
//						m_busy.push_back(m_busy[i - 1] + 1);
//
//						N.push_back(N[min]);
//					}
//					else if (n_busy[i - 1] == n && m_busy[i - 1] == m) {
//						n_busy.push_back(n);
//						m_busy.push_back(m);
//
//						work[i] = false;
//						N.push_back(0.);
//						t_no.push_back(0.);
//						t_obs.push_back(0.);
//						t_ko.push_back(0.);
//					}
//					t_no.push_back(t_no[min]);
//				}
//			}
//
//			t_ko.push_back(t_no[i] + t_obs[i]);
//
//			fprintf_s(file, "%1i | %3.3f %3.3f %3.3f %3.3f %3i %3i %3i %3.3f %3.3f %3.3f", i, r, q, dt, t_pr, n_busy, m_busy, N, t_no, t_obs, t_ko);
//		}
//
//		r.clear();
//		q.clear();
//		dt.clear();
//		t_pr.clear();
//		n_busy.clear();
//		m_busy.clear();
//		N.clear();
//		t_no.clear();
//		t_obs.clear();
//		t_ko.clear();
//
//		fclose(file);
//	}
//}