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
 * (c) 2009,2010 Lehrstuhl Softwaremodellierung und Verifikation (I2), RWTH Aachen University
 *           and Lehrstuhl Logik und Theorie diskreter Systeme (I7), RWTH Aachen University
 * Author: David R. Piegdon <david-i2@piegdon.de>
 *
 */

#include <libmVCA/mVCA.h>
// for libmVCA::deserialize :
#include <libmVCA/deterministic_mVCA.h>
#include <libmVCA/nondeterministic_mVCA.h>

#ifdef _WIN32
# include <stdio.h>
# include <winsock.h>
#else
# include <arpa/inet.h>
#endif

namespace libmVCA {

using namespace std;

static const char* libmVCA_version_string = "libmVCA version " VERSION;

const char* libmVCA_version()
{{{
	return libmVCA_version_string;
}}}



// for mVCA_run:
// NOTE: all comparison-functions don't care about the prefix! this way,
// we can easily make a breadth-first search and remember visited
// m/state tuples in a set<mVCA_run> this is used in mVCA::shortest_run
bool operator<(const mVCA_run first, const mVCA_run second)
{{{
	if(first.m < second.m)
		return true;
	if(first.m == second.m)
		return first.state < second.state;
	return false;
}}}
bool operator==(const mVCA_run first, const mVCA_run second)
{{{
	return (first.m == second.m && first.state == second.state);
}}}
bool operator>(const mVCA_run first, const mVCA_run second)
{{{
	if(first.m > second.m)
		return true;
	if(first.m == second.m)
		return first.state > second.state;
	return false;
}}}



mVCA::mVCA()
{{{
	state_count = 0;
	initial_state = -1;
	m_bound = -1;
}}}
void mVCA::set_alphabet(pushdown_alphabet & alphabet)
{{{
	this->alphabet = alphabet;
}}}
void mVCA::unset_alphabet()
{{{
	this->alphabet.clear();
}}}
pushdown_alphabet mVCA::get_alphabet()
{{{
	return this->alphabet;
}}}
int mVCA::get_alphabet_size()
{{{
	return this->alphabet.get_alphabet_size();
}}}

int mVCA::get_initial_state()
{{{
	return initial_state;
}}}
set<int> mVCA::get_initial_states()
{{{
	set<int> ret;
	ret.insert(initial_state);
	return ret;
}}}
set<int> mVCA::get_final_states()
{{{
	return final_states;
}}}
bool mVCA::set_initial_state(int state)
{{{
	if(state >= 0 && state < state_count) {
		this->initial_state = state;
		return true;
	} else {
		return false;
	}
}}}
bool mVCA::set_final_state(const set<int> & states)
{{{
	set<int>::iterator si;
	for(si = states.begin(); si != states.end(); ++si)
		if(*si < 0 || *si >= state_count)
			return false;
	final_states = states;
	return true;
}}}

bool mVCA::contains_initial_states(const set<int> & states)
{{{
	set<int>::iterator si;
	for(si = states.begin(); si != states.end(); ++si)
		if(*si == initial_state)
			return true;
	return false;
}}}
bool mVCA::contains_final_states(const set<int> & states)
{{{
	set<int>::iterator si;
	for(si = states.begin(); si != states.end(); ++si)
		if(final_states.find(*si) != final_states.end())
			return true;
	return false;
}}}

set<int> mVCA::transition(int from, int & m, int label)
{{{
	set<int> src;
	src.insert(from);
	return transition(src, m, label);
}}}

set<int> mVCA::run(const set<int> & from, int & m, list<int>::iterator word, list<int>::iterator word_limit)
{{{
	set<int> current = from;

	while(word != word_limit)
		if(!endo_transition(current, m, *word))
			break; // current is cleared by endo_transition.

	return current;
}}}



list<int> mVCA::shortest_run(const set<int> & from, int m, const set<int> & to, int to_m, bool &reachable)
{{{
	// width-first search
	set<mVCA_run> visited; // NOTE that mVCA_run only discriminates by m/state.
	set<int>::iterator si;
	list<mVCA_run> run_fifo;
	mVCA_run current, next;

	// fill fifo with initial states
	current.m = m;
	for(si = from.begin(); si != from.end(); ++si) {
		current.state = *si;
		run_fifo.push_back(current);
	}

	while(!run_fifo.empty()) {
		current = run_fifo.front();
		run_fifo.pop_front();

		// skip visited states
		// (NOTE that mVCA_run comparators only look at m and state, not at prefix!)
		if(visited.find(current) != visited.end())
			continue;

		// mark as visited
		visited.insert(current);

		// check final
		if((to_m < 0 || to_m == current.m) && to.find(current.state) != to.end()) {
			reachable = true;
			return current.prefix;
		}

		// iterate over all successors
		for(int sigma = 0; sigma < alphabet.get_alphabet_size(); sigma++) {
			set<int> src;
			set<int> dst;
			src.insert(current.state);
			next.m = m;
			dst = this->transition(src, next.m, sigma);

			if(next.m < 0 || dst.empty())
				continue;

			next.prefix = current.prefix;
			next.prefix.push_back(sigma);

			for(si = dst.begin(); si != dst.end(); ++si) {
				next.state = *si;
				run_fifo.push_back(next);
			}
		}
	}
	list<int> ret;
	reachable = false;
	return ret; // empty word
}}}

bool mVCA::contains(list<int> & word)
{{{
	return this->contains(word.begin(), word.end());
}}}
bool mVCA::contains(list<int>::iterator word, list<int>::iterator word_limit)
{{{
	set<int> s;
	set<int> ini;
	int m = 0;
	ini = this->get_initial_states();
	s = this->run(ini, m, word, word_limit);
	if(m != 0)
		return false;
	return contains_final_states(s);
}}}
// obtain shortest word in language resp. test if language is empty,
list<int> mVCA::get_sample_word(bool & is_empty)
{{{
	set<int> i = get_initial_states();
	set<int> f = get_final_states();
	bool reachable;
	list<int> w;
	w = shortest_run(i, 0, f, 0, reachable);
	is_empty = ! reachable;
	return w;
}}}
bool mVCA::is_empty()
{{{
	bool ret;
	get_sample_word(ret);
	return ret;
}}}

basic_string<int32_t> mVCA::serialize()
{{{
	basic_string<int32_t> ret;
	set<int>::iterator si;

	ret += 0; // size, will be filled in later.

	ret += htonl( (int) this->get_derivate_id() );
	ret += htonl(state_count);
	ret += alphabet.serialize();
	ret += htonl(initial_state);
	ret += htonl(final_states.size());
	for(si = final_states.begin(); si != final_states.end(); ++si)
		ret += htonl(*si);
	ret += htonl(m_bound);
	ret += this->serialize_derivate();

	ret[0] = htonl(ret.length() - 1);

	return ret;
}}}
bool mVCA::deserialize(basic_string<int32_t>::iterator &it, basic_string<int32_t>::iterator limit)
{
	
}

mVCA * deserialize_mVCA(basic_string<int32_t>::iterator &it, basic_string<int32_t>::iterator limit)
{{{
	basic_string<int32_t>::iterator adv;
	if(it == limit) return NULL;
	if(ntohl(*it) < 1) return NULL;
	adv = it;
	adv++;
	if(adv == limit) return NULL;
	enum mVCA::mVCA_derivate derivate_type;
	derivate_type = (enum mVCA::mVCA_derivate)ntohl(*adv);

	mVCA * ret;

	switch(derivate_type) {
		case mVCA::DERIVATE_DETERMINISTIC:
			ret = new deterministic_mVCA;
			break;
		case mVCA::DERIVATE_NONDETERMINISTIC:
			ret = new nondeterministic_mVCA;
			break;
		default:
			return NULL;
	}

	if(!ret->deserialize(it, limit)) {
		delete ret;
		return NULL;
	}

	return ret;
}}}


} // end of namespace libmVCA

