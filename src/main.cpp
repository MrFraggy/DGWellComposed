#include <iostream>
#include <SFML/Graphics.hpp>
#include <imageProcess.hpp>
#include <listener.hpp>
#include <unistd.h>
#include <memory>
#include <thread>
#include <mask.hpp>

bool bStopped = false;

class Viewer: public Listener
{
public:
	Viewer(sf::Image& image) : 
		image(image), 
		window(sf::VideoMode(image.getSize().x,image.getSize().y, 32), "Non well-composed image repair viewer")
	{
		CurrentImage.image = image;
		CurrentImage.pixel.x = 0;
		CurrentImage.pixel.y = 0;
		imageTex.create(image.getSize().x, image.getSize().y);
		imageTex.update(image);
		maskTex.create(1,1);
		sf::Image i;
		i.create(1,1,sf::Color::Red);
		maskTex.update(i);
		window.setSize({800,600});
		bMaskModified = false;
	}

	void display()
	{
		window.clear();
		{
			std::lock_guard<std::mutex> lk(listenerMutex);
			CurrentImage.image.setPixel(Mask.pos.x, Mask.pos.y, sf::Color::Red);
			imageTex.update(CurrentImage.image);
			sf::Sprite sprite(imageTex);
			window.draw(sprite);

			if(bMaskModified)
				recreateMask();

			sf::Sprite spriteMask(maskTex);
			spriteMask.setPosition(Mask.pos.x-1, Mask.pos.y -1);
			window.draw(spriteMask);
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

	void recreateMask()
	{
		maskTex.create(Mask.size.x+2, Mask.size.y+2);
		sf::Image m;
		m.create(Mask.size.x+2, Mask.size.y+2, sf::Color(0,0,0,0));
		for(int x = 0; x<Mask.size.x+2; ++x)
			for(int y = 0; y<Mask.size.y+2; ++y)
			{
				if(x == 0 || y == 0 || x == Mask.size.x+1 || y == Mask.size.y +1)
					m.setPixel(x,y,sf::Color::Red);
			}
		
		maskTex.update(m);
		bMaskModified = false;
	}
	void onImageModified() {}
	void onMaskMoved() {}

	void onMaskModified() 
	{
		bMaskModified = true;
	}

	sf::Image& image;
	sf::RenderWindow window;
	sf::Texture imageTex, maskTex;
	sf::Image maskImage;
	bool bMaskModified;
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

	//////////////
	//	Tests
	//////////////
	//	Rotation
	//////////////
	/*
	Mask mask33_test = 
    {
        {CellType::Enabled, sf::Color::Black}, {CellType::Enabled, sf::Color::White}, {CellType::Disabled, sf::Color::Black}, {CellType::Disabled, sf::Color::Black},
        {CellType::Enabled, sf::Color::White}, {CellType::Enabled, sf::Color::Black}, {CellType::Enabled, sf::Color::Black}, {CellType::Disabled, sf::Color::Black},
        {CellType::Enabled, sf::Color::Black}, {CellType::Enabled, sf::Color::Black}, {CellType::Enabled, sf::Color::White}, {CellType::Disabled, sf::Color::Black},
        {CellType::Enabled, sf::Color::White}, {CellType::Enabled, sf::Color::Black}, {CellType::Disabled, sf::Color::Black}, {CellType::Disabled, sf::Color::Black}
    };

    Mask mask33_test_rotated = mask33_test.rotate();

    mask33_test.print();
    mask33_test_rotated.print();
    */

    // Processing
    std::thread repairingthread([&](){
    	if(!isWellComposed(image))
	    {
	    	// binary = repairWellComposeNaive(image);
	    	binary = repairWellCompose3x3(image);
	    } else
	    	std::cout << "Well composed!" << std::endl;
    });

    while(viewer->isOpen() && !bStopped)
    {
    	sf::Event event;
    	while(viewer->pollEvent(event))
    	{
    		if(event.type == sf::Event::Closed)
    		{
    			bStopped = true;
    			break;
    		}
    		else if(event.type == sf::Event::KeyPressed)
    		{
    			if(event.key.code == sf::Keyboard::Escape)
    			{	bStopped = true;
    				break;
    	   		}
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
