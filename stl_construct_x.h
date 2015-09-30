#ifndef _STL_CONSTRUCT_X_H
#define _STL_CONSTRUCT_X_H

#include <new>
#include "stl_type_traits_x.h"
#include "stl_iterator_traits_x.h"

namespace _LXX
{
	template<class T1, class T2>
	void construct(T1 * p, const T2 & val)
	{
		new(p)T1(val);
	}

	template<class T>
	void destroy(T* ptr)
	{
		ptr->~T();
	}

	template<class Iter>
	void _destroy_aux(Iter first, Iter last, __true_type){}

	template<class Iter>
	void _destroy_aux(Iter first, Iter last, __false_type)
	{
		while (first != last)
		{
			destroy(&*first);
			++first;
		}
	}

	template<class Iter, class T>
	void _destroy(Iter first, Iter last, T*)
	{
		typedef typename __type_traits<T>::has_trival_destructor has_destructor;
		_destroy_aux(first, last, has_destructor());
	}

	template<class Iter>
	void destroy(Iter first, Iter last)
	{
		_destroy(first, last, value_type(first));
	}

	template<class Iter, class Size>
	void _destroy_aux(Iter first, Size n, __true_type){}

	template<class Iter, class Size>
	void _destroy_aux(Iter first, Size n, __false_type)
	{
		while (n--)
		{
			destroy(&*first);
			--first;
		}
	}

	template<class Iter, class Size,class T>
	void _destroy(Iter first, Size n, T*)
	{
		typedef typename __type_traits<T>::has_trival_destructor has_trival;
		_destroy_aux(first, n, has_trival());
	}

	template<class Iter,class Size>
	void destroy(Iter first, Size n)
	{
		_destroy(first, n, value_type(first));
	}

	void destroy(char *, char*){}
	void destroy(wchar_t*, wchar_t*){}
}

#endif