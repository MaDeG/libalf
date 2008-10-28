/* $Id$
 * vim: fdm=marker
 *
 * libalf - Automata Learning Factory
 *
 * (c) by David R. Piegdon, i2 Informatik RWTH-Aachen
 *        <david-i2@piegdon.de>
 *
 * see LICENSE file for licensing information.
 */

#ifndef __libalf_automata_h__
# define __libalf_automata_h__

#include <list>
#include <string>

namespace libalf {

using namespace std;

enum automaton_type {
	FINITE_AUTOMATON,
	FINITE_LANGUAGE_AUTOMATON,
	NONDETERMINISTIC_FINITE_AUTOMATON,
	DETERMINISTIC_FINITE_AUTOMATON
};

enum automaton_implementation {
	IMP_NO_IMPLEMENTATION,
	IMP_AMORE
};

// an automaton implementation may throw this exception in case
// a function/operation is not defined
/*
class automaton_implementation_exception {
	virtual ~automaton_implementation_exception() { };
	virtual const char* what() const throw() = 0;
};
*/

class finite_automaton {
	public:
		virtual ~finite_automaton() { };

		virtual enum automaton_type get_type() {
			return FINITE_AUTOMATA;
		}

		virtual enum automaton_implementation get_implementation() {
			return IMP_NO_IMPLEMENTATION;
		}

		virtual finite_automaton * clone() = 0;

		virtual string generate_dotfile() = 0;
}'

class finite_language_automaton : public finite_automaton {
	public:
		virtual ~finite_language_automaton() { };

		virtual enum automaton_type get_type() {
			return FINITE_LANGUAGE_AUTOMATON;
		}

		virtual finite_language_automaton * clone() = 0;

		// LANGUAGE/AUTOMATON OPERATIONS

		// UNARY TESTS
		// is the language of this automaton empty?
		virtual bool is_empty() = 0;
		// get a random sample word from this automaton
		virtual list<int> get_sample_word() = 0;

		// BINARY TESTS
		// test if this == other
		virtual bool operator==(finite_language_automaton &other) = 0;
		// test if this includes subautomaton
		virtual bool includes(finite_language_automaton &subautomaton) = 0;
		// test if this is a subset of superautomaton
		virtual bool is_subset_of(finite_language_automaton &superautomaton) = 0;

		// test if word is contained in language of this
		virtual bool contains(list<int>) = 0;

		// UNARY OPERATIONS
		virtual void minimize() = 0;
		virtual void lang_complement() = 0;

		// BINARY OPERATIONS
		// this+b
		virtual finite_language_automaton * lang_union(finite_language_automaton &other) = 0;
		// this AND b
		virtual finite_language_automaton * lang_intersect(finite_language_automaton &other) = 0;
		// (this+b) - (this AND b)
		virtual finite_language_automaton * lang_difference(finite_language_automaton &other) = 0;
		// this-b
		virtual finite_language_automaton * lang_without(finite_language_automaton &other) = 0;
		// this.b
		virtual finite_language_automaton * lang_concat(finite_language_automaton &other) = 0;
};

class deterministic_finite_automaton : public finite_language_automaton {
	public:
		virtual ~deterministic_finite_automaton() { };

		virtual enum automaton_type get_type() {
			return DETERMINISTIC_FINITE_AUTOMATA;
		}

		virtual deterministic_finite_automaton * clone() = 0;

		virtual nondeterministic_finite_automaton* nondeterminize() = 0;
}

class nondeterministic_finite_automaton : public finite_language_automaton {
	public:
		virtual ~nondeterministic_finite_automaton() { };

		virtual enum automaton_type get_type() {
			return NONDETERMINISTIC_FINITE_AUTOMATA;
		}

		virtual nondeterministic_finite_automaton * clone() = 0;

		virtual deterministic_finite_automaton * determinize() = 0;
}



}; // end namespace libalf

#endif

