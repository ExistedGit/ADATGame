#pragma once

#include <Windows.h>
#include <fstream>
#include <string>

using namespace std;

// ���� �������� ��������, ���������� L""
wstring savefilename(const wchar_t* filter = L"All Files (*.*)\0*.*\0", HWND owner = NULL, LPCWSTR title = L"��������� ����...");

// ���� �������� ��������, ���������� L""
wstring openfilename(const wchar_t* filter = L"All Files (*.*)\0*.*\0", HWND owner = NULL, LPCWSTR title = L"������� ����...");

bool fileExists(const wstring name);