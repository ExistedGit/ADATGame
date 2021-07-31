#pragma once
#include "Level.h"
#include "tinyxml.h"

class ConfigManager
{
public:
	static vector<Level> loadLevels(RenderWindow* wnd = nullptr);
};

