/* $Id$
 * vim: fdm=marker
 *
 * This file is part of libmVCA.
 *
 * libmVCA is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libmVCA is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libmVCA.  If not, see <http://www.gnu.org/licenses/>.
 *
 * (c) 2008,2009,2010 Lehrstuhl Softwaremodellierung und Verifikation (I2), RWTH Aachen University
 *           and Lehrstuhl Logik und Theorie diskreter Systeme (I7), RWTH Aachen University
 * Author: David R. Piegdon <david-i2@piegdon.de>
 *
 */

#include <set>
#include <ostream>

#include <string>

#ifdef _WIN32
# include <winsock.h>
# include <stdio.h>
#else
# include <arpa/inet.h>
#endif

namespace libmVCA {

using namespace std;

// the set_*-functions are very generic and may be used for any kind of set

template <class T>   bool set_includes(const set<T> &superset, const set<T> &subset)
{{{
	typename set<T>::iterator Si, si;

	Si = superset.begin();
	si = subset.begin();

	// set::size() is O(1)
	if(subset.size() > superset.size())
		return false;

	while( Si != superset.end() && si != subset.end() ) {
		if(*si == *Si) {
			++si;
		} else {
			if(*si < *Si)
				return false;
			else
				++Si;
		}
	}

	return ( Si != superset.end() || si == subset.end() );
}}}

template <class T>   set<T> set_union(set<T> s, const set<T> &t)
{{{
	typename set<T>::iterator si;

	for(si = t.begin(); si != t.end(); ++si)
		s.insert(*si);

	return s;
}}}

template <class T>   void set_insert(set<T> &into, const set<T> &subset)
{{{
	typename set<T>::iterator si;

	for(si = subset.begin(); si != subset.end(); ++si)
		into.insert(*si);
}}}

template <class T>   set<T> set_intersect(const set<T> &s, const set<T> &t)
{{{
	set<T> ret;

	typename set<T>::iterator si;

	for(si = s.begin(); si != s.end(); ++si) {
		if(t.find(*si) != t.end())
			ret.insert(*si);
		// set::size() is O(1)
		if(ret.size() >= s.size() || ret.size() >= t.size())
			break;
	}

	return ret;
}}}

template <class T>   set<T> set_without(const set<T> &s, const set<T> &remove)
{{{
	set<T> ret;
	typename set<T>::iterator si, rmi;

	si = s.begin();
	rmi = remove.begin();

	while( si != s.end() && rmi != remove.end() ) {
		if(*si < *rmi) {
			ret.insert(*si);
			++si;
		} else {
			if(*si == *rmi) {
				++si;
				++rmi;
			} else {
				++rmi;
			}
		}
	}
	while(si != s.end()) {
		ret.insert(*si);
		++si;
	}

	return ret;
}}}

template <class T>   void print_set(ostream &con, const set<T> &s)
{{{
	typename set<T>::iterator si;

	con << "{ ";
	si = s.begin();
	while(si != s.end()) {
		con << *si;
		si++;
		if(si != s.end())
			con << ", ";
	}
	con << " }";
}}}

inline basic_string<int32_t> serialize_integer_set(const set<int> & s)
{{{
	set<int>::iterator si;
	basic_string<int32_t> ret;

	ret += htonl(s.size());
	for(si = s.begin(); si != s.end(); ++si)
		ret += htonl(*si);

	return ret;
}}}

inline bool deserialize_integer_set(set<int> & s, basic_string<int32_t>::iterator &it, basic_string<int32_t>::iterator limit, int & progress)
{{{
	int size;

	s.clear();
	progress = 0;

	if(it == limit) return false;

	size = ntohl(*it);

	if(size < 0) return false;

	++it; ++progress;

	while(size) {
		if(it == limit) goto deserialization_failed;
		s.insert(ntohl(*it));
		++it; --size; ++progress;
	}

	return true;

deserialization_failed:
	s.clear();
	return false;
}}}

} // end namespace libmVCA

