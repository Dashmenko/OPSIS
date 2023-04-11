#include <iostream>
#include <windows.h>
using namespace std;

int m[25]; // глобальный массив для вывода на экран

CRITICAL_SECTION cs;  //объявляем критическую секцию

DWORD WINAPI ThreadFunc(LPVOID lpParam)
{
    for (int i = 0; i < 25; i++) {
        EnterCriticalSection(&cs);      //Процесс отвечает за выделение памяти, используемой объектом критического раздела, который он может сделать, объявив переменную типа CRITICAL_SECTION
                                        //Чтобы обеспечить взаимоисключающий доступ к общему ресурсу, каждый поток вызывает функцию
                                        //блокируется до тех пор, пока поток не сможет стать владельцем критического раздела. 
        cout << m[i] << "  ";           //поэлементный вывод массива на экран
        LeaveCriticalSection(&cs);      //исп. для отказа от владения, позволяя другому потоку стать владельцем и получить доступ к защищенному ресурсу
    }
    cout << endl;
    return 0;
}

int main()
{
    SetConsoleOutputCP(1251);
    InitializeCriticalSection(&cs);     //инициализация объекта , исп. перед использ. крит.раздела
    cout << "Вывод двух возрастающих последовательностей:"
        << endl;
    for (int i = 0; i < 25; m[i++] = i);   // массив заполняется рядом чисел

    HANDLE Th;          // дескриптор потока
// Создаем дополнительный поток и начинаем печатать последовательность
    Th = CreateThread(NULL, 0, ThreadFunc, NULL, 0, NULL);
    WaitForSingleObject(Th, INFINITE); //дожидание всех потоков
    for (int i = 0; i < 25; i++) {
        EnterCriticalSection(&cs);
        cout << m[i] << "  ";
        LeaveCriticalSection(&cs);
    }
    cout << endl;
    // дожидаемся завершения другого потока
    // результат WaitForSingleObject не важен

    system("pause");
    DeleteCriticalSection(&cs);        //освобождения системных ресурсов, выделенных при инициализации объекта критического раздела. 
                                       //После вызова этой функции объект критического раздела больше не может использоваться для синхронизации
                                       
    return 0;
}