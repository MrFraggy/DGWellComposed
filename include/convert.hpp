#pragma once

#ifndef C_CONVERT_HPP
#define C_CONVERT_HPP

#include <sstream>

template<typename T>
T fromString(const std::string& val)
{
	std::istringstream iss(val);
	T t;
	iss >> t;
	return t;
}

#endif