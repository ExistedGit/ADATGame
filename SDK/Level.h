#pragma once
#include <vector>
#include "TileMap.h"
#include "Object.h"
#include "Player.h"
#include "InteractiveArray.h"


class Level : public InteractiveArray
{
private:
	vector<pair<int, TileMap>> tileLayers;
	map<string, Object*> namedObjects;
	vector<Object*> objects;
	Vector2f size;
	string name;

	string filename;

	void insertWithPriority(vector<pair<int, TileMap>>& layers, pair<int, TileMap> tmap);
	bool _bordered = false;

	static Font debugFont;

public:
	Vector2f spawn;
	static bool debug;

	Level();
	
	bool bordered() const;
	
	const Vector2f& getSize() const noexcept;
	
	
	Level& load(string xmlDoc, const RenderWindow* window = nullptr, const string& name = "LEVEL", map<string, function<void()>> useMap = {});
	void reload();;

	void checkCollision(Player& player, float deltaTime);

	const string& getName()const;

	void Draw(RenderWindow& wnd, Player* player = nullptr) const;
	
	void applyUseMap(map<string, function<void()>> map);
	
	void Update(Player& player) override;
	void Update(float deltaTime);
};

