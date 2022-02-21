
#include "../includes.hpp"
#include <iostream>
#include <set>
#include <cstdlib>
#include <iterator>
#include <string>
#include <climits>
#ifndef NTEST
# define NTEST 1000
#endif

namespace nstest_map {
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

	template<>
	ft::pair<std::string, std::string> get_value<ft::pair<std::string, std::string> >() {
		return (ft::pair<std::string, std::string>(get_value<std::string>(), get_value<std::string>()));
	}

	template<>
	ft::pair<int, int> get_value<ft::pair<int, int> >() {
		return (ft::pair<int, int>(get_value<int>(), get_value<int>()));
	}

	template<>
	std::pair<std::string, std::string> get_value<std::pair<std::string, std::string> >() {
		return (std::pair<std::string, std::string>(get_value<std::string>(), get_value<std::string>()));
	}

	template<>
	std::pair<int, int> get_value<std::pair<int, int> >() {
		return (std::pair<int, int>(get_value<int>(), get_value<int>()));
	}

	template<class C, template<class, class> class P>
	void print_full_map(const C &m) {
		std::cout << "empty: " << m.empty() << std::endl;
		std::cout << "size: " << m.size() << std::endl;
	//	std::cout << "capacity: " << m.capacity() << std::endl;
	//	std::cout << "max_size: " << m.max_size() << std::endl;
		std::cout << "content:" << std::endl;
		for (typename C::const_iterator i = m.begin(); i != m.end(); i++)
			std::cout << " " << i->first << "-> " << i->second << std::endl;
		std::cout << "reverse content:" << std::endl;
		for (typename C::const_reverse_iterator i = m.rbegin(); i != m.rend(); i++)
			std::cout << " " << i->first << "-> " << i->second << std::endl;
	}

	template<class C, template<class, class> class P>
	void test_clear(C &m1, C &) {
		std::cout << "test_clear:" << std::endl;
		C v3(m1);
		print_full_map<C, P>(m1);
		m1.clear();
		print_full_map<C, P>(v3);
		v3.clear();
	}

	template<class C, template<class, class> class P>
	void test_copy_construct_equal(C &m1, C &m2) {
		std::cout << "test_copy_construct_equal:" << std::endl;

		C v3(m1);
		print_full_map<C, P>(m1);
		print_full_map<C, P>(m2);
		print_full_map<C, P>(v3);
		v3 = m1;
		print_full_map<C, P>(m1);
		print_full_map<C, P>(m2);
		print_full_map<C, P>(v3);
		m1 = v3;
		m1 = m2;
		m2 = v3;
		v3 = C(v3);
		print_full_map<C, P>(m1);
		print_full_map<C, P>(m2);
		print_full_map<C, P>(v3);
		v3 = C(m1);
	}

	template<class C, template<class, class> class P>
	void test_construct(C &m1, C &) {
		switch (std::rand() % 5) {
			case 0:
				std::cout << "test_construct:" << std::endl;
				const std::set<typename C::value_type> s(m1.begin(), m1.end());
				m1 = C(s.begin(), s.end());
				print_full_map<C, P>(m1);
				break;
		}
	}

	template<class C, template<class, class> class P>
	void test_at(C &m1, C &m2) {
		std::cout << "test_at:" << std::endl;

		for (int i = 0; i < 5; i++) {
			try {
				if (!m1.empty())
					m1.at(get_value<typename C::key_type>()) = get_value<typename C::mapped_type>();
				if (!m2.empty())
					std::cout << m2.at(get_value<typename C::key_type>()) << std::endl;
				std::cout << m1.at(get_value<typename C::key_type>()) << std::endl;
				std::cout << m2.at(get_value<typename C::key_type>()) << std::endl;
			}
			catch (std::out_of_range &e) {
				std::cout << "out_of_range catched" << std::endl;
			}
		}
		print_full_map<C, P>(m1);
		print_full_map<C, P>(m2);
	}

	template<class C, template<class, class> class P>
	void test_operator_index(C &m1, C &m2) {
		std::cout << "test_operator_index:" << std::endl;

		for (int i = 0; i < 5; i++) {
			if (!m1.empty())
				m1[get_value<typename C::key_type>()] = get_value<typename C::mapped_type>();
			if (!m2.empty())
				std::cout << m2[get_value<typename C::key_type>()] << std::endl;
		}
		print_full_map<C, P>(m1);
		print_full_map<C, P>(m2);
	}

	template<class C, template<class, class> class P>
	void test_insert(C &m1, C &m2) {
		std::cout << "test_insert:" << std::endl;

		P<typename C::iterator, bool> ret;
		for (int i = 0; i < 5; i++) {
			typename C::iterator it;
			if (!m1.empty())
				it = add_iterator(m1.begin(), (std::rand() % m1.size()));
			else
				it = m1.end();
			switch (std::rand() % 3) {
				case (0):
					ret = m1.insert(get_value<typename C::value_type>());
					std::cout << ret.first->first << " " << ret.second << std::endl;
					break;
				case (1):
					if (!m1.empty() && !m2.empty())
						m2.insert(it, add_iterator(it, (std::rand() % (m1.size() - nstest_map::distance(m1.begin(), it)))));
					else if (!m2.empty())
						m2.insert(it, m1.end());
					else
						m2.insert(it, m1.end());
					break;
				case (2):
					std::cout << m1.insert(it, get_value<typename C::value_type>())->first << std::endl;
					break;
			}
		}
		print_full_map<C, P>(m1);
	}

