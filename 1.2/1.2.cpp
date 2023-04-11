#include <iostream>
#include "windows.h"
#include "ShellAPI.h"

using namespace std;

int main()
{                                                                    //SW_RESTORE - АктивSизирует и отображает окно.
	ShellExecute(NULL, L"open", L"qwrty.docx", NULL, NULL, SW_RESTORE);//функция, позволяющая запускать приложение, //запускает документы
																	 //в данном случае,текстовой файл . txt
	return 0;
}
