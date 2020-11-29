#include <iostream>
#include<windows.h>
#include<iomanip>
#include <chrono>
const long int N = 100000000;
const int SizeB = 10 * 830815;
const int Num_B = N / SizeB + 1;
HANDLE* Threads;
HANDLE* Events;
int* Num_block;
using namespace std;
double pi = 0;
int Num_thread;
LPCRITICAL_SECTION lpCriticalSection = new CRITICAL_SECTION;

DWORD WINAPI CalculationPi(LPVOID IpParamenter)
{
	double x = 0, y = 0;
	int Num_B_1 = Num_block[(int)IpParamenter];
	int begin_b = Num_B_1;
	int k = 0;
	while (begin_b < Num_B)
	{
		y = 0;
		begin_b = (int)IpParamenter + k * Num_thread;
		int Last_b = (begin_b + 1) * SizeB;
		if (Last_b > N)	Last_b = N;
		for (int i = begin_b*SizeB; i < Last_b; i++) {
			x = (i + 0.5) / N;
			y += 4 / (1 + x * x);
		}
		k++;
		EnterCriticalSection(lpCriticalSection);
		pi += y;
		LeaveCriticalSection(lpCriticalSection);
	}
	return 0;
}
int main()
{
	int BTh;
	int SHI;+
	system("chcp 1251");
	system("cls");
	for (int i = 0;i < 6;i++)
	{
		InitializeCriticalSection(lpCriticalSection);
		cout << "Введите количество потоков: ";
		cin >> Num_thread;
		if (Num_thread > 0)
		{
			unsigned int start_time = clock(); // начальное время
			Num_block = new int[Num_thread];
			Threads = new HANDLE[Num_thread];
			for (int i = 0;i < Num_thread;i++)
			{
				Num_block[i] = i;
				Threads[i] = CreateThread(NULL, 0, CalculationPi, (LPVOID)i, CREATE_SUSPENDED, NULL);
				if (!Threads[i])
				{
					cout << "Произошла ошибка при создании потока " << GetLastError << endl;
					system("exit");
				}
			}
			for (int i = 0; i < Num_thread; i++)
			{
				if (ResumeThread(Threads[i]) == -1)
				{
					cout << "Произошла ошибка " << GetLastError << endl;
					system("exit");
				}
			}
			WaitForMultipleObjects(Num_thread,Threads, true, INFINITE);
			unsigned int end_time = clock(); // конечное время
			cout << "Время выполнения: " << (float)(end_time - start_time) / CLOCKS_PER_SEC << " сек" << endl;
			pi = pi / N;
			cout << "pi = " << setprecision(N) << pi << endl;
			for (int i = 0;i < Num_thread;i++)
			{
				SuspendThread(Threads[i]);
				CloseHandle(Threads[i]);
			}
			delete[] Num_block;
		}
		else
			cout << "Введено малое количество потоков <= 0" << endl;
	}
	return 0;
}