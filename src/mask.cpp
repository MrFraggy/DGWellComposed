#include <mask.hpp>
#include <iostream>

Mask::Mask(int size) : size(size)
{
	for(int i = 0; i < size*size; ++i)
		colors.push_back({CellType::Disabled, sf::Color::Black});
}

Mask::Mask(std::initializer_list<Cell> list) : colors(list)
{
	std::cout << "constructed with a " << colors.size() << "-element list" << std::endl;
	size = static_cast<int>(std::sqrt(colors.size()));
}

bool Mask::compare(const sf::Image& image, int x, int y)
{
	int necessary = 0;

	for(int i = 0; i < size; ++i)
	{
		for(int j = 0; j < size; ++j)
		{
			if(	x + i >= (int)image.getSize().x || y + j >= (int)image.getSize().y )
				continue;

			Cell& c = colors[i*size+j];

			if(c.first == CellType::Enabled)
			{
				if(image.getPixel(x + i, y + j) != c.second)
				{
					return false;
				}
			}

			if(c.first == CellType::Necessary)
				if(image.getPixel(i, j) != c.second)
					necessary++;
		}
	}

	if(necessary < 1)
	{
		std::cout << "compare false necessary" << std::endl;
		return false;
	}

	std::cout << "compare truuuuuuuuuuuuuuuuueeeeeeeeeeeeeeeeeeeeeeeeeeeee" << std::endl;
	return true;
}

Mask Mask::rotate()
{
	Mask out(size);

	for(int i = 0; i < size; ++i)
	{
		for(int j = 0; j < size; ++j)
		{
			out.colors[i*size+j] = colors[(size-j-1)*size+i];
		}
	}

	return out;
}

void Mask::print()
{
	for(int i = 0; i < size; ++i)
	{
		for(int j = 0; j < size; ++j)
		{
			Cell& c = colors[i*size+j];

			if(c.first == CellType::Necessary)
			{
				std::cout << "n ";
			}
			else if(c.first == CellType::Disabled)
			{
				std::cout << "d ";
			}
			else
			{
				if(c.second == sf::Color::Black)
				{
					std::cout << "1 ";
				}
				else
				{
					std::cout << "0 ";
				}
			}
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}