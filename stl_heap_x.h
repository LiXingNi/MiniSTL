#ifndef _STL_HEAP_X_H
#define _STL_HEAP_X_H

#include "stl_iterator_traits_x.h"

namespace _LXX
{
	
	template<class Iter, class Diff, class Value>
	void _push_heap_aux(Iter first, Diff holeIndex, Diff topIndex, Value val)
	{
		Diff parent = (holeIndex - 1) / 2;
		while (holeIndex > topIndex && *(first + parent) < val)
		{
			*(first + holeIndex) = *(first + parent);
			holeIndex = parent;
			parent = (holeIndex - 1) / 2;
		}
		*(first + holeIndex) = val;
	}

	template<class Iter, class Diff, class Value>
	void _push_heap(Iter first, Iter last, Diff, Value*)
	{
		_push_heap_aux(first, Diff((last - first) - 1), Diff(0), Value(*(last - 1)));
	}

	template<class Iter>
	void push_heap(Iter first, Iter last)
	{
		_push_heap(first, last, Iter::difference_type(0), value_type(first));
	}

	template<class Iter, class Dis, class Val>
	void _adjust_heap(Iter first, Dis holeIndex, Dis len, Val value)
	{
		Dis topIndex = holeIndex;
		Dis secondChild = 2 * holeIndex + 2;
		while (secondChild < len)
		{
			if (*(first + secondChild - 1) > *(first + secondChild))
				--secondChild;
			*(first + holeIndex) = *(first + secondChild);
			holeIndex = secondChild;
			secondChild = holeIndex * 2 + 2;
		}
		if (secondChild == len)
		{
			*(first + holeIndex) = *(first + secondChild - 1);
			holeIndex = secondChild - 1;
		}

		_push_heap_aux(first, holeIndex, topIndex, value);
	}

	template<class Iter, class Dis, class Val>
	void _pop_heap_aux(Iter first, Dis holeIndex, Dis len, Val value)
	{
		*(first + len) = *first;
		_adjust_heap(first, holeIndex, len, value);
	}


	template<class Iter,class Dis,class Val>
	void _pop_heap(Iter first, Dis holeIndex, Val *)
	{
		_pop_heap_aux(first, Dis(0), holeIndex, Val(*(first + holeIndex)));
	}

	template<class Iter>
	void pop_heap(Iter first, Iter last)
	{
		_pop_heap(first, Iter::difference_Type(last - first - 1), value_type(first));
	}

	template<class RandomAccessIterator>
	void sort_heap(RandomAccessIterator first, RandomAccessIterator last)
	{
		while (last - first > 1)
			pop_heap(first, last--);
	}
}


#endif