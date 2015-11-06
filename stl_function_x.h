#ifndef _STL_FUNCTION_X_H
#define _STL_FUNCTION_X_H

namespace _LXX
{
	//unary_function
	template<class Arg,class Result>
	struct unary_function
	{
		typedef Arg argument_type;
		typedef Result result_type;
	};

	//binary_function
	template<class Arg1,class Arg2,class Result>
	struct binary_function
	{
		typedef Arg1 first_argument_type;
		typedef Arg2 second_argument_type;
		typedef Result result_type;
	};


	//----------------------------------------------------- arithmetic functor ----------------------------------------------------------
	// plus<T>
	template<class T>
	struct plus : public binary_function < T, T, T >
	{
		T operator()(const T& x, const T& y){ return x + y; }
	};

	// minus<T>
	template<class T>
	struct minus : public binary_function < T, T, T >
	{
		T operator()(const T & x, const T & y) { return x - y; }
	};

	//multiplies<T>
	template<class T>
	struct multiplies : public binary_function < T, T, T >
	{
		T operator()(const T & x, const T & y){ return x * y; }
	};

	//divides<T>
	template<class T>
	struct divides : public binary_function < T, T, T >
	{
		T operator()(const T & x, const T & y){ return x / y; }
	};

	//modulus<T>
	template<class T>
	struct modulus : public binary_function < T, T, T >
	{
		T operator()(const T & x, const T & y){ return x % y; }
	};

	//negate<T>
	template<class T>
	struct negate : public unary_function < T, T >
	{
		T operator()(const T & x){ return -x; }
	};

	//------------------------------------------------------ relational functor ----------------------------------------------------------------
	template<class T>
	struct less : public binary_function < T, T, bool >
	{
		bool operator()(const T&x, const T&y)const{ return x < y; }
	};

	template<class T>
	struct less_equal : public binary_function < T, T, bool >
	{
		bool operator()(const T&x, const T&y)const{ return x <= y; }
	};

	template<class T>
	struct greater : public binary_function < T, T, bool >
	{
		bool operator()(const T&x, const T&y)const{ return x > y; }
	};

	template<class T>
	struct greater_equal : public binary_function < T, T, bool >
	{
		bool operator()(const T&x, const T&y)const{ return x >= y; }
	};

	template<class T>
	struct equal_to : public binary_function < T, T, bool >
	{
		bool operator()(const T&x, const T&y)const{ return x == y; }
	};

	template<class T>
	struct not_equal_to : public binary_function < T, T, bool >
	{
		bool operator()(const T&x, const T&y)const{ return x != y; }
	};


	//----------------------------------------------------- identity, select, project ---------------------------------------------------------------
	template<class T>
	struct identity : public unary_function < T, T >
	{
		T operator()(const T & x){ return x; }
	};

	template<class Pair>
	struct select1st : public unary_function < Pair, typename Pair::first_type >
	{
		const typename Pair::first_type& operator()(const Pair& x){ return x.first; }
	};

	template<class Pair>
	struct select2nd : public unary_function < Pair, typename Pair::second_type >
	{
		const typename Pair::second_type& operator()(const Pair& x){ return x.second; }
	};

	template<class Arg1,class Arg2>
	struct project1st : public binary_function < Arg1, Arg2, Arg1 >
	{
		Arg1 operator()(const Arg1 & x, const Arg2& y){ return x; }
	};

	template<class Arg1, class Arg2>
	struct project2nd : public binary_function < Arg1, Arg2, Arg2 >
	{
		Arg2 operator()(const Arg1 & x, const Arg2& y){ return y; }
	};


	//------------------------------------------------function adapter ------------------------------------------------------------------------------------

	// unary_negate
	template<class Predicate>
	class unary_negate : public unary_function <typename Predicate::argument_type, bool >
	{
	protected:
		Predicate pred;
	public:
		explicit unary_negate(const Predicate& x) :pred(x){}
		
		bool operator()(const typename Predicate::argument_type & x)const
		{
			return !pred(x);
		}

	};

	template<class Predicate>
	unary_negate<Predicate> not1(const Predicate& x)
	{
		return unary_negate<Predicate>(x);
	}

	// binary_negate
	template<class Predicate>
	class binary_negate : public binary_function<typename Predicate::first_argument_type,typename Predicate::second_argument_type,typename Predicate::result_type>
	{
	protected:
		Predicate pred;

	public:
		explicit binary_negate(const binary_negate<Predicate>& x) :pred(x){}

		bool operator()(const typename Predicate::first_argument_type & x, const typename Predicate::second_argument_type & y)const
		{
			return !pred(x, y);
		}
	};


	template<class Predicate>
	binary_negate<Predicate> not2(const Predicate & x)
	{
		return binary_negate<Predicate>(x);
	}

	// binder1st

}

#endif