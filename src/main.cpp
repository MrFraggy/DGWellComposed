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
		//window.setVerticalSyncEnabled(true);

		CurrentImage.image = image;
		CurrentImage.pixel.x = 0;
		CurrentImage.pixel.y = 0;
		texture.create(image.getSize().x, image.getSize().y);
		texture.update(image);
		window.setSize({800,600});
	}

	void display()
	{
		window.clear();
		{
			std::lock_guard<std::mutex> lk(listenerMutex);
			CurrentImage.image.setPixel(Mask.pos.x, Mask.pos.y, sf::Color::Red);
			texture.update(CurrentImage.image);
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
		//CurrentImage.image.setPixel(CurrentImage.pixel.x, CurrentImage.pixel.y, sf::Color::Red);
		//texture.update(CurrentImage.image);
		//display();
		//sleep(1);
	}

	void onMaskModified() 
	{
		//std::cout << Mask.pos.x << " " << Mask.pos.y << std::endl;
	}

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
	    	binary = repairWellComposeNaive(image);
	    } else
	    	std::cout << "Well composed!" << std::endl;
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
