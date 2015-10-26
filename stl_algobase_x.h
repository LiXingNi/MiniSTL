#ifndef _STL_ALGOBASE_X_H
#define _STL_ALGOBASE_X_H

//#include <iostream>
//#include <string>
//using std::string;
//using std::cout;
//using std::endl;
#include "stl_iterator_traits_x.h"
#include "stl_pair_x.h"
#include "stl_type_traits_x.h"

namespace _LXX
{

	//------------------------- equal ---------------------------------------------------
	template<class InputIterator1, class InputIterator2>
	bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2)
	{
		for (; first1 != last1; ++first1, ++first2)
		{
			if (*first1 != *first2)
				return false;
		}
		return true;
	}

	template<class InputIterator1, class InputIterator2,class BinaryOperation>
	bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2,BinaryOperation op)
	{
		for (; first1 != last1; ++first1, ++first2)
		{
			if (!op(*first1,*first2))
				return false;
		}
		return true;
	}


	//----------------------- fill -------------------------------------------------
	template<class InputIterator,class T>
	void fill(InputIterator first, InputIterator last, const T & value)
	{
		while (first != last)
		{
			*first = value;
			++first;
		}
	}

	//--------------------- fill_n -------------------------------------------------
	template<class InputIterator,class size,class T>
	InputIterator fill_n(InputIterator first, size n, const T & value)
	{
		while (n--)
		{
			*first = value;
			++first;
		}
		return first;
	}

	//------------------- iter_swap -------------------------------------------------
	template<class InputIterator1,class InputIterator2>
	void iter_swap(InputIterator1 a, InputIterator2 b)
	{
		_iter_swap(a, b, value_type(a));
	}

	template<class InputIterator1, class InputIterator2,class T>
	void _iter_swap(InputIterator1 a, InputIterator2 b, T*)
	{
		T tmp = *a;
		*a = *b;
		*b = tmp;
	}

	//------------------- max ------------------------------------------------------------
	template<class T>
	T max(const T & a, const T & b)
	{
		return a < b ? b : a;
	}

	template<class T,class Compare>
	T max(const T & a, const T & b, Compare comp)
	{
		return comp(a, b) ? b : a;
	}

	//----------------- min --------------------------------------------------------------
	template<class T>
	T min(const T & a, const T & b)
	{
		return b < a ? b : a;
	}

	template<class T, class Compare>
	T min(const T & a, const T & b, Compare comp)
	{
		return comp(b, a) ? b : a;
	}

	//--------------- swap ----------------------------------------------------------------
	template<class T>
	inline void swap(T & a, T & b)
	{
		T tmp = a;
		a = b;
		b = tmp;
	}

	//------------- mismatch --------------------------------------------------------------
	template<class InputIterator1,class InputIterator2>
	pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2)
	{
		while (first1 != last1 && *first1 == *first2)
		{
			++first1;
			++first2;
		}
		return _LXX::make_pair(first1, first2);
	}


	template<class InputIterator1, class InputIterator2,class BinaryOperation>
	pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2,BinaryOperation op)
	{
		while (first1 != last1 && op(*first1,*first2))
		{
			++first1;
			++first2;
		}
		return _LXX::make_pair(first1, first2);
	}


	//--------- lexicographical_compare ------------------------------------------------------
	template<class InputIterator1,class InputIterator2>
	bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2)
	{
		for (; first1 != last1 && first2 != last2; ++first1, ++first2)
		{
			if (*first1 < *first2)
				return true;
			if (*first1 > *first2)
				return false;
		}

		return first1 == last1 && first2 != last2;
	}

	template<class InputIterator1, class InputIterator2,class BinaryOperation>
	bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2,BinaryOperation op)
	{
		for (; first1 != last1 && first2 != last2; ++first1, ++first2)
		{
			if (op(*first1,*first2))
				return true;
			if (op(*first2, *first1))
				return false;
		}

		return first1 == last1 && first2 != last2;
	}

	inline bool lexicographical_compare(const char * first1, const char * last1, const char * first2, const char * last2)
	{
		const size_t len1 = last1 - first1;
		const size_t len2 = last2 - first2;

		const int result = memcmp(first1, first2, _LXX::min(len1, len2));

		return result != 0 ? result < 0 : len1 < len2;
	}


	//------------ copy-------------------------------------------------------------------------

	//void p(const string& str)
	//{
	//	cout << str << endl;
	//}

	template<class InputIterator,class OutputIterator>
	OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result)
	{
		//p("copy");
		return __copy_dispatch<InputIterator, OutputIterator>()(first, last, result);
	}

	template<>
	char * copy(const char * first, const char* last, char* result)
	{
		//p("copy(char*)");
		std::memmove(result, first, (last - first));
		return result + (last - first);
	}

	template<>
	wchar_t* copy(const wchar_t* first, const wchar_t * last, wchar_t* result)
	{
		//p("copy(wchar_t*)");
		std::memmove(result, first, sizeof(wchar_t)*(last - first));
		return result + (last - first);
	}

	template<class InputIterator,class OutputIterator>
	struct __copy_dispatch
	{
		OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result)
		{
			//p("__copy_dispatch");
			return __copy(first, last, result, iterator_category(first));
		}
	};

	template<class T>
	struct __copy_dispatch<const T*,T*>
	{
		T* operator()(const T* first, const T* last, T* result)
		{
			//p("__copy_dispatch(const T*)");
			return __copy_t(first, last, result, __type_traits<T>::has_trival_assignment_operator());
		}
	};


	template<class T>
	struct __copy_dispatch < T*, T* >
	{
		T* operator()(T* first, T* last, T* result)
		{
			//p("__copy_dispatch(T*)");
			return __copy_t(first, last, result, __type_traits<T>::has_trival_assignment_operator());
		}
	};

	template<class T>
	T* __copy_t(const T* first, const T* last, T* result, __true_type)
	{
		//p("__copy_t(true_type)");
		std::memmove(result, first, sizeof(T)*(last - first));
		return result + (last - first);
	}

	template<class T>
	T* __copy_t(const T* first, const T*last, T* result, __false_type)
	{
		//p("__copy_t(false_type)");
		return __copy_d(first, last, result, (ptrdiff_t*)0);
	}

	template<class InputIterator,class OutputIterator>
	OutputIterator __copy(InputIterator first, InputIterator last, OutputIterator result, input_iterator_tag)
	{
		//p("__copy(InputIter)");
		for (; first != last; ++first, ++result)
			*result = *first;
		return result;
	}

	template<class RandomAccessIterator, class OutputIterator>
	OutputIterator __copy(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, random_access_iterator_tag)
	{
		//p("__copy(RandomIter)");
		return __copy_d(first, last, result, distance_type(first));
	}
	
	template<class RandomAccessIterator, class OutputIterator,class Distance>
	OutputIterator __copy_d(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, Distance *)
	{
		//p("__copy_d");
		for (Distance n = last - first; n > 0; --n, ++first, ++result)
			*result = *first;
		return result;
	}


	//------------------------------------ copy_backward -------------------------------------------------
	template<class BidirectionIterator1,class BidirectionIterator2>
	BidirectionIterator2 copy_backward(
		BidirectionIterator1 first,
		BidirectionIterator1 last,
		BidirectionIterator2 result)
	{
		return __copy_backward_dispatch<BidirectionIterator1,BidirectionIterator2>()(first, last, result);
	}

	template<class BidirectionIterator1,class BidirectionIterator2>
	struct __copy_backward_dispatch
	{
		BidirectionIterator2 operator()(BidirectionIterator1 first, BidirectionIterator1 last, BidirectionIterator2 result)
		{
			return __copy_backward(first, last, result);
		}
	};

	template<class T>
	struct __copy_backward_dispatch< T*, T* >
	{
		T* operator()(T* first, T*last, T*result)
		{
			return __copy_backward_t(first, last, result, __type_traits<T>::has_trival_assignment_operator());
		}
	};

	template<class T>
	struct __copy_backward_dispatch < const T*, T* >
	{
		T* operator()(const T* first, const T* last, T* result)
		{
			return __copy_backward_t(first, last, result, __type_traits<T>::has_trival_assignment_operator());
		}
	};

	template<class T>
	T* __copy_backward_t(const T* first, const T* last, T * result, __true_type)
	{
		const ptrdiff_t n = last - first;
		memmove(result - n, first, sizeof(T)*n);
		return result - n;
	}

	template<class T>
	T* __copy_backward_t(T* first, T* last, T* result, __false_type)
	{
		return  __copy_backward(first, last, result);
	}
	

	template<class BidirectionIterator1 ,class BidirectionIterator2>
	BidirectionIterator2 __copy_backward(BidirectionIterator1 first, BidirectionIterator1 last, BidirectionIterator2 result)
	{
		while (first != last) *--result = *--last;
		return result;
	}


	

}

#endif