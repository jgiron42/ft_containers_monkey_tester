
#include "../includes.hpp"
#include <iostream>
#include <vector>
#include <set>
#include <cstdlib>
#include "logger.hpp"
#include <string>
#include <climits>
#include "input_iterator.hpp"
#include <sstream>
#include <iostream>
#include "common.hpp"
#ifndef NTEST
# define NTEST 1000
#endif

extern class logger logger;

namespace monkey_vector {

	template<typename T>
	T get_value() {
		return (T());
	}

	template<>
	std::string get_value<std::string>() {
		std::string default_val[] = { "", "amigo de la tornada", "amigo de pepito",
									 "hola amigo",  "un", "dos", "atencion", "un dos tres quatro", "la pantera"};
		return (std::string(default_val[std::rand() % 8]));
	}

	template<>
	int get_value<int>() {
		return (int(std::rand() % UINT_MAX));
	}

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

	template<class C>
	void test_clear(const std::string &n1, C &v1, const std::string &, C &) {
		logger.log<logger::TITLE>("test_clear:");
		logger.create_block();
		logger.log<logger::CPP>("C v3(" + n1 + ");");
		C v3(v1);
		print_full_vect(v1, n1);
		logger.log<logger::CPP>(n1 + ".clear();");
		v1.clear();
		print_full_vect(v3, "v3");
		logger.log<logger::CPP>("v3.clear();");
		v3.clear();
		logger.pop_block();
	}

	template<class C>
	void test_copy_construct_equal(const std::string &n1, C &v1, const std::string &n2, C &v2) {
		logger.log<logger::TITLE>("test_copy_construct_equal:");

		logger.create_block();
		logger.log<logger::CPP>("C v3(" + n1 + ");");
		C v3(v1);
		logger.log<logger::CPP>(n1 + " = " + n2 + ";");
		v1 = v2;
		logger.log<logger::CPP>(n2 + " = v3;");
		v2 = v3;
		logger.pop_block();
	}

	template<class C>
	void test_construct(const std::string &n1, C &v1, const std::string &, C &) {
		unsigned long int rand;
		typename C::value_type val;
		switch (std::rand() % 10) {
			case 0:
				logger.log<logger::TITLE>("test_construct: count");
				rand = std::rand() % 20;
				val = get_value<typename C::value_type>();
	            v1 = C(rand, val);
				logger.log<logger::CPP>(n1 + " = C(" + SSTR(rand) + ", \"" + val + "\");");
				break;
			case 1:
				logger.log<logger::TITLE>("test_construct: range");
				logger.create_block();
				logger.log<logger::CPP>("const std::set<C::value_type> s(" + n1 + ".begin(), " + n1 + ".end());");
				const std::set<typename C::value_type> s(v1.begin(), v1.end());
				logger.log<logger::CPP>(n1 + " = C(s.begin(), s.end());");
				v1 = C(s.begin(), s.end());
				logger.pop_block();
				break;
		}
	}

	template<class C>
	void test_assign(const std::string &n1, C &v1, const std::string &, C &) {
		unsigned long int rand;
		typename C::value_type val;
		switch (std::rand() % 10) {
			case 0:
				logger.log<logger::TITLE>("test_assign: count");
				rand = std::rand() % 20;
				val = get_value<typename C::value_type>();
				v1.assign(rand, val);
				logger.log<logger::CPP>(n1 + ".assign(" + SSTR(rand) + ", \"" + val + "\");");
				break;
			case 1:
				logger.log<logger::TITLE>("test_assign: range");
				logger.create_block();
				logger.log<logger::CPP>("const std::set<C::value_type> s(" + n1 + ".begin(), " + n1 + ".end());");
				const std::set<typename C::value_type> s(v1.begin(), v1.end());
				logger.log<logger::CPP>(n1 + ".assign(s.begin(), s.end());");
				v1.assign(s.begin(), s.end());
				logger.pop_block();
				break;
		}
	}

