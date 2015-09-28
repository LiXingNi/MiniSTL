#ifndef _STL_CONSTRUCT_X_H
#define _STL_CONSTRUCT_X_H

#include <new>

namespace _LXX
{
	template<class T1, class T2>
	void construct(T1 * p, const T2 & val)
	{
		new(p)T1(val);
	}

}

#endif