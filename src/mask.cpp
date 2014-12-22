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

bool Mask::apply(const sf::Image& image, int x, int y)
{
	for(int i = -size/2; i <= size/2; ++i)
	{
		for(int j = -size/2; j <= size/2; ++j)
		{
			if(	x + i < 0
			|| 	x + i >= (int)image.getSize().x
			|| 	y + j < 0
			|| 	y + j >= (int)image.getSize().y)
			{
				continue;
			}
			int idx = (i+size/2)*size + (j+size/2);
			Cell& c = colors[idx];

			if(c.first == CellType::Disabled && (image.getPixel(i, j) != c.second))
				return false;
		}
	}
	return true;
}