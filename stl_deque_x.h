#ifndef _STL_DEQUE_X_H
#define _STL_DEQUE_X_H

#include "stl_allocate_x.h"
#include "stl_uninitialize_x.h"

namespace _LXX
{
	template< class T, class Ref, class Ptr, size_t BS = 0> 
	class __deque_iterator
	{
	public:
		typedef	T							value_type;
		typedef Ref							reference;
		typedef Ptr							pointer;
		typedef ptrdiff_t					difference_type;
		typedef random_access_iterator_tag	iterator_category;

		typedef __deque_iterator<T, T&, T*,BS>	iterator;
		typedef __deque_iterator<T, Ref, Ptr,BS>	self;

	private:
		typedef T**	map_pointer;

		static size_t buffer_size(){ return BS != 0 ? BS : (sizeof(T) < 512 ? 512 / sizeof(T) : size_t(1)); }

	public:

		T * first;
		T* last;
		T* curr;
		map_pointer node;


		void set_node(map_pointer new_node)
		{
			node = new_node;
			first = *new_node;
			last = first + difference_type(buffer_size());
		}


		__deque_iterator(const iterator & x) :first(x.first), last(x.last), curr(x.curr), node(x.node){}
		__deque_iterator() :first(0), last(0), curr(0), node(0){}

		difference_type operator-(const self & x)const
		{
			return difference_type((node - x.node)*buffer_size() + (curr - first) + (x.last - x.curr));
		}
		reference operator*()const{ return *curr; }
		pointer operator->()const{ return &(operator*()); }

		self& operator++()
		{
			++curr;
			if (curr == last)
			{
				set_node(node + 1);
				curr = first;
			}
			return *this;
		}

		self operator++(int)
		{
			self tmp = *this;
			++(*this);
			return tmp;
		}

		self & operator--()
		{
			if (curr == first)
			{
				set_node(node - 1);
				curr = last;
			}
			--curr;
			return *this;
		}

		self operator--(int)
		{
			self tmp = *this;
			--(*this);
			return tmp;
		}

		self& operator+=(difference_type n)
		{
			difference_type offset_bytes = n + curr - first;
			if (offset_bytes >= 0 && offset_bytes < difference_type(buffer_size()))
			{
				curr = first + offset_bytes;
			}
			else
			{
				difference_type offset_node = offset_bytes > 0 ? difference_type(offset_bytes / buffer_size()) : difference_type((offset_bytes + 1) / difference_type(buffer_size()) - 1);
				set_node(node + offset_node);
				curr = first + (offset_bytes - offset_node * difference_type(buffer_size()));
			}
			return *this;
		}

		self  operator+(difference_type n)
		{
			self tmp = *this;
			tmp += n;
			return tmp;
		}

		self& operator-=(difference_type n)
		{
			return *this += -n;
		}

		self operator-(difference_type n)
		{
			self tmp = *this;
			tmp -= n;
			return tmp;
		}

		reference operator[](difference_type n)
		{
			return *(*this + n);
		}

		bool operator==(const self& x)const
		{
			return curr == x.curr;
		}

		bool operator!=(const self & x)const
		{
			return !(*this == x);
		}

		bool operator<(const self & x)const
		{
			return (node == x.node) ? curr < x.curr : node < x.node;
		}
	};

	template<class T, class Alloc = _LXX::default_alloc, size_t BS = 0>
	class deque
	{
	public:
		typedef T			value_type;
		typedef size_t		size_type;
		typedef T*			pointer;
		typedef const T*	const_pointer;
		typedef T&			reference;
		typedef const T&	const_reference;
		typedef ptrdiff_t	difference_type;

		typedef __deque_iterator<T, T&, T*, BS>	iterator;
		typedef __deque_iterator<T, const T&, const T*, BS> const_iterator;

	private:
		typedef T**	map_pointer;
		typedef simpleAlloc<T, Alloc>	dataAlloc;
		typedef simpleAlloc<T*, Alloc>	nodeAlloc;

		map_pointer map;
		iterator	start;
		iterator	finish;
		size_type	map_size;

	public:
		deque() :start(), finish(), map(0), map_size(0){}
		deque(size_type n, const value_type & val){ fill_and_initialize(n, val); }
		template< class Iter>
		deque(Iter first, Iter last)
		{
			create_map_node(last - first);
			iterator tmp = start;
			while (tmp != finish)
			{
				construct(tmp.curr, *first);
				++first;
				++tmp;
			}
		}

