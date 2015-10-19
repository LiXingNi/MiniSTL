#ifndef _STL_HASHTABLE_X_H
#define _STL_HASHTABLE_X_H

#include "stl_allocate_x.h"

namespace _LXX
{
	template<class Value>
	struct __hashtable_node
	{
		__hashtable_node * next;
		Value val;
	};

	template<class Value, class Key, class HashFcn,
			 class ExtractKey, class EqualKey, class Alloc>
	struct hashtable;

	template<class Value, class Key, class HashFcn,
			 class ExtractKey, class EqualKey, class Alloc>
	struct __hashtable_const_iterator;

	template<class Value, class Key, class HashFcn,
			 class ExtractKey, class EqualKey, class Alloc>
	struct __hashtable_iterator
	{
		typedef hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> hashtable;
		typedef __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> iterator;
		typedef __hashtable_const_iterator<Value, Key, HashFcn,ExtractKey, EqualKey, Alloc> const_iterator;

		typedef __hashtable_node<Value> node;

		typedef forward_iterator_tag iterator_category;
		typedef Value value_type;
		typedef ptrdiff_t difference_type;
		typedef size_t size_type;
		typedef Value& reference;
		typedef Value* pointer;

		node * cur;
		hashtable* ht;

		__hashtable_iterator(node * n, hashtable* tab) :cur(n), ht(tab){}
		__hashtable_iterator(){}
		reference operator*()const{ return cur->val; }
		pointer operator->()const{ return &(operator*()); }

		iterator& operator++()
		{
			const node * old_cur = cur;
			cur = cur->next;
			if (!cur)
			{
				size_type bucket = ht->bkt_num(old_cur->val);
				while (!cur && ++bucket < ht->buckets.size())
				{
					cur = ht->buckets[bucket];
				}
			}
			return *this;
		}
		iterator operator++(int)
		{
			iterator tmp = *this;
			++*this;
			return tmp;
		}
		bool operator==(const iterator& it)const{ return cur == it.curr; }
		bool operator!=(const iterator& it)const{ return cur != it.curr; }
	};


	template<class Value,class Key,class HashFcn,
			class ExtractKey,class EqualKey,class Alloc>
	class hashtable
	{
	public:
		typedef Key key_type;
		typedef Value value_type;
		typedef HashFcn hasher;
		typedef EqualKey key_equal;


	};



}

#endif