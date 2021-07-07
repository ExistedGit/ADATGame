#pragma once

#include <Windows.h>
#include <conio.h>
#include <iomanip>
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <SFML/Audio.hpp>
#include "Files.h"

using namespace std;
using namespace sf;

using std::filesystem::current_path;
const wstring dir = current_path().wstring();

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77



class SoundMatrix
{
private:
	struct Coord {
		int x;
		int y;
		Coord(int _x = 0, int _y = 0) {
			x = _x;
			y = _y;
		}
	};

	bool** _matrix;

	int row, col;

	int tempo;

	const int offset = 8; // Пространственный сдвиг визуальной части меню(0 и 1)

	vector<SoundBuffer> buffers;
	vector<Sound> sounds;
public:
	enum ConsoleColor
	{
		Black = 0,
		Blue = 1,
		Green = 2, Cyan = 3, Red = 4, Magenta = 5, Brown = 6, LightGray = 7, DarkGray = 8,
		LightBlue = 9, LightGreen = 10, LightCyan = 11, LightRed = 12, LightMagenta = 13, Yellow = 14, White = 15
	};

	void SetColor(int text = 7, int background = 0) {
		HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
	};

	void gotoxy(int x, int y)
	{
		COORD coord;
		coord.X = x;
		coord.Y = y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	}

	void ShowConsoleCursor(bool showFlag)
	{
		HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

		CONSOLE_CURSOR_INFO     cursorInfo;

		GetConsoleCursorInfo(out, &cursorInfo);
		cursorInfo.bVisible = showFlag; // set the cursor visibility
		SetConsoleCursorInfo(out, &cursorInfo);
	}

	SoundMatrix(int _row = 4, int _col = 40) {
		row = _row, col = _col;

		_matrix = new bool* [row];
		for (int i = 0; i < row; i++) {
			_matrix[i] = new bool[col];
			for (int j = 0; j < col; j++) {
				_matrix[i][j] = false;
			}
		}
		tempo = 0;

		vector<string> filenames = { dir + "\\Sounds\\kick.wav", dir + "\\Sounds\\hihat.wav", dir + "\\Sounds\\click.wav", dir + "\\Sounds\\clap.wav" };
		
		for (int i = 0; i < filenames.size(); i++) {
			SoundBuffer buff;
			buff.loadFromFile(filenames[i]);
			buffers.push_back(buff);
		}

		
		for (int i = 0; i < buffers.size(); i++) {
			sounds.push_back(Sound(buffers[i]));
		}
	}

	void clear() {
		for (int i = 0; i < row; i++) {
			for (int j = 0; j < col; j++) {
				_matrix[i][j] = false;
			}
		}
	}

	void play() {
		

		bool interrupted = false;
		int j;
		while (!interrupted) {
			for (int i = 0; i < col; i++) {
				for (j = 0; j < row; j++) {
					// Выставляем проигранным колонкам изначальный цвет
					if (i > 0) {
						gotoxy(i - 1 + offset + (i != 0) * i, j);
						SetColor();
						cout << (int)_matrix[j][i - 1];
					}

					if (_matrix[j][i])sounds[j].play();

					// Передвижение красной "каретки"
					gotoxy(i + offset + 1 + (i != 0) * i, j);
					SetColor(0, Red);
					cout << (int)_matrix[j][i];
					SetColor();
				}
				// В случае нажатия кнопки прогрыща ещё раз останавливаем всё это дело
				if (_kbhit()) {
					char c = _getch();
					if (tolower(c) == 'e') {
						for (int k = 0; k < row; k++) {
							gotoxy(i + 1 + offset + (i != 0) * i, k);
							SetColor();
							cout << (int)_matrix[k][i];
						}
						interrupted = true;
						break;
					}
				}


				Sleep(100 - tempo);
			}
		}

		for (int i = 0; i < row; i++) {
			gotoxy(col-1 + offset + (col != 0) * col, i);
			SetColor();
			cout << (int)_matrix[i][col-1];
		}

	}

	void displayTempo() {
		gotoxy(0, row + 2);
		SetColor();
		cout << "                                            ";
		gotoxy(0, row + 2);
		cout << "Темп: " << tempo + 100;
	
	}

	void saveToFile() {
			wstring fileName = savefilename(L"LootyFroops(*.lf)\0*.lf\0Text(*.txt)\0*.txt\0");

			if (fileExists(fileName))
			{
				if (MessageBox(GetConsoleWindow(), L"Этот файл уже существует. Вы точно хотите его перезаписать?", L"Внимание!", MB_ICONWARNING | MB_YESNO) == IDNO) return;
			}

			ofstream fout(fileName);
			fout << tempo << endl;
			for (int i = 0; i < row; i++) {
				for (int j = 0; j < col; j++) {
					fout << (int)_matrix[i][j];
				}
				fout << endl;
			}
	}

	void loadFromFile(wstring fileName = L"") {
		if(fileName == L"") fileName = openfilename(L"LootyFroops(*.lf)\0*.lf\0Text(*.txt)\0*.txt\0");

		if (!fileExists(fileName)) return;

	

		char buff[200];
		
		ifstream fin(fileName);
		if (fin >> tempo) {
			fin.ignore();
			int i = 0;
			while (fin.getline(buff, 200)) {
				for (int j = 0; j < col; j++) {
					_matrix[i][j] = buff[j] == '1';
				}
				i++;
			}
			system("cls");
			drawTracks();
		}
	}

	void drawTracks() {
		vector<string> soundTypes = { "Kick", "Hi-Hat", "Snare", "Clap" };
		for (int i = 0; i < row; i++) {
			cout << setw(offset - 1) << soundTypes[i] << ": ";
			for (int j = 0; j < col; j++) {
				cout << (int)_matrix[i][j] << " ";
			}
			cout << endl;
		}

	}

	void menu() {
		
		drawTracks();
		

		bool on = true;
		int posX = 0, posY = 0;
		Coord oldPos = { 0, 0 };
		char c;
		while (on) {
			displayTempo();

			ShowConsoleCursor(false);

			gotoxy(oldPos.x + offset + 1 + (oldPos.x != 0) * oldPos.x, oldPos.y);
			SetColor();
			cout << _matrix[oldPos.y][oldPos.x];

			gotoxy(posX + offset + 1 + (posX != 0) * posX, posY);
			SetColor(0, 15);
			cout << _matrix[posY][posX];

			SetColor();

			c = _getch();
			oldPos = { posX, posY };

			switch (c) {

			case KEY_UP:
				if (posY > 0) posY--;
				break;
			case KEY_DOWN:
				if (posY < row - 1) posY++;
				break;
			case KEY_LEFT:
				if (posX > 0) posX--;
				break;
			case KEY_RIGHT:
				if (posX < col - 1) posX++;
				break;
			case VK_SPACE:
				_matrix[posY][posX] = !_matrix[posY][posX];
				break;
			case 'E': case 'e':
				play();
				break;
			case '[':
				if(tempo > -100) tempo--;
				break;
			case ']':
				if(tempo < 100) tempo++;
				break;
			case 'S': case 's':
				saveToFile();
				break;
			case 'O': case 'o':
				loadFromFile();
				break;
			case 'C': case 'c':
				clear();
				system("cls");
				drawTracks();
				break;
			case VK_ESCAPE:
				on = false;
				break;
			default:
				break;
			}
		}
	}
};

