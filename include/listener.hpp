#pragma once

#ifndef C_LISTENER_HPP
#define C_LISTENER_HPP

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <mutex>


namespace sf
{
	typedef Vector2<unsigned int> Vector2ui;
}

class Listener
{
public:
	virtual ~Listener() {};

	void setMaskPosition(const sf::Vector2ui& pos)
	{
		std::lock_guard<std::mutex> lk(listenerMutex);
		Mask.pos = pos;
		onMaskModified();
	}

	void setMaskSize(const sf::Vector2ui& size)
	{
		std::lock_guard<std::mutex> lk(listenerMutex);
		Mask.size = size;
		onMaskModified();
	}

	void setImageModified(const sf::Image& image, const sf::Vector2ui& pos)
	{
		{
			std::lock_guard<std::mutex> lk(listenerMutex);
			CurrentImage.image = image;
			CurrentImage.pixel = pos;
			onImageModified();
		}
		//sleep(5);
	}

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