#ifndef _STL_ALGO_X_H
#define _STL_ALGO_X_H

#include "stl_iterator_traits_x.h"

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
}

#endif