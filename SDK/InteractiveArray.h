#pragma once
#include "Object.h"
#include <vector>
#include "Player.h"
#include <functional>

using namespace std;

typedef unsigned int uint;

enum class IntObjType {
	Button, Lever, Field
};

class InteractiveObject : public Object {
protected:
	string name;
	IntObjType type;


	bool active = true;

	// На случай, если объект будет одноразовым
	bool oneTime = false;
public:
	InteractiveObject(Animation* text, Vector2f size, Vector2f pos, string name, IntObjType type, function<void()> use = []() {}, bool oneTime = false);

	function<void()> use;

	static Keyboard::Key interactKey;

	const string& getName() const;
	const IntObjType& getType() const;

	virtual void Update() {};

	
	bool isActive() const;;
	bool isOneTime() const;
};

class InteractiveButton : public InteractiveObject {
public:	
	InteractiveButton(Animation* text, Vector2f size, Vector2f pos, string name, function<void()> use = []() {}, bool oneTime = false);

	bool pressed = false;

	void Update() override;
};

class InteractiveLever : public InteractiveObject {

public:
	InteractiveLever(Animation* text, Vector2f size, Vector2f pos, string name, function<void()> use = []() {}, bool oneTime = false);;

	void Update() override;;
	bool on = false;
};

class InteractiveField : public InteractiveObject {
public:
	InteractiveField(Vector2f size, Vector2f pos, string name, function<void()> use = []() {}, bool oneTime = false) :
		InteractiveObject(nullptr, size, pos, name, IntObjType::Field, use, oneTime)
	{};
};

class InteractiveArray
{
private:
	Text hintText;
	float hintOpacity = 0;
	float opacityOffset = 0.4;
	
protected:
	vector<InteractiveObject*> interactives;
public:
	InteractiveArray();
	void checkInteraction(Event& ev, Player& player);

	// Отвечает за отрисовку кнопки над головой игрока при контакте с интерактивным объектом 
	void drawHint(RenderWindow& wnd, Player& player, Font& font);
	
	void addObject(InteractiveObject* obj);

	virtual void Update(Player& player) = 0;
};

