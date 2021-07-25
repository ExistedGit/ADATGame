#include "Button.h"

ClickButton::ClickButton(Texture* text, const Vector2f& size, const Vector2f& pos) {

	body = RectangleShape(size);
	body.setPosition(pos);
	body.setTexture(text);
	
}

bool ClickButton::intersects(const Vector2f& pos) const {
	return FloatRect(body.getPosition(), body.getSize()).contains(pos);
}

void ClickButton::draw(RenderWindow& wnd) {
	wnd.draw(body);
}

void IButtonArray::draw(RenderWindow& wnd) {
	for (auto& it : buttons) it.draw(wnd);
}

void IButtonArray::addButton(const ClickButton& button) {
	buttons.push_back(button);
}

IButtonArray::IButtonArray(const std::initializer_list<ClickButton>& il) : buttons(il) {}

MusicPlayer::MusicPlayer(const std::vector<Song*>& src) : songs(src) {
	
	vector<Texture*> buttonTextures = {new Texture()};
	buttonTextures[0]->loadFromFile("Textures/button.png");

	addButton(ClickButton(buttonTextures[0], Vector2f(64, 64), Vector2f(400, 400)));
}

void MusicPlayer::CheckClick(const Event& ev, RenderWindow& wnd, const View& view) {
	if (ev.type == Event::MouseButtonReleased) 
		if (ev.mouseButton.button == Mouse::Left) 
			for (int i = 0; i < buttons.size(); i++) {
				auto& button = buttons[i];
				if (button.intersects
						(Vector2f(ev.mouseButton.x - (wnd.getSize().x - (view.getCenter().x + wnd.getSize().x / 2)),
								  ev.mouseButton.y - (wnd.getSize().y - (view.getCenter().y + wnd.getSize().y / 2))))
					) 
					Click(i);
			}
}

void MusicPlayer::Click(int index) {
	switch (index) {
	case 0:
		if (songs[currentIndex]->getStatus() == Music::Status::Stopped ||
			songs[currentIndex]->getStatus() == Music::Status::Paused) {
			songs[currentIndex]->play();
		}
		else songs[currentIndex]->pause();
		break;
	}
}

void MusicPlayer::setPosition(float pos) {
	songs[currentIndex]->setPlayingOffset(seconds(0));
}

void MusicPlayer::setVolume(float volume) {
	if (volume >= 0) {
		if (cl.getElapsedTime().asSeconds() - lastSwitchTime >= 0.02) {
			songs[currentIndex]->setVolume(volume);
			lastSwitchTime = cl.getElapsedTime().asSeconds();
		}
	}
}

void MusicPlayer::mute() { songs[currentIndex]->setVolume(0); }

void MusicPlayer::unmute() { songs[currentIndex]->setVolume(100);}

bool MusicPlayer::muted() { return songs[currentIndex]->getVolume() == 0; }

void MusicPlayer::next() {
	songs[currentIndex]->pause();
	if (currentIndex + 1 >= songs.size())
		currentIndex = 0;
	else
		currentIndex++;

}

void MusicPlayer::prev() {
	songs[currentIndex]->pause();
	if (currentIndex - 1 < 0)
		currentIndex = songs.size();
	else
		currentIndex--;

}

void MusicPlayer::restart() {
	setPosition(0);
}

// Возвращает правду, если музыка ещё играет

bool MusicPlayer::pause() {
	if (songs[currentIndex]->getStatus() == Music::Status::Playing) {
		songs[currentIndex]->pause();
		return true;
	}
	else return false;
}

// Возвращает правду, если музыку ещё не играет

bool MusicPlayer::play() {
	if (songs[currentIndex]->getStatus() != Music::Status::Playing) {
		songs[currentIndex]->play();
		return true;
	}
	else return false;
}

Music::Status MusicPlayer::getStatus() {
	return songs[currentIndex]->getStatus();
}

float MusicPlayer::getVolume() const {
	return songs[currentIndex]->getVolume();
}

const string& MusicPlayer::getSongName() {
	return songs[currentIndex]->getName();
}

bool MusicPlayer::setMusic(unsigned int index) {
	if (index > songs.size()) return false;
	songs[currentIndex]->pause();
	currentIndex = index;
	return true;
}

Song::Song(const string& songname, const string& filename) :
	name(songname) {
	openFromFile(filename);
}

const string& Song::getName() {
	return name;
}
