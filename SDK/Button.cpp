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

const Vector2f& RectButton::getSize() const noexcept {
	return body.getSize();
}

void RectButton::setPosition(const Vector2f& pos) {
	body.setPosition(pos);
}

const Vector2f& RectButton::getPosition() const noexcept {
	return body.getPosition();
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

MusicPlayer::MusicPlayer(const string& xmlDoc) {
	load(xmlDoc);
	currentSong = (*songs.begin()).first;
}

void MusicPlayer::setPosition(float pos) noexcept {
	songs[currentSong]->setPlayingOffset(seconds(pos));
}

void MusicPlayer::load(const string& xmlDoc) {
	TiXmlDocument doc(xmlDoc.c_str());
	doc.LoadFile();
	for (TiXmlElement* child = doc.FirstChildElement("song"); child != nullptr && string(child->Value()) == "song"; child = child->NextSiblingElement()) {
		songs[child->Attribute("name")] = shared_ptr<Song>(new Song(child->Attribute("name"), "Music/" + string(child->Attribute("name")) + ".wav"));
	}
}

void MusicPlayer::setVolume(float volume) noexcept {
	if (volume >= 0) {
		this->volume;
		if (cl.getElapsedTime().asSeconds() - lastSwitchTime >= 0.02) {
			for(auto& song : songs) 
				song.second->setVolume(volume);
			lastSwitchTime = cl.getElapsedTime().asSeconds();
		}
	}
}

void MusicPlayer::mute() noexcept { if (!songs.empty()) songs[currentSong]->setVolume(0); }

void MusicPlayer::unmute() noexcept { if(!songs.empty()) songs[currentSong]->setVolume(100);}

bool MusicPlayer::muted() const noexcept { return !songs.empty() && songs.at(currentSong)->getVolume() == 0; }

void MusicPlayer::restart()noexcept {
	setPosition(0);
}

// Возвращает правду, если музыка ещё играет

bool MusicPlayer::pause()noexcept {
	if (songs.empty()) return false;

	if (songs[currentSong]->getStatus() == Music::Status::Playing) {
		songs[currentSong]->pause();
		return true;
	}
	else return false;
}

// Возвращает правду, если музыку ещё не играет

bool MusicPlayer::play() noexcept {
	if (songs.empty()) return false;
	
	if (interrupted)
		interrupted = false;

	if (songs[currentSong]->getStatus() != Music::Status::Playing) {
		songs[currentSong]->play();
		return true;
	}
	else return false;
}

Music::Status MusicPlayer::getStatus() const noexcept  {
	if (songs.empty()) return Music::Status::Stopped;
	return songs.at(currentSong)->getStatus();
}

float MusicPlayer::getVolume() const noexcept  {
	if (songs.empty()) return 0;
	return songs.at(currentSong)->getVolume();
}

const string& MusicPlayer::getSongName() const noexcept {
	if (songs.empty()) return "undefinedSong";
	return songs.at(currentSong)->getName();
}

bool MusicPlayer::setMusic(const string& name) {

	if (songs.empty()) 
		return false;
	if (name == currentSong)
		return false;

	songs[currentSong]->stop();
	currentSong = name;
	return true;
}

bool MusicPlayer::isInterrupted() const noexcept {
	return interrupted;
}

void MusicPlayer::stop() noexcept { songs[currentSong]->stop(); }

Song::Song(const string& songname, const string& filename) :
	name(songname) {
	openFromFile(filename);
}

const string& Song::getName() const noexcept {
	return name;
}
