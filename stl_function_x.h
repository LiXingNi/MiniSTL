#ifndef _STL_FUNCTION_X_H
#define _STL_FUNCTION_X_H

namespace _LXX
{
	// for  less function object
	template<class Arg1,class Arg2,class Result>
	struct binary_function
	{
		typedef Arg1 first_argument_type;
		typedef Arg2 second_argument_type;
		typedef Result result_type;
	};

	template<class T>
	struct less : public binary_function < T, T, bool >
	{
		bool operator()(const T&x, const T&y)const{ return x < y; }
	};


	// for identyty function object
	template <class Arg,class Result>
	struct unary_function
	{
		typedef Arg argument_type;
		typedef Result result_type;
	};

	template<class T>
	struct identity : public unary_function < T, T >
	{
		const T& operator()(const T&x)const{ return x; }
	};

	//for select1st function object
	template<class Pair>
	struct select1st : public unary_function < Pair, typename Pair::first_type >
	{
		const typename Pair::first_type & operator()(const Pair & x)const
		{
			return x.first;
		}
	};
}

#endif