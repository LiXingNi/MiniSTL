#ifndef _STL_ALGO_X_H
#define _STL_ALGO_X_H

#include "stl_iterator_traits_x.h"
#include "stl_algobase_x.h"

namespace _LXX
{

	template<class RandomAccessIterator, class T, class Distance>
	RandomAccessIterator __lower_bound(RandomAccessIterator first, RandomAccessIterator last, const T & value, Distance *, random_access_iterator_tag)
	{
		Distance len = last - first;
		Distance half;
		RandomAccessIterator middle;

		while (len > 0)
		{
			half = len >> 1;
			middle = first = half;
			if (*middle < value)
			{
				first = middle + 1;
				len = len - half - 1;
			}
			else
				len = half;
		}
		return first;
	}

	template<class ForwardIterator, class T>
	inline ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last, const T & value)
	{
		return __lower_bound(first, last, value, distance_type(first), iterator_category(first));
	}

	//------------------------------------ some algorithm about set-------------------------------------------
	template<class InputIterator1, class InputIterator2, class OutputIterator>                    //s1 ∩ s2 , all elements in this section is from s1 
	OutputIterator set_intersection(
		InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2,
		OutputIterator result)
	{
		while (first1 != last1 && first2 != last2)
		{
			if (*first1 < *first2)
				++first1;
			else if (*first2 < *first1)
				++first2;
			else
			{
				*result = *first1;
				++first1;
				++first2;
				++result;
			}
		}
		return result;
	}

	template<class InputIterator1, class InputIterator2, class OutputIterator>
	OutputIterator set_union(
		InputIterator1 first1, InputIterator1 last1,                            //s1 U s2 . in s1,the number of 'a' is n; in s2, the number of 'a' is m . so 'a' in this section is max(n,m),  where there is n 'a', comes from s1 
		InputIterator2 first2, InputIterator2 last2,
		OutputIterator result)
	{
		while (first1 != last1 && first2 != last2)
		{
			if (*first1 < *first2)
			{
				*result = *first1;
				++first1;
			}
			else if (*first2 < *first1)
			{
				*result = *first2;
				++first2;
			}
			else
			{
				*result = *first1;
				++first1;
				++first2;
			}
			++result;
		}
		return _LXX::copy(first2, last2, _LXX::copy(first1, last1, result));
	}

	template<class InputIterator1, class InputIterator2, class OutputIterator>
	OutputIterator set_difference(
		InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2,
		OutputIterator result)
	{
		while (first1 != last1 && first2 != last2)
		{
			if (*first1 < *first2)
			{
				*result = *first1;
				++first1;
				++result;
			}
			else if (*first2 < *first1)
				++first2;
			else
			{
				++first1;
				++first2;
			}
		}
		return _LXX::copy(first1, last1, result);
	}

	template<class InputIterator1, class InputIterator2, class OutputIterator>
	OutputIterator set_symmetric_difference(
		InputIterator1 first1, InputIterator1 last1,
		InputIterator2 first2, InputIterator2 last2,
		OutputIterator result)
	{
		while (first1 != last1 && first2 != last2)
		{
			if (*first1 < *first2)
			{
				*result = *first1;
				++first1;
				++result;
			}
			else if (*first2 < *first1)
			{
				*result = *first2;
				++first2;
				++result;
			}
			else
			{
				++first1;
				++first2;
			}
		}
		return  _LXX::copy(first2, last2, copy(first1, last1, result));
	}


	//------------------------------- adjacent_find --------------------------------------------------------------
	template<class InputIterator>
	InputIterator adjacent_find(InputIterator first, InputIterator last)
	{
		if (first == last) return last;
		ForwardIterator next = first;
		while (++next != last)
		{
			if (*next = *first)
				return first;
			first = next;
		}
		return last;
	}

	template<class InputIterator, class BinaryPredicate>
	InputIterator adjacent_find(InputIterator first, InputIterator last, BinaryPredicate binary_pred)
	{
		if (first == last) return last;
		ForwardIterator next = first;
		while (++next != last)
		{
			if (binary_pred(*first, *next))
				return first;
			first = next;
		}
		return last;
	}

	//-------------------------------- count ----------------------------------------------------------------------
	template<class InputIterator, class T>
	typename _LXX::iterator_traits<InputIterator>::difference_type count(InputIterator first, InputIterator last, const T& value)
	{
		typename _LXX::iterator_traits<InputIterator>::difference_type  n = 0;
		for (; first != last; ++first)
		{
			if (*first == value)
				++n;
		}
		return n;
	}

	template<class InputIterator, class T, class Size>
	void count(InputIterator first, InputIterator last, const T& value, Size & n)
	{
		for (; first != last; ++first)
		{
			if (*first == value)
				++n;
		}
	}


	//----------------------------- count_if --------------------------------------------------------------------------
	template<class InputIterator, class Predicate>
	typename _LXX::iterator_traits<InputIterator>::diffenrence_type count_if(InputIterator first, InputIterator last, Predicate pred)
	{
		typename _LXX::iterator_traits<InputIterator>::difference_type n = 0;
		for (; first != last; ++first)
		{
			if (pred(*first))
				++n;
		}
		return n;
	}

	template<class InputIterator, class Predicate, class Size>
	void count_if(InputIterator first, InputIterator last, Predicate pred, Size & n)
	{
		for (; first != last; ++first)
		{
			if (pred(*first))
				++n;
		}
	}

	//------------------------------ find -------------------------------------------------------------------------------
	template<class InputIterator, class T>
	InputIterator find(InputIterator first, InputIterator last, const T& value)
	{
		while (first != last && *first != value) ++first;
		return first;
	}

	//------------------------------ find_if -------------------------------------------------------------------------------
	template<class InputIterator, class Predicate>
	InputIterator find_if(InputIterator first, InputIterator last, Predicate pred)
	{
		while (first != last && !pred(*first)) ++first;
		return first;
	}

	//----------------------------- search ----------------------------------------------------------------------------------
	template<class InputIterator1, class InputIterator2>
	InputIterator1 search(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2)
	{
		return __search(first1, last1, first2, last2, _LXX::distance_type(first1), _LXX::distance_type(first2));
	}

	template<class InputIterator1, class InputIterator2, class Distance1, class Distance2>
	InputIterator1 __search(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, Distance1*, Distance2*)
	{
		Distance1 d1 = _LXX::distance(first1, last1);
		Distance2 d2 = _LXX::distance(first2, last2);

		if (d1 < d2)
			return last1;

		InputIterator1 curr1 = first1;
		InputIterator2 curr2 = first2;

		while (curr2 != last2)
		{
			if (*curr1 = *curr2)
			{
				++curr1;
				++curr2;
			}
			else
			{
				if (d1 == d2)
					return last2;
				else
				{
					current1 = ++first1;
					current2 = first2;
					--d1;
				}
			}
		}
		return first1;
	}


	//----------------------------- search_n ------------------------------------------------------------------------------
	template<class InputIterator, class Integer, class T>
	InputIterator search_n(InputIterator first, InputIterator last, Integer count, const T& value)
	{
		if (count <= 0)
			return first;
		else
		{
			first = _LXX::find(first, last, value);
			while (first != last)
			{
				Integer n = count - 1;
				InputIterator i = first;
				++i;
				while (n > 0 && i != last && *i == value)
				{
					++i;
					--n;
				}
				if (n == 0)
					return first;
				else
					first = _LXX::find(i, last, value);
			}
			return last;
		}
	}

	template<class InputIterator, class Integer, class T, class BinaryPredicate>
	InputIterator search_n(InputIterator first, InputIterator last, Integer count, const T& value, BinaryPredicate binary_pred)
	{
		if (count <= 0)
			return first;
		else
		{
			while (first != last)
			{
				if (binary_pred(*first, value)) break;
				++first;
			}
			while (first != last)
			{
				Integer n = count - 1;
				InputIterator i = first;
				++i;
				while (n > 0 && i != last && binary_pred(*i, value))
				{
					--n;
					++i;
				}
				if (n == 0)
					return first;
				else
				{
					while (i != last)
					{
						if (binary_pred(*i, value))  break;
						++i;
					}
					first = i;
				}
			}
			return last;
		}
	}

	//------------------------------ find_end --------------------------------------------------------------------------------
	template<class InputIterator1, class InputIterator2>
	InputIterator1 find_end(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2)
	{
		typedef typename _LXX::iterator_traits<InputIterator1>::iterator_category category1;
		typedef typename _LXX::iterator_traits<InputIterator2>::iterator_category category2;
		return __find_end(first1, last1, first2, last2, category1(), category2());
	}

	template<class InputIterator1, class InputIterator2>
	InputIterator1 __find_end(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, forward_iterator_tag, forward_iterator_tag)
	{
		if (first2 == last2)
			return last1;
		else
		{
			InputIterator1 result = last1;
			while (1)
			{
				InputIterator1 new_result = _LXX::search(first1, last1, first2, last2);
				if (new_result == last1)
					return result;
				else
				{
					result = new_result;
					first1 = new_result;
					++first1;
				}
			}
		}
	}

	//------------------------------ find_first_of -----------------------------------------------------------------------------
	template<class InputIterator1, class InputIterator2>
	InputIterator1 find_first_of(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2)
	{
		for (; first1 != last1; ++first1)
		{
			for (InputIterator2 curr = first2; curr != last2; ++curr)
			{
				if (*first1 == *curr)
					return first1;
			}
		}
		return last1;
	}

	template<class InputIterator1, class InputIterator2, class BinaryCompare>
	InputIterator1 find_first_of(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, BinaryCompare comp)
	{
		for (; first1 != last1; ++first1)
		{
			for (InputIterator2 curr = first2; curr != last2; ++curr)
			{
				if (comp(*first1, *curr))
					return first1;
			}
		}
		return last1;
	}

	//----------------------------------for_each ----------------------------------------------------------------------------------
	template<class InputIterator, class Function>
	Function for_each(InputIterator first, InputIterator last, Function f)
	{
		for (; first != last; ++first)
			f(*first);
		return f;
	}

	//---------------------------------- generate-----------------------------------------------------------------------------------
	template<class ForwardIterator, class Generator>
	void generate(ForwardIterator first, ForwardIterator last, Generator generator)
	{
		for (; first != last; ++first)
			(*first) = generator()
	}

	template<class OutputIterator, class Size, class Generator>
	OutputIterator generator_n(OutputIterator first, OutputIterator last, Size n, Generator gen)
	{
		while (n--)
		{
			*first = gen();
			++first;
		}
		return first;
	}


	//------------------------------------ include (must be sorted sequence) --------------------------------------------------------
	template<class InputIterator1, class InputIterator2>
	bool includes(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2)
	{
		while (first2 != last2)
		{
			if (*first1 > *first2)
				return false;
			else if (*first1 < *first2)
				++first1;
			else
			{
				++first1;
				++first2;
			}
		}
		return true;
	}

	template<class InputIterator1, class InputIterator2, class Compare>
	bool includes(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, Compare comp)
	{
		while (first2 != last2)
		{
			if (comp(*first2, *first1))
				return false;
			else if (comp(*first1, *first2))
				++first1;
			else
				++first1, ++first2;
		}
		return true;
	}

	//---------------------------------- max_element --------------------------------------------------------------------------------
	template<class ForwardIterator>
	ForwardIterator max_element(ForwardIterator first, ForwardIterator last)
	{
		if (first == last) return first;
		ForwardIterator result = first;
		while (++first != last)
		{
			if (*result < *first) result = first;
		}
		return result;
	}

	template<class ForwardIterator, class Compare>
	ForwardIterator max_element(ForwardIterator first, ForwardIterator last, Compare comp)
	{
		if (first == last) return first;
		ForwardIterator result = first;
		while (++first != last)
		{
			if (comp(*result, *first)) result = first;
		}
		return result;
	}

	//---------------------------------- merge-----------------------------------------------------------------------------------------
	template<class InputIterator1, class InputIterator2, class OutputIterator>
	OutputIterator merge(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result)
	{
		while (first1 != last1 && first2 != last2)
		{
			if (*first2 < *fist1)
			{
				*result = *first2;
				++first2;
			}
			else
			{
				*result = *first1;
				++first1;
			}
			++result;
		}
		return _LXX::copy(first2, last2, _LXX::copy(first1, last1, result));
	}

	template<class InputIterator1, class InputIterator2, class OutputIterator, class Compare>
	OutputIterator merge(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result, Compare comp)
	{
		while (first1 != last1 && first2 != last2)
		{
			if (comp(*first2, *first1))
			{
				*result = *first2;
				++first2;
			}
			else
			{
				*result = *first1;
				++first1;
			}
			++result;
		}
		return _LXX::copy(first2, last2, _LXX::copy(first1, last1, result));
	}


	//--------------------------------- min_element -----------------------------------------------------------------------------------
	template<class ForwardIterator>
	ForwardIterator min_element(ForwardIterator first, ForwardIterator last)
	{
		if (first == last) return first;
		ForwardIterator result = first;
		while (++first != last)
		{
			if (*first < *result) result = first;
		}
		return result;
	}

	template<class ForwardIterator, class Compare>
	ForwardIterator min_element(ForwardIterator first, ForwardIterator last, Compare comp)
	{
		if (first == last) return first;
		ForwardIterator result = first;
		while (++first != last)
		{
			if (comp(*first, *result)) result = first;
		}
		return result;
	}

	//---------------------------------- partition ------------------------------------------------------------------------------------
	template<class BidirectionIterator, class Predicate>
	BidirectionIterator partition(BidirectionIterator first, BidirectionIterator last, Predicate pred)
	{
		while (true)
		{
			while (true)  //从左往右寻找不满足条件的位置
			{
				if (first == last) return first;
				else if (pred(*first)) ++first;
				else break;
			}

			--last; //从上一次\满足条件的位置的前一个位置开始

			while (true) //从右往左寻找满足条件的位置
			{
				if (first == last) return first;
				else if (!pred(*last)) --last;
				else break;
			}
			_LXX::iter_swap(first, last);  //交换找到的两个位置
			++first;  //从上一次满足条件的位置的下一个位置开始
		}
	}

	//--------------------------------- remove_copy-------------------------------------------------------------------------------------
	template<class InputIterator,class OutputIterator,class T>
	OutputIterator remove_copy(InputIterator first, InputIterator last, OutputIterator result, const T& value)
	{
		for (; first != last;++first)
		{
			if (*first != value)
			{
				*result = *first;
				++result;
			}
		}
		return result;
	}

	//--------------------------------- remove -----------------------------------------------------------------------------------------
	template<class ForwardIterator,class T>
	ForwardIterator remove(ForwardIterator first, ForwardIterator last, const T & value)
	{
		first = _LXX::find(first, last, value);
		ForwardIterator next = first;
		return first == last ? first : _LXX::remove_copy(++next, last, first, value);
		//return _LXX::remove_copy(first, last,first, value);  ////可仅用该句话替代上面的代码，效率与功能均相同
	}

}

#endif