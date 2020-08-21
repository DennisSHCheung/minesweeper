#include "cell.h"
#include <iostream>

typedef std::vector<std::vector<cell>> grid;

void set_mines(grid&);
void set_cell_data(grid&);
bool is_valid(int, int);

sf::Vector2f get_mouse_coord(sf::RenderWindow& window)
{
	sf::Vector2i mouse_position = sf::Mouse::getPosition(window); // Mouse position related to the window
	sf::Vector2f translated_position = window.mapPixelToCoords(mouse_position); // Translate mouse position into window coordinates
	return translated_position;
}

/***************************************** Initialize game *****************************************/
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
			if (generate_mine() && col.data != 'm')
			{
				col.data = 'm';
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

			game_grid.at(i).at(j).data = mines_count + '0';
		}
	}
}

/**********************************************************************************/

bool is_valid(int i, int j)
{
	return i >= 0 && i < 9 && j >= 0 && j < 9;
}

bool is_empty_cell(grid& grid, int i, int j)
{
	return grid.at(i).at(j).data == '0';
}

void search_adjacent_empty_cell(grid& game_grid, std::vector <std::pair<int, int>>& queue, int i, int j)
{
	if (is_valid(i, j))
	{
		if (is_empty_cell(game_grid, i, j) && !game_grid.at(i).at(j).is_visible)
			queue.push_back(std::make_pair(i, j));
		game_grid.at(i).at(j).is_visible = true;
	}
}

void reveal_adjacent_cells(grid& game_grid, int i, int j)
{
	std::vector<std::pair<int, int>> queue;
	queue.push_back(std::make_pair(i ,j));

	while (!queue.empty())
	{
		i = queue.at(0).first;
		j = queue.at(0).second;
		queue.erase(queue.begin());

		// North
		search_adjacent_empty_cell(game_grid, queue, i - 1, j);
		// South
		search_adjacent_empty_cell(game_grid, queue, i + 1, j);
		// East
		search_adjacent_empty_cell(game_grid, queue, i, j + 1);
		// West
		search_adjacent_empty_cell(game_grid, queue, i, j - 1);
		// North-East
		search_adjacent_empty_cell(game_grid, queue, i - 1, j + 1);
		// North-West
		search_adjacent_empty_cell(game_grid, queue, i - 1, j - 1);
		// South-East
		search_adjacent_empty_cell(game_grid, queue, i + 1, j + 1);
		// South-West
		search_adjacent_empty_cell(game_grid, queue, i + 1, j - 1);
	}
}

void count_adjacent_flag_cell(grid& game_grid, int i, int j, int& flag_count)
{
	if (is_valid(i, j))
		if (game_grid.at(i).at(j).is_flagged)
			flag_count++;
}

int count_adjacent_flags(grid& game_grid, int i, int j)
{
	int flag_count = 0;
	count_adjacent_flag_cell(game_grid, i - 1, j, flag_count); // North
	count_adjacent_flag_cell(game_grid, i + 1, j, flag_count); // South
	count_adjacent_flag_cell(game_grid, i, j + 1, flag_count); // East
	count_adjacent_flag_cell(game_grid, i, j - 1, flag_count); // West
	count_adjacent_flag_cell(game_grid, i - 1, j + 1, flag_count); // North-East
	count_adjacent_flag_cell(game_grid, i - 1, j - 1, flag_count); // North-West
	count_adjacent_flag_cell(game_grid, i + 1, j + 1, flag_count); // South-East
	count_adjacent_flag_cell(game_grid, i + 1, j - 1, flag_count); // South-West
	return flag_count;
}

void display(sf::RenderWindow& window, grid& game_grid)
{
	window.clear(sf::Color::White);

	for (auto i : game_grid)
	{
		for (auto j : i)
		{
			window.draw(j.get_sprite());
		}
	}

	window.display();
}

void event_handler(sf::RenderWindow& window, sf::Event& event, bool& is_left_clicked, bool& is_right_clicked)
{
	if (event.type == sf::Event::Closed)
		window.close();
	else if (event.type == sf::Event::MouseButtonPressed)
	{
		if (event.mouseButton.button == sf::Mouse::Left) is_left_clicked = true;
		if (event.mouseButton.button == sf::Mouse::Right) is_right_clicked = true;
	}
	else if (event.type == sf::Event::MouseButtonReleased)
	{
		if (event.mouseButton.button == sf::Mouse::Left) is_left_clicked = false;
		if (event.mouseButton.button == sf::Mouse::Right) is_right_clicked = false;
	}
}

void input_handler(sf::RenderWindow& window, grid& game_grid, bool is_left_clicked, bool is_right_clicked)
{
	if (is_left_clicked && is_right_clicked)
	{
		for (int i = 0; i < game_grid.size(); i++)
		{
			for (int j = 0; j < game_grid.at(0).size(); j++)
			{
				if (game_grid.at(i).at(j).is_visible && !game_grid.at(i).at(j).is_flagged && !is_empty_cell(game_grid, i, j))
				{
					if (count_adjacent_flags(game_grid, i, j) + '0' == game_grid.at(i).at(j).data)
					{
						reveal_adjacent_cells(game_grid, i, j);
					}
				}
			}
		}
	}
	else if (is_left_clicked)
	{
		for (int i = 0; i < game_grid.size(); i++)
			for (int j = 0; j < game_grid.at(0).size(); j++)
				if (game_grid.at(i).at(j).cell_sprite.getGlobalBounds().contains(get_mouse_coord(window)))
					if (game_grid.at(i).at(j).is_changable())
					{
						if (is_empty_cell(game_grid, i, j))
						{
							reveal_adjacent_cells(game_grid, i, j);
						}
						else
						{
							game_grid.at(i).at(j).is_visible = true;
						}
					}
	}
	else if (is_right_clicked)
	{
		for (auto& i : game_grid)
			for (auto& j : i)
			if (j.cell_sprite.getGlobalBounds().contains(get_mouse_coord(window)))
				if (j.is_changable())
					j.toggle_flag();
	}
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(500, 500), "Minesweeper");
	std::string path = cell::get_exe_location();
	sf::Texture texture;
	texture.loadFromFile(path + "tiles.jpg");

	grid game_grid = init_game_grid(texture);
	bool is_left_clicked = false, is_right_clicked = false;

	while (window.isOpen())
	{
		sf::Event event;
		
		while (window.pollEvent(event))
			event_handler(window, event, is_left_clicked, is_right_clicked);
		input_handler(window, game_grid, is_left_clicked, is_right_clicked);

		display(window, game_grid);
	}

	return 0;
}