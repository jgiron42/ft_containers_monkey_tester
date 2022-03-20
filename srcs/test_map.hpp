#include "../includes.hpp"
#include "common.hpp"
#include <iostream>
#include <map>
#include <set>
#include <cstdlib>
#include <iterator>
#include <string>
#include <climits>
#include "logger.hpp"
#include <sstream>
#include "input_iterator.hpp"
#ifndef NTEST
# define NTEST 1000
#endif

extern class logger logger;

namespace monkey_map {

	/**
	 * this class is given in template argument to the tested map in order to check if
	 * key_compare is used for comparisons
	 */
	class my_compare {
	public:
		my_compare(){}
		std::string reverse(std::string s) const
		{
			long  int i = 0;
			long  int j = s.length() - 1;
			char swap;
			while (i < j)
			{
				swap = s[i];
				s[i] = s[j];
				s[j] = swap;
				++i;
				--j;
			}
			return (s);
		}
		bool	operator()(const std::string &l, const std::string &r) const
		{
			return (reverse(l) > reverse(r));
		}
	};

	template<class C, template<class, class> class P>
	void test_clear(const std::string &n1, C &m1, const std::string &,  C &) {
		logger.log<logger::TITLE>("test_clear:");
		logger.create_block();
		logger.log<logger::CPP>("C m3(" + n1 + ");");
		C m3(m1);
		logger.log<logger::CPP>(n1 + ".clear();");
		m1.clear();
		monkey::print_full_map<C, P>(m3, "m3");
		logger.log<logger::CPP>("m3.clear();");
		m3.clear();
		logger.pop_block();
	}

	template<class C, template<class, class> class P>
	void test_copy_construct_equal(const std::string &n1, C &m1, const std::string &n2,  C &m2) {
		logger.log<logger::TITLE>("test_copy_construct_equal:");

		logger.create_block();
		logger.log<logger::CPP>("C m3(" + n1 + ");");
		C v3(m1);
		logger.log<logger::CPP>(n1 + " = " + n2 + ";");
		m1 = m2;
		logger.log<logger::CPP>(n2 + " = m3;");
		m2 = v3;
		logger.pop_block();
	}

	template<class C, template<class, class> class P>
	void test_construct(const std::string &n1, C &m1, const std::string &,  C &) {
		logger.log<logger::TITLE>("test_construct:");
		logger.create_block();
		const std::set<typename C::value_type> s(m1.begin(), m1.end());
		logger.log<logger::CPP>("std::set<C::value_type> set(" + n1 + ".begin(), " + n1 + ".end());");
		m1 = C( input_iterator<typename std::set<typename C::value_type>::iterator>(s.begin()),  input_iterator<typename std::set<typename C::value_type>::iterator>(s.end()));
		logger.log<logger::CPP>( n1 + " = C( input_iterator<std::set<C::value_type>::iterator>(set.begin()),  input_iterator<std::set<C::value_type>::iterator>(set.end()));");
		logger.pop_block();
	}

	template<class C, template<class, class> class P>
	void test_operator_index(const std::string &n1, C &m1, const std::string &,  C &) {
		logger.log<logger::TITLE>("test_operator_index:");
		std::string key;
		std::string val;

		for (int i = 0; i < 5; i++) {
			key = monkey::get_value<typename C::mapped_type>();
			val = monkey::get_value<typename C::mapped_type>();
			m1[key] = val;
			logger.log<logger::CPP>(n1 + "[\"" + key +"\"] = \"" + val + "\";");

			key = monkey::get_value<typename C::mapped_type>();
			logger.log<logger::CPP>("std::cout << " + n1 + "[\"" + key +"\"] << std::endl;");
			std::cout << m1[key] << std::endl;
		}
//		monkey::print_full_map<C, P>(m1);
//		monkey::print_full_map<C, P>(m2);
	}

