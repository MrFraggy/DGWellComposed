#pragma once

#ifndef DG_IMAGEPROCESS_HPP
#define DG_IMAGEPROCESS_HPP

#include <SFML/Graphics.hpp>

class Listener;

sf::Image binarize(const sf::Image& im, unsigned char treshold);

sf::Image repairWellComposeNaive(const sf::Image& im);

sf::Image repairWellCompose3x3(const sf::Image& im);

bool isWellComposed(const sf::Image& im);

void addListener(Listener* l);

#endif // DG_IMAGEPROCESS_HPP