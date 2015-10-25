#ifndef _STL_ALGOBASE_X_H
#define _STL_ALGOBASE_X_H

#include "stl_iterator_traits_x.h"
#include "stl_pair_x.h"

namespace _LXX
{

	//------------------------- equal ---------------------------------------------------
	template<class InputIterator1, class InputIterator2>
	bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2)
	{
		for (; first1 != last1; ++first1, ++first2)
		{
			if (*first1 != *first2)
				return false;
		}
		return true;
	}

	template<class InputIterator1, class InputIterator2,class BinaryOperation>
	bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2,BinaryOperation op)
	{
		for (; first1 != last1; ++first1, ++first2)
		{
			if (!op(*first1,*first2))
				return false;
		}
		return true;
	}


	//----------------------- fill -------------------------------------------------
	template<class InputIterator,class T>
	void fill(InputIterator first, InputIterator last, const T & value)
	{
		while (first != last)
		{
			*first = value;
			++first;
		}
	}

	//--------------------- fill_n -------------------------------------------------
	template<class InputIterator,class size,class T>
	InputIterator fill_n(InputIterator first, size n, const T & value)
	{
		while (n--)
		{
			*first = value;
			++first;
		}
		return first;
	}

	//------------------- iter_swap -------------------------------------------------
	template<class InputIterator1,class InputIterator2>
	void iter_swap(InputIterator1 a, InputIterator2 b)
	{
		_iter_swap(a, b, value_type(a));
	}

	template<class InputIterator1, class InputIterator2,class T>
	void _iter_swap(InputIterator1 a, InputIterator2 b, T*)
	{
		T tmp = *a;
		*a = *b;
		*b = tmp;
	}

	//------------------- max ------------------------------------------------------------
	template<class T>
	T max(const T & a, const T & b)
	{
		return a < b ? b : a;
	}

	template<class T,class Compare>
	T max(const T & a, const T & b, Compare comp)
	{
		return comp(a, b) ? b : a;
	}

	//----------------- min --------------------------------------------------------------
	template<class T>
	T min(const T & a, const T & b)
	{
		return b < a ? b : a;
	}

	template<class T, class Compare>
	T min(const T & a, const T & b, Compare comp)
	{
		return comp(b, a) ? b : a;
	}

	//--------------- swap ----------------------------------------------------------------
	template<class T>
	inline void swap(T & a, T & b)
	{
		T tmp = a;
		a = b;
		b = tmp;
	}

	//------------- mismatch --------------------------------------------------------------
	template<class InputIterator1,class InputIterator2>
	pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2)
	{
		while (first1 != last1 && *first1 == *first2)
		{
			++first1;
			++first2;
		}
		return _LXX::make_pair(first1, first2);
	}


	template<class InputIterator1, class InputIterator2,class BinaryOperation>
	pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2,BinaryOperation op)
	{
		while (first1 != last1 && op(*first1,*first2))
		{
			++first1;
			++first2;
		}
		return _LXX::make_pair(first1, first2);
	}


	//--------- lexicographical_compare ------------------------------------------------------
	template<class InputIterator1,class InputIterator2>
	bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2)
	{
		for (; first1 != last1 && first2 != last2; ++first1, ++first2)
		{
			if (*first1 < *first2)
				return true;
			if (*first1 > *first2)
				return false;
		}

		return first1 == last1 && first2 != last2;
	}

	template<class InputIterator1, class InputIterator2,class BinaryOperation>
	bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2,BinaryOperation op)
	{
		for (; first1 != last1 && first2 != last2; ++first1, ++first2)
		{
			if (op(*first1,*first2))
				return true;
			if (op(*first2, *first1))
				return false;
		}

		return first1 == last1 && first2 != last2;
	}

	inline bool lexicographical_compare(const char * first1, const char * last1, const char * first2, const char * last2)
	{
		const size_t len1 = last1 - first1;
		const size_t len2 = last2 - first2;

		const int result = memcmp(first1, first2, _LXX::min(len1, len2));

		return result != 0 ? result < 0 : len1 < len2;
	}

}

#endif