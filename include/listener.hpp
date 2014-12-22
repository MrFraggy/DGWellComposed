#pragma once

#ifndef C_LISTENER_HPP
#define C_LISTENER_HPP

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <mutex>
#include <thread>
#include <chrono>
#include <iostream>

class Listener
{
public:
	Listener(unsigned int speed) : iSpeed(speed) {}
	virtual ~Listener() {};

	void setMaskPosition(const sf::Vector2i& pos);

	void setMaskSize(const sf::Vector2i& size);

	void setImageModified(const sf::Image& image, const sf::Vector2i& pos);

	void setSpeed(int speed) { iSpeed = speed; }

protected:
	virtual void onMaskMoved() = 0;
	virtual void onMaskModified() = 0;
	virtual void onImageModified() = 0;

	struct {
		sf::Vector2i size, pos;
	} Mask;

	struct {
		sf::Image image;
		sf::Vector2i pixel;
	} CurrentImage;

	std::mutex listenerMutex;
	unsigned int iSpeed;
};


#endif