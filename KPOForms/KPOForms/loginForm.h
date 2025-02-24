#pragma once

#include <string>
#include "TransVariables.h"
#include <winsock2.h>
#include <thread>
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
	/// Сводка для loginForm
	/// </summary>
	public ref class loginForm : public System::Windows::Forms::Form
	{
	public:
		SOCKET* sock;
	private: System::Windows::Forms::Label^ ErrLabel;
	private: System::Windows::Forms::Button^ RegisterButton;

	public:
		char* username;
		loginForm(SOCKET* send)
		{
			InitializeComponent();
			sock = send;
			username = new char[BUFFER_SIZE];
			//
			//TODO: добавьте код конструктора
			//
		}

	protected:
		/// <summary>
		/// Освободить все используемые ресурсы.
		/// </summary>
		~loginForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::TextBox^ UsernameTextBox;
	protected:

	private: System::Windows::Forms::TextBox^ PasswordTextBox;
	private: System::Windows::Forms::Label^ UsernameLabel;
	private: System::Windows::Forms::Label^ PasswordLabel;
	private: System::Windows::Forms::Button^ LoginButton;
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
			System::ComponentModel::ComponentResourceManager^ resources = (gcnew System::ComponentModel::ComponentResourceManager(loginForm::typeid));
			this->UsernameTextBox = (gcnew System::Windows::Forms::TextBox());
			this->PasswordTextBox = (gcnew System::Windows::Forms::TextBox());
			this->UsernameLabel = (gcnew System::Windows::Forms::Label());
			this->PasswordLabel = (gcnew System::Windows::Forms::Label());
			this->LoginButton = (gcnew System::Windows::Forms::Button());
			this->ErrLabel = (gcnew System::Windows::Forms::Label());
			this->RegisterButton = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// UsernameTextBox
			// 
			this->UsernameTextBox->Location = System::Drawing::Point(12, 32);
			this->UsernameTextBox->Name = L"UsernameTextBox";
			this->UsernameTextBox->Size = System::Drawing::Size(382, 22);
			this->UsernameTextBox->TabIndex = 0;
			// 
			// PasswordTextBox
			// 
			this->PasswordTextBox->Location = System::Drawing::Point(12, 85);
			this->PasswordTextBox->Name = L"PasswordTextBox";
			this->PasswordTextBox->Size = System::Drawing::Size(382, 22);
			this->PasswordTextBox->TabIndex = 1;
			// 
			// UsernameLabel
			// 
			this->UsernameLabel->AutoSize = true;
			this->UsernameLabel->Location = System::Drawing::Point(13, 13);
			this->UsernameLabel->Name = L"UsernameLabel";
			this->UsernameLabel->Size = System::Drawing::Size(129, 16);
			this->UsernameLabel->TabIndex = 2;
			this->UsernameLabel->Text = L"Имя пользователя";
			// 
			// PasswordLabel
			// 
			this->PasswordLabel->AutoSize = true;
			this->PasswordLabel->Location = System::Drawing::Point(12, 66);
			this->PasswordLabel->Name = L"PasswordLabel";
			this->PasswordLabel->Size = System::Drawing::Size(56, 16);
			this->PasswordLabel->TabIndex = 3;
			this->PasswordLabel->Text = L"Пароль";
			// 
			// LoginButton
			// 
			this->LoginButton->Location = System::Drawing::Point(224, 145);
			this->LoginButton->Name = L"LoginButton";
			this->LoginButton->Size = System::Drawing::Size(166, 36);
			this->LoginButton->TabIndex = 4;
			this->LoginButton->Text = L"Войти";
			this->LoginButton->UseVisualStyleBackColor = true;
			this->LoginButton->Click += gcnew System::EventHandler(this, &loginForm::LoginButton_Click);
			// 
			// ErrLabel
			// 
			this->ErrLabel->AutoSize = true;
			this->ErrLabel->Location = System::Drawing::Point(13, 119);
			this->ErrLabel->Name = L"ErrLabel";
			this->ErrLabel->RightToLeft = System::Windows::Forms::RightToLeft::Yes;
			this->ErrLabel->Size = System::Drawing::Size(44, 16);
			this->ErrLabel->TabIndex = 5;
			this->ErrLabel->Text = L"label1";
			this->ErrLabel->Visible = false;
			// 
			// RegisterButton
			// 
			this->RegisterButton->Location = System::Drawing::Point(12, 145);
			this->RegisterButton->Name = L"RegisterButton";
			this->RegisterButton->Size = System::Drawing::Size(166, 36);
			this->RegisterButton->TabIndex = 6;
			this->RegisterButton->Text = L"Зарегистрироваться";
			this->RegisterButton->UseVisualStyleBackColor = true;
			this->RegisterButton->Click += gcnew System::EventHandler(this, &loginForm::RegisterButton_Click);
			// 
			// loginForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(402, 193);
			this->Controls->Add(this->RegisterButton);
			this->Controls->Add(this->ErrLabel);
			this->Controls->Add(this->LoginButton);
			this->Controls->Add(this->PasswordLabel);
			this->Controls->Add(this->UsernameLabel);
			this->Controls->Add(this->PasswordTextBox);
			this->Controls->Add(this->UsernameTextBox);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^>(resources->GetObject(L"$this.Icon")));
			this->Name = L"loginForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Окно Авторизации";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void LoginButton_Click(System::Object^ sender, System::EventArgs^ e) {
		String^ Username = UsernameTextBox->Text;
		String^ Password = PasswordTextBox->Text;
		std::string login = msclr::interop::marshal_as<std::string>(Username);
		std::string password = msclr::interop::marshal_as<std::string>(Password);
		SOCKET s = *(this->sock);
		char buffer[BUFFER_SIZE];
		memset(buffer, 0, BUFFER_SIZE);
		strcpy(buffer, "LOG");
		send(s, buffer, BUFFER_SIZE, 0);
		strcpy(buffer, login.c_str());
		send(s, buffer, BUFFER_SIZE, 0);
		recv(s, buffer, BUFFER_SIZE, 0);
		if (strcmp(buffer, AnswerOK) == 0) {
			strcpy(buffer, password.c_str());
			send(s, buffer, BUFFER_SIZE, 0);
			recv(s, buffer, BUFFER_SIZE, 0);
			if (strcmp(buffer, AnswerOK) == 0) {
				strcpy(this->username, login.c_str());
				this->DialogResult = System::Windows::Forms::DialogResult::OK;
				this->Close();
			}
			else {
				ErrLabel->Text = "Неверное имя или пароль";
				ErrLabel->Visible = true;
			}
		}
		else {
			ErrLabel->Text = "Неверное имя или пароль";
			ErrLabel->Visible = true;
		}
	}

private: System::Void RegisterButton_Click(System::Object^ sender, System::EventArgs^ e) {
	String^ Username = UsernameTextBox->Text;
	String^ Password = PasswordTextBox->Text;
	std::string login = msclr::interop::marshal_as<std::string>(Username);
	std::string password = msclr::interop::marshal_as<std::string>(Password);
	SOCKET s = *(this->sock);
	char buffer[BUFFER_SIZE];
	strcpy(buffer, "REG");
	send(s, buffer, BUFFER_SIZE, 0);
	strcpy(buffer, login.c_str());
	send(s, buffer, BUFFER_SIZE, 0);
	recv(s, buffer, BUFFER_SIZE, 0);
	if (strcmp(buffer, AnswerOK) == 0) {
		strcpy(buffer, password.c_str());
		send(s, buffer, BUFFER_SIZE, 0);
		strcpy(this->username, login.c_str());
		this->DialogResult = System::Windows::Forms::DialogResult::OK;
		this->Close();
	}
	else {
		ErrLabel->Text = "Пользователь с таким именем уже существует";
		ErrLabel->Visible = true;
	}
}
};
}