	template<class C, template<class, class> class P>
	void test_insert(const std::string &n1, C &m1, const std::string &n2,  C &m2) {
		logger.log<logger::TITLE>("test_insert:");
		P<typename C::iterator, bool>												pair_it_bool;
		P<typename C::key_type, typename C::mapped_type> 					val;
		typename C::iterator														it1;
		typename C::iterator														it2;
		unsigned int 														rand;
		std::string key;
		std::string value;

		for (int i = 0; i < 5; i++) {

			switch (std::rand() % 3) {
				case (0):
					logger.log<logger::TITLE>("insert value_type:");
					key = monkey::get_value<typename C::key_type>();
					value = monkey::get_value<typename C::mapped_type>();
					val = NAMESPACE::make_pair(key, value);
					logger.log<logger::CPP>("val = _P<C::key_type, C::mapped_type>(\"" + key + "\", \"" + value + "\");");
					logger.log<logger::CPP>("pair_it_bool = " + n1 + ".insert(val);");
					pair_it_bool = m1.insert(val);
					logger.log<logger::NONE>("ret = " + SSTR(pair_it_bool.first->first) + " " + SSTR(pair_it_bool.second));
					logger.log<logger::CPP_LOG>("\"ret = \" + SSTR(pair_it_bool.first->first) + SSTR(pair_it_bool.second)");
					break;
				case (1):
					logger.log<logger::TITLE>("insert range:" );
					rand = std::rand();
					it1 = monkey::get_itn(m1, rand);
					rand = std::rand();
					it2 = monkey::get_itn(m1, rand, it1);
					logger.log<logger::CPP>("it1 = monkey::get_itn(" + n1 + ", " + SSTR(rand) + ");");
					logger.log<logger::CPP>("it2 = monkey::get_itn(" + n1 + ", " + SSTR(rand) + ", it1);");
					logger.log<logger::CPP>(n2 + ".insert(input_iterator<C::iterator>(it1), input_iterator<C::iterator>(it2));");
					m2.insert(input_iterator<typename 	C::iterator>(it1), input_iterator<typename C::iterator>(it2));
					break;
				case (2):
					logger.log<logger::TITLE>("insert hint:" );
					rand = std::rand();
					it1 = monkey::get_itn(m1, rand);
					logger.log<logger::CPP>("it1 = monkey::get_itn(" + n1 + ", " + SSTR(rand) + ");");
					key = monkey::get_value<typename C::key_type>();
					value = monkey::get_value<typename C::mapped_type>();
					val = NAMESPACE::make_pair(key, value);
					logger.log<logger::CPP>("val = _P<C::key_type, C::mapped_type>(\"" + key + "\", \"" + value + "\");");
					logger.log<logger::CPP>("it1 = " + n1 + ".insert(it1, val);");
					it1 = m1.insert(it1, val);
					logger.log<logger::NONE>("ret = " + SSTR(it1->first) + " " + SSTR(it1->second));
					logger.log<logger::CPP_LOG>("\"ret = \" + SSTR(it1->first) + SSTR(it1->second)");
					break;
			}
		}
		monkey::print_full_map<C, P>(m1, n1);
	}

	template<class C, template<class, class> class P>
	void test_erase(const std::string &n1, C &m1, const std::string &,  C &) {
		typename C::mapped_type																val;
		typename C::iterator														it1;
		typename C::iterator														it2;
		int	rand;
		int ret;

		logger.log<logger::TITLE>("test_erase:" );
		for (int i = 0; i < 5; i++) {
			switch (std::rand() % 3) {
				case (0):
					if (!m1.empty()) {
						logger.log<logger::TITLE>("erase it:" );
						rand = std::rand();
						it1 = monkey::get_itn(m1, rand);
						logger.template log<logger::CPP>("it1 = monkey::get_itn(" + n1 + ", " + SSTR(rand) + ");");
						m1.erase(it1);
						logger.template log<logger::CPP>("if (!" +n1 + ".empty())");
						logger.create_block();
						logger.template log<logger::CPP>(n1 + ".erase(it1);");
						logger.pop_block();
					}
					break;
				case (1):
					if (!m1.empty()) {
						logger.log<logger::TITLE>("erase key_type:" );
						val = monkey::get_value<typename C::key_type>();
						ret = m1.erase(val);
						logger.log<logger::CPP>("ret = " + n1 + ".erase(\"" + val + "\");");
						logger.log<logger::CPP_LOG>("\"ret = \" + SSTR(ret)");
						logger.log<logger::NONE>("ret = " + SSTR(ret));
					}
					break;
				case (2):
					logger.log<logger::TITLE>("erase range:" );
					rand = std::rand();
					it1 = monkey::get_itn(m1, rand);
					logger.log<logger::CPP>("it1 = monkey::get_itn(" + n1 + ", " + SSTR(rand) + ");");
					rand = std::rand();
					it2 = monkey::get_itn(m1, rand, it1);
					logger.log<logger::CPP>("it2 = monkey::get_itn(" + n1 + ", " + SSTR(rand) + ", it1);");
					m1.erase(it1, it2);
					logger.log<logger::CPP>(n1 + ".erase(it1, it2);");
			}
		}
	}

