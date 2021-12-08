#define DEBUG
#include "map.hpp"
#include "vector.hpp"
#include "stack.hpp"
#include <string>
#include "pair.hpp"
#include <map>
#include <vector>
#include <stack>
#include <unistd.h>
#include <sys/time.h>
#include "test_map.hpp"
#include "test_vector.hpp"
#include "test_stack.hpp"
#ifndef NAMESPACE
# define NAMESPACE ft
#endif



int main(int argc, char **argv)
{
	int seed = 42;
	int	ret = 0;

	if (argc >= 2)
	{
		if (argc == 3)
			seed = atoi(argv[2]);
		if (std::string(argv[1]) == std::string("vector"))
			test_vector<NAMESPACE::vector<std::string> >(seed);
		else if (std::string(argv[1]) == std::string("map"))
			test_map<NAMESPACE::map<std::string, std::string>, NAMESPACE::pair>(seed);
		else if (std::string(argv[1]) == std::string("stack"))
			test_stack<NAMESPACE::stack<std::string> >(seed);
		else
		{
			std::cerr << "Invalid arguments" << std::endl;
			ret = 1;
		}
	}
	else
	{
		std::cerr << "Invalid arguments" << std::endl;
		ret = 1;
	}
	if (!isatty(1))
		std::cout << (char)'\3' << std::endl;
	return ret;
}
