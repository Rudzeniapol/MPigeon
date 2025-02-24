#pragma once

#include <string>
#include "TransVariables.h"
#include <winsock2.h>
#pragma warning(disable: 4996)
#include <ws2tcpip.h> // Для дополнительных сетевых функций
#pragma comment(lib, "Ws2_32.lib") // Для линковки библиотеки
#include "MyForm.h"
#include "NativeInterop.h"

namespace KPOForms {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Сводка для ServerConnectForm
	/// </summary>
	public ref class ServerConnectForm : public System::Windows::Forms::Form
	{
	public:
		SOCKET* s;
		SOCKET* rec_sock;
		SOCKADDR_IN* sa;
		ServerConnectForm(SOCKET* send, SOCKET* receive, SOCKADDR_IN* sad)
		{
			InitializeComponent();
			s = send;
			rec_sock = receive;
			sa = sad;
			//
			//TODO: добавьте код конструктора
			//
		}

	protected:
		/// <summary>
		/// Освободить все используемые ресурсы.
		/// </summary>
		~ServerConnectForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TextBox^ ServerIPTextBox;
	protected:

	protected:
	private: System::Windows::Forms::Label^ InputIPLabel;
	private: System::Windows::Forms::Label^ ErrIPLabel;
	private: System::Windows::Forms::Button^ ConnectButton;


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
			this->ServerIPTextBox = (gcnew System::Windows::Forms::TextBox());
			this->InputIPLabel = (gcnew System::Windows::Forms::Label());
			this->ErrIPLabel = (gcnew System::Windows::Forms::Label());
			this->ConnectButton = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// ServerIPTextBox
			// 
			this->ServerIPTextBox->Location = System::Drawing::Point(12, 47);
			this->ServerIPTextBox->Name = L"ServerIPTextBox";
			this->ServerIPTextBox->Size = System::Drawing::Size(383, 22);
			this->ServerIPTextBox->TabIndex = 0;
			// 
			// InputIPLabel
			// 
			this->InputIPLabel->AutoSize = true;
			this->InputIPLabel->Location = System::Drawing::Point(9, 19);
			this->InputIPLabel->Name = L"InputIPLabel";
			this->InputIPLabel->Size = System::Drawing::Size(184, 16);
			this->InputIPLabel->TabIndex = 1;
			this->InputIPLabel->Text = L"Введите IP-адрес Сервера:";
			// 
			// ErrIPLabel
			// 
			this->ErrIPLabel->AutoSize = true;
			this->ErrIPLabel->Location = System::Drawing::Point(12, 72);
			this->ErrIPLabel->Name = L"ErrIPLabel";
			this->ErrIPLabel->Size = System::Drawing::Size(44, 16);
			this->ErrIPLabel->TabIndex = 2;
			this->ErrIPLabel->Text = L"label2";
			this->ErrIPLabel->Visible = false;
			// 
			// ConnectButton
			// 
			this->ConnectButton->Location = System::Drawing::Point(12, 128);
			this->ConnectButton->Name = L"ConnectButton";
			this->ConnectButton->Size = System::Drawing::Size(383, 23);
			this->ConnectButton->TabIndex = 3;
			this->ConnectButton->Text = L"Подключиться";
			this->ConnectButton->UseVisualStyleBackColor = true;
			this->ConnectButton->Click += gcnew System::EventHandler(this, &ServerConnectForm::ConnectButton_Click);
			// 
			// ServerConnectForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(407, 163);
			this->Controls->Add(this->ConnectButton);
			this->Controls->Add(this->ErrIPLabel);
			this->Controls->Add(this->InputIPLabel);
			this->Controls->Add(this->ServerIPTextBox);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"ServerConnectForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Подключение к серверу...";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void ConnectButton_Click(System::Object^ sender, System::EventArgs^ e) {
		System::String^ serverIp = ServerIPTextBox->Text;

		// Преобразуем IP из String^ в std::string
		std::string ip = msclr::interop::marshal_as<std::string>(serverIp);
		*(this->rec_sock) = socket(AF_INET, SOCK_STREAM, 0);
		*(this->s) = socket(AF_INET, SOCK_STREAM, 0);
		*this->sa = { 0 };
		(*this->sa).sin_family = AF_INET;
		(*this->sa).sin_port = htons(ServerPort);
		(*this->sa).sin_addr.s_addr = inet_addr(ip.c_str());
		if (connect(*this->s, (const sockaddr*)(this->sa), sizeof(*this->sa)) != 0) {
			ErrIPLabel->Text = "Невозможно подключится к этому IP";
			ErrIPLabel->Visible = true;
		}
		else {
			connect(*this->rec_sock, (const sockaddr*)(this->sa), sizeof(*this->sa));
			this->DialogResult = System::Windows::Forms::DialogResult::OK;
			this->Close();
		}
	}
};
}
