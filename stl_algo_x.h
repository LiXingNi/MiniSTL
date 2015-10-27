#ifndef _STL_ALGO_X_H
#define _STL_ALGO_X_H

#include "stl_iterator_traits_x.h"
#include "stl_algobase_x.h"

namespace _LXX
{

	template<class RandomAccessIterator,class T,class Distance>
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

	template<class ForwardIterator,class T>
	inline ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last, const T & value)
	{
		return __lower_bound(first, last, value, distance_type(first), iterator_category(first));
	}

	//------------------------------------ some algorithm about set-------------------------------------------
	template<class InputIterator1,class InputIterator2,class OutputIterator>                    //s1 กษ s2 , all elements in this section is from s1 
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
		return std::copy(first2, last2, std::copy(first1, last1, result));
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
		return std::copy(first1,last1,result);
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
		return std::copy(first2, last2, copy(first1, last1, result));
	}

}

#endif