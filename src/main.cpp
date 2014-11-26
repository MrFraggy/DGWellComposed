#include <iostream>
#include <SFML/Graphics.hpp>

int main(int argc, char* argv[])
{
	sf::Image image;
	if(argc > 1)
	{
		image.loadFromFile(std::string(argv[1]));
	}
	else
		image.create(128,128,sf::Color::Green);
	
	if(argc > 2)
	{
		image.saveToFile(std::string(argv[2]));
	}
	else
		image.saveToFile("bin/out.png");
	return 0;
}