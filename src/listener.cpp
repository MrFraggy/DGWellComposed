#include <listener.hpp>

void Listener::setMaskPosition(const sf::Vector2i& pos)
{
	{
		std::lock_guard<std::mutex> lk(listenerMutex);
		Mask.pos = pos;
		onMaskMoved();
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(100-iSpeed));
}


void Listener::setMaskSize(const sf::Vector2i& size)
{
	std::lock_guard<std::mutex> lk(listenerMutex);
	Mask.size = size;
	onMaskModified();
}

void Listener::setImageModified(const sf::Image& image, const sf::Vector2i& pos)
{
	{
		std::lock_guard<std::mutex> lk(listenerMutex);
		CurrentImage.image = image;
		CurrentImage.pixel = pos;
		onImageModified();
	}

	//std::this_thread::sleep_for(std::chrono::seconds(5));
}