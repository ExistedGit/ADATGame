#include "World.h"

const Clock cl;

#define VIEW_HEIGHT 1080

int main() {
	SetConsoleCP(65001); SetConsoleOutputCP(65001);
	cout.setf(ios::boolalpha);
	
	World mainWorld(Vector2f(1366, 768));
	mainWorld.start();


	
	
	//levels[1].applyUseMap({
	//		{"lever", [&viewCentered]() mutable {
	//			viewCentered = !viewCentered;
	//		}
	//		}
	//	});
	
	return 0;
}