	template<class C>
	void test_at(const std::string &n1, C &v1, const std::string &n2, C &v2) {
		unsigned long int rand;
		typename C::value_type val;
		for (int i = 0; i < 5; i++) {
			try {
				logger.log<logger::TITLE>("test_at: ");
				logger.log<logger::CPP>("try");
				logger.create_block();
				if (!v1.empty())
				{
					rand = std::rand();
					val = get_value<typename C::value_type>();
					v1.at((int) rand % v1.size()) = val;
					logger.log<logger::CPP>(n1 +".at(" + SSTR(rand) + " % " + n1 + ".size()) = \"" + SSTR(val) + "\";");
				}
				if (!v2.empty())
				{
					rand = std::rand();
					logger.log<logger::NONE>(v2.at((int) rand % v2.size()));
					logger.log<logger::CPP_LOG>(n2 + ".at(" + SSTR(rand) + " % " + n2 +".size())");
				}
				rand = std::rand();
				logger.log<logger::NONE>(v1.at((int) rand));
				logger.log<logger::CPP_LOG>(n1 + ".at((unsigned int) " + SSTR(rand) + ")");
				rand = std::rand();
				logger.log<logger::NONE>(v2.at((int) rand));
				logger.log<logger::CPP_LOG>(n2 + ".at((unsigned int) " + SSTR(rand) + ")");
				logger.pop_block();
				logger.log<logger::CPP>("catch (std::out_of_range &e)");
				logger.create_block();
				logger.log<logger::CPP_LOG>("\"out_of_range catched\"");
				logger.pop_block();
			}
			catch (std::out_of_range &e) {
				logger.pop_block();
				logger.log<logger::CPP>("catch (std::out_of_range &e)");
				logger.create_block();
				logger.log<logger::CPP_LOG>("\"out_of_range catched\"");
				logger.pop_block();
				logger.log<logger::NONE>("out_of_range catched");
			}
		}
	}

	template<class C>
	void test_operator_index(const std::string &n1, C &v1, const std::string &n2, C &v2) {
		unsigned int rand;
		typename C::value_type val;
		logger.log<logger::TITLE>("test_operator_index:");
		for (int i = 0; i < 5; i++) {
			rand = std::rand();
			val = get_value<typename C::value_type>();
			if (!v1.empty())
			{
				v1[(int) rand % v1.size()] = val;
				logger.log<logger::CPP>(n1 + "[(int) " + SSTR(rand) + " % " + n1 + ".size()] = \"" + SSTR(val) + "\";");
			}
			rand = std::rand();
			if (!v2.empty())
			{
				logger.log<logger::NONE>(SSTR(v2[(int) rand % v2.size()]));
				logger.log<logger::CPP_LOG>(n2 + "[(int) " + SSTR(rand) + " % " + n2 + ".size()]" );
			}
		}
	}

