#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <mutex>
#include <vector>
#include <map>
#include <functional>
#include "Animation.h"
#include <type_traits>

extern const Clock cl;

using namespace sf;
using namespace std;

enum class ClickButtonType {
	RectButton, HoverButton
};

class BaseButton {
protected:
	Animation* anim;
	string name;
	ClickButtonType type;
	
public:
	function<void()> use;

	BaseButton(Animation* anim, const string& name, function<void()> use = []() {}, ClickButtonType type = ClickButtonType::RectButton);

	const string& getName() const noexcept;
	const ClickButtonType& getType() const noexcept;

	virtual void draw(RenderWindow& wnd) const noexcept = 0;
	virtual bool intersects(const Vector2f& pos) const noexcept =0;
	virtual void Update() {};
	virtual void setPosition(const Vector2f& pos) = 0;
};

class RectButton : public BaseButton
{
protected:
	RectangleShape body;
	string animName;
public:
	RectButton(Animation * text, const string& animName, const string& name, const Vector2f& size, const Vector2f& pos, function<void()> use = []() {}, ClickButtonType type = ClickButtonType::RectButton);;
	void draw(RenderWindow& wnd) const noexcept override;
	bool intersects(const Vector2f& pos) const noexcept override;
	virtual void Update() override;

	const Vector2f& getSize() const noexcept;

	void setPosition(const Vector2f& pos) override;
}; 

class HoverButton : public RectButton {
private:
	map<string, IntRect> states;
	bool hover = false;
public:
	HoverButton(Animation* text, const string& animName, const string& name, const Vector2f& size, const Vector2f& pos, function<void()> use = []() {});;

	// Выставляет визуальное состояние кнопки(активна или нет)
	void setHover(bool hover);
	bool getHover() const noexcept;;
};

template<
		 typename ButtonType, 
		 typename = enable_if_t<is_base_of<BaseButton, ButtonType>::value>
		>
class IButtonArray {
protected:
	std::vector<ButtonType> buttons;
public:	
	inline IButtonArray(const std::initializer_list<ButtonType>& il) : buttons(il) {}
	inline IButtonArray() {};

	inline virtual bool CheckClick(const Event& ev, RenderWindow& wnd, const View& view) {
		
	if (ev.type == Event::MouseButtonReleased)
		if (ev.mouseButton.button == Mouse::Left)
			for (int i = 0; i < buttons.size(); i++) {
				auto& button = buttons[i];
				if (ev.type == Event::MouseButtonReleased)
					if (button.intersects
						// Из-за класса View графические представления объектов смещаются
						// При этом их координаты остаются прежними

						// Это вынуждает отнять от координат мыши разницу между
						// координатами крайней стороны текущего вида
						// и крайней стороны окна
						(Vector2f(ev.mouseButton.x - (wnd.getSize().x - (view.getCenter().x + wnd.getSize().x / 2)),
							ev.mouseButton.y - (wnd.getSize().y - (view.getCenter().y + wnd.getSize().y / 2))))
						) {
								button.use();
						return true;
					}
			}
		return false;
	};

	inline void drawButtons(RenderWindow& wnd) {
		for (auto& it : buttons) it.draw(wnd);
	};

	inline void addButton(const ButtonType& button) {
		buttons.push_back(button);
	};

	inline void applyUseMap(map<string, function<void()>> useMap) {
		for (auto& button : buttons) {
			if (useMap.count(button.getName())) {
				button.use = useMap[button.getName()];
			}
		}
	};
	
};


class Song : public Music {
private:
	string name;
public:
	Song(const string& songname, const string& filename);

	const string& getName() const noexcept;
};

class MusicPlayer : public IButtonArray<RectButton> {
private:  
	std::vector<Song*> songs;
	int currentIndex = 0;

	float lastSwitchTime =0;
	bool interrupted = false;
public:
	

	MusicPlayer(const std::vector<Song*>& src = {});
	void setPosition(float pos) noexcept;;
	void setVolume(float volume) noexcept;
	float getVolume() const noexcept;

	void mute() noexcept;
	void unmute() noexcept;

	bool muted() const noexcept;

	void next() noexcept;;

	void prev() noexcept;;

	void restart() noexcept;
	// Возвращает правду, если музыка ещё играет
	bool pause() noexcept;
	// Возвращает правду, если музыку ещё не играет
	bool play() noexcept;
	void stop() noexcept;
	// То же самое, что и стоп, но используется, если музыка прервана e.g. открытием меню
	inline void interrupt() noexcept { 
		if(pause())
			interrupted = true; 
	}
	bool isInterrupted() const noexcept;

	Music::Status getStatus() const noexcept;

	const string& getSongName() const noexcept;
	bool setMusic(unsigned int index);
};