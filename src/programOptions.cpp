#include <programOptions.hpp>

ProgramOptions::ProgramOptions(char** args, int count)
{
	int max = 0;
	std::string last = "";
	while(max < count)
	{
		std::string tmp = args[max++];
		if(tmp[0] == '-' || last == "")
		{
			options[tmp] = "";
			last = tmp;
		} else
		{
			options[last] = tmp;
			last = "";
		}
	}
}

bool ProgramOptions::hasValue(const std::string& option)
{
	return options.find(option) != options.end() ? true : false;
}

std::string ProgramOptions::getValue(const std::string& option)
{
	return options[option];
}