#include "InteractiveArray.h"
Keyboard::Key InteractiveObject::interactKey = Keyboard::Key::E;



InteractiveArray::InteractiveArray() {
	hintText.setString("E");
	hintText.setOrigin(Vector2f(hintText.getCharacterSize(), hintText.getCharacterSize() / 2) / 2.0f);
	hintText.setColor(Color(255, 255, 255, hintOpacity));
}

void InteractiveArray::checkInteraction(Event& ev, Player& player) {
	if (ev.type == Event::KeyReleased ||
		ev.type == Event::KeyPressed)
		for (auto& obj : interactives)
			switch (obj->getType()) {
			case IntObjType::Button: {
				auto* button = (InteractiveButton*)obj;
				if (button->isActive())
					if (ev.key.code == InteractiveObject::interactKey)
						if (player.getCollider().collides(button->getCollider())) {
							if (ev.type == Event::KeyPressed
								&& button->getCurrFrame() == 0) {
								button->Update();
								button->pressed = true;
								if (button->isOneTime()) button->use();
							}
							else if (ev.type == Event::KeyReleased
								&& button->getCurrFrame() == 1)
								button->Update();
							if (ev.type == Event::KeyReleased)
								button->use();
						}
			}
								   break;
			case IntObjType::Lever: {
				if (ev.type == Event::KeyReleased) {
					auto* lever = (InteractiveLever*)obj;
					if (lever->isActive())
						if (ev.key.code == InteractiveObject::interactKey)
							if (player.getCollider().collides(lever->getCollider())) {
								lever->Update();
								lever->use();
							}
				}
			}
								  break;
			case IntObjType::Door:
				if (ev.type == Event::KeyReleased) {
					auto* door = (InteractiveDoor*)obj;
					if (door->isActive())
						if (ev.key.code == InteractiveObject::interactKey)
							if (player.getCollider().collides(door->getCollider()))
								door->use();
							
				}
				break;
			}
}

void InteractiveArray::addObject(InteractiveObject* obj) {
	interactives.push_back(obj);
}

void InteractiveArray::drawHint(RenderWindow& wnd, Player& player, Font& font) {
	if (hintText.getFont() == nullptr ||
		hintText.getFont()->getInfo().family != font.getInfo().family)
		hintText.setFont(font);

	bool collides = false;
	for (auto& it : interactives)
		if (it->isActive())
			if (it->getCollider().collides(player.getCollider())) {
				collides = true;
				if (hintOpacity + opacityOffset < 255)
					hintOpacity += opacityOffset;
			}
	if (!collides)
		if (hintOpacity - opacityOffset > 0)
			hintOpacity -= opacityOffset;
	if (hintOpacity != 0) {
		hintText.setPosition(player.getCollider().getPosition());
		hintText.move(0, -player.getCollider().getHS().y - hintText.getCharacterSize());
		wnd.draw(hintText);
	}

	hintText.setColor(Color(hintText.getColor().r,
		hintText.getColor().g,
		hintText.getColor().b,
		hintOpacity));
}

InteractiveObject::InteractiveObject(Animation* text, const Vector2f& size, const Vector2f& pos, const string& name, const IntObjType& interactiveType, function<void()> use, bool oneTime, 
	const ObjectType& type) :
	Object(text, size, pos, ObjectType(type | Interactive)),
	name(name),
	interactiveType(interactiveType),
	use(use),
	oneTime(oneTime)
{
	body.setTextureRect(anim->uvRect);
}

const string& InteractiveObject::getName() const {
	return name;
}

const IntObjType& InteractiveObject::getType() const { return interactiveType; }


bool InteractiveObject::isOneTime() const { return oneTime; }

bool InteractiveObject::isActive() const { return active; }

InteractiveButton::InteractiveButton(Animation* text, Vector2f size, Vector2f pos, string name, function<void()> use, bool oneTime) :
	InteractiveObject(text, size, pos, name, IntObjType::Button, use, oneTime)
{}

void InteractiveButton::Update() {
	anim->Update(0, false, "default");
	body.setTextureRect(anim->uvRect);
	if (isOneTime()) active = false;
}

InteractiveLever::InteractiveLever(Animation* text, Vector2f size, Vector2f pos, string name, function<void()> use, bool oneTime) :
	InteractiveObject(text, size, pos, name, IntObjType::Lever, use, oneTime)
{}

void InteractiveLever::Update() {
	anim->Update(0, false, "default");
	body.setTextureRect(anim->uvRect);
	on = !on;

	if (isOneTime()) active = false;
}

bool InteractiveDoor::isOpen() const { return open; }
