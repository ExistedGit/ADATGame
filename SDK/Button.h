#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <mutex>
#include <vector>
#include <map>
#include <functional>

using namespace sf;
using namespace std;

class BaseButton {
protected:
	RectangleShape body;
	string name;
public:
	const string& getName() const noexcept;
	function<void()> use;
	BaseButton(const string& name, function<void()> use);
	virtual bool intersects(const Vector2f& pos) const noexcept = 0;
};

extern const Clock cl;
class ClickButton : public BaseButton
{
public:
	ClickButton(Texture* text, const string& name, const Vector2f& size, const Vector2f& pos, function<void()> use = []() {});;

	bool intersects(const Vector2f& pos) const noexcept override ;
	void draw(RenderWindow& wnd);
};

class IButtonArray {
protected:
	std::vector<ClickButton> buttons;
public:
	void CheckClick(const Event& ev, RenderWindow& wnd, const View& view);

	void draw(RenderWindow& wnd);

	void addButton(const ClickButton& button);

	void applyUseMap(map<string, function<void()>> useMap);

	IButtonArray(const std::initializer_list<ClickButton>& il = {});;
};

class Song : public Music {
private:
	string name;
public:
	Song(const string& songname, const string& filename);

	const string& getName() const noexcept;
};

class MusicPlayer : public IButtonArray {
private:  
	std::vector<Song*> songs;
	int currentIndex = 0;
	//std::map<string, any> mpMap = {
	//{
	//	"playButton",
	//	std::function<void()>([this]()->void {
	//		if (!this->play()) this->pause();
	//	})
	//}
	//};;

	float lastSwitchTime =0;
public:
	

	MusicPlayer(const std::vector<Song*>& src);
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

	Music::Status getStatus() const noexcept;

	const string& getSongName() const noexcept;
	bool setMusic(unsigned int index);
};