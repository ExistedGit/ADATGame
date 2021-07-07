#include "Files.h"


// Если открытие отменено, возвращает L""
wstring savefilename(const wchar_t* filter, HWND owner, LPCWSTR title) {

	OPENFILENAME ofn;
	wchar_t fileName[MAX_PATH] = L"C:\\Untitled.lf";
	ZeroMemory(&ofn, sizeof(ofn));



	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = owner;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = (LPWSTR)fileName;
	ofn.lpstrTitle = title;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = L"";
	//ofn.lpstrInitialDir = L"C:\\Users\\paytv\\source\\repos\\STEPGame\\STEPGame\\Maps";
	wstring fileNameStr;
	if (GetSaveFileName(&ofn))
		fileNameStr = fileName;

	return fileNameStr;
}

// Если открытие отменено, возвращает L""
wstring openfilename(const wchar_t* filter, HWND owner, LPCWSTR title) {

	OPENFILENAME ofn;
	wchar_t fileName[MAX_PATH] = L"";
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = owner;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = (LPWSTR)fileName;
	ofn.lpstrTitle = title;
	//ofn.lpstrInitialDir = L"C:\\Users\\paytv\\source\\repos\\STEPGame\\STEPGame\\Maps";
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = L"";

	wstring fileNameStr;
	if (GetOpenFileName(&ofn))
		fileNameStr = fileName;

	return fileNameStr;
}

bool fileExists(const wstring name) {
	ifstream f(name.c_str());
	return f.good();
}
