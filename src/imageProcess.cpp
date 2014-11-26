#include <imageProcess.hpp>

sf::Image binarize(const sf::Image& im, unsigned char treshold)
{
	sf::Image image;
	image.create(im.getSize().x, im.getSize().y, sf::Color::Black);

	for(unsigned int i = 0; i<im.getSize().x ; ++i)
	{
		for(unsigned int j = 0; j<im.getSize().y; ++j)
		{
			sf::Color color = im.getPixel(i,j);
			// Average value of the pixel (greyscale value)
			unsigned char avg = static_cast<unsigned char>(((int)color.r + color.g+color.b)/3);
			if(avg < treshold)
				image.setPixel(i,j,sf::Color::Black);
			else
				image.setPixel(i,j,sf::Color::White);
		}
	}
	return image;
}

sf::Image repairWellCompose(const sf::Image& im)
{
	return im;
}

bool isWellComposed(const sf::Image& im)
{
	return false;
}