
#include "vector.hpp"
#include <vector>
#include <set>
#include <cstdlib>
#include <string>
#include <climits>
#ifndef NTEST
# define NTEST 1000
#endif

namespace nstest_vector {


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
	void print_full_vect(const C &v) {
		std::cout << "empty: " << v.empty() << std::endl;
		std::cout << "size: " << v.size() << std::endl;
//	std::cout << "capacity: " << v.capacity() << std::endl;
//	std::cout << "max_size: " << v.max_size() << std::endl;
		if (!v.empty()) {
			std::cout << "front: " << v.front() << std::endl;
			std::cout << "back: " << v.back() << std::endl;
		}
		std::cout << "content:" << std::endl;
		for (typename C::const_iterator i = v.begin(); i != v.end(); i++)
			std::cout << " " << *i << std::endl;
		std::cout << "reverse content:" << std::endl;
		for (typename C::const_reverse_iterator i = v.rbegin(); i != v.rend(); i++)
			std::cout << " " << *i << std::endl;
	}

	template<class C>
	void test_clear(C &v1, C &) {
		std::cout << "test_clear:" << std::endl;
		C v3(v1);
		print_full_vect(v1);
		v1.clear();
		print_full_vect(v3);
		v3.clear();
	}

	template<class C>
	void test_copy_construct_equal(C &v1, C &v2) {
		std::cout << "test_copy_construct_equal:" << std::endl;

		C v3(v1);
		print_full_vect(v1);
		print_full_vect(v2);
		print_full_vect(v3);
		v3 = v1;
		print_full_vect(v1);
		print_full_vect(v2);
		print_full_vect(v3);
		v1 = v3;
		v1 = v2;
		v2 = v3;
		v3 = C(v3);
		print_full_vect(v1);
		print_full_vect(v2);
		print_full_vect(v3);
		v3 = C(v1);
	}

	template<class C>
	void test_construct(C &v1, C &) {
		switch (std::rand() % 10) {
			case 0:
				std::cout << "test_construct:" << std::endl;\
            v1 = C(std::rand() % 20, get_value<typename C::value_type>());
				print_full_vect(v1);
				break;
			case 1:
				std::cout << "test_construct:" << std::endl;
				const std::set<typename C::value_type> s(v1.begin(), v1.end());
				v1 = C(s.begin(), s.end());
				print_full_vect(v1);
				break;
		}
	}

	template<class C>
	void test_assign(C &v1, C &v2) {
		std::cout << "test_assign:" << std::endl;

		C v3;

		v3.assign(12, get_value<typename C::value_type>());
		print_full_vect(v1);
		print_full_vect(v2);
		print_full_vect(v3);
		v1.assign(v3.begin(), v3.end() - 1);
		print_full_vect(v1);
		print_full_vect(v2);
		print_full_vect(v3);
	}

	template<class C>
	void test_at(C &v1, C &v2) {
		std::cout << "test_at:" << std::endl;

		for (int i = 0; i < 5; i++) {
			try {
				if (!v1.empty())
					v1.at((int) std::rand() % v1.size()) = get_value<typename C::value_type>();
				if (!v2.empty())
					std::cout << v2.at((int) std::rand() % v2.size()) << std::endl;
				std::cout << v1.at((unsigned int) std::rand());
				std::cout << v2.at((unsigned int) std::rand());
			}
			catch (std::out_of_range &e) {
				std::cout << "out_of_range catched" << std::endl;
			}
		}
		print_full_vect(v1);
		print_full_vect(v2);
	}

	template<class C>
	void test_operator_index(C &v1, C &v2) {
		std::cout << "test_operator_index:" << std::endl;

		for (int i = 0; i < 5; i++) {
			if (!v1.empty())
				v1[(int) std::rand() % v1.size()] = get_value<typename C::value_type>();
			if (!v2.empty())
				std::cout << v2[(int) std::rand() % v2.size()] << std::endl;
		}
		print_full_vect(v1);
		print_full_vect(v2);
	}

