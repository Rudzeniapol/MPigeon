#include "ServerConnectForm.h"
#include "MyForm.h"
#include "loginForm.h"
#include <winsock2.h>
#include <windows.h>
#pragma comment(lib, "Ws2_32.lib")

using namespace System;
using namespace System::Windows::Forms;

[STAThread]
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        MessageBox::Show("WSAStartup failed.");
        return 1;
    }

    Application::EnableVisualStyles();
    Application::SetCompatibleTextRenderingDefault(false);

    SOCKET s = INVALID_SOCKET, rec_sock = INVALID_SOCKET;
    SOCKADDR_IN sa;

    // Подключение к серверу
    KPOForms::ServerConnectForm connectForm(&s, &rec_sock, &sa);
    if (connectForm.ShowDialog() == System::Windows::Forms::DialogResult::OK) {
        // Логин
        KPOForms::loginForm login(&s);
        if (login.ShowDialog() == System::Windows::Forms::DialogResult::OK) {
            if (s == INVALID_SOCKET || rec_sock == INVALID_SOCKET) {
                MessageBox::Show("Сокет недействителен после логина.");
                WSACleanup();
                return 1;
            }

            KPOForms::MyForm form(s, rec_sock);  // Ваша главная форма
            Application::Run(% form);
        }
    }
    closesocket(s);
    closesocket(rec_sock);
    WSACleanup();
    return 0;
}
