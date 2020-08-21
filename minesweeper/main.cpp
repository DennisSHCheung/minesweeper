#include "cell.h"

typedef std::vector<std::vector<cell>> grid;

void set_mines(grid&);
void set_cell_data(grid&);

sf::Sprite init_sprite(sf::Texture& texture, int left)
{
	sf::Sprite sprite;
	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(left, 0, 31, 31));
	return sprite;
}

grid init_game_grid(sf::Texture& texture)
{
	grid game_grid(9, std::vector<cell>(9, cell(texture)));
	set_mines(game_grid);
	set_cell_data(game_grid);
	for (int i = 0; i < game_grid.size(); i++)
		for (int j = 0; j < game_grid.at(0).size(); j++)
			game_grid.at(i).at(j).cell_sprite.setPosition(sf::Vector2f(i * 32, j * 32));
	return game_grid;
}

bool is_valid(int i, int j)
{
	return i >= 0 && i < 9 && j >= 0 && j < 9;
}

void display(sf::RenderWindow& window, grid& game_grid)
{
	window.clear(sf::Color::White);

	/*for (int i = 0; i < game_grid.size(); i++)
	{
		for (int j = 0; j < game_grid.at(0).size(); j++)
		{
			game_grid.at(i).at(j).cell_sprite.setPosition(sf::Vector2f(i * 32, j * 32));
			window.draw(game_grid.at(i).at(j).cell_sprite);
		}
	}*/

	for (auto i : game_grid)
	{
		for (auto j : i)
		{
			window.draw(j.cell_sprite);
		}
	}

	window.display();
}

bool generate_mine()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> rng(1, 9 * 9);
	return rng(gen) == 9;
}

void add_mines(grid& game_grid, int& num_of_mines, int total_num_of_mines)
{
	for (auto& row : game_grid)
	{
		for (auto& col : row)
		{
			if (generate_mine())
			{
				col.set_sprite('m');
				num_of_mines++;
				if (num_of_mines == total_num_of_mines) return;
			}
		}
	}
}

void set_mines(grid& game_grid)
{
	int num_of_mines = 0;
	int total_num_of_mines = 10;
	while (num_of_mines != total_num_of_mines)
		add_mines(game_grid, num_of_mines, total_num_of_mines);
}

bool is_mine(grid& game_grid, int i, int j)
{
	return game_grid.at(i).at(j).data == 'm';
}

void find_adjacent_mines(grid& game_grid, int i, int j, int& mines_count)
{
	if (is_valid(i, j))
		if (is_mine(game_grid, i, j))
			mines_count++;
}

void set_cell_data(grid& game_grid)
{
	for (int i = 0; i < game_grid.size(); i++)
	{
		for (int j = 0; j < game_grid.at(0).size(); j++)
		{
			if (is_mine(game_grid, i, j)) continue;

			int mines_count = 0;
			find_adjacent_mines(game_grid, i - 1, j, mines_count); // North
			find_adjacent_mines(game_grid, i + 1, j, mines_count); // South
			find_adjacent_mines(game_grid, i, j + 1, mines_count); // East
			find_adjacent_mines(game_grid, i, j - 1, mines_count); // West
			find_adjacent_mines(game_grid, i - 1, j + 1, mines_count); // North-East
			find_adjacent_mines(game_grid, i - 1, j - 1, mines_count); // North-West
			find_adjacent_mines(game_grid, i + 1, j + 1, mines_count); // South-East
			find_adjacent_mines(game_grid, i + 1, j - 1, mines_count); // South-West

			game_grid.at(i).at(j).set_sprite(mines_count + '0');
		}
	}
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(500, 500), "Minesweeper");
	std::string path = cell::get_exe_location();
	sf::Texture texture;
	texture.loadFromFile(path + "tiles.jpg");

	grid game_grid = init_game_grid(texture);

	sf::Sprite hidden_cell_sprite = init_sprite(texture, 320);
	sf::Sprite flag_cell_sprite = init_sprite(texture, 352);


	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		display(window, game_grid);
	}

	return 0;
}