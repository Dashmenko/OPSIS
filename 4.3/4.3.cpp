//Алгоритм Петерсона — алгоритм параллельного программирования для взаимного исключения потоков исполнения кода
//Алгоритм Петерсона позволяет добиться взаимного исключения двух процессов.
#include <iostream>
#include <windows.h>
using namespace std;

int m[25];
int turn;     //чья очередь                                         |глобальные
int interested[2] = { 0,0 };   //изначально значения равны 0        |переменные

/*
Прежде чем войти в критическую область, процесс вызывает процедуру enter_region со своим номером в качестве параметра. 
После выхода из критической области процесс вызывает процедуру leave_region и разрешает другому процессу войти в критическую область. 
Этот алгоритм является достаточно надежным, но использует активное ожидание.
*/

void enter_region(int process) {     //номер процесса 0 или 1
	int other;						//номер другого процесса
	other = 1 - process;             //точка 1, противостоящий процесс
	interested[process] = TRUE;		 //точка 2(заинтересованный процесс), демонстрация заинтересованности
	turn = process;					 //точка 3, установка флажка
	while (turn == process && interested[other] == TRUE) {}; //активное ожидание, пока оно выполняется,то всё идёт дальше, цикл без инструкции
}

void leave_region(int process) {    //номер процесса 0 или 1, процесс покидающий крит. область (секция)
	interested[process] = FALSE;    //признак выхода из критической секции
}

DWORD WINAPI ThreadFunc(LPVOID lpParam) {
	enter_region(1);
	for (int i = 0; i < 25; i++)
		cout << m[i] << "  ";
	cout << endl;
	leave_region(1);

	return 0;
}


int main() {
	SetConsoleOutputCP(1251);
	cout << "Вывод двух возрастающих последовательностей:"
		<< endl;
	for (int i = 0; i < 25; m[i++] = i);
	HANDLE Th;
	Th = CreateThread(NULL, 0, ThreadFunc, NULL, 0, NULL);

	enter_region(0);

	for (int i = 0; i < 25; i++)
		cout << m[i] << "  ";
	cout << endl;


	leave_region(0);

	WaitForSingleObject(Th, INFINITE);
	system("pause");
	return 0;
}

