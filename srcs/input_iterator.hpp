//
// Created by joachim on 3/9/22.
//

#ifndef FT_CONTAINERS_MONKEY_TESTER_INPUT_ITERATOR_HPP
# define FT_CONTAINERS_MONKEY_TESTER_INPUT_ITERATOR_HPP
#include <stdlib.h>
#include <set>
#include <exception>
#include <iterator>

/**
 * this class downgrade any iterator satisfying the input iterator
 * requirement to the most basic input iterator
 * @tparam base_it the iterator to downgrade
 * @warning an incrementation invalidate all copies
 */
template <class base_it>
class	input_iterator
{
	base_it	it;
	bool	valid;
public:
	typedef typename std::iterator_traits<base_it>::value_type value_type;
	typedef typename std::iterator_traits<base_it>::pointer pointer;
	typedef typename std::iterator_traits<base_it>::reference reference;
	typedef typename std::iterator_traits<base_it>::difference_type difference_type;
	typedef typename std::input_iterator_tag iterator_category;
	std::set<input_iterator*> equivalents;
	input_iterator() : it(), valid(true), equivalents() {}
	input_iterator(const base_it it) : it(it), valid(true), equivalents() {}
	input_iterator(const input_iterator &src) : it(src.it), valid(src.valid), equivalents(src.equivalents) {
		if (valid)
		{
			this->equivalents.insert((input_iterator *)&src);
			for (typename std::set<input_iterator *>::iterator i = this->equivalents.begin();
				 i != this->equivalents.end(); i++)
				(*i)->equivalents.insert(this);
		}
	}
	~input_iterator()
	{
		for (typename std::set<input_iterator *>::iterator i = this->equivalents.begin(); i != this->equivalents.end(); i++)
			(*i)->equivalents.erase(this);
		this->equivalents.clear();
	}
	input_iterator &operator=(input_iterator const & src) {
		for (typename std::set<input_iterator *>::iterator i = this->equivalents.begin(); i != this->equivalents.end(); i++)
			(*i)->equivalents.erase(this);
		this->equivalents.clear();
		this->it = src.it;
		this->valid = src.valid;
		if (valid) {
			this->equivalents.insert(&src);
			for (typename std::set<input_iterator *>::iterator i = this->equivalents.begin(); i != this->equivalents.end(); i++)
				(*i)->equivalents.insert(this);
		}
	}
	class invalid_inputit : public std::exception
	{
		const char * what() const throw()
		{
			return ("an invalidated input iterator was used");
		}
	};
	friend bool operator==(input_iterator const &l, input_iterator const &r){
		return (l.it == r.it);
	}
	friend bool operator!=(input_iterator const &l, input_iterator const &r){
		return (l.it != r.it);
	}
	const typename std::iterator_traits<base_it>::value_type &operator*() const {
		if (!this->valid)
			throw invalid_inputit();
		return (*it);
	}
	const typename std::iterator_traits<base_it>::pointer operator->() const {
		if (!this->valid)
			throw invalid_inputit();
		return (&*it);
	}
	input_iterator &operator++() {
		if (!this->valid)
			throw invalid_inputit();
		for (typename std::set<input_iterator *>::iterator i = this->equivalents.begin(); i != this->equivalents.end(); i++)
		{
			(*i)->valid = false;
			(*i)->equivalents.clear();
		}
		this->equivalents.clear();
		++(this->it);
		return(*this);
	}
	input_iterator operator++(int) {
		if (!this->valid)
			throw invalid_inputit();
		input_iterator ret(*this);
		++(*this);
		return(ret);
	}
};

#endif //FT_CONTAINERS_MONKEY_TESTER_INPUT_ITERATOR_HPP
