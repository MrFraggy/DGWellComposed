#include <imageProcess.hpp>
#include <listener.hpp>
#include <mask.hpp>
#include <iostream>

std::vector<Listener*> listeners;

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
    sf::Image out = im;
    sf::Color pattern1[4] = {sf::Color::Black, sf::Color::White, sf::Color::Black, sf::Color::White};
    sf::Color pattern2[4] = {sf::Color::White, sf::Color::Black, sf::Color::White, sf::Color::Black};
    sf::Color patternTest[4];

    for(auto l: listeners)
        l->setMaskSize({2,2});

    for(unsigned int j = 0; j<out.getSize().y-1; ++j)
    {
        for(unsigned int i = 0; i<out.getSize().x-1 ; ++i)
        {
            for(auto l: listeners)
                l->setMaskPosition({i,j});

            patternTest[0] = out.getPixel(i,j);
            patternTest[1] = out.getPixel(i+1,j);
            patternTest[2] = out.getPixel(i+1,j+1);
            patternTest[3] = out.getPixel(i,j+1);
 
            if (patternTest[0]==pattern1[0] && patternTest[1]==pattern1[1] && patternTest[2]==pattern1[2] && patternTest[3]==pattern1[3])
            {
            	out.setPixel(i+1,j+1, sf::Color::White);

                std::cout << listeners.size() << std::endl; 
            }
            else if (patternTest[0]==pattern2[0] && patternTest[1]==pattern2[1] && patternTest[2]==pattern2[2] && patternTest[3]==pattern2[3])
            {
            	out.setPixel(i+1,j+1, sf::Color::Black);
                
                std::cout << listeners.size() << std::endl; 
               
            }

             for(auto l: listeners)
                    l->setImageModified(out, sf::Vector2ui(i+1,j+1));
        }
    }
	return out;
}

sf::Image repairWellCompose3x3(const sf::Image& im)
{
    // configuration 1
    Mask mask33_1 = {
                        {CellType::Enabled, sf::Color::Black}, {CellType::Enabled, sf::Color::White}, {CellType::Disabled, sf::Color::Black},
                        {CellType::Enabled, sf::Color::White}, {CellType::Enabled, sf::Color::Black}, {CellType::Disabled, sf::Color::Black},
                        {CellType::Disabled, sf::Color::Black}, {CellType::Disabled, sf::Color::Black}, {CellType::Enabled, sf::Color::Black}
                    };
    
    // configuration 2
    Mask mask33_2 = {
                        {CellType::Enabled, sf::Color::Black}, {CellType::Disabled, sf::Color::Black}, {CellType::Enabled, sf::Color::Black},
                        {CellType::Enabled, sf::Color::Black}, {CellType::Disabled, sf::Color::Black}, {CellType::Enabled, sf::Color::Black},
                        {CellType::Enabled, sf::Color::Black}, {CellType::Disabled, sf::Color::Black}, {CellType::Enabled, sf::Color::Black}
                    };

    // configuration 3
    Mask mask33_3 = {
                        {CellType::Enabled, sf::Color::Black}, {CellType::Disabled, sf::Color::Black}, {CellType::Enabled, sf::Color::Black},
                        {CellType::Enabled, sf::Color::Black}, {CellType::Disabled, sf::Color::Black}, {CellType::Enabled, sf::Color::Black},
                        {CellType::Enabled, sf::Color::Black}, {CellType::Disabled, sf::Color::Black}, {CellType::Enabled, sf::Color::Black}
                    };

    // configuration 4
    Mask mask33_4 = {
                        {CellType::Enabled, sf::Color::Black}, {CellType::Disabled, sf::Color::Black}, {CellType::Enabled, sf::Color::Black},
                        {CellType::Enabled, sf::Color::Black}, {CellType::Disabled, sf::Color::Black}, {CellType::Enabled, sf::Color::Black},
                        {CellType::Enabled, sf::Color::Black}, {CellType::Disabled, sf::Color::Black}, {CellType::Enabled, sf::Color::Black}
                    };
    
    // repair
    sf::Image out = im;

    sf::Color pattern2[9] = {sf::Color::White, sf::Color::Black, sf::Color::White, sf::Color::Black};
    sf::Color patternTest[4];

    for(auto l: listeners)
        l->setMaskSize({2,2});

    for(unsigned int j = 0; j<out.getSize().y-1; ++j)
    {
        for(unsigned int i = 0; i<out.getSize().x-1 ; ++i)
        {
            for(auto l: listeners)
                l->setMaskPosition({i,j});

            patternTest[0] = out.getPixel(i,j);
            patternTest[1] = out.getPixel(i+1,j);
            patternTest[2] = out.getPixel(i+1,j+1);
            patternTest[3] = out.getPixel(i,j+1);
 
           /* if (patternTest[0]==pattern1[0] && patternTest[1]==pattern1[1] && patternTest[2]==pattern1[2] && patternTest[3]==pattern1[3])
            {
                out.setPixel(i+1,j+1, sf::Color::White);

                std::cout << listeners.size() << std::endl; 
            }
            else */if (patternTest[0]==pattern2[0] && patternTest[1]==pattern2[1] && patternTest[2]==pattern2[2] && patternTest[3]==pattern2[3])
            {
                out.setPixel(i+1,j+1, sf::Color::Black);
                
                std::cout << listeners.size() << std::endl; 
               
            }

             for(auto l: listeners)
                    l->setImageModified(out, sf::Vector2ui(i+1,j+1));
        }
    }
    return out;
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
            patternTest[2] = im.getPixel(i+1,j+1);
            patternTest[3] = im.getPixel(i,j+1);

            if (patternTest[0]==pattern1[0] && patternTest[1]==pattern1[1] && patternTest[2]==pattern1[2] && patternTest[3]==pattern1[3])
            {
                std::cout << "1 not well composed at : (" << i << ","<< j << ");" << std::endl;
                return false;
            }
            else if (patternTest[0]==pattern2[0] && patternTest[1]==pattern2[1] && patternTest[2]==pattern2[2] && patternTest[3]==pattern2[3])
            {
                std::cout << "2 not well composed at : (" << i << ","<< j << ");" << std::endl;
                return false;
            }
        }
    }

    return true;
}

void addListener(Listener* l)
{
    listeners.push_back(l);
}