		iterator begin(){ return start;}
		iterator end(){ return finish; }
		const_iterator begin()const{ return start; }
		const_iterator end()const{ return finish; }
		reference operator[](size_type n){ return begin()[n]; }
		bool empty()const{ return start == finish; }
		reference front()const{ return *start; }
		reference back()const
		{
			iterator tmp = finish;
			return *(--tmp);
		}
		size_type size()const{ return finish - start; }
		void push_back(const value_type & val);
		void push_front(const value_type & val);
		void pop_back();
		void pop_front();
		void clear();
		iterator erase(iterator pos);
		iterator erase(iterator first, iterator last);
		

	private:
		static size_type buffer_size(){ return BS != 0 ? BS : (sizeof(T) < 512 ? 512 / sizeof(T) : size_t(1)); }
		static size_type initial_mapsize(){ return 8; }

		void fill_and_initialize(size_type n, const value_type & val);
		T* allocate_node(){ return (T*)dataAlloc::allocate(buffer_size()); }
		void deallocate_node(T* ptr){ dataAlloc::deallocate(ptr, buffer_size()); }
		void create_map_node(size_type n);

		void push_back_aux(const value_type &);
		void reserve_map_back(size_type n = 1);
		void reallocate_map(size_type, bool);
		void push_front_aux(const value_type &);
		void reserve_map_front(size_type n = 1);

		void pop_back_aux();
		void pop_front_aux();
	};

	/*template<class T, class Alloc, size_t BS>
	typename deque<T, Alloc, BS>::iterator deque<T, Alloc, BS>::erase(iterator first, iterator last)
	{
		if (first == start && last == finish)
		{
			clear();
			return finish;
		}
		else
		{
			size_type n = last - first;
			size_type before_element = first - start;

			if (before_element < (size() - n) / 2)
			{
				std::copy_backward(start, first, last);
				iterator new_start = start + n;
				destroy(start, new_start);
				for (map_pointer tmp = start.node; tmp < new_start.node; ++tmp)
					deallocate_node(*tmp);
				start = new_start;
			}
			else
			{
				std::copy(last, finish, first);
				iterator new_finish = finish - n;
				destroy(new_finish, finish);
				for (map_pointer tmp = new_finish.node + 1; tmp <= finish.node; ++tmp)
				{
					deallocate_node(*tmp);
				}
				finish = new_finish;
			}
			return start + before_element;
		}
	}*/

	template<class T, class Alloc, size_t BS>
	typename deque<T, Alloc, BS>::iterator deque<T,Alloc,BS>::erase(iterator pos)
	{
		iterator next = pos;
		++next;
		difference_type index = pos - start;
		if (index < (size() >> 2))
		{
			std::copy_backward(start, pos, next);
			pop_front();
		}
		else
		{
			std::copy(pos + 1, finish, pos);
			pop_back();
		}
		return start + index;
	}

	template<class T, class Alloc, size_t BS>
	void deque<T, Alloc, BS>::clear()
	{
		map_pointer tmp;
		for (tmp = start.node + 1; tmp < finish.node; ++tmp)
		{
			destroy(*tmp,buffer_size());
			deallocate_node(*tmp);
		}

		if (start.node != finish.node)
		{
			destroy(start.curr, start.last);
			destroy(finish.first, finish.curr);
			deallocate_node(finish.first);
		}
		else
		{
			destroy(start.curr, finish.curr);
		}
		finish = start;
	}

	template<class T, class Alloc, size_t BS>
	void deque<T, Alloc, BS>::pop_front_aux()
	{
		destroy(start.curr);
		deallocate_node(start.first);
		start.set_node(start.node + 1);
		start.curr = start.first;
	}

	template<class T, class Alloc, size_t BS>
	void deque<T, Alloc, BS>::pop_front()
	{
		if (start.curr != start.last - 1)
		{
			destroy(start.curr);
			++start.curr;
		}
		else
			pop_front_aux();
	}

	template<class T, class Alloc, size_t BS>
	void deque<T, Alloc, BS>::pop_back_aux()
	{
		deallocate_node(finish.first);
		finish.set_node(finish.node - 1);
		finish.curr = finish.last - 1;
		destroy(finish.curr);
	}

