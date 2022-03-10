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
#include "logger.hpp"
#ifndef NAMESPACE
# define NAMESPACE ft
#endif
#ifndef NTEST
# define NTEST 1000
#endif
#ifndef CPP_LOG_PATH
# define CPP_LOG_PATH "./log/"
#endif
#define STR_EXPAND(tok) #tok
#define STR(tok) STR_EXPAND(tok)

class logger logger;

int main(int argc, char **argv)
{
	int seed = 42;
	int	ret = 0;

	if (argc == 2)
		seed = atoi(argv[1]);
#ifdef CPP_LOGGING
	if (NTEST != -1) {
		logger.open(std::string(CPP_LOG_PATH) + "/log.cpp" );
		logger.raw = true;
	}
#endif
	logger.init_cpp();
	logger.define("NAMESPACE", "ft");
	logger.define("_P", "NAMESPACE::pair");
	logger.include("includes.hpp");
	logger.include("input_iterator.hpp");
	logger.include("common.hpp");
	logger.log<logger::CPP>("class logger logger;");
	logger.create_function("int main()");
#ifdef MONKEY_MAP
	logger.set_typedef("NAMESPACE::map<std::string, std::string>", "C");
	test_map<NAMESPACE::map<std::string, std::string, monkey_map::my_compare>, NAMESPACE::pair>(seed);
#endif
#ifdef MONKEY_VECTOR
	logger.set_typedef("NAMESPACE::vector<std::string>", "C");
	test_vector<NAMESPACE::vector<std::string> >(seed);
#endif
#ifdef MONKEY_STACK
	logger.set_typedef("NAMESPACE::stack<std::string>", "C");
	test_stack<NAMESPACE::stack<std::string> >(seed);
#endif
	logger.pop_block();
	if (!isatty(1))
		logger.log<logger::NONE>("\3");
	return ret;
}
