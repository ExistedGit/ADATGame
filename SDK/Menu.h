#pragma once
#include "Button.h"
#include "tinyxml.h"
#include "Animation.h"
#include <sstream>

struct MenuGrid {
	int x = 0, y = 0, indent =0;
};

class Menu : public IButtonArray<HoverButton>
{
private:
	Vector2f pos;
	bool centered;
	MenuGrid grid;
public:
	inline Menu() : pos(0, 0), centered(false) {}
	inline Menu(const Vector2f pos, bool centered = false) : pos(pos), centered(centered) {}
	
	inline void load(const string& xmlDoc, const string& modelDoc) {
		
		Animation* anim = new Animation(xmlDoc);
		
		TiXmlDocument model(modelDoc.c_str());
		if(!model.LoadFile()) throw runtime_error(u8"Menu.load(): файл модели не найден");
		
		string globalAnim;
		vector<string> list;
		TiXmlElement* buttonList = model.FirstChildElement("list");
		{
			if (buttonList->Attribute("globalAnim") != nullptr)
				globalAnim = buttonList->Attribute("globalAnim");

			string data = buttonList->GetText();
			istringstream ss(data);
			
			string name;
			while (ss >> name) {
				list.push_back(name);
			}
		}

		Vector2f currPos = pos;

		Vector2f originPos = pos;
		int x = 1, y = 1;
		for (int i = 0; i < list.size(); i++) {
			Vector2f size = Vector2f(anim->uvRect.width, anim->uvRect.height);
			
			TiXmlElement* gridElem = model.FirstChildElement("grid");
			if (gridElem != nullptr) {
				if (gridElem->Attribute("x") != nullptr)
					grid.x = atoi(gridElem->Attribute("x"));
				if (gridElem->Attribute("y") != nullptr)
					grid.y = atoi(gridElem->Attribute("y"));

				if (gridElem->Attribute("indent") != nullptr)
					grid.indent = atoi(gridElem->Attribute("indent"));

				if (grid.x != 0 || grid.y != 0)
					if (list.size() > (grid.x + !grid.x) * (grid.y + !grid.y))
						throw u8"Menu.load(): кнопок больше заданной сетки, переназначьте значения";

				addButton(
					HoverButton(
						anim, globalAnim.empty() ? list[i] : globalAnim, list[i], size,

						Vector2f(currPos.x + bool(x - 1) * grid.indent - centered * size.x/2, currPos.y + bool(y - 1) * grid.indent - centered * size.y / 2)
					));
				if (grid.x == 0 && grid.y == 0 && grid.indent != 0)
					currPos.y += size.y + grid.indent;
				else if (grid.x != 0) {
					if (x++ < grid.x)
						currPos.x += size.x + grid.indent;
					else if (grid.y != 0) {
						y++;
						x = 1;
						currPos.x = originPos.x;
						currPos.y += size.y + grid.indent;
					}
				}
			}
			else throw u8"Menu.load(): в модели не найдена сетка";
			
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
		Vector2f currPos = pos,
				 originPos = pos,
				 size;
		
		int	  	 x = 1, 
				 y = 1;
		
		for (auto& button : buttons) {
			size = button.getSize();

			button.setPosition(
				Vector2f(currPos.x + bool(x - 1) * grid.indent - centered * size.x / 2, currPos.y + bool(y - 1) * grid.indent - centered * size.y / 2)
			);

			if (grid.x == 0 && grid.y == 0 && grid.indent != 0)
				currPos.y += size.y + grid.indent;
			else if (grid.x != 0) {
				if (x++ < grid.x)
					currPos.x += size.x + grid.indent;
				else if (grid.y != 0) {
					y++;
					x = 1;
					currPos.x = originPos.x;
					currPos.y += size.y + grid.indent;
				}
			}
		}

	};
};

