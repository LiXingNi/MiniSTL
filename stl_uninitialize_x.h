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

	template <typename Iter,typename Size,typename T,typename T1>
	Iter _uninitialized_fill_n(Iter beg, Size n, const T& value, T1*)
	{
		typedef typename __type_traits<T1>::is_POD_type	is_POD;
		return _uninitialized_fill_n_aux(beg, n, value,is_POD());
	}

	template<typename Iter,typename Size,typename T>
	Iter uninitialized_fill_n(Iter beg, Size n, const T& value)
	{
		return _uninitialized_fill_n(beg, n, value,value_type(beg));
	}
}

#endif