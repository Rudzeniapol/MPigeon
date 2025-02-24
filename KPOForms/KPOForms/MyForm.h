#pragma once

#include <string>
#include "TransVariables.h"
#include <winsock2.h>
#include <thread>
#include <algorithm> // Для std::find
#include <future>
#include <chrono>
#include <vector>
#pragma warning(disable: 4996)
#include <ws2tcpip.h> // Для дополнительных сетевых функций
#pragma comment(lib, "Ws2_32.lib") // Для линковки библиотеки
#include "MyForm.h"
#include "NativeInterop.h"

extern std::vector<std::string> Names;

namespace KPOForms {



	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	/// <summary>
	/// Сводка для MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
		SOCKET sock_send;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::Label^ label3;
		   SOCKET sock_recv;
	public:
		MyForm(SOCKET send, SOCKET recv)
		{
			InitializeComponent();
			sock_send = send;
			sock_recv = recv;
			//
			//TODO: добавьте код конструктора
			//
		}

	protected:
		/// <summary>
		/// Освободить все используемые ресурсы.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TextBox^ InputMsgBox;
	protected:

	protected:

	private: System::Windows::Forms::Button^ SendMsgButton;
	private: System::Windows::Forms::ListBox^ UsersList;
	private: System::Windows::Forms::TextBox^ MessagesBox;

	protected:




