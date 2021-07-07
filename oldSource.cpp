//#pragma once
//
//#include "SoundMatrix.h"
//
//
//using namespace std;
//
//std::wstring s2ws(const std::string& str)
//{
//	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
//	std::wstring wstrTo(size_needed, 0);
//	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
//	return wstrTo;
//}
//
//int main(int argc, char** argv)
//{
//	//cout.setf(ios::boolalpha);
//	SetConsoleCP(1251);
//	SetConsoleOutputCP(1251);
//	SetConsoleTitle(L"Фрутилупсы по скидке");
//	SoundMatrix sm;
//	//argv = new char* [] {argv[0], new char[] {"В"}};
//	if (argc > 1) {
//		sm.loadFromFile(s2ws((const char*)argv[1]));
//	}
//	sm.SetColor();
//	sm.menu();
//}