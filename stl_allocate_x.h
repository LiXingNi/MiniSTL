#ifndef _STL_ALLOCATE_X_H
#define _STL_ALLOCATE_X_H

#include <stdexcept>

namespace _LXX
{

	/////////////////////////////////////////////////////////// __malloc_alloc_template
	template<int inst>
	class __malloc_alloc_template
	{
	private:
		static void * oom_malloc(size_t);
		static void * oom_realloc(void * ,size_t);
		
		static void(*__malloc_alloc_handler)();
	
	public:
		static void * allocate(size_t n)
		{
			void * result = malloc(n);
			if (0 == result)
				result = oom_malloc(n);
			return result;
		}
		static void * reallocate(void * ptr, size_t n)
		{
			void * result = realloc(ptr, n);
			if (0 == result)
				result = oom_realloc(ptr, n);
			return result;
		}

		static void deallocate(void * p,size_t)
		{
			free(p);
		}

		static void(*set_malloc_handler(void(*f)()))()
		{
			void(*old)() = __malloc_alloc_handler;
			__malloc_alloc_handler = f;
			return old;
		}
	};
	
	template <int inst>
	void(*__malloc_alloc_template<inst>::__malloc_alloc_handler)() = 0;

	template<int inst>
	void * __malloc_alloc_template<inst>::oom_malloc(size_t n)
	{
		void * result;
		void(*my_handle)();
		for (;;)
		{
			my_handle = __malloc_alloc_handler;
			if (0 == my_handle) throw std::runtime_error("out_of_memory");;
			(*my_handle)();
			result = malloc(n);
			if (result) break;
		}
		return result;
	}

	template<int inst>
	void * __malloc_alloc_template<inst>::oom_realloc(void * ptr, size_t n)
	{
		void * result;
		void(*my_handler)();
		for (;;)
		{
			my_handler = __malloc_alloc_handler;
			if (0 == my_handle) throw;
			(*my_handle)();
			result = realloc(ptr,n);
			if (result) break;
		}
		return result;
	}

	typedef __malloc_alloc_template<0> malloc_alloc;

	////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////////// __default_alloc_template

	enum{ _ALIGN = 8 };
	enum{_MAX_BYTES = 128};
	enum{_NFREELIST = _MAX_BYTES / _ALIGN};

	template<bool thread,int inst>
	class __default_alloc_template
	{
	private:
		//data member
		union obj
		{
			obj * free_list_link;
			char data[1];
		};
		static obj* free_list[_NFREELIST];
		static char * start_storage;
		static char * end_storage;

		//functuin member
		static size_t RoudUp(size_t n)
		{
			return _ALIGN * ((n - 1) / _ALIGN + 1);
		}
		
		static size_t Index(size_t n)
		{
			size_t bytes = RoudUp(n);
			return (bytes / _ALIGN - 1);
		}

		static void * chunk_size(size_t & n, size_t size)
		{
			size_t bytes_to_get = n * size;
			size_t left_bytes = end_storage - start_storage;
			void * result;
			if (left_bytes >= bytes_to_get)
			{
				result = start_storage;
				start_storage += bytes_to_get;
				return result;
			}
			else if (left_bytes >= size)
			{
				n = left_bytes / size;
				result = start_storage;
				start_storage += n * size;
				return result;
			}
			else if (left_bytes > 0)
			{
				obj ** my_free_list = free_list + Index(left_bytes);
				((obj*)(start_storage))->free_list_link = *my_free_list;
				*my_free_list = (obj*)start_storage;
			}

			size_t bytes_total = 2 * bytes_to_get;
			start_storage = (char*)malloc(bytes_total);
			
			if (0 == start_storage)
			{
				size_t i;
				obj** my_free_list;
				for (i = size; i <= _MAX_BYTES; i += _ALIGN)
				{
					my_free_list = free_list + Index(i);
					if (*my_free_list != 0)
					{
						obj * current = *my_free_list;
						*my_free_list = current->free_list_link;
						start_storage = (char*)current;
						end_storage = start_storage + i;
						return chunk_size(n, size);
					}
				}
				end_storage = 0;
				start_storage = (char*) malloc_alloc::allocate(bytes_total);
			}
			end_storage = start_storage + bytes_total;
			return chunk_size(n, size);
		}

		static void * refill(size_t size)
		{
			size = RoudUp(size);
			size_t n = 20;
			char * result = (char*)chunk_size(n, size);
			if (1 == n) return result;

			obj** my_free_link = free_list + Index(size);
			obj * current = (obj*)(result + size);
			*my_free_link = current;
			while (--n > 1)
			{
				current->free_list_link = (obj*)(((char*)current + size));
				current = current->free_list_link;
			}
			current->free_list_link = 0;  //最后一个内存块的下一个节点值为0.
			return result;
		}

	public:
		static void * allocate(size_t n)
		{
			if (n > _MAX_BYTES)
				return malloc_alloc::allocate(n);
			void * result;
			obj ** my_free_list = free_list + Index(n);
			result = *my_free_list;
			if (0 == result)
			{
				result = refill(RoudUp(n));
				return result;
			}
			*my_free_list = ((obj*)result)->free_list_link;
			return result;
		}

		static void deallocate(void * ptr,size_t n)
		{
			obj ** my_free_list = free_list + Index(n);
			if (n > _MAX_BYTES)
			{
				malloc_alloc::deallocate(ptr, n); 
			}
			((obj*)ptr)->free_list_link = *my_free_list;
			*my_free_list = (obj*)ptr;
		}
	};

	template<bool thread,int inst>
	typename __default_alloc_template<thread, inst>::obj* __default_alloc_template<thread, inst>::free_list[_NFREELIST] =
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	template<bool thread,int inst>
	char * __default_alloc_template<thread, inst>::start_storage = 0;

	template<bool thread,int inst>
	char * __default_alloc_template<thread, inst>::end_storage = 0; 

	typedef __default_alloc_template<false, 0> default_alloc;


	////////////////////////////////////////////////////////////////// STL Interface simpleAlloc

	template <class T, class Alloc = default_alloc>
	class simpleAlloc
	{
	public:
		static T * allocate(size_t n)
		{
			return 0 == n? 0 : (T*)Alloc::allocate(n * sizeof(T));
		}

		static void * allocate()
		{
			return (T*)Alloc::allocate(sizeof(T));
		}

		static void deallocate(void * ptr)
		{
			Alloc::deallocate(ptr, sizeof(T));
		}
		
		static void deallocate(void * ptr, size_t n)
		{
			Alloc::deallocate(ptr, n * sizeof(T));
		}
	};
}


#endif