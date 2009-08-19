
#ifndef _ITERATOR_HACKS_H_
#define _ITERATOR_HACKS_H_

template<class T> struct selectFirst {
	typedef typename T::first_type result_type;
	result_type operator()(T a) const
	{
		return a.first;
	}
};

template<class T> struct selectSecond {
	typedef typename T::second_type result_type;
	result_type operator()(T a) const
	{
		return a.second;
	}
};

template<class T, class U> struct spGetBase {
	typedef T result_type;
	result_type operator()(U a) const
	{
		return T(a);
	}
};

#endif