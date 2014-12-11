#pragma once

#ifndef DG_IMAGEPROCESS_HPP
#define DG_IMAGEPROCESS_HPP

#include <SFML/Graphics.hpp>

class Listener;

sf::Image binarize(const sf::Image& im, unsigned char treshold);

sf::Image repairWellCompose(const sf::Image& im);

bool isWellComposed(const sf::Image& im);

void addListener(Listener* l);

#endif // DG_IMAGEPROCESS_HPP