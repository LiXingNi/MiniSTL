#ifndef _STL_HASHTABLE_X_H
#define _STL_HASHTABLE_X_H

#include "stl_allocate_x.h"
#include "stl_vector_x.h"
#include "stl_algo_x.h"

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
			const node * old_cur = cur; //当 cur 的下一个是end()时
			cur = cur->next; //cur = 0;
			if (!cur)
			{
				size_type bucket = ht->bkt_num(old_cur->val);
				while (!cur && ++bucket < ht->buckets.size()) //++bucket == buckets.size()，该循环不被执行
				{
					cur = ht->buckets[bucket];
				}
			}
			return *this; //此时，返回的指针指向0
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

	template <class Value, class Key, class HashFcn,
	class ExtractKey, class EqualKey, class Alloc>
	struct __hashtable_const_iterator {
		typedef hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>
			hashtable;
		typedef __hashtable_iterator<Value, Key, HashFcn,
			ExtractKey, EqualKey, Alloc>
			iterator;
		typedef __hashtable_const_iterator<Value, Key, HashFcn,
			ExtractKey, EqualKey, Alloc>
			const_iterator;
		typedef __hashtable_node<Value> node;

		typedef forward_iterator_tag iterator_category;
		typedef Value value_type;
		typedef ptrdiff_t difference_type;
		typedef size_t size_type;
		typedef const Value& reference;
		typedef const Value* pointer;

		const node* cur;
		const hashtable* ht;

		__hashtable_const_iterator(const node* n, const hashtable* tab)
			: cur(n), ht(tab) {}
		__hashtable_const_iterator() {}
		__hashtable_const_iterator(const iterator& it) : cur(it.cur), ht(it.ht) {}
		reference operator*() const { return cur->val; }
		pointer operator->() const { return &(operator*()); }
		const_iterator& operator++();
		const_iterator operator++(int);
		bool operator==(const const_iterator& it) const { return cur == it.cur; }
		bool operator!=(const const_iterator& it) const { return cur != it.cur; }
	};


	static const int __stl_num_primes = 28;
	static const unsigned long __stl_prime_list[__stl_num_primes] =
	{
		53, 97, 193, 389, 769,
		1543, 3079, 6151, 12289, 24593,
		49157, 98317, 196613, 393241, 786433,
		1572869, 3145739, 6291469, 12582917, 25165843,
		50331653, 100663319, 201326611, 402653189, 805306457,
		1610612741, 3221225473ul, 4294967291ul
	};

	inline unsigned long __stl_next_prime(unsigned long n)
	{
		const unsigned long * first = __stl_prime_list;
		const unsigned long * last = __stl_prime_list + __stl_num_primes;
		const unsigned long * pos = lower_bound(first, last, n);
		return pos == last ? *(last - 1) : *pos;
	}


	template<class Value,class Key,class HashFcn,
			class ExtractKey,class EqualKey,class Alloc>
	class hashtable
	{
	public:
		typedef Key key_type;
		typedef Value value_type;
		typedef HashFcn hasher;
		typedef EqualKey key_equal;

		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;
		typedef value_type&	reference;
		typedef const value_type& const_reference;

		hasher hash_funct()const{ return hash; }
		key_equal key_eq()const{ return equals; }


	private:
		HashFcn hash;
		key_equal equals;
		ExtractKey get_key;

		typedef __hashtable_node<Value> node;
		typedef _LXX::simpleAlloc<node, Alloc> node_allocator;

		vector<node*, Alloc> buckets;
		size_type num_elements;

	public:
		typedef __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> iterator;
		typedef __hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc> const_iterator;

		friend struct __hashtable_iterator < Value, Key, HashFcn, ExtractKey, EqualKey, Alloc > ;
		friend struct __hashtable_const_iterator < Value, Key, HashFcn, ExtractKey, EqualKey, Alloc > ;

	public:
		hashtable(size_t n, const HashFcn& hf, const EqualKey& eql, const ExtractKey& ext)
			: hash(hf), equals(equl), get_key(ext), num_elements(0)
		{
			initial_buckets(n);
		}

		hashtable(size_type n,const HashFcn& hf, const EqualKey&   eql)
			: hash(hf), equals(eql), get_key(ExtractKey()), num_elements(0)
		{
			initialize_buckets(n);
		}

		void initial_buckets(size_type n)
		{
			const size_type n_buckets = next_size(n);
			buckets.reserve(n_buckets);
			buckets.insert(buckets.end(), n_buckets, (node*)0);
			num_elements = 0;
		}

		size_type next_size(size_type n)const{ return __stl_next_prime(n); }

		//bkt_num
		size_type bkt_num(const value_type& obj)const
		{
			return bkt_num_key(get_key(obj));
		}

		size_type bkt_num_key(const key_type& key)const
		{
			return bkt_num_key(key, buckets.size());
		}

		size_type bkt_num_key(const key_type & key, size_t n)const
		{
			return hash(key) % n;
		}

		//attach func:
		size_type size()const{ return num_elements; }
		bool empty()const{ return size() == 0; }

		void swap(hashtable & ht)
		{
			std::swap(hash, ht.hash);
			std::swap(equals, ht.equals);
			std::swap(get_key, ht.get_key);
			buckets.swap(ht.buckets);
			std::swap(num_elements, ht.num_elements);
		}

		iterator begin()
		{
			for (size_type n = 0; n < buckets.size(); ++n)
			{
				if (buckets[n])
					return iterator(buckets[n], this);
			}
			return end();
		}
		iterator end()
		{
			return iterator(0, this);
		}

		const_iterator begin() const
		{
			for (size_type n = 0; n < buckets.size(); ++n)
				if (buckets[n])
					return const_iterator(buckets[n], this);
			return end();
		}

		const_iterator end() const { return const_iterator(0, this); }

		//size_control
	public:
		
		size_type bucket_count()const{ return buckets.size(); }
		size_type elems_in_bucket(size_type bucket)const //count how much elements in a bucket
		{
			size_type result = 0;
			for (node * curr = buckets[bucket]; curr; curr = curr->next)
			{
				result += 1;
			}
			return result;
		}

		//insert && resize
	public:

		void resize(size_type num_elements_hint)
		{
			const size_type old_n = buckets.size();
			if (num_elements_hint > old_n)  //this decision is strange,insert an element may cause the vector resize
 			{
				const size_type n = next_size(num_elements_hint);
				if (n > old)
				{
					vector<node*, A> tmp(n, (node*)0);
					try{
						for (size_type bucket = 0; bucket < old_n; ++bucket)
						{
							node * first = buckets[bucket];
							while (first)
							{
								size_type new_bucket = bkt_num(first->val, n); //calculate the new location
								buckets[bucket] = first->next;
								first->next = tmp[new_bucket];
								tmp[new_bucket] = first;
								first = buckets[bucket];
							}
							buckets.swap(tmp);
						}
					}
					catch (...)
					{
						for (size_type bucket = 0; bucket < tmp.size; ++bucket)
						{
							while (tmp[bucket])
							{
								node * next = tmp[bucket]->next;
								delete_node(tmp[bucket]);
								tmp[bucket] = next;
							}
						}
						throw;
					}
				}
			}
		}



	};



}

#endif