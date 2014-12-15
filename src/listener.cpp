#include <listener.hpp>

void Listener::setMaskPosition(const sf::Vector2ui& pos)
{
	{
		std::lock_guard<std::mutex> lk(listenerMutex);
		std::cout << Mask.pos.x << " " << Mask.pos.y << std::endl;
		Mask.pos = pos;
		onMaskModified();
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
}


void Listener::setMaskSize(const sf::Vector2ui& size)
{
	std::lock_guard<std::mutex> lk(listenerMutex);
	Mask.size = size;
	onMaskModified();
}

void Listener::setImageModified(const sf::Image& image, const sf::Vector2ui& pos)
{
	{
		std::lock_guard<std::mutex> lk(listenerMutex);
		CurrentImage.image = image;
		CurrentImage.pixel = pos;
		onImageModified();
	}

	//std::this_thread::sleep_for(std::chrono::seconds(5));
}