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

#ifndef __libalf_answer_h__
# define __libalf_answer_h__

namespace libalf {

// a possible <answer> has to implement the following:
//
// bool operator==(&answer)
// bool operator!=(&answer)
// bool operator>(&answer) , where true > possibly true > unknown > possibly false > false
// void operator=(&answer)
//
// bool operator==(bool)
// bool operator!=(bool)
// bool operator>(bool)
// void operator=(bool)
//
// all the above are valid for the builtin-type bool.


// an example implementation:

enum e_extended_bool {
	EBOOL_FALSE = 0,
	EBOOL_UNKNOWN = 1,
	EBOOL_TRUE = 1
};

class extended_bool {
	public:
		enum e_extended_bool value;


		bool operator==(extended_bool &other);

		bool operator!=(extended_bool &other);

		void operator=(extended_bool &other);

		bool operator>(extended_bool &other);

		bool operator==(bool other);

		bool operator>(bool other);

		void operator=(bool other);
};

extern bool operator==(extended_bool a, extended_bool b);

}

#endif // __libalf_answer_h__

