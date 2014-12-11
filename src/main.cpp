#include <iostream>
#include <SFML/Graphics.hpp>
#include <imageProcess.hpp>
#include <listener.hpp>
#include <unistd.h>
#include <memory>
#include <thread>

class Viewer: public Listener
{
public:
	Viewer(sf::Image& image) : 
		image(image), 
		window(sf::VideoMode(image.getSize().x,image.getSize().y, 32), "Non well-composed image repair viewer")
	{
		window.setVerticalSyncEnabled(true);
		texture.create(image.getSize().x, image.getSize().y);
		texture.update(image);
	}

	void display()
	{
		window.clear();
		{
			std::lock_guard<std::mutex> lk(listenerMutex);
			sf::Sprite sprite(texture);
			window.draw(sprite);
		}
		window.display();
	}

	bool isOpen()
	{
		return window.isOpen();
	}

	bool pollEvent(sf::Event& e)
	{
		return window.pollEvent(e);
	}

protected:
	void onImageModified()
	{
		//std::cout << "modified: " << CurrentImage.pixel.x << " " << CurrentImage.pixel.y << std::endl;
		CurrentImage.image.setPixel(CurrentImage.pixel.x, CurrentImage.pixel.y, sf::Color::Red);
		texture.update(CurrentImage.image);
		//display();
		//sleep(1);
	}

	void onMaskModified() {}

	sf::Image& image;
	sf::RenderWindow window;
	sf::Texture texture;
};
int main(int argc, char* argv[])
{
    // Get image
	sf::Image image;
	if(argc > 1)
	{
		image.loadFromFile(std::string(argv[1]));
	}
	else
		image.create(128,128,sf::Color::Green);

	sf::Image binary = binarize(image, 127);

	std::unique_ptr<Viewer> viewer(new Viewer(image));
	addListener(viewer.get());
    // Processing
    std::thread repairingthread([&](){
    	if(!isWellComposed(image))
	    {
	    	binary = repairWellCompose(image);
	    }
    });
    
    bool bQuit = false;
    while(viewer->isOpen() && !bQuit)
    {
    	sf::Event event;
    	while(viewer->pollEvent(event))
    	{
    		if(event.type == sf::Event::Closed)
    		{
    			bQuit = true;
    			break;
    		}
    	}
    	viewer->display();
    }
    repairingthread.join();
    // Save image
	if(argc > 2)
	{
		binary.saveToFile(std::string(argv[2]));
	}
	else
    {
		binary.saveToFile("bin/out.png");
    }

	return 0;
}
