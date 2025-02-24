#include <winsock2.h>
#include <iostream>
#include <windows.h>
#include <ws2tcpip.h> // For additional network functions
#include <future>
#include <chrono>
#pragma comment(lib, "Ws2_32.lib")
#include <iphlpapi.h>
#include <string>
#include <fstream>
#include <map>
#include <io.h>
#include <fcntl.h>
#include "../MyLibs/TranslationVariables.h"
#pragma comment(lib, "iphlpapi.lib")
#pragma warning(disable: 4996)

#define MY_TRUE 1
#define MY_FALSE 0

std::string file_name;
std::map<std::string, std::string>* final_map;

std::string trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) {
        return "";
    }
    size_t end = str.find_last_not_of(" \t\n\r");
    return str.substr(start, end - start + 1);
}

BOOL WINAPI ExitProg(DWORD signal) {
    std::ofstream keys(file_name);
    std::cout << "Please wait while the program is closing..." << std::endl;
    for (auto it = (*final_map).begin(); it != (*final_map).end(); ++it) {
        keys << it->first << std::endl;
        keys << it->second << std::endl;
    }
    keys.close();
    WSACleanup();
    return TRUE;
}

void ReadKeys(std::string filename, std::map<std::string, std::string>* map) {
    std::ifstream fileDesc(filename);
    final_map = map;
    file_name = filename;
    if (!fileDesc) {
        std::ofstream newfile(filename, std::ios::app);
        if (newfile) {
            std::cout << "Key file created successfully!" << std::endl;
            newfile.close();
            fileDesc.close();
            return;
        }
        std::cout << "Error creating key file..." << std::endl;
        return;
    }
    else {
        std::string login;
        std::string pass;
        while (std::getline(fileDesc, login)) {
            std::getline(fileDesc, pass);
            (*map)[login] = pass;
        }
        std::cout << "Keys read successfully!" << std::endl;
        return;
    }
}

void Handle_Client(SOCKET s, std::map<std::string, std::string>* map, std::map<std::string, SOCKET>* rec_socks, SOCKET clSocket) {
    char* buffer = new char[BUFFER_SIZE];
    bool isLogged = false;
    char username[BUFFER_SIZE];
    char temp[BUFFER_SIZE];
    try {
        while (!isLogged) {
            memset(buffer, 0, BUFFER_SIZE);
            recv(s, buffer, BUFFER_SIZE, 0);
            if (strcmp(buffer, CloseConnectMess) == 0) {
                closesocket(s);
                rec_socks->erase(username);
                return;
            }
            std::string inp(buffer);
            if (inp == "LOG") {
                std::cout << "Authorization in progress..." << std::endl;
                memset(buffer, 0, BUFFER_SIZE);
                recv(s, buffer, BUFFER_SIZE, 0);
                if (strcmp(buffer, CloseConnectMess) == 0) {
                    closesocket(s);
                    rec_socks->erase(username);
                    return;
                }
                if (!(map->empty())) {
                    auto iter = (*map).find(buffer);
                    if (iter != (*map).end()) {
                        strcpy(temp, buffer);
                        strcpy(buffer, AnswerOK);
                        send(s, buffer, BUFFER_SIZE, 0);
                        memset(buffer, 0, BUFFER_SIZE);
                        recv(s, buffer, BUFFER_SIZE, 0);
                        if (strcmp(buffer, CloseConnectMess) == 0) {
                            closesocket(s);
                            rec_socks->erase(username);
                            return;
                        }
                        std::string UserResp(buffer);
                        std::string pass = iter->second;
                        if (pass == UserResp) {
                            strcpy(username, temp);
                            isLogged = true;
                            (*rec_socks)[username] = clSocket;
                            strcpy(buffer, AnswerOK);
                            send(s, buffer, BUFFER_SIZE, 0);
                        }
                        else {
                            strcpy(buffer, AnswerERR);
                            send(s, buffer, BUFFER_SIZE, 0);
                        }
                    }
                    else {
                        strcpy(buffer, AnswerERR);
                        send(s, buffer, BUFFER_SIZE, 0);
                    }
                }
                else {
                    strcpy(buffer, AnswerERR);
                    send(s, buffer, BUFFER_SIZE, 0);
                }
            }
            else if (strcmp(buffer, "REG") == 0) {
                std::cout << "Registration in progress..." << std::endl;
                memset(buffer, 0, BUFFER_SIZE);
                recv(s, buffer, BUFFER_SIZE, 0);
                if (strcmp(buffer, CloseConnectMess) == 0) {
                    closesocket(s);
                    rec_socks->erase(username);
                    return;
                }
                std::string login(buffer);
                auto iter = (*map).find(login);
                if (map->empty() || iter == (*map).end()) {
                    strcpy(username, login.c_str());
                    (*rec_socks)[username] = clSocket;
                    strcpy(buffer, AnswerOK);
                    send(s, buffer, BUFFER_SIZE, 0);
                    memset(buffer, 0, BUFFER_SIZE);
                    recv(s, buffer, BUFFER_SIZE, 0);
                    if (strcmp(buffer, CloseConnectMess) == 0) {
                        closesocket(s);
                        rec_socks->erase(username);
                        return;
                    }
                    std::string pass(buffer);
                    (*map)[login] = pass;
                    isLogged = true;
                }
                else {
                    strcpy(buffer, AnswerERR);
                    send(s, buffer, BUFFER_SIZE, 0);
                }
            }
            else {
                std::cout << "Registration or authorization error" << std::endl;
            }
        }
        if (!rec_socks->empty()) {
            const char Conn[BUFFER_SIZE] = "Connected";
            for (auto it = (*rec_socks).begin(); it != (*rec_socks).end(); ++it) {
                if (it->second != clSocket) {
                    strcpy(buffer, Conn);
                    send(it->second, buffer, BUFFER_SIZE, 0);
                    strcpy(buffer, username);
                    send(it->second, buffer, BUFFER_SIZE, 0);
                }
            }
        }
        if (!map->empty()) {
            for (auto it = (*rec_socks).begin(); it != (*rec_socks).end(); ++it) {
                if (it->second != clSocket) {
                    strcpy(buffer, it->first.c_str());
                    send(clSocket, buffer, BUFFER_SIZE, 0);
                }
            }
        }
        strcpy(buffer, endofmap);
        send(clSocket, buffer, BUFFER_SIZE, 0);
        std::string fname = std::string(username) + ".txt";
        std::ifstream ffile(fname); // Open file for reading
        if (ffile.is_open()) {
            std::string line;
            while (std::getline(ffile, line)) {
                std::getline(ffile, line);
                std::getline(ffile, line);
                strcpy(buffer, line.c_str());
                send(clSocket, buffer, BUFFER_SIZE, 0);
            }
            ffile.close();
        }
        strcpy(buffer, endoflogs);
        send(clSocket, buffer, BUFFER_SIZE, 0);
        std::string content;
        while (true) {
            std::ofstream file;
            std::string filename = std::string(username) + ".txt";
            file.open(filename, std::ios::app);
            char Receiver[BUFFER_SIZE];
            recv(s, Receiver, BUFFER_SIZE, 0);
            if (strcmp(buffer, CloseConnectMess) == 0) {
                closesocket(s);
                rec_socks->erase(username);
                return;
            }
            auto iter = (*map).find(std::string(Receiver));
            auto iterRec = (*rec_socks).find(std::string(Receiver));
            if (iter == (*map).end() || iterRec == (*rec_socks).end()) {
                strcpy(buffer, AnswerERR);
                send(s, buffer, BUFFER_SIZE, 0);
            }
            else {
                SOCKET curr_rec = (*rec_socks)[Receiver];
                strcpy(buffer, AnswerOK);
                send(s, buffer, BUFFER_SIZE, 0);
                recv(s, buffer, BUFFER_SIZE, 0);
                if (strcmp(buffer, CloseConnectMess) == 0) {
                    closesocket(s);
                    rec_socks->erase(username);
                    return;
                }
                std::string rfname = std::string(Receiver) + ".txt";
                std::ofstream rfile;
                rfile.open(rfname, std::ios::app);
                rfile << Receiver << std::endl;
                rfile << username << std::endl;
                rfile << buffer << std::endl;
                file << Receiver << std::endl;
                file << username << std::endl;
                file << buffer << std::endl;
                std::cout << "Message received: ";
                std::cout << buffer;
                std::cout << " from ";
                std::cout << username;
                std::cout << " to ";
                std::cout << Receiver << std::endl;
                std::string CorrectedStr = std::string(buffer).substr(std::string(buffer).find(']') + 1);
                std::string toSend = CorrectedStr.substr(CorrectedStr.find(":") + 1);
                send(curr_rec, toSend.c_str(), BUFFER_SIZE, 0);
                send(curr_rec, username, BUFFER_SIZE, 0);
                file.close();
                rfile.close();
            }
        }
    }
    catch (...) {
        std::cout << "Client handling error..." << std::endl;
    }
    closesocket(s);
    rec_socks->erase(username);
    std::cout << "Connection closed" << std::endl;
    return;
}

