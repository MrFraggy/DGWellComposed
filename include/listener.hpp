#pragma once

#ifndef C_LISTENER_HPP
#define C_LISTENER_HPP

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <mutex>
#include <thread>
#include <chrono>
#include <iostream>

namespace sf
{
	typedef Vector2<unsigned int> Vector2ui;
}

class Listener
{
public:
	virtual ~Listener() {};

	void setMaskPosition(const sf::Vector2ui& pos);

	void setMaskSize(const sf::Vector2ui& size);

	void setImageModified(const sf::Image& image, const sf::Vector2ui& pos);

protected:
	virtual void onMaskModified() = 0;
	virtual void onImageModified() = 0;

	struct {
		sf::Vector2ui size, pos;
	} Mask;

	struct {
		sf::Image image;
		sf::Vector2ui pixel;
	} CurrentImage;

	std::mutex listenerMutex;
};


#endif