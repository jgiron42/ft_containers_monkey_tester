#define DEBUG
#ifdef MONKEY_MAP
# include "test_map.hpp"
#endif
#ifdef MONKEY_VECTOR
# include "test_vector.hpp"
#endif
#ifdef MONKEY_STACK
# include "test_stack.hpp"
#endif
#include "../includes.hpp"
#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <stack>
#include <unistd.h>
#include <cstdlib>
#include <sys/time.h>
#ifndef NAMESPACE
# define NAMESPACE ft
#endif
#ifndef NTEST
# define NTEST 1000
#endif

int main(int argc, char **argv)
{
	int seed = 42;
	int	ret = 0;

	if (argc == 2)
		seed = atoi(argv[1]);
#ifdef MONKEY_MAP
	test_map<NAMESPACE::map<std::string, std::string>, NAMESPACE::pair>(seed);
#endif
#ifdef MONKEY_VECTOR
	test_vector<NAMESPACE::vector<std::string> >(seed);
#endif
#ifdef MONKEY_STACK
	test_stack<NAMESPACE::stack<std::string> >(seed);
#endif


//	if (argc >= 2)
//	{
//		if (argc == 3)
//			seed = atoi(argv[2]);
//		if (std::string(argv[1]) == std::string("vector"))
//			test_vector<NAMESPACE::vector<std::string> >(seed);
//		else if (std::string(argv[1]) == std::string("map"))
//			test_map<NAMESPACE::map<std::string, std::string>, NAMESPACE::pair>(seed);
//		else if (std::string(argv[1]) == std::string("stack"))
//			test_stack<NAMESPACE::stack<std::string> >(seed);
//		else
//		{
//			std::cerr << "Invalid arguments" << std::endl;
//			ret = 1;
//		}
//	}
//	else
//	{
//#if NTEST == -1
//		std::cerr << "Invalid arguments" << std::endl;
//			ret = 1;
//#else
//		test_stack<NAMESPACE::stack<std::string> >(seed);
//		test_map<NAMESPACE::map<std::string, std::string>, NAMESPACE::pair>(seed);
//		test_vector<NAMESPACE::vector<std::string> >(seed);
//#endif
//	}
	if (!isatty(1))
		std::cout << (char)'\3' << std::endl;
	return ret;
}
