#include <iostream>
#include <SFML/Graphics.hpp>
#include <imageProcess.hpp>

int main(int argc, char* argv[])
{
	sf::Image image;
	if(argc > 1)
	{
		image.loadFromFile(std::string(argv[1]));
	}
	else
		image.create(128,128,sf::Color::Green);

	sf::Image binary = repairWellCompose(binarize(image, 127));
	
	if(argc > 2)
	{
		binary.saveToFile(std::string(argv[2]));
	}
	else
		binary.saveToFile("bin/out.png");
	return 0;
}