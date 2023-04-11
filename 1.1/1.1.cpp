#include <iostream>
#include <windows.h>
using namespace std;

int main() {

	STARTUPINFO si; //структура, поток
	PROCESS_INFORMATION pi;//информация о потоке
	ZeroMemory(&si, sizeof(STARTUPINFO));//обнуляем поток
	si.cb = sizeof(si);//размер потока
	ZeroMemory(&pi, sizeof(STARTUPINFO));//очистка информации о потоке

	if (!CreateProcess(L"C:\\Windows\\System32\\contшrol.exe", NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) { //путь,.....//запускает приложения
		cout << "CreateProcess failedl\n"; //выводим, если не запущен процесс
	}
	system("pause");
	return 0;
}
