#ifndef Convert_H
#define Convert_H

#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <fstream>

class Convert
{
public:
	template <typename T>
	static std::string T_to_string(T const &val)
	{
		std::ostringstream ostr;
		ostr << val;

		return ostr.str();
	}

	template <typename T>
	static T string_to_T(std::string const &val)
	{
		std::istringstream istr(val);
		T returnVal;
			
		return returnVal;
	}

	//template <>
	static std::string string_to_T(std::string const &val)
	{
		return val;
	}
};

#endif