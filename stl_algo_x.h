#ifndef _STL_ALGO_X_H
#define _STL_ALGO_X_H

#include "stl_iterator_traits_x.h"
#include "stl_algobase_x.h"
#include "stl_heap_x.h"

namespace _LXX
{
	//------------------------------------ sort ---------------------------------------------------------------------------------------------------
#define __stl_threshold 16

	template<class RandomAccessIterator,class T>
	void __unguarded_insertion_sort_aux(RandomAccessIterator first, RandomAccessIterator last, T*)
	{
		for (RandomAccessIterator i = first; i != last; ++i)
			_LXX::__unguarded_liner_insert(i, T(*i));
	}

	template<class RandomAccessIterator>
	void __unguarded_insertion_sort(RandomAccessIterator first, RandomAccessIterator last)
	{
		__unguarded_insertion_sort_aux(first, last, _LXX::value_type(first));
	}

	template<class RandomAccessIterator>
	void __final_insertion_sort(RandomAccessIterator first, RandomAccessIterator last)
	{
		if (last - first > __stl_threshold)
		{
			_LXX::__insert_sort(first, first + __stl_threshold);
			_LXX::__unguarded_insertion_sort(first + __stl_threshold, last);
		}
		else
			_LXX::__insert_sort(first, last);
	}


	template<class RandomAccessIterator,class T,class Size>
	void __introsort_loop(RandomAccessIterator first, RandomAccessIterator last, T*, Size limit_depth)
	{
		while (last - first > __stl_threshold)
		{
			if (limit_depth == 0)
			{
				_LXX::partial_sort(first, last,last);
					return;
			}
			--limit_depth;

			RandomAccessIterator curr = __unguarded_partition(first, last, T(_LXX::__median(*first, *(last - 1), *(first + (last - first) / 2))));
			__introsort_loop(curr, last,_LXX::value_type(first), limit_depth);
			last = curr;
		}
	}

	template<class RandomAccessIterator>
	void sort(RandomAccessIterator first, RandomAccessIterator last)
	{
		if (first != last)
		{
			__introsort_loop(first, last, _LXX::value_type(first), __lg(last - first) * 2);
			__final_insertion_sort(first, last);
		}
	}

	template<class Size>
	inline Size __lg(Size n)
	{
		Size k;
		for (k = 0; n > 1; ++k, n >>= 1);
		return k;
	}

	//----------------------------------- partition ----------------------------------------------------------------------------------------------
	template<class RandomAccessIterator,class T>
	RandomAccessIterator __unguarded_partition(RandomAccessIterator first, RandomAccessIterator last, const T& pivot)
	{
		while (true)
		{
			while (*first < pivot) ++first;
			--last;
			while (pivot < *last) --last;
			
			if (!(first < last)) return first;
			_LXX::iter_swap(first, last);
			
			++first;
		}
	}

	//---------------------------------- median --------------------------------------------------------------------------------------------------
	template<class T>
	inline const T& __median(const T& a, const T& b, const T&c)
	{
		if (a < b)
		{
			if (c < a)
				return a;
			else if (b < c)
				return b;
			else
				return c;
		}
		else if (a < c)
			return a;
		else if (c < b)
			return b;
		else
			return c;
	}
	
	template<class T,class Compare>
	inline const T& __median(const T& a, const T& b, const T& c, Compare comp)
	{
		if (comp(a,b))
		{
			if (comp(c,a))
				return a;
			else if (comp(b,c))
				return b;
			else
				return c;
		}
		else if (comp(a,c))
			return a;
		else if (comp(c,b))
			return b;
		else
			return c;
	}

	//---------------------------------- insert_sort ----------------------------------------------------------------------------------------------
	template<class RandomAccessIterator>
	void __insert_sort(RandomAccessIterator first, RandomAccessIterator last)
	{
		if (first == last) return;
		for (RandomAccessIterator i = first + 1; i != last; ++i)
			__liner_insert(first, i, _LXX::value_type(first));
	}

	template<class RandomAccessIterator,class T>
	void __liner_insert(RandomAccessIterator first, RandomAccessIterator last, T*)
	{
		T value = *last;
		if (value < *first)
		{
			_LXX::copy_backward(first, last, last + 1);
			*first = value;
		}
		else
		{
			__unguarded_liner_insert(last, value);
		}
	}

	template<class RandomAccessIterator,class T>
	void __unguarded_liner_insert(RandomAccessIterator last, const T& value)
	{
		RandomAccessIterator next = last;
		--next;
		while (value < *next)
		{
			*last = *next;
			last = next;
			--next;
		}
		*last = value;
	}

