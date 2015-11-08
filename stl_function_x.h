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
	template<class Operator>
	class binder1st : public unary_function < typename Operator::second_argument_type, typename Operator::result_type >
	{
	protected:
		Operator op;
		typename Operator::first_argument_type value;

	public:
		explicit binder1st(const Operator &ops, const typename Operator::first_argument_type & x) :op(ops), value(x){}

		typename Operator::result_type operator()(const typename Operator::second_argument_type & x)const
		{
			return op(value, x);
		}
	};


	template<class Operator,class T>
	binder1st<Operator> bind1st(const Operator& op, const T & t)
	{
		typedef typename Operator::first_argument_type arg1_type;
		return binder1st<Operator>(op, arg1_type(t));
	}

	//bind2erst
	template<class Operation>
	class binder2st : public unary_function < typename Operation::first_argument_type, typename Operation::result_type >
	{
	protected:
		Operation op;
		typename Operation::second_argument_type value;

	public:
		explicit binder2st(const Operation & ops, const typename Operation::second_argument_type & y) :op(ops), value(y){}

		typename Operation::result_type operator()(const typename Operation::first_argument_type & x)const
		{
			return op(x, value);
		}
	};

	template<class Operation,class T>
	typename binder2st<Operation> bind2st(const Operation & op, const T & value)
	{
		typedef typename Operation::second_argument_type arg2_type;
		return binder2st<Operation>(op, value);
	}


	//compose1
	template<class Operation1,class Operation2>
	class unary_compose : public unary_function < typename Operation2::argument_type, typename Operation1::result_type >
	{
	protected:
		Operation1 op1;
		Operation2 op2;

	public:
		unary_compose(const Operation1 & ops1, const Operation2 & ops2) :op1(ops1), op2(ops2){}

		typename Operation1::result_type operator()(const typename Operation2::argument_type & value)const
		{
			return op1(op2(value));
		}
	};

	template<class Operation1,class Operation2>
	unary_compose<Operation1, Operation2> compose1(const Operation1 & op1, const Operation2 & op2)
	{
		return unary_compose<Operation1, Operation2>(op1, op2);
	}

	//compose2
	template<class Operation1,class Operation2,class Operation3>
	class binary_compose : public unary_function < typename Operation2::argument_type,typename Operation1::result_type >
	{
	protected:
		Operation1 op1;
		Operation2 op2;
		Operation3 op3;

	public:
		binary_compose(const Operation1 & ops1, const Operation2 & ops2,const Operation3 & ops3) :op1(ops1), op2(ops2),op3(ops3){}
		
		typename Operation1::result_type operator()(const typename Operation2::argument_type & x)const
		{
			return op1(op2(x), op3(x));
		}
	};

	template<class Operation1,class Operation2,class Operation3>
	binary_compose<Operation1, Operation2, Operation3> compose2(const Operation1 & op1, const Operation2 & op2, const Operation3 & op3)
	{
		return binary_compose<Operation1, Operation2, Operation3>(op1, op2, op3);
	}


	//ptr_fun unary_function
	template<class Arg,class Result>
	class pointer_to_unary_function : public unary_function < Arg, Result >
	{
	protected:
		Result(*func)(Arg);

	public:
		pointer_to_unary_function(){}
		pointer_to_unary_function(Result(*f)(Arg)) : func(f){}

		Result operator()(Arg x)const
		{
			return (*func)(x);
		}
	};

	template<class Arg,class Result>
	pointer_to_unary_function<Arg, Result> ptr_fun(Result(*f)(Arg))
	{
		return pointer_to_unary_function<Arg, Result>(f);
	}

	//ptr_fun binary_function
	template<class Arg1,class Arg2,class Result>
	class pointer_to_binary_function : public binary_function < Arg1, Arg2, Result >
	{
	protected:
		Result(*ptr)(Arg1, Arg2);
	public:
		pointer_to_binary_function(){}
		pointer_to_binary_function(Result(*f)(Arg1, Arg2)) :ptr(f){}

		Result operator()(Arg1 x1, Arg2 x2)const 
		{
			return (*ptr)(x1, x2);
		}
	};

	template<class Arg1,class Arg2,class Result>
	pointer_to_binary_function<Arg1, Arg2, Result> ptr_fun(Result(*f)(Arg1, Arg2))
	{
		return pointer_to_binary_function<Arg1, Arg2, Result>(f);
	}

	//-----------------------------------mem_func -----------------------------------------
	template<class S,class T>
	class mem_fun_t : public unary_function < T*, S >
	{
	protected:
		S (T::*f)();
	public:
		mem_fun_t(S(T::*pf)()) : f(pf){}
		S operator()(T * p)
		{
			return (p->*f)();
		}
	};

	template<class S,class T>
	mem_fun_t<S,T> mem_fun(S(T::*f)())
	{
		return mem_fun_t<S, T>(f);
	}

	template<class S,class T>
	class const_mem_fun_t : public unary_function <const T*, S >
	{
	protected:
		S(T::*f)()const;
	public:
		const_mem_fun_t(S(T::*pf)()const) :f(pf){}
		S operator()(const T * p)
		{
			return (p->*f)();
		}
	};


	template<class S,class T>
	const_mem_fun_t<S, T> mem_fun(S(T::*p)()const)
	{
		return const_mem_fun_t<S, T>(p);
	}
}

#endif