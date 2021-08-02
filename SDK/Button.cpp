#include "Button.h"

const string& BaseButton::getName() const noexcept { return name; }

const ClickButtonType& BaseButton::getType() const noexcept {
	return type;
}

BaseButton::BaseButton(Animation* anim, const string& name, function<void()> use, ClickButtonType type) :
	name(name), use(use), anim(anim), type(type)
{

}

bool RectButton::intersects(const Vector2f& pos) const noexcept {
	return FloatRect(body.getPosition(), body.getSize()).contains(pos);
}

void RectButton::Update() {
	anim->Update(0, false, animName);
}

RectButton::RectButton(Animation * text, const string& animName, const string& name, const Vector2f& size, const Vector2f& pos, function<void()> use, ClickButtonType type) :
	BaseButton(text, name, use, ClickButtonType::RectButton), animName(animName){

	body = RectangleShape(size);
	body.setPosition(pos);
	body.setTexture(text->getTexture());
	text->setAnim(animName);
	body.setTextureRect(text->uvRect);
}

void RectButton::draw(RenderWindow& wnd)const noexcept {
	wnd.draw(body);
}
HoverButton::HoverButton(Animation* text, const string& animName, const string& name, const Vector2f& size, const Vector2f& pos, function<void()> use) :
	RectButton(text, animName, name, size, pos, use, ClickButtonType::HoverButton)
{
	text->setAnim(animName);
	states["inactive"] =text->uvRect;
	text->Update(0, false, animName);
	states["active"] = text->uvRect;
}
void HoverButton::setHover(bool hover) {
	if (this->hover != hover) {
		this->hover = hover;
		body.setTextureRect(states[hover ? "active" : "inactive"]);
	}
}

bool HoverButton::getHover() const noexcept { return hover; }

MusicPlayer::MusicPlayer(const std::vector<Song*>& src) : songs(src) {
	
	vector<Animation*> buttonTextures = {new Animation("Models/button.xml")};
	
	addButton(RectButton(buttonTextures[0], "default", "playButton", Vector2f(64, 64), Vector2f(400, 700),
		[this]() {
			if (!this->play()) this->pause();
		}));
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

void MusicPlayer::mute() noexcept { if (!songs.empty()) songs[currentIndex]->setVolume(0); }

void MusicPlayer::unmute() noexcept { if(!songs.empty()) songs[currentIndex]->setVolume(100);}

bool MusicPlayer::muted() const noexcept { return !songs.empty() && songs[currentIndex]->getVolume() == 0; }

void MusicPlayer::next()noexcept {
	if (songs.empty()) return;

	songs[currentIndex]->pause();
	if (currentIndex + 1 >= songs.size())
		currentIndex = 0;
	else
		currentIndex++;

}

void MusicPlayer::prev()noexcept {

	if (songs.empty()) return;

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
	if (songs.empty()) return false;

	if (songs[currentIndex]->getStatus() == Music::Status::Playing) {
		songs[currentIndex]->pause();
		return true;
	}
	else return false;
}

// Возвращает правду, если музыку ещё не играет

bool MusicPlayer::play() noexcept {
	if (songs.empty()) return false;

	if (songs[currentIndex]->getStatus() != Music::Status::Playing) {
		songs[currentIndex]->play();
		return true;
	}
	else return false;
}

Music::Status MusicPlayer::getStatus() const noexcept  {
	if (songs.empty()) return Music::Status::Stopped;
	return songs[currentIndex]->getStatus();
}

float MusicPlayer::getVolume() const noexcept  {
	if (songs.empty()) return 0;
	return songs[currentIndex]->getVolume();
}

const string& MusicPlayer::getSongName() const noexcept {
	if (songs.empty()) return "undefinedSong";
	return songs[currentIndex]->getName();
}

bool MusicPlayer::setMusic(unsigned int index) {

	if (songs.empty()) return false;

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
