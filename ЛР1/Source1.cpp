#include<iostream>
#include<vector>
#include<cmath>
#include<algorithm>
#include<ctime>
using namespace std;

long double Function_dt(long double r, int lambda) {      // функция для расчёта
	return -log(r) / lambda;							  // интервалов между прибытием
}														  // соседних заявок

long double Function_t_obs(long double q, int mu) {		  // функция для расчёта
	return -log(q) / mu;								  // времени обслуживания 
}														  // заявки

double MO(vector<double> x) {							  // 
	double MO = 0;										  // 
	for (int i = 0; i < x.size(); ++i) {				  // функция для расчёта
		MO += x[i];										  // математического ожидания
	}													  // 
	return MO / x.size();								  // 
}														  // 

double SKO(vector<double> x, double MO) {				  //
	double SKO = 0;										  //
	for (int i = 0; i < x.size(); ++i) {				  // функция для расчёта
		SKO += pow(MO - x[i], 2);						  // среднего 
	}													  // квадратического 
	return sqrt(SKO / x.size());						  // отклонения
}														  //

int main() {

	int _n = 8, m = 5, lambda = 82, mu = 6, j_k = 400, k_j = 7;		// входные параметры задачи
	
	srand(time(0));

	FILE* file, *file1;												 
	fopen_s(&file, "table.txt", "w");								

	// вектора вещественного типа для подсчёта 
	vector<double> count_obs(100),  // числа обслуженных заявок
		Q(100),                     // относительной пропускной способности
		A(100), 					// абсолютной пропускной способности
		Mt_obs(100), 				// среднего времени обслуживания
		Mt_m(100), 					// среднего времени нахождения заявки в очереди
		Mn_busy(100), 				// среднего числа занятых каналов
		Mm_busy(100);				// среднего числа занятых мест в очереди
	
	for (int j = 0; j < 100; ++j) {

		// вещественные переменные: 
		double dt = 0,				// интервал между прибытием соседних заявок
			r = 0, 					// случайное число r
			q = 0, 					// случайное число q
			t_pr = 0, 				// время прибытия заявки
			t_obs = 0, 				// время обслуживания заявки 
			count_N_busy = 0, 		// число занятых каналов на момент прибытия 
			count_M_busy = 0, 		// число занятых мест в очереди на момент прибытия 
			t_no = 0, 				// время начала обслуживания
			index = 1000, 			// номер обслуживающего канала
			t_m = 0;				// время нахождения заявки в очереди
		
		// парные векторы из булевой переменной, показывающей освобождён ли канал или очередь,
		vector<pair<bool, double>> n_busy, m_busy;			// и времени окончания работы канала и 
		vector<int> m_busy_n;								// вектор из номеров каналов для очереди  ----------?
		
		int n = _n;	

		n_busy.assign(n, make_pair(false, 0));				// заполнение парных векторов 
		m_busy.assign(m, make_pair(false, 0));				// значениями <false, 0> - канал свободен и время окончания его работы равно 0
		m_busy_n.assign(m, 1000);							// заполнение вектора

		// запись заголовка таблицы в файл на нулевой итерации
		if (j == 0) {
			fprintf_s(file, "%4s | %5s | %5s | %5s | %6s | %3s | %3s | %3s | %6s | %7s | %8s | \n", 
				"i", "r", "q", "dt", "t_pr", "n", "m", "N", "t_no", "t_obs", "t_ko");
		}


		for (int i = 0; i < 1000; ++i) {

			double min = 100000000;			// наименьшее время окончания работы канала
			int min_index = 1000;			// номер канала с наименьшим временем окончания работы канала
											   
			double min_m = 100000000;		// наименьшее время окончания работы канала в очереди
			int min_m_index = 1000;			// номер канала с наименьшим временем окончания работы канала в очереди
											   
			bool exp = false;				// булевая переменная, показывающая выполняется заявка или нет

			// смена числа работающих каналов после прихода j_k-ой заяявки
			if (i == j_k-1) {

				if (count_N_busy == n) count_N_busy--;
				
				vector<int> n_del;						// номер канала, который перестаёт работать после j_k-ой заявки
				vector<pair<double, int>> min_del;		// парный вектор из времени окончания работы канала и номера этого канала

				for (int c = 0; c < _n; ++c) {
					min_del.push_back(make_pair(n_busy[c].second, c));         // очищается ли min_del ------------?
				}

				sort(min_del.begin(), min_del.end());
				
				for (int k = 0; k < _n - k_j; ++k) {
					n_busy[min_del[k].second].first = false;
					n_busy[min_del[k].second].second = 10000;
					n_del.push_back(min_del[k].second);
				}

				for (int k = 0; k < n_del.size(); ++k) {
					for (int l = 0; l < m_busy_n.size(); ++l) {

						if (m_busy_n[l] == n_del[k]) {
							m_busy[l].second = 0;
							m_busy[l].first = false;
							m_busy_n[l] = 1000;
							count_obs[j]--;
						}

					}
				}
				n--;
			}

			if (i != 0) {           // а где для i == 0

				r = abs(2.0 * rand() / RAND_MAX - 1.0);     // генерация случайного числа r
				dt = Function_dt(r, lambda);				// расчёт интервала между прибытием соседних заявок
				t_pr = t_pr + dt;							// расчёт времени прибытия текущей заявки

				for (int k = 0; k < _n; ++k) {

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

					if (min_m > m_busy[k].second && m_busy[k].first) {
						min_m = m_busy[k].second;
						min_m_index = k;
					}

				}

			}

			q = abs(2.0 * rand() / RAND_MAX - 1.0);			// генерация случайного числа q
			t_obs = Function_t_obs(q, mu);					// расчёт времени обслуживания заявки 
			
			// добавление в таблицу данных для i-ой заявки первого испытания
			if (j == 0) {
				fprintf_s(file, "%4i | %3.3f | %3.3f | %3.3f | %6.3f | %3.0f | %3.0f |", i+1, r, q, dt, t_pr, count_N_busy, count_M_busy);
			}
			
			// есть свободные каналы
			if (count_N_busy < n) {

				for (int k = 0; k < _n; ++k) {

					if (!n_busy[k].first && n_busy[k].second < 10000) {
						t_no = t_pr;
						n_busy[k].second = t_no + t_obs;
						n_busy[k].first = true;
						index = k;					
						break;
					}

				}
				count_N_busy++;

			}

			// все каналы заняты, но в очереди есть места
			else if (count_N_busy == n && count_M_busy <= m) {
				
				// заявка прибыла позже окончания работы канала с самым ранним временем окночания
				if (min_index < 1000 && n_busy[min_index].second <= t_pr) {
					t_no = n_busy[min_index].second;
					n_busy[min_index].second = t_no + t_obs;
					index = min_index;
				}
				
				// заявка прибыла раньше окончания работы канала с самым ранним временем окночания
				else {

					// есть свободныеместа в очереди 
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

					// свободные места в очереди отсутствуют
					else {

						// заявка прибыла позже окончания работы канала в очереди с самым ранним временем окночания
						if (min_m_index < 1000 && m_busy[min_m_index].second <= t_pr) {

							m_busy[min_m_index].second = t_pr;					// ------------ ?
							t_no = n_busy[m_busy_n[min_m_index]].second;
							n_busy[min_index].second = t_no + t_obs;
							index = m_busy_n[min_m_index];

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

			// заполнение таблицы, если заявка выполняется
			if (!exp) {

				count_obs[j]++;
				Mn_busy[j] += t_obs;

				if (j == 0) {
					fprintf_s(file, " %3.0f | %6.3f | %6.5f | %8.5f | \n", index + 1, t_no, t_obs, n_busy[index].second);
				}
			}

			// заполнение таблицы, если заявка не выполняется
			else if (j == 0) {
				fprintf_s(file, " %3s | %6s | %7s | %8s | \n", "__", "__", "__", "__");
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
	}

	fopen_s(&file1, "output.txt", "w");

	// заполнение файла output.txt
	fprintf_s(file1, "Число обслуженных заявок: \n Математическое ожидание = %3.3f \n СКО = %3.3f \n\n", MO(count_obs), SKO(count_obs, MO(count_obs)));
	fprintf_s(file1, "Относительная пропускная способность: \n Математическое ожидание = %3.3f \n СКО = %3.3f \n\n", MO(Q), SKO(Q, MO(Q)));
	fprintf_s(file1, "Абсолютная пропускная способность: \n Математическое ожидание = %3.3f \n СКО = %3.3f \n\n", MO(A), SKO(A, MO(A)));
	fprintf_s(file1, "Среднее время обслуживания: \n Математическое ожидание = %3.3f \n СКО = %3.3f \n\n", MO(Mt_obs), SKO(Mt_obs, MO(Mt_obs)));
	fprintf_s(file1, "Среднее время нахождения заявки в очереди: \n Математическое ожидание = %3.3f \n СКО = %3.3f \n\n", MO(Mt_m), SKO(Mt_m, MO(Mt_m)));
	fprintf_s(file1, "Среднее число занятых каналов: \n Математическое ожидание = %3.3f \n СКО = %3.3f \n\n", MO(Mn_busy), SKO(Mn_busy, MO(Mn_busy)));
	fprintf_s(file1, "Среднее число занятых мест в очереди: \n Математическое ожидание = %3.3f \n СКО = %3.3f \n\n", MO(Mm_busy), SKO(Mm_busy, MO(Mm_busy)));
	
	fclose(file1);
	fclose(file);
}
