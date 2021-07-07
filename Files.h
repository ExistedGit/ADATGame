#pragma once

#include <Windows.h>
#include <fstream>
#include <string>

using namespace std;

// Если открытие отменено, возвращает L""
wstring savefilename(const wchar_t* filter = L"All Files (*.*)\0*.*\0", HWND owner = NULL, LPCWSTR title = L"Сохранить файл...");

// Если открытие отменено, возвращает L""
wstring openfilename(const wchar_t* filter = L"All Files (*.*)\0*.*\0", HWND owner = NULL, LPCWSTR title = L"Открыть файл...");

bool fileExists(const wstring name);