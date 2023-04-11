/*
Мьютекс-это объект синхронизации потоков, который позволяет одновременно обращаться к ресурсу только одному потоку. 
Доступ к другим ресурсам разрешается только тогда, когда процесс переходит в сигнальное состояние.

предназначен для организации взаимоисключающего доступа к общим данным для нескольких потоков с использованием барьеров памяти
*/

#include <iostream>
#include <windows.h>
using namespace std;

int m[25]; // глобальный массив для вывода на экран

DWORD iVal = 0;
HANDLE mutex = NULL;                 

// функция потока
DWORD WINAPI ThreadFunc(LPVOID lpParam)
{
    while (iVal < 2) {
        iVal++;                                 // ждем сигнала от рабочего потока
        WaitForSingleObject(mutex, INFINITE);   //позволяет синхронизировать выполнение потока с состоянием объектов синхронизации, 
                                                //в частности других потоков и процессов.
        for (int i = 0; i < 25; i++) {
            cout << m[i] << "  ";               // поэлементный вывод массива на экран
        }
        ReleaseMutex(mutex);                    //ReleaseMutex должна быть вызвана столько же раз, сколько было вызовов WaitFor
    }
    cout << endl;
    return 0;
}

int main()
{
    mutex = CreateMutex(NULL, FALSE, L"Same name");            //(атрибуты безопасности по умолчанию, изначально не принадлежит, названный мьютекс)

    SetConsoleOutputCP(1251);
    cout << "Вывод двух возрастающих последовательностей:"
        << endl;
    for (int i = 0; i < 25; m[i++] = i);   // массив заполняется рядом чисел

    HANDLE Th;          // дескриптор потока
// Создаем дополнительный поток и начинаем печатать последовательность
    Th = CreateThread(NULL, 0, ThreadFunc, NULL, 0, NULL);

    while (iVal < 2) {
        iVal++;
        WaitForSingleObject(mutex, INFINITE);           // запрашивает владение мьютексом
        //дескриптор для мьютекса, нет интервала тайм-аута
        for (int i = 0; i < 25; i++) {
            cout << m[i] << "  ";
        }
        cout << endl;
        ReleaseMutex(mutex); //освобождение мьютекса     
    }
    // дожидаемся завершения другого потока
    // результат WaitForSingleObject не важен
    WaitForSingleObject(Th, INFINITE);

    system("pause");
    return 0;
}