	template<class C>
	void test_insert(const std::string &n1, C &v1, const std::string &n2, C &v2) {
		typename C::iterator it1;
		typename C::iterator it2;
		typename C::iterator it3;
		unsigned int rand;
		typename C::value_type val;

		if (std::rand() % 50 == 1)
		{
			val = get_value<typename C::value_type>();
			v1.insert(v1.begin(), 0, ""); // yeah you must pass this one
			logger.log<logger::CPP>(n1 + ".insert(" + n1 + ".begin(), 0, \"\" );");
		}

		for (int i = 0; i < 5; i++) {
			rand = std::rand();
			it1 = monkey::get_itn(v1, rand);
			logger.log<logger::CPP>("it1 = monkey::get_itn(" + n1 + ", " + SSTR(rand) + ");");
			try {
				logger.log<logger::CPP>("try");
				logger.create_block();
				switch (std::rand() % 3) {
					val = get_value<typename C::value_type>();
					case (0):
						logger.log<logger::TITLE>("test_insert value");
						logger.log<logger::CPP_LOG>("*" + n1 + ".insert(it1, \"" + val + "\")");
						logger.log<logger::NONE>("ret = " + SSTR(*v1.insert(it1, val)));
						break;
					case (1):
						logger.log<logger::TITLE>("test_insert range");
						rand = std::rand();
						it2 = monkey::get_itn(v2, rand);
						logger.log<logger::CPP>("it2 = monkey::get_itn(" + n2 + ", " + SSTR(rand) + ");");
						rand = std::rand();
						it3 = monkey::get_itn(v2, rand, it2);
						logger.log<logger::CPP>("it3 = monkey::get_itn(" + n2 + ", " + SSTR(rand) + ", it2);");
						v1.insert(it1, input_iterator<typename C::iterator>(it2),
									  input_iterator<typename C::iterator>(it3));
						logger.log<logger::CPP>(n1 + ".insert(it1, input_iterator<C::iterator>(it2), input_iterator<C::iterator>(it3));");
						break;
					case (2):
						logger.log<logger::TITLE>("test_insert count");
						rand = std::rand();
						logger.log<logger::CPP>(n1 + ".insert(it1, (int)" + SSTR(rand % 5) + ", \"" + SSTR(val) + "\" );");
						v1.insert(it1, rand % 5, val);
				}
				logger.pop_block();
				logger.log<logger::CPP>("catch (std::out_of_range &e)");
				logger.create_block();
				logger.log<logger::CPP_LOG>("e.what()");
				logger.pop_block();
			}
			catch (std::exception &e) {
				std::cout << e.what() << std::endl;
			}
		}
	}

	template<class C>
	void test_erase(const std::string &n1, C &v1, const std::string &, C &) {
		unsigned int rand;
		typename C::iterator it1;
		typename C::iterator it2;
		for (int i = 0; i < 5; i++) {
			rand = std::rand();
			it1 = monkey::get_itn(v1, rand);
			logger.log<logger::CPP>("it1 = monkey::get_itn(" + n1 + ", " + SSTR(rand) + ");");
			switch (std::rand() % 3) {
				case (0):
					if (it1 == v1.end())
						break;
					logger.log<logger::TITLE>("test_erase it");
					it1 = v1.erase(it1);
					logger.log<logger::CPP>("it1 = " + n1 + "erase(it1);");
					if (it1 < v1.end())
					{
						logger.log<logger::CPP_LOG>("*it1");
						logger.log<logger::NONE>(*it1);
					}
					break;
				case (2):
					logger.log<logger::TITLE>("test_erase range");
					rand = std::rand();
					it2 = monkey::get_itn(v1, rand, it1);
					logger.log<logger::CPP>("it2 = monkey::get_itn(" + n1 + ", " + SSTR(rand) + ", it1);");
					v1.erase(it1, it2);
					logger.log<logger::CPP>(n1 + ".erase(it1, it2);");
			}

		}
	}

	template<class C>
	void test_push_back(const std::string &n1, C &v1, const std::string &, C &) {
		logger.log<logger::TITLE>("test_push_back:");
		typename C::value_type val = get_value<typename C::value_type>();
		v1.push_back(val);
		logger.log<logger::CPP>(n1 + ".push_back(\"" + SSTR(val) + "\");");
	}

	template<class C>
	void test_pop_back(const std::string &n1, C &v1, const std::string &, C &) {
		logger.log<logger::TITLE>("test_pop_back:");

		if (!v1.empty()) {
			v1.pop_back();
			logger.log<logger::CPP>(n1 + ".pop_back();");
		}
	}

	template<class C>
	void test_resize(const std::string &n1, C &v1, const std::string &, C &) {
		logger.log<logger::TITLE>("test_resize:");
		typename C::value_type val = get_value<typename C::value_type>();
		int new_size = std::rand() % (v1.empty() ? 10 : (2 * v1.size()));
		v1.resize(new_size, val);
		logger.log<logger::CPP>(n1 + ".resize(" + SSTR(new_size) + ", \"" + SSTR(val) + "\");");

	}

