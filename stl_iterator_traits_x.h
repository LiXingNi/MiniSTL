#ifndef _STL_ITERATOR_TRAITS_X_H
#define _STL_ITERATOR_TRAITS_X_H

namespace _LXX
{
	
	struct input_iterator_tag{};
	struct output_iterator_tag{};
	struct forward_iterator_tag : public input_iterator_tag{};
	struct bidirectional_iterator_tag : public forward_iterator_tag{};
	struct random_access_iterator_tag : public bidirectional_iterator_tag{};

	template <class Category, class T, class Distance = ptrdiff_t,
	class Pointer = T*, class Reference = T&>
	struct iterator
	{
		typedef Category	iterator_category;
		typedef T			value_type;
		typedef Distance	difference_type;
		typedef Pointer		pointer;
		typedef Reference	reference;
	};

	template <typename Iterator>
	struct iterator_traits
	{
		typedef typename Iterator::iterator_category	iterator_category;
		typedef	typename Iterator::value_type			value_type;
		typedef typename Iterator::difference_type		difference_type;
		typedef typename Iterator::pointer				pointer;
		typedef typename Iterator::reference			reference;
	};


	template<class T>
	struct iterator_traits < T * >
	{
		typedef random_access_iterator_tag	iterator_category;
		typedef T							value_type;
		typedef ptrdiff_t					difference_type;
		typedef T*							pointer;
		typedef T&							reference;
	};

	template<class T>
	struct iterator_traits <const T * >
	{
		typedef random_access_iterator_tag	iterator_category;
		typedef T							value_type;
		typedef ptrdiff_t					difference_type;
		typedef const T*					pointer;
		typedef const T&					reference;
	};

	template<class Iterator>
	inline typename iterator_traits<Iterator>::iterator_category iterator_category(const Iterator &)
	{
		typedef typename iterator_traits<Iterator>::iterator_category	category;   //此处不可直接使用 Iterator::iterator_category 的原因是因为对内置类型的指针而言，该操作会报错
		return category();
	}

	template<class Iterator>
	inline typename iterator_traits<Iterator>::value_type* value_type(const Iterator &)
	{
		return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
	}

	template<class Iterator>
	inline typename iterator_traits<Iterator>::difference_type* distance_type(const Iterator&)
	{
		return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
	}


	template<class InputIterator>
	inline typename iterator_traits<InputIterator>::difference_type __distance(InputIterator first, InputIterator last, input_iterator_tag)
	{
		typename iterator_traits<InputIterator>::difference_type n = 0;
		while (first != last)
		{
			++first;
			++n;
		}
		return n;
	}

	template<class RandomAccessIterator>
	inline typename iterator_traits<RandomAccessIterator>::difference_type __distance(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag)
	{
		return last - first;
	}

	template <class InputIterator>
	inline typename iterator_traits<InputIterator>::difference_type distance(InputIterator first, InputIterator last)
	{
		typedef typename iterator_traits<InputIterator>::iterator_category	category;
		return __distance(first, last, category());
	}


	template<class RandomAccessIterator,class Distance>
	void __advance(RandomAccessIterator &iter, Distance n, random_access_iterator_tag)
	{
		iter += n;
	}

	template<class RandomAccessIterator, class Distance>
	void __advance(RandomAccessIterator &iter, Distance n, input_iterator_tag)
	{
		while (n--)
		{
			++iter;
		}
	}

	template<class InputIterator,class Distance>
	void advance(InputIterator& iter, Distance n)
	{
		__advance(iter, n, iterator_traits<InputIterator>::iterator_category());
	}
}

#endif