	template<class T, class Alloc, size_t BS>
	void deque<T, Alloc, BS>::pop_back()
	{
		if (finish.curr != finish.first)
		{
			--finish.curr;
			destroy(finish.curr);
		}
		else
			pop_back_aux();
	}

	template<class T, class Alloc, size_t BS>
	void deque<T, Alloc, BS>::reserve_map_front(size_type need_node)
	{
		if (need_node > start.node - map)
			reallocate_map(need_node, true);
	}

	template<class T, class Alloc, size_t BS>
	void deque<T, Alloc, BS>::push_front_aux(const value_type & val)
	{
		reserve_map_front();
		*(start.node - 1) = allocate_node();
		start.set_node(start.node - 1);
		start.curr = start.last - 1;
		construct(start.curr, val);
	}

	template<class T, class Alloc, size_t BS>
	void deque<T, Alloc, BS>::push_front(const value_type & val)
	{
		if (start.curr != start.first)
		{
			--start.curr;
			construct(start.curr, val);
		}
		else push_front_aux(val);
	}


	template<class T, class Alloc, size_t BS>
	void deque<T, Alloc, BS>::reallocate_map(size_type need_num, bool is_front)
	{
		size_type old_nodes = finish.node - start.node + 1;
		size_type new_nodes = old_nodes + need_num;
		map_pointer new_start;
		if (map_size > 2 * new_nodes)
		{
			new_start = map + (map_size - new_nodes) / 2 + (is_front ? need_num : 0);
			if (new_start < start.node)
				std::copy(start.node, finish.node + 1, new_start);
			else
				std::copy_backward(start.node, finish.node + 1, new_start + old_nodes);
		}
		else
		{
			size_type new_map_size = map_size + std::max(old_nodes, need_num) + 2;
			map_pointer new_map = nodeAlloc::allocate(new_map_size);
			new_start = new_map + (map_size - new_nodes) / 2 + (is_front ? need_num : 0);
			std::copy(start.node, finish.node + 1, new_start);
			nodeAlloc::deallocate(map, map_size);
			map = new_map;
			map_size = new_map_size;                      
		}
		start.set_node(new_start);
		finish.set_node(new_start + old_nodes - 1);
	}

	template<class T, class Alloc, size_t BS>
	void deque<T, Alloc, BS>::reserve_map_back(size_type need_num)
	{
		if (need_num + 1 > map_size - (finish.node - map))
		{
			reallocate_map(need_num, false);
		}
	}

	template<class T, class Alloc, size_t BS>
	void deque<T, Alloc, BS>::push_back_aux(const value_type & val)
	{
		reserve_map_back();
		*(finish.node + 1) = allocate_node();
		construct(finish.curr, val);
		finish.set_node(finish.node + 1);
		finish.curr = finish.first;
	}

	template<class T, class Alloc, size_t BS>
	void deque<T, Alloc, BS>::push_back(const value_type & val)
	{
		if (finish.curr != finish.last - 1)
		{
			construct(finish.curr, val);
			++finish.curr;
		}
		else
			push_back_aux(val);
	}     

	template<class T, class Alloc, size_t BS>
	void deque<T, Alloc, BS>::fill_and_initialize(size_type n, const value_type & val)
	{
		create_map_node(n);
		map_pointer curr;
		for (curr = start.node; curr < finish.node; ++curr)
			uninitialized_fill_n(*curr, n, val);
		uninitialized_fill_n(finish.first, n % buffer_size(), val);
	}


	template<class T, class Alloc, size_t BS>
	void deque<T, Alloc, BS>::create_map_node(size_type n)
	{
		size_type node_num = n / buffer_size() + 1;
		map_size = std::max(initial_mapsize(), node_num + 2);
		map = nodeAlloc::allocate(map_size);

		map_pointer nstart = map + (map_size - node_num) / 2;
		map_pointer nfinish = nstart + node_num - 1;
		map_pointer ncurr;
		try
		{
			for (ncurr = nstart; ncurr <= nfinish; ++ncurr)
			{
				*ncurr = allocate_node();
			}
		}
		catch (...)
		{
			map_pointer ntmp = nstart;
			while (ntmp != ncurr)
				dataAlloc::deallocate(*ntmp, buffer_size());
			nodeAlloc::deallocate(nstart, map_size);
		}
		start.set_node(nstart);
		finish.set_node(nfinish);
		start.curr = start.first;
		finish.curr = finish.first + (n % buffer_size());
	}
}


#endif