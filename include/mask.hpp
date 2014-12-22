#pragma once

#ifndef C_MASK_HPP
#define C_MASK_HPP

#include <vector>
#include <initializer_list>
#include <SFML/Graphics.hpp>

enum class CellType: char{
	Disabled,
	Enabled,
	Necessary
} ;

typedef std::pair<CellType, sf::Color> Cell;

class Mask
{
public:
	Mask(int size);
	Mask(std::initializer_list<Cell> list);
	bool compare(const sf::Image& im, int i, int j);
	Mask rotate();
	void print();

protected:
	std::vector<Cell> colors;
	int size;
};

#endif