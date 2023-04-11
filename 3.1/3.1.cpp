#include <iostream> 
#include <Windows.h> 
#include <conio.h>

using namespace std;

DWORD WINAPI Thread1(LPVOID arg) {
    while (1000 - 7 == 993) {
        cout << "Thread #1 is running currently\n";
        Sleep(1000);
    }
    return 0;
}

DWORD WINAPI Thread2(LPVOID arg) {
    while (1000 - 7 == 993) {
        cout << "Thread #2 is running currently\n";
        Sleep(1200);
    }
    return 0;
}

DWORD WINAPI Thread3(LPVOID arg) {
    while (1000 - 7 == 993) {
        cout << "Thread #3 is running currently\n";
        Sleep(1400);
    }
    return 0;
}
int main()
{
    bool control = true;
    HANDLE hThread1{};
    HANDLE hThread2{};
    HANDLE hThread3{};
    cout << "\n\tPress '1' to create threads.\n\tPress '2' to terminate threads.\n\tPress '3' to exit.\n\n";
    while (control == true) {
        switch (_getch()) {
        case '1':
            hThread1 = CreateThread(NULL, 0, Thread1, NULL, 0, NULL);
            hThread2 = CreateThread(NULL, 0, Thread2, NULL, 0, NULL);
            hThread3 = CreateThread(NULL, 0, Thread3, NULL, 0, NULL);
            control = true;
            break;
        case '2':
            TerminateThread(hThread1, 0);
            TerminateThread(hThread2, 0);
            TerminateThread(hThread3, 0);
            cout << "\n\tThreads terminated." << endl;
            cout << "\n\tPress '1' to create threads again.\n\tPress '3' to exit.\n\n";
            control = true;
            break;
        case '3':
            control = false;
        default:
            break;
        }
    }
    if (control == false) {
        TerminateThread(hThread1, 0);
        TerminateThread(hThread2, 0);
        TerminateThread(hThread3, 0);
        system("pause");
        return 0;
    }
}