	//---------------------------------- partial_sort ------------------------------------------------------------------------------------------------
	template<class RandomAccessIterator>
	void partial_sort(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last)
	{
		__partial_sort(first, middle, last, _LXX::value_type(first));
	}

	template<class RandomAccessIterator,class T>
	void __partial_sort(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last, T*)
	{
		_LXX::make_heap(first, middle);
		for (RandomAccessIterator i = middle; i != last; ++i)
		{
			if (*i < *first)
			{
				_LXX::_pop_heap(first, middle, i, T(*i), _LXX::distance_type(first));
			}
		}
		_LXX::sort_heap(first, middle);
	}

	//----------------------------------- random_shuffle------------------------------------------------------------------------------------------------
	template<class RandomAccessIterator>
	inline void random_shuffle(RandomAccessIterator first, RandomAccessIterator last)
	{
		__random_shuffle(first, last, _LXX::distance_type(first));
	}

	template<class RandomAccessIterator, class Distance>
	inline void __random_shuffle(RandomAccessIterator first, RandomAccessIterator last, Distance*)
	{
		if (first == last) return;
		for (RandomAccessIterator i = first + 1; i != last; ++i)
		{
			_LXX::iter_swap(i, first + Distance(std::rand() % ((i - first) + 1)));
		}
	}

	template<class RandomAccessIterator, class RandomNumberGenerator>
	inline void random_shuffle(RandomAccessIterator first, RandomAccessIterator last, RandomNumberGenerator & rand)
	{
		if (first == last) return;
		for (RandomAccessIterator i = first + 1; i != last; ++i)
		{
			_LXX::iter_swap(i, first + rand(i - first + 1));
		}
	}

	//----------------------------------- prev_permutation ------------------------------------------------------------------------------------------
	template<class BidirectionalIterator>
	bool prev_premutation(BidirectionalIterator first, BidirectionalIterator last)
	{
		if (first == last) return false;
		BidirectionalIterator i = first;
		++i;
		if (i == last) return false;
		i = last;
		--i;

		for (;;)
		{
			BidirectionalIterator ii = i;
			--i;
			if (*ii < *i)
			{
				BidirectionalIterator j = last;
				while (!(*(--j) < *i));
				_LXX::iter_swap(i, j);
				_LXX::reverse(ii, last);
				return true;
			}
			if (i == first)
			{
				_LXX::reverse(first, last);
				return false;
			}
		}
	}
	
	//------------------------------------ next_permutation --------------------------------------------------------------------------------------------
	template<class BidirectionalIterator>
	bool next_permutation(BidirectionalIterator first, BidirectionalIterator last)
	{
		if (first == last) return false;
		BidirectionalIterator i = first;
		++i;
		if (i == last) return false; //仅有一个元素
		i = last;
		--i;

		for (;;)
		{
			BidirectionalIterator ii = i;
			--i;
			if (*i < *ii)
			{
				BidirectionalIterator j = last;
				while (!(*i < *(--j)));
				_LXX::iter_swap(i, j);
				_LXX::reverse(ii, last);
				return true;
			}
			if (i == first)
			{
				_LXX::reverse(first, last);
				return false;
			}
		}
	}

	//------------------------------------ binary_search ---------------------------------------------------------------------------------------------
	template<class ForwardIterator,class T>
	bool binary_search(ForwardIterator first, ForwardIterator last, const T& value)
	{
		ForwardIterator i = _LXX::lower_bound(first, last, value);
		return i != last && !(value < *i);
	}

	//------------------------------------ upper_bound -----------------------------------------------------------------------------------------------

	template<class RandomAccessIterator, class T, class Distance>
	RandomAccessIterator __upper_bound(RandomAccessIterator first, RandomAccessIterator last, const T &value, Distance *,random_access_iterator_tag)
	{
		Distance len = _LXX::distance(first, last);
		RandomAccessIterator middle;
		Distance half = 0;

		while (len > 0)
		{
			half = len >> 1;
			middle = first + half;

			if (value < *middle)
			{
				len = half;
			}
			else
			{
				first = middle + 1;
				len = len - half - 1;
			}
		}
		return first;
	}