	template<class C>
	void test_swap(const std::string &n1, C &v1, const std::string &n2, C &v2) {
		logger.log<logger::TITLE>("test_swap:");
		logger.log<logger::CPP>(n1 + ".swap(" + n2 + ");");
		v1.swap(v2);
	}

	template<class C>
	void test_stdswap(const std::string &n1, C &v1, const std::string &n2, C &v2) {
		logger.log<logger::TITLE>("test_stdswap:");
		logger.log<logger::CPP>("std::swap(" + n1 + ", " + n2 + ");");
		std::swap(v1, v2);
	}

	template<class C>
	void test_comparison(const std::string &n1, C &v1, const std::string &n2, C &v2) {
		logger.log<logger::TITLE>("test_comparison:");

		logger.log<logger::NONE>(n1 + " < " + n2 + ": " + SSTR(v1 < v2));
		logger.log<logger::NONE>(n1 + " > " + n2 + ": " + SSTR(v1 > v2));
		logger.log<logger::NONE>(n1 + " <= " + n2 + ": " + SSTR(v1 <= v2));
		logger.log<logger::NONE>(n1 + " >= " + n2 + ": " + SSTR(v1 >= v2));
		logger.log<logger::NONE>(n1 + " == " + n2 + ": " + SSTR(v1 == v2));
		logger.log<logger::NONE>(n1 + " != " + n2 + ": " + SSTR(v1 != v2));

		logger.log<logger::CPP_LOG>("\"" + n1 + " < " + n2 + ": \" + SSTR( " + n1 + " < " + n2 + ")");
		logger.log<logger::CPP_LOG>("\"" + n1 + " > " + n2 + ": \" + SSTR( " + n1 + " > " + n2 + ")");
		logger.log<logger::CPP_LOG>("\"" + n1 + " <= " + n2 + ": \" + SSTR( " + n1 + " <= " + n2 + ")");
		logger.log<logger::CPP_LOG>("\"" + n1 + " >= " + n2 + ": \" + SSTR( " + n1 + " >= " + n2 + ")");
		logger.log<logger::CPP_LOG>("\"" + n1 + " == " + n2 + ": \" + SSTR( " + n1 + " == " + n2 + ")");
		logger.log<logger::CPP_LOG>("\"" + n1 + " != " + n2 + ": \" + SSTR( " + n1 + " != " + n2 + ")");
	}
}

template <class C>
void	test_vector(int seed)
{
	std::srand(seed);
	void (*array[])(const std::string &, C &, const std::string &, C &) = {
			&monkey_vector::test_assign,
			&monkey_vector::test_construct,
			&monkey_vector::test_copy_construct_equal,
			&monkey_vector::test_at,
			&monkey_vector::test_operator_index,
			&monkey_vector::test_clear,
			&monkey_vector::test_insert,
			&monkey_vector::test_erase,
			&monkey_vector::test_pop_back,
			&monkey_vector::test_push_back,
			&monkey_vector::test_resize,
			&monkey_vector::test_swap,
			&monkey_vector::test_stdswap,
			&monkey_vector::test_comparison
	};

	C v1;
	C v2;
	logger.log<logger::CPP>("C v1;");
	logger.log<logger::CPP>("C v2;");
	logger.log<logger::CPP>("C::iterator it1;");
	logger.log<logger::CPP>("C::iterator it2;");
	logger.log<logger::CPP>("C::iterator it3;");
	for (int i = 0; NTEST == -1 || i < NTEST; i++)
	{
		if (!logger.log<logger::BREAKPOINT>(""))
			logger.log<logger::NONE>("========================================");
		int rand = std::rand() % sizeof(array) / sizeof (void (*)(C &, C &));
		if (std::rand() % 2)
			array[rand]("v1", v1, "v2", v2);
		else
			array[rand]("v2", v2, "v1", v1);
		logger.log<logger::TITLE>("printing v1:");
		monkey_vector::print_full_vect(v1, "v1");
		logger.log<logger::TITLE>("printing v2:");
		monkey_vector::print_full_vect(v2, "v2");
	}
}
