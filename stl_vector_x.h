#ifndef _STL_VECTOR_X_H
#define _STL_VECTOR_X_H
#include "stl_allocate_x.h"
#include "stl_uninitialize_x.h"

namespace _LXX
{
	template < typename T, typename Alloc = _LXX::default_alloc >
	class vector
	{
	public:
		typedef T			value_type;
		typedef size_t		size_type;
		typedef ptrdiff_t	difference_type;
		typedef T*			iterator;
		typedef const T*	const_iterator;

	private:
		T* start;
		T* finish;
		T* end_of_storage;
		typedef simpleAlloc<T, Alloc>	dataAlloc;


		void fill_initialize(size_t n, const T& value)
		{
			start = allocate_and_fill(n, value);
			finish = start + n;
			end_of_storage = finish;
		}

		iterator allocate_and_fill(size_t n, const T& value)
		{
			iterator result = dataAlloc::allocate(n);
			uninitialized_fill_n(result, n, value);
			return result;
		}

	public:
		vector() :start(0), finish(0), end_of_storage(0){}
		vector(size_t n, const T & value){ fill_initialize(n, value); }
		explicit vector(size_t n){ fill_initialize(n, T()); }
		
		template <class Iter>
		vector(Iter first, Iter last)
		{
			typename iterator_traits<Iter>::difference_type n = _LXX::distance(first, last);
			start = dataAlloc::allocate(n);
			finish = start;
			while (first != last)
			{
				construct(finish, *first);
				++first;
				++finish;
			}
			end_of_storage = finish;
		}

		iterator begin(){ return start; }
		iterator end(){ return finish; }
		iterator operator[](const size_type & n){return start + n;}
		iterator at(const size_type & n){ return operator[](n); }

	};	
}

#endif