	template<class C, template<class, class> class P>
	void test_erase(C &m1, C &m2) {
		std::cout << "test_erase:" << std::endl;

		for (int i = 0; i < 5; i++) {
			typename C::iterator it2 = add_iterator(m2.begin(), (m2.empty() ? 0 : (std::rand() % m2.size())));
			switch (std::rand() % 3) {
				case (0):
					if (!m1.empty()) {
						m1.erase(add_iterator(m1.begin(), (std::rand() % m1.size())));
					}
					break;
				case (1):
					if (!m1.empty()) {
						std::cout << m1.erase(get_value<typename C::mapped_type>()) << std::endl;
					}
					break;
				case (2):
					if (!m2.empty() && it2 != m2.end()) {
						m2.erase(it2, add_iterator(it2, (std::rand() % nstest_map::distance(it2, m2.end()))));
					}
			}

		}
	}

	template<class C, template<class, class> class P>
	void test_count(C &m1, C &) {
		std::cout << "test_count:" << std::endl;
		std::cout << m1.count(get_value<typename C::mapped_type>()) << std::endl;
	}

	template<class C, template<class, class> class P>
	void test_find(C &m1, C &) {
		std::cout << "test_find:" << std::endl;
		typename C::iterator ret =  m1.find(get_value<typename C::mapped_type>());
		if (ret != m1.end())
			std::cout << ret->first << std::endl;
	}

	template<class C, template<class, class> class P>
	void test_equal_range(C &m1, C &) {
		std::cout << "test_equal_range:" << std::endl;
		P<typename C::iterator, typename C::iterator> ret =  m1.equal_range(get_value<typename C::mapped_type>());
		if (ret.first != m1.end())
			std::cout << ret.first->first << std::endl;
		if (ret.second != m1.end())
			std::cout << ret.second->first << std::endl;
	}

	template<class C, template<class, class> class P>
	void test_upper_bound(C &m1, C &) {
		std::cout << "test_upper_bound:" << std::endl;
		typename C::iterator ret =  m1.upper_bound(get_value<typename C::mapped_type>());
		if (ret != m1.end())
			std::cout << ret->first << std::endl;
	}

	template<class C, template<class, class> class P>
	void test_lower_bound(C &m1, C &) {
		std::cout << "test_lower_bound:" << std::endl;
		typename C::iterator ret =  m1.lower_bound(get_value<typename C::mapped_type>());
		if (ret != m1.end())
			std::cout << ret->first << std::endl;
	}

	template<class C, template<class, class> class P>
	void test_swap(C &m1, C &m2) {
		std::cout << "test_swap:" << std::endl;

		m1.swap(m2);
	}

	template<class C, template<class, class> class P>
	void test_stdswap(C &m1, C &m2) {
		std::cout << "test_stdswap:" << std::endl;

		std::swap(m1, m2);
	}

	template<class C, template<class, class> class P>
	void test_comparison(C &m1, C &m2) {
		std::cout << "test_comparison:" << std::endl;

		std::cout << "m1 < m2: " << (m1 < m2) << std::endl;
		std::cout << "m1 > m2: " << (m1 > m2) << std::endl;
		std::cout << "m1 <= m2: " << (m1 <= m2) << std::endl;
		std::cout << "m1 >= m2: " << (m1 >= m2) << std::endl;
		std::cout << "m1 == m2: " << (m1 == m2) << std::endl;
		std::cout << "m1 != m2: " << (m1 != m2) << std::endl;
	}
}
template<class C, template<class, class> class P>
void	test_map(int seed) {
		std::srand(seed);
		void  (*array[])(C &,C &) = {
				&nstest_map::test_construct<C, P>,
				&nstest_map::test_copy_construct_equal<C, P>,
				&nstest_map::test_operator_index<C, P>,
				&nstest_map::test_clear<C, P>,
				&nstest_map::test_insert<C, P>,
				&nstest_map::test_count<C, P>,
				&nstest_map::test_find<C, P>,
				&nstest_map::test_equal_range<C, P>,
				&nstest_map::test_upper_bound<C, P>,
				&nstest_map::test_lower_bound<C, P>,
				&nstest_map::test_erase<C, P>,
				&nstest_map::test_swap<C, P>,
				&nstest_map::test_stdswap<C, P>,
				&nstest_map::test_comparison<C, P>
		};
		C m1;
		C m2;
		for (int i = 0; NTEST == -1 || i < NTEST; i++) {
			int rand = std::rand() % sizeof(array) / sizeof(void (*)(C &, C &));
			if (std::rand() % 2)
				array[rand](m1, m2);
			else
				array[rand](m2, m1);
			nstest_map::print_full_map<C, P>(m1);
			nstest_map::print_full_map<C, P>(m2);
			std::cout << "========================================" << std::endl;
		}
	}
