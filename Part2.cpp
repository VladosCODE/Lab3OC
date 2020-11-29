#include <iostream>
#include <Windows.h>
#include <ctime>
#include <omp.h>
#include <iomanip>
#include <chrono>
using namespace std;
const DWORD N = 100000000;
const DWORD SizeB = 830815 * 10;
double CalculatPi(int Num_th)
{
	double x = 0,pi = 0;
	omp_set_dynamic(1);
	unsigned int start_time = clock(); // начальное время
	if (omp_get_dynamic())
	{
#pragma omp parallel
		{
#pragma omp for private(x) reduction(+:pi) schedule(dynamic,SizeB) omp_set_num_threads(Num_th)
			for (int i = 0; i < N; i++)
			{
				x = (i + 0.5) / N;
				pi += 4 / (1 + x * x);
			}
		}
		pi = pi / N;
		unsigned int end_time = clock(); // конечное время
		cout << "Времы выполнения: " << (float)(end_time - start_time)/ CLOCKS_PER_SEC <<" сек"<< endl;
		return pi;
	}
	else
		cout << "Произошла ошибка " << endl;
}
int main()
{
	int Num_th;
	double pi;
	system("chcp 1251");
	system("cls");
	for (int i = 0;i < 6;i++)
	{
		cout << "Введите количество потоков: ";
		cin >> Num_th;
		if (Num_th != 0)
		{
			pi = CalculatPi(Num_th);
			cout << "" << setprecision(N) << pi << endl;
		}
		else
			cout << "Количество потоков не может быть 0" << endl;
	}
	return 0;
}