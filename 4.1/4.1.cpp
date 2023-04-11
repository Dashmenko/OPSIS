#include <iostream>
#include <windows.h>
using namespace std;

int m[25]; // глобальный массив для вывода на экран

// функция потока
DWORD WINAPI ThreadFunc(LPVOID lpParam)
{
    for (int i = 0; i < 25; i++)
        cout << m[i] << " "; // поэлементный вывод массива на экран
    cout << endl;
    return 0;
}

int main()
{
    SetConsoleOutputCP(1251);
    cout << "Вывод двух возрастающих последовательностей:"
        << endl;
    for (int i = 0; i < 25; m[i++] = i);   // массив заполняется рядом чисел

    HANDLE Th;          // дескриптор потока
// Создаем дополнительный поток и начинаем печатать последовательность
    for (int i = 0; i < 25; i++)
        cout << m[i] << " ";
    cout << endl;

    Th = CreateThread(NULL, 0, ThreadFunc, NULL, 0, NULL);

    // дожидаемся завершения другого потока
    // результат WaitForSingleObject не важен
    WaitForSingleObject(Th, INFINITE);
    system("pause");
    return 0;
}