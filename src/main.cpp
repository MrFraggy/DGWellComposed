#include <iostream>
#include <SFML/Graphics.hpp>
#include <imageProcess.hpp>
#include <programOptions.hpp>
#include <viewer.hpp>
#include <convert.hpp>
#include <memory>
#include <thread>
#include <mask.hpp>

bool bStopped = false;

int main(int argc, char* argv[])
{
	ProgramOptions options(argv+1, argc-1);
    
    if(options.hasValue("-h") || options.hasValue("--help"))
    {
    	std::cout << "Available options: " << std::endl;
    	std::cout << "-g [--gui]:\tLanches a window which displays progress" << std::endl;
    	std::cout << "-h [--help]:\tDisplays help" << std::endl;
    	std::cout << "-i [--input]:\tSpecify the image to process" << std::endl;
    	std::cout << "-o [--output]:\tSpecify the result image to save in" << std::endl;
    	std::cout << "-s [--speed]:\tSpecify the speed of the processing" << std::endl;
    	return 0;
    }

	sf::Image image;
	// On vérifie que l'utilisateur a bien spécifié une image en entrée
	if(options.hasValue("-i") || options.hasValue("--input"))
	{
		std::string im;
		if(options.hasValue("-i"))
			im = options.getValue("-i");
		else 
			im = options.getValue("--input");
		image.loadFromFile(std::string(im));
	}
	else
	{
		std::cerr << "You must specify an input image with -i [--input] option." << std::endl;
		std::cerr << "See -h [--help] for more" << std::endl;
		return -1;
	}
	
	sf::Image binary = binarize(image, 127);


	std::unique_ptr<Viewer> viewer(nullptr);

	// On vérifie si on a demandé la fenêtre
	if(options.hasValue("-g") || options.hasValue("--gui"))
	{
		int speed = 10;
		if(options.hasValue("-s"))
			speed = fromString<int>(options.getValue("-s"));
		else if(options.hasValue("--speed"))
			speed = fromString<int>(options.getValue("--speed"));
		if(speed <= 0 ||speed > 100)
		{
			std::cerr << "Speed must be in range [1,100]" << std::endl;
			return -1;
		}

		viewer.reset(new Viewer(image, speed));
		addListener(viewer.get());
	}

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


    // On teste si on a un viewer car la fenêtre peut ne pas être lancée.
    if(viewer)
    {
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
	    viewer->setSpeed(100);
	}

	// La magie du multithreading: on attend que l'algo
	// ait fini de travailler.
    repairingthread.join();


    // Save image
    std::string im = "bin/out.png";
	if(options.hasValue("-o"))
		im = options.getValue("-o");
	else if(options.hasValue("--output"))
		im = options.getValue("--output");

	binary.saveToFile(im);

	std::cout << "Processed image saved: " << im << std::endl;
	return 0;
}
