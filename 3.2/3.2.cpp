#include <iostream>
#include <Windows.h>

using namespace std;


long sum1 = 0, sum2 = 0;
CRITICAL_SECTION section;

struct MyStruct {
	int* mass;
	int N;
	int row;
	MyStruct() {}
	MyStruct(int* mass, int N, int row) {
		this->mass = mass;      //Дает указатель на объект, для которого вызвана функция
		this->N = N;			//переменная находится в структуре
		this->row = row;
	}
};

DWORD WINAPI Thread(LPVOID lpParam) {
	struct MyStruct* Struct = (MyStruct*)lpParam;
	int* mass = Struct->mass;
	int N = Struct->N;     //-> используется, если у нас есть указатель на объект и мы хотим получить его поле
	int row = Struct->row;
	int rowSum = 0;
	for (int i = 0; i < N; i++) {
		rowSum += mass[i];
	}
	EnterCriticalSection(&section);		//Процесс отвечает за выделение памяти, используемой объектом критического раздела, который он может сделать, объявив переменную типа CRITICAL_SECTION
										//Чтобы обеспечить взаимоисключающий доступ к общему ресурсу, каждый поток вызывает функцию
										//блокируется до тех пор, пока поток не сможет стать владельцем критического раздела. 
	sum1 += rowSum;
	LeaveCriticalSection(&section);		//исп. для отказа от владения, позволяя другому потоку стать владельцем и получить доступ к защищенному ресурсу
	delete Struct;
	return 0;
}

int main() {
	int N;
	cout << "Enter square matrix size: ";
	cin >> N;

	int** mass = new int* [N];
	for (int i = 0; i < N; i++) {
		mass[i] = new int[N];
	}

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			mass[i][j] = rand() % 10;
			cout << mass[i][j] << "\t";
		}
		cout << "\n";
	}




	HANDLE* handle = new HANDLE[N];
	InitializeCriticalSection(&section);					//инициализация объекта , исп. перед использ. крит.раздела
	for (int row = 0; row < N; row++) {
		struct MyStruct* Struct = new MyStruct(mass[row], N, row);
		handle[row] = CreateThread(NULL, 0, &Thread, Struct, 0, NULL);
	}
	for (int i = 0; i < N; i++) {
		WaitForSingleObject(handle[i], INFINITE);		    // запрашивает владение мьютексом
        //дескриптор для мьютекса, нет интервала тайм-аута
	}
	DeleteCriticalSection(&section);						//освобождения системных ресурсов, выделенных при инициализации объекта критического раздела. 
															//После вызова этой функции объект критического раздела больше не может использоваться для синхронизации
	
	cout << "\nThe sum using threads = " << sum1 <<  endl;
		



	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			sum2 += mass[i][j];
		}
	}
	cout << "\nThe sum using double for = " << sum2 <<  endl;
	for (int i = 0; i < N; i++) {
		if (handle[i] != INVALID_HANDLE_VALUE) {
			CloseHandle(handle[i]);
		}
		delete[] mass[i];
	}
	delete[] mass;
	return 0;
}