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

MusicPlayer::MusicPlayer(const std::vector<pair<std::string, std::string>>& src) : sources(src) {
	
	vector<Texture*> buttonTextures = {new Texture()};
	buttonTextures[0]->loadFromFile("Textures/button.png");

	addButton(ClickButton(buttonTextures[0], Vector2f(64, 64), Vector2f(400, 400)));

	if (!src.empty()) {
		music.openFromFile(src[0].second);
	}
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
		if (music.getStatus() == Music::Status::Stopped ||
			music.getStatus() == Music::Status::Paused) {
			music.play();
		}
		else music.pause();
		break;
	}
}

void MusicPlayer::setPosition(float pos) {
	music.setPlayingOffset(seconds(0));
}

void MusicPlayer::setVolume(float volume) {
	if (volume >= 0) {
		if (cl.getElapsedTime().asSeconds() - lastSwitchTime >= 0.02) {
			music.setVolume(volume);
			lastSwitchTime = cl.getElapsedTime().asSeconds();
			_muted = volume == 0;
		}
	}
}

void MusicPlayer::mute() { music.setVolume(0); _muted = true; }

void MusicPlayer::unmute() { music.setVolume(100); _muted = false; }

bool MusicPlayer::muted() { return _muted; }

float MusicPlayer::getVolume() const {
	return music.getVolume();
}

bool MusicPlayer::setMusic(int index) {
	return music.openFromFile(sources[index].second);
}