	private:
		/// <summary>
		/// Обязательная переменная конструктора.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Требуемый метод для поддержки конструктора — не изменяйте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(MyForm::typeid));
			this->InputMsgBox = (gcnew System::Windows::Forms::TextBox());
			this->SendMsgButton = (gcnew System::Windows::Forms::Button());
			this->UsersList = (gcnew System::Windows::Forms::ListBox());
			this->MessagesBox = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// InputMsgBox
			// 
			this->InputMsgBox->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->InputMsgBox->ForeColor = System::Drawing::SystemColors::WindowFrame;
			this->InputMsgBox->Location = System::Drawing::Point(12, 472);
			this->InputMsgBox->Name = L"InputMsgBox";
			this->InputMsgBox->Size = System::Drawing::Size(876, 22);
			this->InputMsgBox->TabIndex = 0;
			// 
			// SendMsgButton
			// 
			this->SendMsgButton->Location = System::Drawing::Point(894, 454);
			this->SendMsgButton->Name = L"SendMsgButton";
			this->SendMsgButton->Size = System::Drawing::Size(94, 40);
			this->SendMsgButton->TabIndex = 1;
			this->SendMsgButton->Text = L"Отправить";
			this->SendMsgButton->UseVisualStyleBackColor = true;
			this->SendMsgButton->Click += gcnew System::EventHandler(this, &MyForm::SendMsgButton_Click);
			// 
			// UsersList
			// 
			this->UsersList->FormattingEnabled = true;
			this->UsersList->ItemHeight = 16;
			this->UsersList->Location = System::Drawing::Point(12, 28);
			this->UsersList->Name = L"UsersList";
			this->UsersList->Size = System::Drawing::Size(284, 420);
			this->UsersList->TabIndex = 2;
			// 
			// MessagesBox
			// 
			this->MessagesBox->Location = System::Drawing::Point(302, 28);
			this->MessagesBox->Multiline = true;
			this->MessagesBox->Name = L"MessagesBox";
			this->MessagesBox->ReadOnly = true;
			this->MessagesBox->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->MessagesBox->Size = System::Drawing::Size(686, 420);
			this->MessagesBox->TabIndex = 3;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(12, 7);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(103, 16);
			this->label1->TabIndex = 4;
			this->label1->Text = L"Пользователи";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(299, 7);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(118, 16);
			this->label2->TabIndex = 5;
			this->label2->Text = L"Ваши сообщения:";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(12, 454);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(138, 16);
			this->label3->TabIndex = 6;
			this->label3->Text = L"Введите сообщение";
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::ActiveCaption;
			this->ClientSize = System::Drawing::Size(1000, 500);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->MessagesBox);
			this->Controls->Add(this->UsersList);
			this->Controls->Add(this->SendMsgButton);
			this->Controls->Add(this->InputMsgBox);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->Name = L"MyForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Месенджер";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &MyForm::MyForm_FormClosing);
			this->Load += gcnew System::EventHandler(this, &MyForm::MyForm_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
		void UpdateUI(System::String^ message) {
			if (this->InvokeRequired) {
				// Если вызывается не из главного потока, используем Invoke для обновления UI
				this->Invoke(gcnew Action<System::String^>(this, &MyForm::UpdateUI), message);
			}
			else {
				// Обновляем UI в главном потоке
				this->MessagesBox->AppendText(message + "\r\n");
			}
		}

		private: System::Void AddMessageToTextBox(String^ message) {
			if (this->MessagesBox->InvokeRequired) {
				this->MessagesBox->Invoke(gcnew Action<String^>(this, &MyForm::AddMessageToTextBox), message);
			}
			else {
				this->MessagesBox->AppendText(message + "\r\n"); // Добавляем сообщение и перенос строки
			}
		}

	public: void HandleMessagesAsync() {
		char buffer[BUFFER_SIZE];
		SOCKET rec_sock = this->sock_recv;
		fd_set readfds;
		timeval timeout = { 1, 0 }; // Таймаут 1 секунда
		while (true) {
			FD_ZERO(&readfds);
			FD_SET(rec_sock, &readfds);
			if (FD_ISSET(rec_sock, &readfds)) {
				recv(rec_sock, buffer, BUFFER_SIZE, 0);
				std::string message(buffer);
				recv(rec_sock, buffer, BUFFER_SIZE, 0);
				std::string receiver(buffer);
				auto it = std::find(Names.begin(), Names.end(), receiver);
				if (it == Names.end()) {
					Names.push_back(receiver);
					this->UsersList->Invoke(gcnew Action<String^>(this, &MyForm::AddUserToList), ConvertToString(receiver));
				}
				String^ formattedMessage = "[" + DateTime::Now.ToString("HH:mm:ss") + "]" + ConvertToString(receiver) + ": " + ConvertToString(message);
				this->Invoke(gcnew Action<String^>(this, &MyForm::AddMessageToTextBox), formattedMessage);
			}
		}
		closesocket(rec_sock);
	}

	private: System::Void AddUserToList(String^ user) {
		this->UsersList->Items->Add(user);
	}

	private: System::Void MyForm_Load(System::Object^ sender, System::EventArgs^ e) {
		char buffer[BUFFER_SIZE];
		do {
			recv(this->sock_recv, buffer, BUFFER_SIZE, 0);
			if (strcmp(buffer, "ENDOFMAP") != 0) {
				Names.push_back(std::string(buffer));
				this->UsersList->Invoke(gcnew Action<String^>(this, &MyForm::AddUserToList), ConvertToString(std::string(buffer)));
			}
			else {
				break;
			}
		} while (strcmp(buffer, "ENDOFMAP") != 0);
		do {
			recv(this->sock_recv, buffer, BUFFER_SIZE, 0);
			if (strcmp(buffer, "ENDOFLOGS") != 0) {
				this->Invoke(gcnew Action<String^>(this, &MyForm::AddMessageToTextBox), ConvertToString(std::string(buffer)));
			}
			else {
				break;
			}
		} while (strcmp(buffer, "ENDOFLOGS") != 0);
		System::Threading::Thread^ messageThread = gcnew System::Threading::Thread(gcnew System::Threading::ThreadStart(this, &MyForm::HandleMessagesAsync));
		messageThread->IsBackground = true; // Позволяет завершить поток при закрытии формы
		messageThread->Start();
	}

	private: System::Void SendMsgButton_Click(System::Object^ sender, System::EventArgs^ e) {
		SOCKET s = this->sock_send;
		char buffer[BUFFER_SIZE];
		if (dynamic_cast<String^>(this->UsersList->SelectedItem) != nullptr) {
			send(s, ConvertToStdString(dynamic_cast<String^>(this->UsersList->SelectedItem)).c_str(), BUFFER_SIZE, 0);
			recv(s, buffer, BUFFER_SIZE, 0);
			if (strcmp(buffer, AnswerOK) == 0) {
				String^ formattedMessage = "[" + DateTime::Now.ToString("HH:mm:ss") + "]" + "BbI->" + this->UsersList->SelectedItem + ": " + this->InputMsgBox->Text;
				send(s, ConvertToStdString(formattedMessage).c_str(), BUFFER_SIZE, 0);
				this->InputMsgBox->Text = "";
				AddMessageToTextBox(formattedMessage);
			}
			else {
				MessageBox::Show("Данный пользователь недоступен :(");
			}
		}
		else {
			MessageBox::Show("Вы не выбрали пользователя, которому адресовано сообщение!");
		}
		
	}
	private: System::Void MyForm_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e) {
		send(sock_send, CloseConnectMess, BUFFER_SIZE, 0);
	}
};
}