	template<class ForwardIterator,class T,class Distance>
	ForwardIterator __upper_bound(ForwardIterator first, ForwardIterator last, const T &value, Distance *, forward_iterator_tag)
	{
		Distance len = _LXX::distance(first, last);
		ForwardIterator middle;
		Distance half = 0;

		while (len > 0)
		{
			half = len >> 1;
			middle = first;
			_LXX::advance(middle, half);
			
			if (value < *middle)
			{
				len = half;
			}
			else
			{
				first = ++middle;
				len = len - half - 1;
			}
		}
		return first;
	}

	template<class ForwardIterator,class T>
	ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last, const T& value)
	{
		return __upper_bound(first, last, value, _LXX::distance_type(first), _LXX::iterator_category(first));
	}

	//------------------------------------- lower_bound -----------------------------------------------------------------------------------------------

	template<class RandomAccessIterator, class T, class Distance>
	RandomAccessIterator __lower_bound(RandomAccessIterator first, RandomAccessIterator last, const T & value, Distance *, random_access_iterator_tag)
	{
		Distance len = last - first;
		Distance half;
		RandomAccessIterator middle;

		while (len > 0)
		{
			half = len >> 1;
			middle = first + half;
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

	template<class ForwardIterator,class T,class Distance>
	ForwardIterator __lower_bound(ForwardIterator first, ForwardIterator last, const T& value, Distance*, forward_iterator_tag)
	{
		Distance len = _LXX::distance(first, last);
		Distance half;
		ForwardIterator middle;

		while (len > 0)
		{
			half = len >> 1;
			middle = first;
			_LXX::advance(middle, half);
			if (*middle < value)  //*middle < value, 所找的值一定在右半部分
			{
				first = ++middle;
				len = len - half - 1;
			}
			else //*middle >= value, 所找的值在左半部分或者在 middle 处
				len = half;  //此处若是写为 len = half + 1, 则当 value == middle, len = 0 时将进入死循环
		}
		return first;
	}

	template<class RandomAccessIterator, class T,class Compare, class Distance>
	RandomAccessIterator __lower_bound(RandomAccessIterator first, RandomAccessIterator last, const T & value, Compare comp,Distance *, random_access_iterator_tag)
	{
		Distance len = last - first;
		Distance half;
		RandomAccessIterator middle;

		while (len > 0)
		{
			half = len >> 1;
			middle = first + half;
			if (comp(*middle,value))
			{
				first = middle + 1;
				len = len - half - 1;
			}
			else
				len = half;
		}
		return first;
	}

	template<class ForwardIterator, class T, class Compare,class Distance>
	ForwardIterator __lower_bound(ForwardIterator first, ForwardIterator last, const T& value, Compare comp,Distance*, forward_iterator_tag)
	{
		Distance len = _LXX::distance(first, last);
		Distance half;
		ForwardIterator middle;

		while (len > 0)
		{
			half = len >> 1;
			middle = first;
			_LXX::advance(middle, half);
			if (comp(*middle,value))  //*middle < value, 所找的值一定在右半部分
			{
				first = ++middle;
				len = len - half - 1;
			}
			else //*middle >= value, 所找的值在左半部分或者在 middle 处
				len = half;  //此处若是写为 len = half + 1, 则当 value == middle, len = 0 时将进入死循环
		}
		return first;
	}

	template<class ForwardIterator, class T>
	inline ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last, const T & value)
	{
		return __lower_bound(first, last, value, distance_type(first), iterator_category(first));
	}

	template<class ForwardIterator,class T,class Comp>
	inline ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last, const T& value, Comp comp)
	{
		return __lower_bound(first, last, value, comp, _LXX::distance_type(first), _LXX::iterator_category(first));
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
	template<class InputIterator, class OutputIterator, class T>
	OutputIterator remove_copy(InputIterator first, InputIterator last, OutputIterator result, const T& value)
	{
		for (; first != last; ++first)
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
	template<class ForwardIterator, class T>
	ForwardIterator remove(ForwardIterator first, ForwardIterator last, const T & value)
	{
		first = _LXX::find(first, last, value);
		ForwardIterator next = first;
		return first == last ? first : _LXX::remove_copy(++next, last, first, value);
		//return _LXX::remove_copy(first, last,first, value);  ////可仅用该句话替代上面的代码，效率与功能均相同
	}

	//---------------------------------remove_if ----------------------------------------------------------------------------------------
	template<class ForwardIterator, class Predicate>
	ForwardIterator remove_if(ForwardIterator first, ForwardIterator last, Predicate pred)
	{
		first = _LXX::find_if(first, last, pred);
		ForwardIterator next = first;
		return _LXX::remove_copy_if(++next, last, first, pred);
	}

	//-------------------------------- remove_copy_if -------------------------------------------------------------------------------------
	template<class InputIterator, class OutputIterator, class Predicate>
	OutputIterator remove_copy_if(InputIterator first, InputIterator last, OutputIterator result, Predicate pred)
	{
		for (; first != last; ++first)
		{
			if (!pred(*first))
			{
				*result = *first;
				++result;
			}
		}
		return result;
	}

	//--------------------------------- replace -----------------------------------------------------------------------------------------
	template<class ForwardIterator, class T>
	void replace(ForwardIterator first, ForwardIterator last, const T& old_value, const T& new_value)
	{
		for (; first != last; ++first)
		{
			if (*first == old_value) *first = new_value;
		}
	}

	template<class InputIterator, class OutputIterator, class T>
	OutputIterator replace_copy(InputIterator first, InputIterator last, OutputIterator result, const T& old_value, const T& new_value)
	{
		for (; first != last; ++first, ++result)
		{
			*result = *first == old_value ? new_value : *first;
		}
		return value;
	}

	//---------------------------------- reverse ------------------------------------------------------------------------------------------
	template<class BidirectionIterator>
	void reverse(BidirectionIterator first, BidirectionIterator last)
	{
		_LXX::__reverse(first, last, _LXX::iterator_category(first));
	}

	template<class BidirectionIterator>
	void __reverse(BidirectionIterator first, BidirectionIterator last, bidirectional_iterator_tag)
	{
		while (true)
		{
			if (first == last || first == --last)
				return;
			else
				_LXX::iter_swap(first++, last);
		}
	}

	template<class RandomAccessIterator>
	void __reverse(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag)
	{
		while (first < last) iter_swap(first++, --last);
	}

	//----------------------------------- reverse_copy -----------------------------------------------------------------------------------
	template<class BidirectionIterator, class OutputIterator>
	OutputIterator reverse_copy(BidirectionIterator first, BidirectionIterator last, OutputIterator result)
	{
		while (last != first)
		{
			--last;
			*result = *last;
			++result;
		}
		return result;
	}

	//--------------------------------- rotate -----------------------------------------------------------------------------------------------
	template<class ForwardIterator>
	void rotate(ForwardIterator first, ForwardIterator middle, ForwardIterator last)
	{
		if (first == middle || middle == last) return;
		__rotate(first, middle, last, _LXX::iterator_category(first),_LXX::distance_type(first));
	}


	template<class ForwardIterator,class Distance>
	void __rotate(ForwardIterator first, ForwardIterator middle, ForwardIterator last, forward_iterator_tag,Distance *)
	{
		for (ForwardIterator i = middle;;)
		{
			_LXX::iter_swap(first,i);
			++first;
			++i;

			if (first == middle)
			{
				if (i == last) return;
				else middle = i;
			}
			else
			{
				if (i == last)
					i = middle;
			}

		}
	}


	template<class BidirectionIterator,class Distance>
	void __rotate(BidirectionIterator first, BidirectionIterator middle, BidirectionIterator last, bidirectional_iterator_tag, Distance *)
	{
		_LXX::reverse(first, middle);
		_LXX::reverse(middle, last);
		_LXX::reverse(first, last);
	}

	template<class RandomAcccessIterator,class Distance>
	void __rotate(RandomAcccessIterator first, RandomAcccessIterator middle, RandomAcccessIterator last, random_access_iterator_tag, Distance *)
	{
		Distance n = _LXX::__gcd(last - first, middle - first);
		while (n--)
		{
			_LXX::__rotate_cycle(first, last, first + n, middle - first, _LXX::value_type(first));
		}
	}

	template<class EuclideanRingElement>
	EuclideanRingElement __gcd(EuclideanRingElement m, EuclideanRingElement n)
	{
		while (n != 0)
		{
			EuclideanRingElement t = m % n;
			m = n;
			n = t;
		}
		return m;
	}

	template<class RandomAccessIterator,class Distance,class T>
	void __rotate_cycle(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator initial, Distance shift, T*)
	{
		T value = *initial;
		RandomAccessIterator ptr1 = initial;
		RandomAccessIterator ptr2 = initial + shift;
		while (ptr2 != initial)
		{

			*ptr1 = *ptr2;
			ptr1 = ptr2;
			if (last - ptr2 > shift)
				ptr2 += shift;
			else
				ptr2 = first + (shift - (last - ptr2));
		}
		*ptr1 = value;
	}

}

#endif