//
// Created by joachim on 3/8/22.
//

#ifndef FT_CONTAINERS_MONKEY_TESTER_LOGGER_HPP
#define FT_CONTAINERS_MONKEY_TESTER_LOGGER_HPP
#include <string>
#include <iostream>
#include <fstream>
#include <queue>
#include <algorithm>
#include <sstream>
#ifndef CPP_LOG_PATH
# define CPP_LOG_PATH "./log"
#endif

/**
 * convert any variable with "<<" overload to string using ostringstream
 */
#ifndef SSTR
# define SSTR( x ) (static_cast< std::ostringstream & >( ( std::ostringstream() << std::dec << (x) ) ).str())
#endif
/**
 * this logger is used both for printing the outputs of the
 * program and generate the cpp file containing the tests
 * performed by the tester
 */
class logger
{
public:
	std::ofstream	file;
	int				block_depth;
	bool			log_cpp;
	bool			raw;
	size_t			line;

	logger() : block_depth(0), log_cpp(false), raw(false), line(0) {}
	~logger()
	{
		if (log_cpp)
			file.close();
	}
	
	enum e_log_type {
		NONE,
		OUTPUT,
		TITLE,
		CPP,
		CPP_LOG,
		ERROR,
		BREAKPOINT
	};

	/**
	 * open a file for cpp logging
	 * @param path path to the file
	 */
	void open(std::string const &path)
	{
		file.open(path.data());
		if (!file.is_open())
			throw std::runtime_error("can't open file");
		else
			this->log_cpp = true;
	}

	typedef enum e_log_type t_log_type;
	/**
	 * log a line without various mode, default behaviour is to print the line in stdout
	 * @tparam mode one of logger::e_log_type
	 * @param s the line to write
	 * @return the number of characters wrote
	 */
	template <t_log_type mode>
	int log(std::string const &s){
		std::cout << s << "\n";
		return (s.length() + 1);
	};

	std::string	put_indent(int n)
	{
		std::string out;
		for (int i = 0; i < n; i++)
			out += "\t";
		return out;
	}

	/**
	 * insert one line of c++ in the cpp file with block management
	 * @param s the line to insert
	 * @return the number of characters inserted
	 */
	template <>
	int log<CPP>(std::string const &s){
		if (!log_cpp)
			return (0);
		std::string out;
		int			pos = this->file.tellp();

		pos -= (2 * block_depth + ((block_depth - 1) * block_depth) / 2);
		this->file.seekp(pos);
		out += put_indent(block_depth);
		out += s;
		out += "\n";
		this->line += std::count(out.begin(), out.end(), '\n');
		for (int i = block_depth - 1; i >= 0; i--)
		{
			out += put_indent(i );
			out += "}\n";
		}

		file << out;
		file.flush();
		return (out.length());
	};

	/**
	 * put one line in stderr
	 * @param s the line to insert
	 * @return the number of characters wrote
	 */
	template <>
	int log<ERROR>(std::string const &s){
		std::cerr << s << "\n";
		return (s.length() + 1);
	};

	/**
	 * output one line in stdout and put an instruction for printing this line in the cpp file
	 * @param s the line to insert
	 * @return the number of characters inserted in the cpp file
	 */
	template <>
	int log<OUTPUT>(std::string const &s){
		this->log<NONE>(s);
		return (this->log<CPP_LOG>("\"" + s + "\""));
	};

	/**
	 * put an instruction for printing a line in the cpp file
	 * @param s the line to print
	 * @return the number of characters wrote in the cpp file
	 */
	template <>
	int log<CPP_LOG>(std::string const &s){
		if (!log_cpp)
			return (0);
		return (this->log<CPP>("logger.log<logger::NONE>(" + s + ");")); //BOOM!!! logger-ception BITCH!
	};

	/**
	 * equivalent of logger::log<OUTPUT>() with title formating if the logger is not set to raw
	 * @param s the line to insert
	 * @return the number of characters inserted in the cpp file
	 */
	template <>
	int log<TITLE>(std::string const &s){
		int	ret;
		if (!raw)
			ret = this->log<OUTPUT>("\033[1m" + s + "\033[0m");
		else
			ret = this->log<OUTPUT>(s);
		return (ret);
	};

	template <>
	int	log<BREAKPOINT>(std::string const &)
	{
		if (!log_cpp)
			return (0);
		this->log<NONE>("[GDB]===================================> br log.cpp:" + SSTR(this->line + 1));
		return (this->log<CPP>("std::cout << \"[GDB]===================================> br \" << __FILE_NAME__ << \":\" <<  __LINE__  << std::endl;"));
	}

	/**
	 * create a new block in the cpp file and place the plot in it
	 * @warning a block must be ended by a call to pop_block()
	 */
	void create_block()
	{
		if (!log_cpp)
			return ;
		this->log<CPP>("{");
		this->log<CPP>("}");
		this->line--;
		this->block_depth++;
	}

	/**
	 * pop the plot out of a block
	 */
	void pop_block()
	{
		if (!log_cpp)
			return ;
		if (this->block_depth > 0)
		{
			this->block_depth--;
			this->line++;
		}
	}

	/**
	 * create a function in the cpp file
	 * @param proto the prototype of the function
	 * @warning a function must be ended by a call to pop_block()
	 */
	void create_function(std::string const &proto)
	{
		if (!log_cpp)
			return ;
		this->log<CPP>(proto);
		this->create_block();
	}

	/**
	 * set a typedef in the cpp file
	 */
	void set_typedef(std::string const &value, std::string const &define)
	{
		if (!log_cpp)
			return ;
		this->log<CPP>("typedef " + value + " " + define + ";");
	}

	/**
	 * insert a define in the cpp file
	 * @param protect whether or not the define should be protected
	 */
	void define(std::string const &define, std::string const &value, bool protect = true)
	{
		if (!log_cpp)
			return ;
		if (protect)
			this->log<CPP>("#ifndef " + define);
		this->log<CPP>("# define " + define + " " + value);
		if (protect)
			this->log<CPP>("#endif");
	}
	/**
	 * create an include in the cpp file
	 * @param file the file to include
	 */
	void include(const std::string &file)
	{
		if (!log_cpp)
			return ;
		this->log<CPP>("#include <" + file + ">");
	}

	void init_cpp()
	{
		if (!log_cpp)
			return ;

#ifdef MONKEY_MAP
		this->define("MONKEY_MAP", "");
#endif
#ifdef MONKEY_VECTOR
		this->define("MONKEY_VECTOR", "");
#endif
#ifdef MONKEY_STACK
		this->define("MONKEY_STACK", "");
#endif
		this->include("string");
		this->include("iostream");
		this->include("map");
		this->include("set");
		this->include("vector");
		this->include("stack");
		this->include("utility");
		this->log<CPP>("#ifndef SSTR");
		this->log<CPP>("# define SSTR( x ) (static_cast< std::ostringstream & >( ( std::ostringstream() << std::dec << (x) ) ).str())");
		this->log<CPP>("#endif");
	}
};

#endif //FT_CONTAINERS_MONKEY_TESTER_LOGGER_HPP
