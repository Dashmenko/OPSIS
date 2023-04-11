#include <iostream>
#include <windows.h>
using namespace std;

LPWSTR CharToLPWSTR(LPCSTR char_string)
{
	LPWSTR res;
	DWORD res_len = MultiByteToWideChar(1251, 0, char_string, -1, NULL, 0);
	res = (LPWSTR)GlobalAlloc(GPTR, (res_len + 1) * sizeof(WCHAR));
	MultiByteToWideChar(1251, 0, char_string, -1, res, res_len);
	return res;
}

// поток для чтения сообщений с сервера
DWORD WINAPI readThread(PVOID param) {
	HANDLE* hName = (HANDLE*)param;
	DWORD readed;
	char message[128] = { 0 };
	while (true) {
		if (!ReadFile(hName, message, 128, &readed, NULL)) {
			continue;
			cout << "Client disconnected" << endl;
		}
		cout << "Server: " << message << "\n";
		if (strcmp(message, "exit") == 0)
		{
			break;
		}
	}
	return NULL;
}

// поток для отправки сообщений серверу
DWORD WINAPI writeThread(PVOID param) {
	HANDLE* hName = (HANDLE*)param;
	DWORD writed;
	cout << endl;
	char message[128] = { 0 };
	while (true) {
		cin.getline(&message[0], 128);
		if (!WriteFile(hName, message, 128, &writed, NULL)) {
			cout << "Connection not established" << endl;
		}
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
	char ServerName[MAX_PATH];

	cout << "Enter server name: ";
	cin >> ServerName;


	char pipeNameClientToServer[MAX_PATH];
	sprintf_s(pipeNameClientToServer, "\\\\%s\\pipe\\5", ServerName);
	char pipeNameServerToClient[MAX_PATH];
	sprintf_s(pipeNameServerToClient, "\\\\%s\\pipe\\5", ServerName);

	if (!WaitNamedPipe(CharToLPWSTR(pipeNameClientToServer), 1000000) || !WaitNamedPipe(CharToLPWSTR(pipeNameServerToClient), 100000)) {
		cout << "Connection not established: " << GetLastError() << endl;
		system("pause");
		return 0;
	}
	cout << "Connection established\n";

	HANDLE hPipeClientToServer;
	HANDLE hPipeServerToClient;

	hPipeClientToServer = CreateFile(CharToLPWSTR(pipeNameClientToServer), GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	hPipeServerToClient = CreateFile(CharToLPWSTR(pipeNameServerToClient), GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);;
	if (hPipeClientToServer == INVALID_HANDLE_VALUE || hPipeServerToClient == INVALID_HANDLE_VALUE) {
		cout << "Can't open pipe:" << GetLastError() << endl;
		system("pause");
		return 0;
	}

	HANDLE readTh = NULL;
	HANDLE writeTh = NULL;

	writeTh = CreateThread(NULL, 0, writeThread, hPipeClientToServer, 0, NULL);
	readTh = CreateThread(NULL, 0, readThread, hPipeServerToClient, 0, NULL);
	WaitForSingleObject(writeTh, INFINITE);

	system("pause");
	return 0;
}