	template<class C, template<class, class> class P>
	void test_count(const std::string &n1, C &m1, const std::string &,  C &) {
		std::string key = monkey::get_value<typename C::mapped_type>();
		logger.log<logger::TITLE>("test_count: [" + SSTR(key) + "]" );
		logger.log<logger::CPP>("ret = " + n1 + ".count(\"" + key + "\");");
		int ret = m1.count(key);
		logger.log<logger::CPP_LOG>("\"ret = \" + SSTR(ret)");
		logger.log<logger::NONE>("ret = " + SSTR(ret));
	}

	template<class C, template<class, class> class P>
	void test_find(const std::string &n1, C &m1, const std::string &,  C &) {
		std::string key = monkey::get_value<typename C::mapped_type>();
		logger.log<logger::TITLE>("test_find: [" + SSTR(key)  + "]");

		logger.log<logger::CPP>("it1 = " + n1 + ".find(\"" + key + "\");");
		typename C::iterator ret =  m1.find(key);

		logger.template log<logger::CPP>("if (it1 != " + n1 + ".end())");
		logger.create_block();
		logger.template log<logger::CPP_LOG>("\"ret = \" + SSTR(it1->first)");
		logger.pop_block();
		if (ret != m1.end())
			std::cout << ret->first << std::endl;
	}

	template<class C, template<class, class> class P>
	void test_equal_range(const std::string &n1, C &m1, const std::string &,  C &) {
		std::string key = monkey::get_value<typename C::mapped_type>();
		logger.log<logger::TITLE>("test_equal_range: [" + SSTR(key) + "]" );
		P<typename C::iterator, typename C::iterator> ret =  m1.equal_range(key);
		logger.template log<logger::CPP>("range = " + n1 + ".equal_range(\"" + key + "\");");
		logger.template log<logger::CPP>("if (range.first != " + n1 + ".end())");
		logger.create_block();
		logger.template log<logger::CPP_LOG>("\"ret1 = \" + SSTR(range.first->first)");
		logger.pop_block();
		logger.template log<logger::CPP>("if (range.second != " + n1 + ".end())");
		logger.create_block();
		logger.template log<logger::CPP_LOG>("\"ret1 = \" + SSTR(range.second->first)");
		logger.pop_block();
		if (ret.first != m1.end())
			std::cout << ret.first->first << std::endl;
		if (ret.second != m1.end())
			std::cout << ret.second->first << std::endl;
	}

	template<class C, template<class, class> class P>
	void test_upper_bound(const std::string &n1, C &m1, const std::string &,  C &) {
		std::string key = monkey::get_value<typename C::mapped_type>();
		logger.log<logger::TITLE>("test_upper_bound: [" + SSTR(key) + "]" );

		typename C::iterator ret =  m1.upper_bound(key);
		logger.log<logger::CPP>("it1 = " + n1 + ".upper_bound(\"" + key + "\");");
		logger.template log<logger::CPP>("if (it1 != " + n1 + ".end())");
		logger.create_block();
		logger.template log<logger::CPP_LOG>("\"ret1 = \" + SSTR(it1->first)");
		logger.pop_block();
		if (ret != m1.end())
			std::cout << ret->first << std::endl;
	}

	template<class C, template<class, class> class P>
	void test_lower_bound(const std::string &n1, C &m1, const std::string &,  C &) {
		std::string key = monkey::get_value<typename C::mapped_type>();
		logger.log<logger::TITLE>("test_lower_bound: [" + SSTR(key) + "]");

		typename C::iterator ret =  m1.lower_bound(key);
		logger.log<logger::CPP>("it1 = " + n1 + ".lower_bound(\"" + key + "\");");
		logger.template log<logger::CPP>("if (it1 != " + n1 + ".end())");
		logger.create_block();
		logger.template log<logger::CPP_LOG>("\"ret1 = \" + SSTR(it1->first)");
		logger.pop_block();
		if (ret != m1.end())
			std::cout << ret->first << std::endl;
	}

	template<class C, template<class, class> class P>
	void test_swap(const std::string &n1, C &m1, const std::string &n2,  C &m2) {
		logger.log<logger::TITLE>("test_swap:");
		logger.log<logger::CPP>(n1 + ".swap(" + n2 + ");");
		m1.swap(m2);
	}

