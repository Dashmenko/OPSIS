#include <iostream>
#include <windows.h>
//#include "stdafx.h"
//2
using namespace std;


void writeFile(HANDLE desc, std::string strText) {
	// Write data to the file
	//strText = "Hello World!"; // For C use LPSTR (char*) or LPWSTR (wchar_t*)

	DWORD bytesWritten;
	WriteFile(
		desc,            // Handle to the file
		strText.c_str(),  // Buffer to write
		strText.size(),   // Buffer size
		&bytesWritten,    // Bytes written
		nullptr);         // Overlapped
}

int main(int argc, char* argv[])
{
	setlocale(0, "ru");
	//STARTUPINFO si;// структура,чтобы определить оконный терминал, рабочий стол, стандартный дескриптор и внешний вид основного окна дл¤ нового процесса.
	//ZeroMemory(&si, sizeof(si));// перед запуском консольного процесса все пол¤ структуры си должны быть заполнены 0 и это делаетс¤ в данной строке 
	//si.cb = sizeof(si);//выводит окно с размером по умолчанию
	//PROCESS_INFORMATION pi; //информаци¤ о недавно созданном процессе и его первичном потоке. 

	cout << "\t\nпод процесс запущен\n" << endl;
	for (int i = 0; i < argc; i++) {
		cout << "\t дескриптор(int): " << argv[i] << endl;
	}
	

	HANDLE desc1;
	HANDLE desc2 = NULL;
	desc1 = (HANDLE)atoi(argv[0]);
	if (argc == 2) //проверка дл¤ отсутстви¤ 2-ого агрумента
		desc2 = (HANDLE)atoi(argv[1]);


	//cout << "\t" << REALTIME_PRIORITY_CLASS << endl;

	cout << "\t\n";
	int menu = 1;
	while (menu > 0) {
		cout << "\t\n»зменить приоретет:\n\t1 - низкий приоритет,\n\t2 - нормальный приоритет,\n\t3 - высокий приоритет,\n\t0 - выход из управл¤ющего процесса (под-процесса): ";
		cin >> menu;
		cout << "\n";
		switch (menu)
		{
		case 1:
			cout << "\tустановлен низкий приоритет\n";
			SetPriorityClass(
				desc1,
				IDLE_PRIORITY_CLASS
			);			
			writeFile(desc2, (string)"установлен низкий приоритет\n");
			break;
		case 2:
			cout << "\tустановлен нормальный приоритет\n";
			SetPriorityClass(
				desc1,
				NORMAL_PRIORITY_CLASS
			);			
			writeFile(desc2, (string)"установлен нормальный приоритет\n");
			break;
		case 3:
			cout << "\tустановлен высокий приоритет\n";
			SetPriorityClass(
				desc1,
				HIGH_PRIORITY_CLASS
			);
			writeFile(desc2, (string)"установлен высокий приоритет\n");
			break;
		case 0:
			cout << "\tвыход.\n";
			menu = -1;
			//exit(1);
			break;
		default:
			menu = 100;
			break;
		}
	}


	cout << "под процесс закрыт" << endl;
	system("pause");
	if (argc == 2) //проверка для отсутстви¤ 2-ого агрумента
		CloseHandle(desc1);
	

}


