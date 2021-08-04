#pragma once
#include "Button.h"
#include "tinyxml.h"
#include "Animation.h"
#include <sstream>

class Menu : public IButtonArray<HoverButton>
{
private:
	Vector2f pos;
	unsigned int distance = 0;
	bool centered;
public:
	inline Menu(const Vector2f pos, bool centered = false) : pos(pos), centered(centered) {}
	
	inline void load(const string& xmlDoc, const string& modelDoc, unsigned int distance) {
		this->distance = distance;
		
		Animation* anim = new Animation(xmlDoc);
		
		TiXmlDocument model(modelDoc.c_str());
		if(!model.LoadFile()) throw runtime_error(u8"Menu.load(): файл модели не найден");
		
		vector<string> list;
		{
			TiXmlElement* buttonList = model.FirstChildElement("list");
			string data = buttonList->GetText();
			istringstream ss(data);
			
			string name;
			while (ss >> name) {
				list.push_back(name);
			}
		}

		Vector2f currPos = pos;
		for (int i = 0; i < list.size(); i++) {
			Vector2f size = Vector2f(anim->uvRect.width, anim->uvRect.height);
			
			addButton(
				HoverButton(
					anim, list[i], list[i], size, 

					centered ? 
						Vector2f(currPos.x - size.x / 2, currPos.y) : 
						currPos
			));
			currPos.y += size.y + distance;
		}
	}

	inline void select_vertical(const Event& ev, RenderWindow& wnd, const View& view) {
		while (!CheckClick(ev, wnd, view)) {
			wnd.clear();
			drawButtons(wnd);
			wnd.display();
		}
	}

	inline bool CheckClick(const Event& ev, RenderWindow& wnd, const View& view) override {
		if (ev.type == Event::MouseMoved)
			for (int i = 0; i < buttons.size(); i++) {
				auto& button = buttons[i];
					
				button.setHover(button.intersects(Vector2f(ev.mouseMove.x - (wnd.getSize().x - (view.getCenter().x + wnd.getSize().x / 2)),
								ev.mouseMove.y - (wnd.getSize().y - (view.getCenter().y + wnd.getSize().y / 2)))));
					
			}
		if (IButtonArray::CheckClick(ev, wnd, view)) return true;
	}

	inline void setPosition(const Vector2f& pos) {
		Vector2f currPos = pos;

		for (auto& button : buttons) {
			Vector2f size = button.getSize();
			button.setPosition(
				centered ?
					Vector2f(currPos.x - size.x / 2, currPos.y) :
					currPos
			);
			currPos.y += size.y + distance;
		}
	};
};