int main() {
    std::map<std::string, SOCKET> rec_socks;
    SetConsoleCtrlHandler(ExitProg, TRUE);
    std::vector<std::future<void>> clientHandlers;
    std::map<std::string, std::string> Keys;
    std::string keysFilename = "Keys.txt";
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cout << "WSAStartup error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        Sleep(3000);
        return 1;
    }
    SOCKET s;
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s == INVALID_SOCKET) {
        std::cout << "TCP socket creation error" << std::endl;
        closesocket(s);
        WSACleanup();
        Sleep(3000);
        return 1;
    }
    sockaddr_in sa{};
    sa.sin_family = AF_INET;
    sa.sin_port = htons(ServerPort);
    std::string ServerIP;
    std::cout << "Enter server IP: ";
    std::getline(std::cin, ServerIP);
    sa.sin_addr.s_addr = inet_addr(ServerIP.c_str());
    if (bind(s, (sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR) {
        std::cout << "Server setup error" << std::endl;
        std::cout << "Error: " << WSAGetLastError() << std::endl;
        closesocket(s);
        WSACleanup();
        Sleep(3000);
        return 1;
    }
    if (listen(s, 100) == SOCKET_ERROR) {
        std::cout << "Listening interrupted..." << std::endl;
        closesocket(s);
        WSACleanup();
        Sleep(3000);
        return 1;
    }
    ReadKeys(keysFilename, &Keys);
    std::cout << "Server listening on port " << ServerPort << std::endl;
    std::cout << "Server IP: ";
    std::cout << ServerIP << std::endl;
    while (true) {
        SOCKET client_sock_send = accept(s, nullptr, nullptr);
        SOCKET client_sock_receive = accept(s, nullptr, nullptr);
        if (client_sock_send == INVALID_SOCKET || client_sock_receive == INVALID_SOCKET) {
            std::cout << "Socket error" << std::endl;
        }
        else {
            clientHandlers.push_back(std::async(std::launch::async, Handle_Client, client_sock_send, &Keys, &rec_socks, client_sock_receive));
            std::cout << "Someone connected" << std::endl;
        }
    }
    return 0;
}