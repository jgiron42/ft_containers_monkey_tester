//
// Created by joachim on 3/8/22.
//

#ifndef FT_CONTAINERS_MONKEY_TESTER_COMMON_HPP
#define FT_CONTAINERS_MONKEY_TESTER_COMMON_HPP

#include "../includes.hpp"
#include <iostream>
#include <map>
#include <set>
#include <cstdlib>
#include <iterator>
#include <string>
#include <climits>
#include "logger.hpp"
#include <sstream>

extern class logger logger;


namespace monkey
{
	template<class C>
	void print_full_vect(const C &v, const std::string &name) {
		logger.log<logger::CPP>("monkey::print_full_vect<C>(" + name + ", \"" + name + "\");");
		logger.log<logger::NONE>("empty: " + SSTR(v.empty()));
		logger.log<logger::NONE>("size: " + SSTR(v.size()));
		logger.log<logger::NONE>("content:");
		for (typename C::const_iterator i = v.begin(); i != v.end(); i++)
			logger.log<logger::NONE>(" [" + SSTR(*i) + "]");
		logger.log<logger::NONE>("reverse content:");
		for (typename C::const_reverse_iterator i = v.rbegin(); i != v.rend(); i++)
			logger.log<logger::NONE>(" [" + SSTR(*i) + "]");
	}

	template<class it>
	it add_iterator(it i, int n) {
		while (n > 0) {
			++i;
			n--;
		}
		return (i);
	}

	template <typename InputIt>
	unsigned int distance(InputIt first, InputIt last)
	{
		unsigned int ret = 0;

		for (;first != last; ++first)
			++ret;
		return(ret);
	}


	template <typename T>
	typename T::iterator get_itn(T &c, int r, typename T::iterator min, std::forward_iterator_tag)
	{
		typename T::iterator ret = min;
		int pos = 0;
		unsigned long int	m = c.size();
		for (typename T::iterator it = min; it != c.end(); it++, m--);
		if (c.size() != m)
			pos = r % (c.size() - m);
		for (int i = 0; i < pos; i++)
			ret++;
		return (ret);
	}

	template <typename T>
	typename T::iterator get_itn(T &c, int r, typename T::iterator min, std::random_access_iterator_tag)
	{
		if (min == c.end())
			return (c.end());
		return (min + (c.end() - min) % r);
	}

	/**
	 * @def get an iterator at a random position of the container c after min based on the random number r
	 * @param c the container to get the iterator from
	 * @param r a random number
	 * @param min the returned iterator is guaranteed to be min or after min
	 */
	template <typename T>
	typename T::iterator get_itn(T &c, int r, typename T::iterator min)
	{
		return (get_itn(c, r, min, typename std::iterator_traits<typename T::iterator>::iterator_category()));
	}

	/**
	 * @def get an iterator at a random position of the container c based on the random number r
	 * @param c the container to get the iterator from
	 * @param r a random number
	 */
	template <typename T>
	typename T::iterator get_itn(T &c, int r)
	{
		return (get_itn(c, r, c.begin()));
	}

	template<typename T>
	T get_value() {
		return (T());
	}

	template<>
	std::string get_value<std::string>() {
		std::string default_val[] = { "", "amigo de la tornada", "amigo de pepito",
									  "hola amigo",  "un", "dos", "atencion", "un dos tres quatro", "la pantera"};
		if (!(std::rand() % 3))
			return (std::string(default_val[std::rand() % 8]));
		std::string ret = "";
		for (int i = 0; i < 5; ++i)
			ret += (std::rand() % 26) + 'a';
		return (ret);
	}

	template<class C, template<class, class> class P>
	void print_full_map(const C &m,const std::string &name) {
		logger.log<logger::CPP>("monkey::print_full_map<C, _P>(" + name + ", \"" + name + "\");");

		logger.log<logger::NONE>("empty: " + SSTR(m.empty()));
		logger.log<logger::NONE>("size: " + SSTR(m.size()));
		logger.log<logger::NONE>("content:");
		for (typename C::const_iterator i = m.begin(); i != m.end(); i++)
			logger.log<logger::NONE>(" [" + SSTR(i->first) + "] -> [" + SSTR(i->second) + "]");
		logger.log<logger::NONE>("reverse content:");
		for (typename C::const_reverse_iterator i = m.rbegin(); i != m.rend(); i++)
			logger.log<logger::NONE>(" [" + SSTR(i->first) + "] -> [" + SSTR(i->second) + "]");
	}
}

#endif //FT_CONTAINERS_MONKEY_TESTER_COMMON_HPP
