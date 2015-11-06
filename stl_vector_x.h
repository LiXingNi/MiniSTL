#ifndef _STL_VECTOR_X_H
#define _STL_VECTOR_X_H
#include "stl_allocate_x.h"
#include "stl_iterator_x.h"
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
		typedef T&			reference;
		typedef const T&	const_reference;
		typedef _LXX::reverse_iterator<iterator> reverse_iterator;

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

		void deallocate()
		{
			if (start)
				dataAlloc::deallocate(start, end_of_storage - start);
		}

		iterator insert_aux(iterator pos, const T & val)
		{
			if (finish != end_of_storage)
			{
				construct(finish, *(finish - 1));
				++finish;
				T val_copy = val;
				std::copy_backward(pos, finish - 2, finish - 1);
				*pos = val_copy;
				return pos;
			}
			else
			{
				size_type old_size = size();
				size_type new_size = 0 == old_size ? 1 : 2 * old_size;
				
				iterator new_start = dataAlloc::allocate(new_size);
				iterator new_finish = new_start;
				iterator new_pos;
				try
				{
					new_finish = uninitialized_copy(start, pos, new_start);
					construct(new_finish, val);
					new_pos = new_finish;
					++new_finish;
					new_finish = uninitialized_copy(pos, finish, new_finish);
				}
				catch (...)
				{
					destroy(new_start, new_finish);
					dataAlloc::deallocate(new_start, new_size);
					throw std::runtime_error("insert_aux runtime error");
				}

				destroy(start, finish);
				deallocate();
				start = new_start;
				finish = new_finish;
				end_of_storage = start + new_size;
				return new_pos;
			}
		}

	public:
		/////////////////////////////////////////////////////////////////////////////
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

		////////////////////////////////////////////////////////////////////////////
		iterator begin()const{ return start; }
		iterator end()const{ return finish; }
		bool empty()const{ return start == finish; }
		iterator operator[](const size_type & n){return start + n;}
		iterator at(const size_type & n)const{ return operator[](n); }
		reference back()const{ return *(end - 1); }
		size_type capacity()const{ return end_of_storage - start; }
		size_type size()const{ return finish - start; }
		reference front()const{ return *start; }
		void push_back(const T & x)
		{
			if (finish != end_of_storage)
			{
				construct(finish, x);
				++finish;
			}
			else
				insert_aux(finish, x);
		}
		void push_front(const T & x){ insert_aux(begin(), x); }
		iterator erase(iterator position)
		{
			if (position + 1 != finish)
				std::copy(position + 1, finish, position);
			--finish;
			destroy(finish); 
			return position;
		}
		iterator erase(iterator first, iterator last)
		{
			iterator i = std::copy(last, finish, first);
			destroy(i, finish);
			finish -= distance(first, last);
			return first;
		}
		void clear(){ erase(start, finish); }
		iterator insert(iterator pos, const T& x){ return insert_aux(pos, x); }
		void insert(iterator pos, size_type n, const T& val);
		void resize(size_type n)
		{
			if (n < size())
				erase(start + n, finish); 
		}
		void resize(size_type n, const T & val)
		{
			if (n < size())
				erase(start + n, finish);
			else 
			{
				insert(end(), n - size(), val);
			}
		}
		reference pop_front(){ return *start; }
		reference pop_back(){ return *(finish - 1); }
		void reserve(size_type n)
		{
			if (n > capacity())
			{
				iterator nstart = dataAlloc::allocate(n);
				iterator nfinish = nstart;
				try
				{
					nfinish = uninitialized_copy(start, finish, nstart);
				}
				catch (...)
				{
					destroy(nstart, nfinish);
					dataAlloc::deallocate(nstart, nfinish - nstart);
					throw std::runtime_error("reserve error");
				}
				destroy(start, finish);
				deallocate();
				start = nstart;
				finish = nfinish;
				end_of_storage = start + n;
			}
		}

		void swap(vector<T, Alloc>& x)
		{
			std::swap(start, x.start);
			std::swap(finish, x.finish);
			std::swap(end_of_storage, x.end_of_storage);
		}

	};	

	template<class T,class Alloc>
	void vector<T, Alloc>::insert(iterator pos, size_type n, const T & val)
	{
		if (n == 0)
			return;
		if (n <= end_of_storage - finish)
		{
			difference_type after_element = finish - pos;
			if (after_element >= n)
			{
				iterator nfinish = uninitialized_copy(finish - n, finish, finish);
				std::copy_backward(pos, finish - n, finish);
				std::fill_n(pos, n, val);
				finish = nfinish;
			}
			else
			{
				uninitialized_fill_n(finish, n - after_element, val);
				iterator nfinish = finish + n - after_element;
				nfinish = uninitialized_copy(pos, finish, nfinish);
				std::fill(pos, finish, val);
				finish = nfinish;
			}
		}
		else
		{
			const size_type old_size = size();
			const size_type nsize = old_size + std::max(old_size, n);
			
			iterator nstart = dataAlloc::allocate(nsize);
			iterator nfinish = nstart;
			try
			{
				nfinish = uninitialized_copy(start, pos, nstart);
				nfinish = uninitialized_fill_n(nfinish, n, val);
				nfinish = uninitialized_copy(pos, finish, nfinish);
			}
			catch (...)
			{
				destroy(nstart, nfinish);
				dataAlloc::deallocate(nstart, distance(nstart,nfinish));
				throw std::runtime_error("insert error");
			}

			destroy(start, finish);
			deallocate();
			start = nstart;
			finish = nfinish;
			end_of_storage = start + nsize;
		}
	}
}

#endif