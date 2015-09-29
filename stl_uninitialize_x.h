#ifndef _STL_UNINITIALIZE_X_H
#define _STL_UNINITIALIZE_X_H

#include "stl_construct_x.h"
#include "stl_type_traits_x.h"
#include "stl_iterator_traits_x.h"

#include<algorithm>

namespace _LXX
{
	template <typename Iter, typename Size, typename T>
	Iter _uninitialized_fill_n_aux(Iter beg, Size n, const T& value, __true_type)
	{
		return std::fill_n(beg, n, value);
	}

	template <typename Iter, typename Size, typename T>
	Iter _uninitialized_fill_n_aux(Iter beg, Size n, const T& value, __false_type)
	{
		while (n--)
		{
			construct(&*beg, value);
			++beg;
		}
		return beg;
	}

	template <typename Iter, typename Size, typename T, typename T1>
	Iter _uninitialized_fill_n(Iter beg, Size n, const T& value, T1*)
	{
		typedef typename __type_traits<T1>::is_POD_type	is_POD;
		return _uninitialized_fill_n_aux(beg, n, value, is_POD());
	}

	template<typename Iter, typename Size, typename T>
	Iter uninitialized_fill_n(Iter beg, Size n, const T& value)
	{
		return _uninitialized_fill_n(beg, n, value, value_type(beg));
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////

	template<typename InputIter, typename OutputIter>
	OutputIter _uninitialized_copy_aux(InputIter first, InputIter last, OutputIter dest, __true_type)
	{
		return std::copy(first, last, dest);
	}

	template<typename InputIter, typename OutputIter>
	OutputIter _uninitialized_copy_aux(InputIter first, InputIter last, OutputIter dest, __false_type)
	{
		OutputIter result = dest;
		while (first != last)
		{
			construct(result, *first);
			++first;
			++result;
		}
		return result;
	}


	template<typename InputIter, typename OutputIter, typename Tp>
	OutputIter _uninitialized_copy(InputIter first, InputIter last, OutputIter dest, Tp*)
	{
		typedef typename __type_traits<Tp>::is_POD_type is_POD;
		return _uninitialized_copy_aux(first, last, dest, is_POD());
	}

	template<typename InputIter,typename OutputIter>
	OutputIter uninitialized_copy(InputIter first, InputIter last, OutputIter dest)
	{
		return _uninitialized_copy(first, last, dest, value_type(first));
	}
}

#endif