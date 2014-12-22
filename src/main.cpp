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
    	std::cout << "-g [--gui]:\tLaunches a window which displays progress" << std::endl;
    	std::cout << "-h [--help]:\tDisplays help" << std::endl;
    	std::cout << "-i [--input]:\tSpecify the image to process" << std::endl;
    	std::cout << "-m [--method naive|3x3]:\tSpecify the algorithm to process the image." << std::endl;
    	std::cout << "\t\t\tAvailable: naive or 3x3" << std::endl;
    	std::cout << "-o [--output]:\tSpecify the result image to save in" << std::endl;
    	std::cout << "-t [--threshold]\tSpecify the value for the binarization" << std::endl;
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
	
	
	int threshold = 127;
	if(options.hasValue("-t"))
		threshold = fromString<int>(options.getValue("-t"));
	else if(options.hasValue("--threshold"))
		threshold = fromString<int>(options.getValue("--threshold"));
	sf::Image binary = binarize(image, threshold);


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

		viewer.reset(new Viewer(binary, speed));
		addListener(viewer.get());
	}

    // Processing
    std::thread repairingthread([&](){
    	if(!isWellComposed(binary))
	    {
	    	auto func = repairWellCompose3x3;
	    	if(options.hasValue("-m"))
	    		if(options.getValue("-m") == "naive")
	    			func = repairWellComposeNaive;
	    	if(options.hasValue("--method"))
	    		if(options.getValue("--method") == "naive")
	    			func = repairWellComposeNaive;

	    	binary = func(binary);
	    } else
	    	std::cout << "Image already well composed!" << std::endl;
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
