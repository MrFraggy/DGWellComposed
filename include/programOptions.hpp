#pragma once

#ifndef C_PROGRAMOPTIONS_HPP
#define C_PROGRAMOPTIONS_HPP

#include <map>
#include <string>

class ProgramOptions
{
public:
	ProgramOptions(char** args, int count);
	bool hasValue(const std::string& option);
	std::string getValue(const std::string& option);

protected:
	std::map<std::string, std::string> options; 
};

#endif