	template<class C>
	void test_insert(C &v1, C &v2) {
		std::cout << "test_insert:" << std::endl;
		for (int i = 0; i < 5; i++) {
			typename C::iterator it;
			if (!v1.empty())
				it = v1.begin() + (std::rand() % v1.size());
			else
				it = v1.end();
			try {
				switch (std::rand() % 3) {
					case (0):
						std::cout << *v1.insert(it, get_value<typename C::value_type>()) << std::endl;
						break;
					case (1):
						if (!v1.empty() && !v2.empty())
							v2.insert(v2.begin() + (std::rand() % v2.size()), it,
									  it + (std::rand() % (v1.size() - (it - v1.begin()))));
						else if (!v2.empty())
							v2.insert(v2.begin() + (std::rand() % v2.size()), it, v1.end());
						else
							v2.insert(v2.begin(), it, v1.end());
						break;
					case (2):
						if (!v2.empty())
							v2.insert(v2.begin() + (std::rand() % v2.size()), std::rand() % 5,
									  get_value<typename C::value_type>());
						else
							v2.insert(v2.begin(), std::rand() % 5, get_value<typename C::value_type>());
;
				}
			}
			catch (std::exception &e) {
				std::cout << e.what() << std::endl;
			}
		}
		print_full_vect(v1);
	}

	template<class C>
	void test_erase(C &v1, C &v2) {
		std::cout << "test_erase:" << std::endl;

		typename C::iterator ret;
		for (int i = 0; i < 5; i++) {
			typename C::iterator it2 = v2.begin() + (v2.empty() ? 0 : (std::rand() % v2.size()));
			switch (std::rand() % 3) {
				case (0):
					if (!v1.empty()) {
						ret = v1.erase(v1.begin() + (std::rand() % v1.size()));
						if (ret < v1.end())
							std::cout << *ret << std::endl;
					}
					break;
				case (2):
					if (!v2.empty()) {
						std::cout << (v2.end() - it2) << std::endl;
						v2.erase(it2, it2 + (std::rand() % (v2.end() - it2)));
					}
			}

		}
	}

	template<class C>
	void test_push_back(C &v1, C &) {
		std::cout << "test_push_back:" << std::endl;

		v1.push_back(get_value<typename C::value_type>());
	}

	template<class C>
	void test_pop_back(C &v1, C &) {
		std::cout << "test_pop_back:" << std::endl;

		if (!v1.empty())
			v1.pop_back();
	}

	template<class C>
	void test_resize(C &v1, C &) {
		std::cout << "test_resize:" << std::endl;
		v1.resize(std::rand() % (v1.empty() ? 10 : (2 * v1.size())), get_value<typename C::value_type>());
	}

	template<class C>
	void test_swap(C &v1, C &v2) {
		std::cout << "test_swap:" << std::endl;
		v1.swap(v2);
	}

	template<class C>
	void test_stdswap(C &v1, C &v2) {
		std::cout << "test_stdswap:" << std::endl;

		std::swap(v1, v2);
	}

	template<class C>
	void test_comparison(C &v1, C &v2) {
		std::cout << "test_comparison:" << std::endl;

		std::cout << "v1 < v2" << (v1 < v2) << std::endl;
		std::cout << "v1 > v2" << (v1 > v2) << std::endl;
		std::cout << "v1 <= v2" << (v1 <= v2) << std::endl;
		std::cout << "v1 >= v2" << (v1 >= v2) << std::endl;
		std::cout << "v1 == v2" << (v1 == v2) << std::endl;
		std::cout << "v1 != v2" << (v1 != v2) << std::endl;
	}
}

template <class C>
void	test_vector(int seed)
{
	std::srand(seed);
	void (*array[])(C &, C &) = {
			&nstest_vector::test_assign,
			&nstest_vector::test_construct,
			&nstest_vector::test_copy_construct_equal,
			&nstest_vector::test_at,
			&nstest_vector::test_operator_index,
			&nstest_vector::test_clear,
			&nstest_vector::test_insert,
			&nstest_vector::test_erase,
			&nstest_vector::test_pop_back,
			&nstest_vector::test_push_back,
			&nstest_vector::test_resize,
			&nstest_vector::test_swap,
			&nstest_vector::test_stdswap,
			&nstest_vector::test_comparison
	};
	C v1;
	C v2;
	for (int i = 0; NTEST == -1 || i < NTEST; i++)
	{
		int rand = std::rand() % sizeof(array) / sizeof (void (*)(C &, C &));
		if (std::rand() % 2)
			array[rand](v1, v2);
		else
			array[rand](v2, v1);
		nstest_vector::print_full_vect(v1);
		nstest_vector::print_full_vect(v2);
		std::cout << "========================================" << std::endl;
	}
}
