#ifndef _STL_ITERATOR_X_H
#define _STL_ITERATOR_X_H

#include "stl_iterator_traits_x.h"

namespace _LXX
{
	//-------------------------------------- back_insert_iterator ----------------------------------------------
	template<class Container>
	class back_insert_iterator
	{
	protected:
		Container * container;
		typedef output_iterator_tag iterator_category;
		typedef void	difference_type;
		typedef void	value_type;
		typedef void	pointer;
		typedef void	reference;

	public:
		back_insert_iterator(Container& x) :container(&x){}
		back_insert_iterator<Container> & operator=(const typename Container::value_type & value)
		{
			container->push_back(value);
			return *this;
		}

		back_insert_iterator<Container> & operator*(){ return *this; }
		back_insert_iterator<Container> & operator++(){ return *this; }
		back_insert_iterator<Container> & operator++(int){ return *this; }
	};

	template<class Container>
	back_insert_iterator<Container> back_inserter(Container & x)
	{
		return back_insert_iterator<Container>(x);
	}

	//---------------------------------------- front_insert_iterator -------------------------------------------------
	template<class Container>
	class front_insert_iterator
	{
	protected:
		Container * container;
		typedef output_iterator_tag	iterator_category;
		typedef void	value_type;
		typedef void	difference_type;
		typedef void	pointer;
		typedef void	reference;

	public:
		front_insert_iterator(Container & x) :container(&x){}
		front_insert_iterator<Container> & operator=(const typename Container::value_type & value)
		{
			container->push_front(value);
			return *this;
		}


		front_insert_iterator<Container>& operator*(){ return *this; }
		front_insert_iterator<Container>& operator++(){ return *this; }
		front_insert_iterator<Container>& operator++(int){ return *this; }
	};

	template<class Container>
	front_insert_iterator<Container> front_inserter(Container& x)
	{
		return front_insert_iterator<Container>(x);
	}

	//------------------------------------- insert_iterator -----------------------------------------------------------------
	template<class Container>
	class insert_iterator
	{
	protect:
		Container * container;
		typename Container::iterator iter;
		typedef output_iterator_tag iterator_category;
		typedef void reference;
		typedef void pointer;
		typedef void value_type;
		typedef void difference_type;

	public:
		insert_iterator(Container & x, typename Container::iterator i) :Container(&x), iter(i){}
		insert_iterator<Container>& operator=(const typename Container::value_type & value)
		{
			iter = container->insert(iter, value);
			++iter;
			return *this;
		}

		insert_iterator<Container>& operator*(){ return *this; }
		insert_iterator<Container>& operator++(){ return *this; }
		insert_iterator<Container>& operator++(int){ return *this; }
	};



}

#endif