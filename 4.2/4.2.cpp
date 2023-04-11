﻿#include <iostream>
#include <windows.h>
using namespace std;

/*вид барьерной инструкции, которая приказывает компилятору(при генерации инструкций) 
и центральному процессору(при исполнении инструкций) устанавливать строгую последовательность между обращениями 
к памяти до и после барьера.
*/

int m[25];  // глобальный массив для вывода на экран
volatile LONG padlock = 0;                                 //+padlock - численное значение показателя, к которому следует стремиться для достижения цели

// функция потока
DWORD WINAPI ThreadFunc(LPVOID lpParam)
{
    while (InterlockedCompareExchange(&padlock, 1, 0)) {}   //Эта функция создает полный барьер памяти (или забор), чтобы обеспечить выполнение операций с памятью по порядку.
                                                            //выполняется цикл до тех пор, пока условие не станет ложным
    for (int i = 0; i < 25; i++)
        cout << m[i] << " ";                                //поэлементный вывод массива на экран
    cout << endl;
    InterlockedExchange(&padlock, 0);                       //Указатель на значение для обмена. Функция устанавливает для этой переменной значение 0 и возвращает его предыдущее значение.
                                                            //Значение, которое необходимо обменять со значением, на которое указывает padlock
    return 0;
}

int main()
{
    setlocale(0, "ru");
    cout << "Вывод двух возрастающих последовательностей:"
        << endl;
    for (int i = 0; i < 25; m[i++] = i);                    // массив заполняется рядом чисел

   HANDLE Th;          // дескриптор потока
// Создаем дополнительный поток и начинаем печатать последовательность
    Th = CreateThread(NULL, 0, ThreadFunc, NULL, 0, NULL);  
    while (InterlockedCompareExchange(&padlock, 1, 0)) {}   //padlock - целевое значение, 1 -значение обмена, 0 - значение, сравниваемое с назначением
                                                            //Функция возвращает начальное значение параметра
                                                            //Функция сравнивает значение padlok со значением 0 . 
                                                            //Если целевое значение равно значению 0 , значение 1 сохраняется в адресе, указанном назначением. 
                                                            //В противном случае операция не выполняется.
    for (int i = 0; i < 25; i++)
        cout << m[i] << " ";
    cout << endl;
  
   
    InterlockedExchange(&padlock, 0);
    // дожидаемся завершения другого потока
    // результат WaitForSingleObject не важен
    WaitForSingleObject(Th, INFINITE);
    system("pause");
    return 0;
}