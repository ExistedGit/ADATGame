#include "InteractiveArray.h"
Keyboard::Key InteractiveObject::interactKey = Keyboard::Key::E;

InteractiveObject* InteractiveArray::checkInteraction(Event& ev, Player& player) {
	if (ev.type == Event::KeyReleased ||
		ev.type == Event::KeyPressed)
		for (auto& obj : interactives) {
			if (obj->getType() == IntObjType::Button) {
				auto* button = (InteractiveButton*)obj;
				if (button->isActive())
					if (ev.key.code == InteractiveObject::interactKey)
						if (player.getCollider().collides(button->getCollider())) {
							if (ev.type == Event::KeyPressed
								&& button->getRow() == 0) {
								button->Update();
								button->pressed = true;
							}
							else if (ev.type == Event::KeyReleased
								&& button->getRow() == 1) {
								button->Update();
							}
							return (InteractiveObject*)button;
						}
			}
			else if (obj->getType() == IntObjType::Lever) {
				auto* lever = (InteractiveLever*)obj;
				if (lever->isActive())
					if (ev.key.code == InteractiveObject::interactKey)
						if (player.getCollider().collides(lever->getCollider())) {
							if (ev.type == Event::KeyReleased)
								lever->Update();
							
							return (InteractiveObject*)lever;
						}
			}
		}
	return nullptr;
}

void InteractiveArray::addObject(InteractiveObject* obj) {
	interactives.push_back(obj);
}

void InteractiveArray::drawHint(Player& player, RenderWindow& wnd, Font& font) {
	if(hintText.getFont() == nullptr || 
		hintText.getFont()->getInfo().family != font.getInfo().family)
		hintText.setFont(font);

	bool collides = false;
	for (auto& it : interactives)
		if (it->isActive())
			if (it->getCollider().collides(player.getCollider())) {
				collides = true;
				if(hintOpacity + opacityOffset < 255)
					hintOpacity += opacityOffset;
				}

	hintText.setPosition(player.getCollider().getPosition());
	hintText.move(0, -player.getCollider().getHS().y - hintText.getCharacterSize());
	wnd.draw(hintText);
	
	if (!collides)
		if (hintOpacity - opacityOffset > 0)
			hintOpacity -= opacityOffset;

	hintText.setColor(Color(hintText.getColor().r,
		hintText.getColor().g,
		hintText.getColor().b,
		hintOpacity));
}

InteractiveObject::InteractiveObject(Texture* text, Vector2f size, Vector2f pos, string name, IntObjType type) :
	Object(text, size, pos),
	name(name),
	type(type),
	anim(text, Vector2u(1, 2), 0)
{
	body.setTextureRect(anim.uvRect);
}

const string& InteractiveObject::getName() const {
	return name;
}

const IntObjType& InteractiveObject::getType() const { return type; }

uint InteractiveObject::getRow() const {
	return anim.getRow();
}

bool InteractiveObject::isOneTime() const { return oneTime; }

bool InteractiveObject::isActive() const { return active; }

InteractiveButton::InteractiveButton(Texture* text, Vector2f size, Vector2f pos, string name) :
	InteractiveObject(text, size, pos, name, IntObjType::Button)
{}

void InteractiveButton::Update() {
	anim.Update((anim.getRow() == 0 ? 1 : 0), 0, false);
	body.setTextureRect(anim.uvRect);
	if (isOneTime()) active = false;
}

InteractiveLever::InteractiveLever(Texture* text, Vector2f size, Vector2f pos, string name) :
	InteractiveObject(text, size, pos, name, IntObjType::Lever)
{	}

void InteractiveLever::Update() {
	anim.Update((anim.getRow() == 0 ? 1 : 0), 0, false);
	body.setTextureRect(anim.uvRect);
	on = !on;

	if (isOneTime()) active = false;
}
