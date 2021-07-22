#pragma once
#include "Object.h"
#include <vector>
#include <Player.h>

using namespace std;

typedef unsigned int uint;

enum class IntObjType {
	Button
};

class InteractiveObject : public Object {
protected:
	string name;
	IntObjType type;
	Animation anim;
	uint row = 0;
public:
	InteractiveObject(Texture* text, Vector2f size, Vector2f pos, Keyboard::Key interactKey = Keyboard::E, string name = "InteractiveObject");
	Keyboard::Key interactKey = Keyboard::E;

	bool active = true;

	const string& getName() const;
	const IntObjType& getType() const;

	virtual void Update() = 0;

	uint getRow() const;
};

class InteractiveButton : public InteractiveObject {
public:	
	InteractiveButton(Texture* text, Vector2f size, Vector2f pos, Keyboard::Key interactKey = Keyboard::E, string name = "InteractiveObject");

	bool pressed = false;

	void Update() override;
};

class InteractiveArray
{
protected:
	vector<InteractiveObject*> interactives;
public:

	InteractiveObject* checkInteraction(Event& ev, Player& player);
	
	void addObject(InteractiveObject* obj);

	virtual void Update(Player& player) = 0;
};

