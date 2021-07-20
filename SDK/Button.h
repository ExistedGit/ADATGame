#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <mutex>
#include <vector>
#include <map>

using namespace sf;
using namespace std;

extern const Clock cl;

class Button
{
private:
	RectangleShape body;
	
public:
	Button(Texture* text, const Vector2f& size, const Vector2f& pos);;

	bool intersects(const Vector2f& pos) const;;
	void draw(RenderWindow& wnd);
};

class IButtonArray {
protected:
	std::vector<Button> buttons;
public:
	virtual void CheckClick(const Event& ev, RenderWindow& wnd, const View& view) = 0;
	virtual void Click(int index) = 0;

	void draw(RenderWindow& wnd);

	void addButton(const Button& button);

	IButtonArray(const std::initializer_list<Button>& il = {});;
};

class MusicPlayer : public IButtonArray {
private:  
	std::vector<pair<std::string, std::string>> sources;
	int currentIndex = 0;

	Music music;

	bool _muted = false;

	float lastSwitchTime =0;
public:
	MusicPlayer(const std::vector<pair<std::string, std::string>>& src);

	void CheckClick(const Event& ev, RenderWindow& wnd, const View& view) override;
	void Click(int index) override;

	void setPosition(float pos);;
	void setVolume(float volume);
	float getVolume() const;

	void mute();
	void unmute();

	bool muted();

	void next() {
		if (currentIndex + 1 >= sources.size()) 
			currentIndex = 0;
		else 
			currentIndex++;
		music.openFromFile(sources[currentIndex].second);
	};

	void prev() {
		if (currentIndex - 1 < 0) 
			currentIndex = sources.size();
		else 
			currentIndex--;
		
		music.openFromFile(sources[currentIndex].second);
	};

	void restart() {
		setPosition(0);
	}

	// Возвращает правду, если музыка ещё играет
	bool pause() {
		if (music.getStatus() == Music::Status::Playing) {
			music.pause();
			return true;
		}
		else return false;
	}

	// Возвращает правду, если музыку ещё не играет
	bool play() {
		if (music.getStatus() != Music::Status::Playing) {
			music.play();
			return true;
		}
		else return false;
	}

	Music::Status getStatus() {
		return music.getStatus();
	}

	const string& getSongName() {
		return sources[currentIndex].first;
	}
	bool setMusic(int index);
};