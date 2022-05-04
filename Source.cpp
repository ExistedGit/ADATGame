#include "World.h"

const Clock cl;

#define VIEW_HEIGHT 1080

int main() {
	SetConsoleCP(65001); SetConsoleOutputCP(65001);
	cout.setf(ios::boolalpha);

	system("start IntroADAT.mp4");
	Sleep(19000);

	World mainWorld(Vector2f(1366, 768));
	mainWorld.start();
	
	return 0;
}