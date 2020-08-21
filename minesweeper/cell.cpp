#include "cell.h"

cell::cell(sf::Texture& texture)
{
	this->cell_sprite.setTexture(texture);
	this->cell_sprite.setTextureRect(sf::IntRect(320, 0, 31, 31));
	//this->cell_sprite.setScale(sf::Vector2f(2.f, 2.f));
}

void cell::set_sprite(char ch)
{
	if (ch == 'm')
		this->cell_sprite.setTextureRect(sf::IntRect(9 * 32, 0, 31, 31));
	else
		this->cell_sprite.setTextureRect(sf::IntRect((ch - '0') * 32, 0, 31, 31));
	this->data = ch;
}

void cell::toggle_flag()
{
	if (this->is_flagged)
	{
		this->is_flagged = false;
		this->is_visible = false;
	}
	else
	{
		this->is_flagged = true;
		this->is_visible = true;
	}
}

bool cell::is_changable()
{
	return !(this->is_visible && !this->is_flagged);
}

std::string cell::get_exe_location()
{
	char result[MAX_PATH];
	std::string path = std::string(result, GetModuleFileName(NULL, result, MAX_PATH));
	path = path.substr(0, path.find_last_of("\\"));
	path += "\\assets\\";
	return path;
}