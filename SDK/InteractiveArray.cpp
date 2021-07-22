#include "InteractiveArray.h"

InteractiveObject* InteractiveArray::checkInteraction(Event& ev, Player& player) {

	if (ev.type == Event::KeyReleased ||
		ev.type == Event::KeyPressed)
		for (auto& obj : interactives)
			if (obj->getType() == IntObjType::Button) {
				auto* button = (InteractiveButton*)obj;
				if (button->active)
					if (ev.key.code == button->interactKey)
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

	return nullptr;
}

void InteractiveArray::addObject(InteractiveObject* obj) {
	interactives.push_back(obj);
}

InteractiveObject::InteractiveObject(Texture* text, Vector2f size, Vector2f pos, Keyboard::Key interactKey, string name) :
	Object(text, size, pos),
	interactKey(interactKey),
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

InteractiveButton::InteractiveButton(Texture* text, Vector2f size, Vector2f pos, Keyboard::Key interactKey, string name) :
	InteractiveObject(text, size, pos, interactKey, name) {
	type = IntObjType::Button;
}

void InteractiveButton::Update() {
	anim.Update((anim.getRow() == 0 ? 1 : 0), 0, false);
	body.setTextureRect(anim.uvRect);
}