	template<class C, template<class, class> class P>
	void test_stdswap(const std::string &n1, C &m1, const std::string &n2,  C &m2) {
		logger.log<logger::TITLE>("test_stdswap:");
		logger.log<logger::CPP>("std::swap(" + n1 + ", " + n2 + ");");
		std::swap(m1, m2);
	}

	template<class C, template<class, class> class P>
	void test_comparison(const std::string &n1, C &m1, const std::string &n2,  C &m2) {
		logger.log<logger::TITLE>("test_comparison:");

		logger.log<logger::NONE>(n1 + " < " + n2 + ": " + SSTR(m1 < m2));
		logger.log<logger::NONE>(n1 + " > " + n2 + ": " + SSTR(m1 > m2));
		logger.log<logger::NONE>(n1 + " <= " + n2 + ": " + SSTR(m1 <= m2));
		logger.log<logger::NONE>(n1 + " >= " + n2 + ": " + SSTR(m1 >= m2));
		logger.log<logger::NONE>(n1 + " == " + n2 + ": " + SSTR(m1 == m2));
		logger.log<logger::NONE>(n1 + " != " + n2 + ": " + SSTR(m1 != m2));

		logger.log<logger::CPP_LOG>("\"" + n1 + " < " + n2 + ": \" + SSTR( " + n1 + " < " + n2 + ")");
		logger.log<logger::CPP_LOG>("\"" + n1 + " > " + n2 + ": \" + SSTR( " + n1 + " > " + n2 + ")");
		logger.log<logger::CPP_LOG>("\"" + n1 + " <= " + n2 + ": \" + SSTR( " + n1 + " <= " + n2 + ")");
		logger.log<logger::CPP_LOG>("\"" + n1 + " >= " + n2 + ": \" + SSTR( " + n1 + " >= " + n2 + ")");
		logger.log<logger::CPP_LOG>("\"" + n1 + " == " + n2 + ": \" + SSTR( " + n1 + " == " + n2 + ")");
		logger.log<logger::CPP_LOG>("\"" + n1 + " != " + n2 + ": \" + SSTR( " + n1 + " != " + n2 + ")");
	}
}
template<class C, template<class, class> class P>
void	test_map(int seed) {
		std::srand(seed);
		void  (*array[])(const std::string &, C &,const std::string &,C &) = {
				&monkey_map::test_construct<C, P>,
				&monkey_map::test_copy_construct_equal<C, P>,
				&monkey_map::test_operator_index<C, P>,
				&monkey_map::test_clear<C, P>,
				&monkey_map::test_insert<C, P>,
				&monkey_map::test_count<C, P>,
				&monkey_map::test_find<C, P>,
				&monkey_map::test_equal_range<C, P>,
				&monkey_map::test_upper_bound<C, P>,
				&monkey_map::test_lower_bound<C, P>,
				&monkey_map::test_erase<C, P>,
				&monkey_map::test_swap<C, P>,
				&monkey_map::test_stdswap<C, P>,
				&monkey_map::test_comparison<C, P>
		};

		C m1; logger.log<logger::CPP>("C m1;");
		C m2; logger.log<logger::CPP>("C m2;");
		logger.log<logger::CPP>("int ret;");
		logger.log<logger::CPP>("C::iterator it1;");
		logger.log<logger::CPP>("input_iterator<C::iterator> inputit1;");
		logger.log<logger::CPP>("C::iterator it2;");
		logger.log<logger::CPP>("input_iterator<C::iterator> inputit2;");
		logger.log<logger::CPP>("_P<C::iterator, bool> pair_it_bool;");
		logger.log<logger::CPP>("_P<C::iterator, C::iterator> range;");
		logger.log<logger::CPP>("_P<C::key_type, C::mapped_type> val;");
		for (int i = 0; NTEST == -1 || i < NTEST; i++) {
			if (!logger.log<logger::BREAKPOINT>(""))
				logger.log<logger::NONE>("========================================");

			int rand = std::rand() % sizeof(array) / sizeof(void (*)(C &, C &));
			if (std::rand() % 2)
				array[rand]("m1", m1,"m2", m2);
			else
				array[rand]("m2", m2,"m1", m1);
			logger.log<logger::TITLE>("printing m1:");
			monkey::print_full_map<C, P>(m1, "m1");
			logger.log<logger::TITLE>("printing m2:");
			monkey::print_full_map<C, P>(m2, "m2");

		}
	}
