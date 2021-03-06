#pragma once
#include <SFML\Graphics.hpp>
#include <string>
#include <windows.h>
#include <random>
#include <vector>

class cell
{
public:
	sf::Sprite cell_sprite;
	char data;
	bool is_flagged = false;
	bool is_visible = false;
	sf::Color color;

	cell(sf::Texture&);
	sf::Sprite get_sprite();
	void toggle_flag();
	bool is_changable();

	static std::string get_exe_location();
};

