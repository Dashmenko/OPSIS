#include <iostream>
#include <windows.h>
using namespace std;

// поток чтения сообщений от клиента
DWORD WINAPI readThread(PVOID param) {
	HANDLE* hName = (HANDLE*)param;
	DWORD readed;
	char message[128] = { 0 };
	while (true) {
		// читаем сообщение
		if (!ReadFile(hName, message, 128, &readed, NULL)) {
			cout << "Client disconnected\n";
			return NULL;
		}
		// выводим его
		cout << "Client: " << message << "\n";
		// если сообщение == exit, то завершаем цикл и выходим из потока
		if (strcmp(message, "exit") == 0)
		{
			break;
		}
	}
	return NULL;
}

// поток записи сообщений клиенту
DWORD WINAPI writeThread(PVOID param) {
	HANDLE* hName = (HANDLE*)param;
	DWORD writed;
	char message[128] = { 0 };
	while (true) {
		// читаем сообщение с клавиатуры
		cin.getline(&message[0], 128);
		// отправляем сообщение клиенту
		if (!WriteFile(hName, message, 128, &writed, NULL)) {
			cout << "Connection not established" << endl;
			return NULL;
		}
		// если сообщение == exit, то завершаем цикл и выходим из потока
		if (strcmp(message, "exit") == 0)
		{
			break;
		}
	}
	return NULL;
}


int main()
{
	SetConsoleOutputCP(1251);
	SetConsoleCP(1251);

	//дескрипторы именованных каналов
	HANDLE hPipeClientToServer;
	HANDLE hPipeServerToClient;
	//атрибуты защиты
	SECURITY_ATTRIBUTES sa;
	//дескриптор безопасности
	SECURITY_DESCRIPTOR sd;

	sa.bInheritHandle = FALSE;
	sa.nLength = sizeof(&sa);

	// инициализируем дескриптор безопасности
	InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);

	//доступ разрешен всем
	SetSecurityDescriptorDacl(&sd, TRUE, NULL, FALSE);
	sa.lpSecurityDescriptor = &sd;

	//создаем именованный канал с доступом для всех
	hPipeClientToServer = CreateNamedPipe(L"\\\\.\\pipe\\5", PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE | PIPE_WAIT | PIPE_ACCEPT_REMOTE_CLIENTS, 255, 0, 0, INFINITE, &sa);
	if (hPipeClientToServer == INVALID_HANDLE_VALUE) {
		cerr << "Error while client to server pipe creating: " << GetLastError() << endl;
		cin.get();
		return 0;
	}

	hPipeServerToClient = CreateNamedPipe(L"\\\\.\\pipe\\5", PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE | PIPE_WAIT | PIPE_ACCEPT_REMOTE_CLIENTS, 255, 0, 0, INFINITE, &sa);
	if (hPipeServerToClient == INVALID_HANDLE_VALUE) {
		cerr << "Error while server to client pipe creating: " << GetLastError() << endl;
		cin.get();
		return 0;
	}


	char ComputerName[MAX_PATH]; // переменная для имени сервера
	DWORD size = 256;
	GetComputerNameA(ComputerName, &size); // получаем имя компьютера
	cout << "Server name: " << ComputerName << endl;

	// ожидаем подключения клиента к pipe
	if (!ConnectNamedPipe(hPipeClientToServer, NULL)) {
		cout << "Connection error: " << GetLastError() << endl;
		return 0;
	}
	cout << "Connection established" << endl;

	HANDLE hWrite;
	HANDLE hRead;
	// создаём потоки для чтения и записи
	hWrite = CreateThread(NULL, 0, writeThread, hPipeServerToClient, 0, NULL);
	hRead = CreateThread(NULL, 0, readThread, hPipeClientToServer, 0, NULL);
	WaitForSingleObject(hWrite, INFINITE); // ожидаем завершения потока

	system("pause");
	return 0;
}
