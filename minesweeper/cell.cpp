#include "cell.h"

cell::cell(sf::Texture& texture)
{
	this->cell_sprite.setTexture(texture);
	this->cell_sprite.setTextureRect(sf::IntRect(320, 0, 31, 31));
}

sf::Sprite cell::get_sprite()
{
	if (this->is_flagged)
		this->cell_sprite.setTextureRect(sf::IntRect(352, 0, 31, 31));
	else if (this->is_visible)
	{
		if (this->data == 'm')
			this->cell_sprite.setTextureRect(sf::IntRect(9 * 32, 0, 31, 31));
		else
			this->cell_sprite.setTextureRect(sf::IntRect((this->data - '0') * 32, 0, 31, 31));
	}
	else
		this->cell_sprite.setTextureRect(sf::IntRect(320, 0, 31, 31));

	return this->cell_sprite;
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