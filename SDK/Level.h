#pragma once
#include <vector>
#include "TileMap.h"
#include "Object.h"
#include "Player.h"

using namespace std;

class Level
{
private:
	vector<pair<int, TileMap>> tileLayers;
	vector<Object> objects;

	void insertWithPriority(vector<pair<int, TileMap>>& layers, pair<int, TileMap> tmap);
	bool _bordered = false;
public:
	bool bordered() const;

	vector<Object>& getObjects();

	Level& load(const string& filename, Vector2f offset = Vector2f(0, 0), const RenderWindow* window = nullptr);

	void Draw(RenderWindow& wnd, Player* player = nullptr) const;
};

