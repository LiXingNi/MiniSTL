#ifndef _STL_ITERATOR_X_H
#define _STL_ITERATOR_X_H

#include <iostream>

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
	protected:
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

	template<class	Container, class Iterator>
	insert_iterator<Container> inserter(Container & x, Iterator i)
	{
		typedef typename Container::iterator iter;
		return insert_iterator<Container>(x, iter(i));
	}

	// ---------------------------------------- reverse_iterator --------------------------------------------------------
	template<class Iterator>
	class reverse_iterator
	{
	protected:
		Iterator current;

	public:
		typedef typename _LXX::iterator_traits<Iterator>::iterator_category	iterator_category;
		typedef typename _LXX::iterator_traits<Iterator>::value_type	value_type;
		typedef typename _LXX::iterator_traits<Iterator>::difference_type	difference_type;
		typedef typename _LXX::iterator_traits<Iterator>::pointer	pointer;
		typedef typename _LXX::iterator_traits<Iterator>::reference reference;

		typedef reverse_iterator<Iterator> self;
		typedef Iterator iterator_type;

	public:
		reverse_iterator(){}
		explicit reverse_iterator(iterator_type x) : current(x){}
		reverse_iterator(const self & x) :current(x.current){}

		iterator_type base(){ return current; }
		reference operator*()const
		{
			Iterator tmp = current;
			return *(--tmp);
		}

		pointer operator->()const
		{
			return &(operator*());
		}

		self& operator++()
		{
			--current;
			return *this;
		}

		self operator++(int)
		{
			self tmp = *this;
			--current;
			return tmp;
		}

		self& operator--()
		{
			++current;
			return *this;
		}

		self operator--(int)
		{
			self tmp = *this;
			++current;
			return tmp;
		}

		self operator+(difference_type n)const
		{
			return self(current - n);
		}

		self& operator+=(difference_type n) const
		{
			current -= n;
			return *this;
		}

		self operator-(difference_type n)const
		{
			return self(current + n);
		}

		self& operator-=(difference_type n)const
		{
			current += n;
			return *this;
		}

		reference operator[](difference_type n)const
		{
			return *(*this + n);
		}

		bool operator==(const self & x)
		{
			return current == x.current;
		}

		bool operator!=(const self&x)
		{
			return !(*this == x);
		}
	};

	//------------------------------------------------------ istream_iterator --------------------------------------------------------------

	template<class T, class Distance = ptrdiff_t>
	class istream_iterator
	{
		template<class Ts, class Distances>
		friend bool operator==(const istream_iterator<Ts, Distances>& x, const istream_iterator<Ts, Distances>& y);

	protected:
		T value;
		std::istream * stream;
		bool end_marker;

		void read()
		{
			end_marker = (*stream) ? true : false;
			if (end_marker) *stream >> value;
			end_marker = (*stream) ? true : false;
		}

	public:
		typedef input_iterator_tag iterator_category;
		typedef T	value_type;
		typedef Distance difference_type;
		typedef const T * pointer;
		typedef const T& reference;

		istream_iterator() :stream(cin), end_marker(false){}
		istream_iterator(std::istream & x) :stream(&x){ read(); }

		reference operator*()const { return value; }
		pointer operator->()const{ return &(operator*()); }

		istream_iterator<T, Distance>& operator++()
		{
			read();
			return *this;
		}

		istream_iterator<T, Distance> operator++(int)
		{
			istream_iterator<T, Distance> tmp = *this;
			read();
			return tmp;
		}

	};

	template<class T, class Distance>
	bool operator==(const istream_iterator<T, Distance> & x, const istream_iterator<T, Distance>& y)
	{
		return (x.stream == y.stream && x.end_marker == y.end_marker) || (x.end_marker == false && y.end_marker == false);
	}

	//-------------------------------------------- ostream_iterator--------------------------------------------------------------
	template<class T>
	class ostream_iterator
	{
	protected:
		std::ostream * stream;
		const char * string;

	public:
		typedef output_iterator_tag iterator_category;
		typedef void value_type;
		typedef void difference_type;
		typedef void reference;
		typedef void pointer;

		ostream_iterator(std::ostream & x) :stream(&x), string(0){}
		ostream_iterator(std::ostream & x, const char * c) : stream(&x), string(c){}

		ostream_iterator<T> & operator=(const T & value)
		{
			(*stream) << value;
			if (string) (*stream) << string;
			return *this;
		}

		ostream_iterator<T>& operator*(){ return *this; }
		ostream_iterator<T>& operator++(){ return *this; }
		ostream_iterator<T>& operator++(int){ return *this; }
	};


}

#endif