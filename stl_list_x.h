#ifndef _STL_LIST_X_H
#define _STL_LIST_X_H

#include "stl_allocate_x.h"
#include "stl_iterator_traits_x.h"

namespace _LXX
{
	template< class T>
	struct __list_node
	{
		T data;
		__list_node * prev;
		__list_node * next;
	};

	template<class T, class Ref, class Ptr>
	class __list_iterator
	{
	public:
		typedef T							value_type;
		typedef Ref							reference;
		typedef Ptr							pointer;
		typedef bidirectional_iterator_tag	iterator_category;
		typedef ptrdiff_t					difference_type;

		typedef __list_iterator<T, T&, T*>	iterator;
		typedef __list_iterator<T, Ref, Ptr>	self;

		typedef __list_node<T>*				link_type;

		link_type node;

		__list_iterator() :node(0){}
		__list_iterator(link_type x) :node(x){}
		__list_iterator(const iterator& x) :node(x.node){}

		bool operator==(const self & x)
		{
			return x.node == node;
		}

		bool operator !=(const self&x)
		{
			return !(operator==(x));
		}

		self & operator++()
		{
			node = node->next;
			return *this;
		}

		self operator++(int)
		{
			self tmp = *this;
			this->operator++();
			return tmp;
		}

		self& operator--()
		{
			node = node->prev;
			return *this;
		}

		self operator--(int)
		{
			self tmp = *this;
			this->operator--();
			return tmp;
		}

		reference operator*()
		{
			return node->data;
		}

		pointer operator->()
		{
			return &(operator*());
		}
		
	};

	template < class T, class Alloc = _LXX::default_alloc >
	class list
	{
	public:
		typedef	T			   				value_type;
		typedef size_t						size_type;
		typedef __list_iterator<T, T&, T*>	iterator;
		typedef __list_iterator<T, const T&, const T*> const_iterator;
		typedef T&		reference;
		typedef T*		pointer;

	private:
		typedef simpleAlloc<__list_node<T>, Alloc>	dataAlloc;
		typedef __list_node<T>*		list_node;
		list_node node;

		list_node get_node(){return (list_node)dataAlloc::allocate();}
		void put_node(list_node ptr){ dataAlloc::deallocate(ptr); }
		list_node create_node(const T & val)
		{
			list_node tmp = get_node();
			construct(&tmp->data, val);
			return tmp;
		}
		void destroy_node(list_node ptr)
		{
			destroy(&ptr->data);
			put_node(ptr);
		}

		void set_null_node()
		{
			node = get_node();
			node->next = node;
			node->prev = node;
		}

	public:
		list(){ set_null_node(); }
		list(size_type n, const T & val)
		{
			set_null_node();
			while (n-- > 0)
			{
				insert(node, val);
			}
		}
		template<class Iter>
		list(Iter first, Iter last)
		{
			set_null_node();
			while (first != last)
			{
				insert(node, *first);
				++first;
			}
		}


		iterator begin()const{ return node->next; }
		iterator end()const{ return node; }
		iterator insert(iterator pos, const T & val)
		{
			list_node tmp = create_node(val);
			tmp->next = pos.node;
			tmp->prev = pos.node->prev;
			pos.node->prev->next = tmp;
			pos.node->prev = tmp;
			return tmp;
		}
		reference back()
		{
			return *(--end());
		}
		reference front()
		{
			return *(begin());
		}
		void push_back(const T& val){ insert(node, val); }
		void push_front(const T & val){ insert(begin(), val); }
		size_type size()const
		{
			size_type len(0);
			iterator it = begin();
			while (it != end())
			{
				++len;
				++it;
			}
			return len;
		}
		bool empty()const{ return node == node->next; }
		iterator erase(iterator pos)
		{
			iterator first = pos.node->prev;
			iterator last = pos.node->next;
			first.node->next = last.node;
			last.node->prev = first.node;
			destroy_node(pos.node);
			return last;
		}
		iterator erase(iterator first, iterator last)
		{
			iterator tmp;
			while (first != last)
			{
				tmp = first.node->next;
				erase(first);
				first = tmp;
			}
			return last;
		}
		void clear(){ erase(begin(), end()); node->next = node; node->prev = node; }
		void pop_front(){ erase(begin()); }
		void pop_back(){ erase(--end()); }
		void remove(const T&);
		void unique();
		void transfer(iterator, iterator, iterator);
		void reverse();
		void merge(list<T, Alloc> &);
		void splice(iterator, list<T, Alloc>&, iterator);
		void swap(list & x)
		{
			list_node tmp = node;
			node = x.node;
			x.node = tmp;
		}
		void sort();
	};

	template<class T,class Alloc>
	void list<T, Alloc>::remove(const T& val)
	{
		iterator it = begin();
		iterator tmp;
		while (it != end())
		{
			tmp = it;
			++it;
			if (*tmp == val)
				erase(tmp);
		}
	}

	template<class T, class Alloc>
	void list<T, Alloc>::unique()
	{
		iterator first = begin();
		iterator last = end();
		if (first == last) return;
		iterator next = first;
		while (++next != last)
		{
			if (*next == *first)
				erase(next);
			else
				first = next;
			next = first;
		}
	}

	template<class T, class Alloc>
	void list<T, Alloc>::transfer(iterator position, iterator first, iterator last)
	{
		position.node->prev->next = first.node;
		first.node->prev->next = last.node;
		last.node->prev->next = position.node;
		iterator tmp = position.node->prev;
		position.node->prev = last.node->prev;
		last.node->prev = first.node->prev;
		first.node->prev = tmp.node;
	}

	template<class T, class Alloc>
	void list<T, Alloc>::reverse()
	{
		if (node->next == node || node->next->next == node)
			return;
		iterator first = ++begin();
		while (first != end())
		{
			iterator tmp = first;
			transfer(begin(), tmp , ++first);
		}
	}

	template<class T, class Alloc>
	void list<T, Alloc>::merge(list<T, Alloc> & x)
	{
		iterator first = begin();
		iterator second = x.begin();
		while (first != end() && second != x.end())
		{
			if (*first <= *second)
				++first;
			else
			{
				iterator tmp = second;
				++second;
				transfer(first, tmp, second);
			}
		}
		if (second != x.end())
		{
			transfer(end(), second, x.end());
		}
	}

	template<class T, class Alloc>
	void list<T, Alloc>::splice(iterator position, list<T, Alloc>&, iterator i)
	{
		iterator j = i;
		++j;
		if (i == position || j == position) return;
		transfer(position, i, j);
	}

	template<class T, class Alloc>
	void list<T, Alloc>::sort()
	{
		if (node->next == node || node->next->next == node) return;
		list<T, Alloc> carray;
		list<T, Alloc> counter[64];
		size_type fill(0);
		while (!empty())
		{
			carray.splice(carray.begin(), *this, begin());
			size_type i(0);
			while (i < fill && !counter[i].empty())
			{
				counter[i].merge(carray);
				carray.swap(counter[i++]);
			}
			carray.swap(counter[i]);
			if (i == fill) ++fill;
		}

		for (size_type i(1); i < fill; ++i)
			counter[i].merge(counter[i - 1]);
		swap(counter[fill - 1]);
	}
}


#endif