#include <imageProcess.hpp>
#include <iostream>

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
            unsigned char avg = static_cast<unsigned char>(((int)color.r + color.g + color.b)/3);
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
    // repair
	return im;
}

bool isWellComposed(const sf::Image& im)
{
    sf::Color pattern1[4] = {sf::Color::Black, sf::Color::White, sf::Color::Black, sf::Color::White};
    sf::Color pattern2[4] = {sf::Color::White, sf::Color::Black, sf::Color::White, sf::Color::Black};

    sf::Color patternTest[4];

    for(unsigned int i = 0; i<im.getSize().x ; ++i)
    {
        for(unsigned int j = 0; j<im.getSize().y; ++j)
        {
            patternTest[0] = im.getPixel(i,j);
            patternTest[1] = im.getPixel(i+1,j);
            patternTest[2] = im.getPixel(i,j+1);
            patternTest[3] = im.getPixel(i+1,j+1);

            if (patternTest[0]==pattern1[0] && patternTest[1]==pattern1[1] && patternTest[2]==pattern1[2] && patternTest[3]==pattern1[3])
            {
                std::cout << "not well composed at : (" << i << ","<< j << ");" << std::endl;
                return false;
            }
            else if (patternTest[0]==pattern2[0] && patternTest[1]==pattern2[1] && patternTest[2]==pattern2[2] && patternTest[3]==pattern2[3])
            {
                std::cout << "not well composed at : (" << i << ","<< j << ");" << std::endl;
                return false;
            }
        }
    }

    return true;
}
