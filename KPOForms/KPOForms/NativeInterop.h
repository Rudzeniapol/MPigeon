#pragma once
#include <string>
#include <msclr/marshal_cppstd.h>

// Функция преобразования из String^ в std::string
std::string ConvertToStdString(System::String^ managedString);
System::String^ ConvertToString(std::string cppString);
