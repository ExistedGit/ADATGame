#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <mutex>
#include <vector>
#include <map>
#include <functional>

using namespace sf;
using namespace std;

extern const Clock cl;
class ClickButton
{
private:
	RectangleShape body;
	string name;
public:
	function<void()> use;
	ClickButton(Texture* text, const string& name, const Vector2f& size, const Vector2f& pos, function<void()> use = []() {});;

	bool intersects(const Vector2f& pos) const noexcept;;
	void draw(RenderWindow& wnd);
	const string& getName() const noexcept;
	
};

class IButtonArray {
protected:
	std::vector<ClickButton> buttons;
public:
	virtual void CheckClick(const Event& ev, RenderWindow& wnd, const View& view) = 0;
	virtual void Click(int index) = 0;

	void draw(RenderWindow& wnd);

	void addButton(const ClickButton& button);

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
	

	void CheckClick(const Event& ev, RenderWindow& wnd, const View& view) override;
	void Click(int index) override;

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