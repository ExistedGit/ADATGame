#include "Button.h"

const string& ClickButton::getName() const noexcept { return name; }

ClickButton::ClickButton(Texture* text, const string& name, const Vector2f& size, const Vector2f& pos, function<void()> use) : name(name), use(use) {

	body = RectangleShape(size);
	body.setPosition(pos);
	body.setTexture(text);
	
}

bool ClickButton::intersects(const Vector2f& pos) const noexcept {
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

	addButton(ClickButton(buttonTextures[0], "playButton", Vector2f(64, 64), Vector2f(400, 700), 
		[this]() {
			if (!this->play()) this->pause();
		}));
}

void MusicPlayer::CheckClick(const Event& ev, RenderWindow& wnd, const View& view) {
	if (ev.type == Event::MouseButtonReleased) 
		if (ev.mouseButton.button == Mouse::Left) 
			for (int i = 0; i < buttons.size(); i++) {
				auto& button = buttons[i];
				if (button.intersects
					// Из-за класса View графические представления объектов смещаются
					// При этом их координаты остаются прежними
					
					// Это вынуждает отнять от координат мыши разницу между
					// координатами крайней стороны текущего вида
					// и крайней стороны окна
				(Vector2f(ev.mouseButton.x - (wnd.getSize().x - (view.getCenter().x + wnd.getSize().x / 2)),
					ev.mouseButton.y - (wnd.getSize().y - (view.getCenter().y + wnd.getSize().y / 2))))
					)
					button.use();
			}
}

void MusicPlayer::Click(int index) {
	if (buttons[index].getName() == "playButton") {
		//std::any_cast<std::function<void()>>(mpMap["playButton"])();
		if (!this->play()) this->pause();
	}
}

void MusicPlayer::setPosition(float pos) noexcept {
	songs[currentIndex]->setPlayingOffset(seconds(0));
}

void MusicPlayer::setVolume(float volume) noexcept {
	if (volume >= 0) {
		if (cl.getElapsedTime().asSeconds() - lastSwitchTime >= 0.02) {
			songs[currentIndex]->setVolume(volume);
			lastSwitchTime = cl.getElapsedTime().asSeconds();
		}
	}
}

void MusicPlayer::mute() noexcept { songs[currentIndex]->setVolume(0); }

void MusicPlayer::unmute() noexcept { songs[currentIndex]->setVolume(100);}

bool MusicPlayer::muted() const noexcept { return songs[currentIndex]->getVolume() == 0; }

void MusicPlayer::next()noexcept {
	songs[currentIndex]->pause();
	if (currentIndex + 1 >= songs.size())
		currentIndex = 0;
	else
		currentIndex++;

}

void MusicPlayer::prev()noexcept {
	songs[currentIndex]->pause();
	if (currentIndex - 1 < 0)
		currentIndex = songs.size();
	else
		currentIndex--;

}

void MusicPlayer::restart()noexcept {
	setPosition(0);
}

// Возвращает правду, если музыка ещё играет

bool MusicPlayer::pause()noexcept {
	if (songs[currentIndex]->getStatus() == Music::Status::Playing) {
		songs[currentIndex]->pause();
		return true;
	}
	else return false;
}

// Возвращает правду, если музыку ещё не играет

bool MusicPlayer::play() noexcept {
	if (songs[currentIndex]->getStatus() != Music::Status::Playing) {
		songs[currentIndex]->play();
		return true;
	}
	else return false;
}

Music::Status MusicPlayer::getStatus() const noexcept  {
	return songs[currentIndex]->getStatus();
}

float MusicPlayer::getVolume() const noexcept  {
	return songs[currentIndex]->getVolume();
}

const string& MusicPlayer::getSongName() const noexcept {
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

const string& Song::getName() const noexcept {
	return name;
}
