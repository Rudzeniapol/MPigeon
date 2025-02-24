#include "NativeInterop.h"
using namespace msclr::interop;

// Реализация функции
std::string ConvertToStdString(System::String^ managedString) {
    return marshal_as<std::string>(managedString);
}

System::String^ ConvertToString(std::string cppString) {
    return marshal_as<System::String^>(cppString);
}
