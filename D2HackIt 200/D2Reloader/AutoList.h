//////////////////////////////////////////////////////////////////////
//
// sting - <stingxp@yahoo.com>
//
//////////////////////////////////////////////////////////////////////

#ifndef __AUTO_LIST_H__
#define __AUTO_LIST_H__

#pragma warning( disable : 4786)
#include <algorithm>
#include <set>
#include <functional>
#include "men_fun_bind.h"


template<class Cont>
class AutoList : public std::set<typename Cont::key_type>
{
public:
	explicit AutoList(Cont& x):cont_(x)
	{
		// works in vc7.1, not with bullshit vc6.0
//		std::for_each( cont_.begin(), cont_.end(), const_mem_fun_bind(*this, AutoList::insert_element) );
		// vc6.0 use this
		std::for_each( cont_.begin(), cont_.end(), const_mem_fun_bind<AutoList,Cont::value_type,void>(*this, AutoList::insert_element) );
	}
	~AutoList()
	{
		std::for_each( begin(), end(), const_mem_fun_bind<AutoList, value_type, void>(*this, AutoList::erase_element) );
	}
private:
	void insert_element(typename Cont::const_reference val)
	{
		insert(val.first);
	}
	void erase_element(const_reference val)
	{
		cont_.erase(val);
	}
public:
	bool test_element(const_reference val)
	{
		return find(val) != end();
	}
private:
	Cont& cont_;
};

#endif // __AUTO_LIST_H__
