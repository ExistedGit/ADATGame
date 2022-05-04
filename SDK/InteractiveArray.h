#pragma once
#include "Object.h"
#include <vector>
#include "Player.h"
#include <functional>

using namespace std;

typedef unsigned int uint;

enum class IntObjType : int {
	Button, Lever, Field, Door
};

class InteractiveObject : public Object {
protected:
	string name;
	IntObjType interactiveType;
	
	// На случай, если объект будет одноразовым
	bool oneTime = false;
public:
	bool interactive = true;
	InteractiveObject(Animation* text, const Vector2f& size, const Vector2f& pos, const string& name, const IntObjType& interactiveType, function<void()> use = []() {}, bool oneTime = false, 
		bool interactive = true, const ObjectType& type = Interactive);

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
	inline InteractiveField(Vector2f size, Vector2f pos, string name, function<void()> use = []() {}, bool oneTime = false) :
		InteractiveObject(nullptr, size, pos, name, IntObjType::Field, use, oneTime)
	{};
};

class InteractiveDoor : public InteractiveObject {
private:
	bool open = false;
	RectangleShape hitbox;
public:
	inline InteractiveDoor(Animation* text, RectangleShape hitbox, Vector2f size, Vector2f pos, string name, bool oneTime, bool interactive = true) :
		InteractiveObject(text, size, pos, name, IntObjType::Door, [this]() {Update(); }, oneTime, interactive)
	{
		this->hitbox = RectangleShape(hitbox.getSize());
		this->hitbox.setPosition(
			Vector2f(
				body.getPosition().x - body.getSize().x/2,
				body.getPosition().y - body.getSize().y / 2
			)
		);
		Update();
		// Не спрашивайте, как были выведены коэфициенты 2 и 3
		// проверено экспериментами — значит работает
		this->hitbox.move(hitbox.getPosition().x*2, hitbox.getPosition().y * 3);
	}
	inline void Update() override {
		anim->Update(0, false, "default");
		body.setTextureRect(anim->uvRect);
		open = !open; type = ObjectType(type ^ Solid);
		
	}
	inline Collider getHitbox(){
		return Collider(hitbox);
	}
	bool isOpen()const;
};



class InteractiveArray
{
private:
	Text hintText;
	float hintOpacity = 0;
	float opacityOffset = 0.4;
	
protected:
	vector<shared_ptr<InteractiveObject>> interactives;
public:
	InteractiveArray();
	void checkInteraction(Event& ev, Player& player);

	// Отвечает за отрисовку кнопки над головой игрока при контакте с интерактивным объектом 
	void drawHint(RenderWindow& wnd, Player& player, Font& font);
	
	void addObject(shared_ptr<InteractiveObject> obj);

	virtual void Update(Player& player) = 0;
};

