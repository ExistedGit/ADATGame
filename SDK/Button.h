#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <mutex>
#include <vector>
#include <map>

using namespace sf;
using namespace std;

extern const Clock cl;

class ClickButton
{
private:
	RectangleShape body;
	
public:
	ClickButton(Texture* text, const Vector2f& size, const Vector2f& pos);;

	bool intersects(const Vector2f& pos) const;;
	void draw(RenderWindow& wnd);
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

	const string& getName();
};

class MusicPlayer : public IButtonArray {
private:  
	std::vector<Song*> songs;
	int currentIndex = 0;

	float lastSwitchTime =0;
public:
	MusicPlayer(const std::vector<Song*>& src);

	void CheckClick(const Event& ev, RenderWindow& wnd, const View& view) override;
	void Click(int index) override;

	void setPosition(float pos);;
	void setVolume(float volume);
	float getVolume() const;

	void mute();
	void unmute();

	bool muted();

	void next();;

	void prev();;

	void restart();

	// ���������� ������, ���� ������ ��� ������
	bool pause();

	// ���������� ������, ���� ������ ��� �� ������
	bool play();

	Music::Status getStatus();

	const string& getSongName();
	bool setMusic(unsigned int index);
};