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


/*
 * BIERMANN is an offline learning algorithm, described e.g. in
 *	* A.W. Biermann, J.A. Feldman - On the Synthesis of Finite-State Machines from Samples of their Behavior
 *	* Martin Leucker - Learning Meets Verification
 *	* Arlindo L. Oliveira and João P.M. Silva - Efficient Algorithms for the Inference of Minimum Size DFAs
 * NOTE that due to it being offline, only knowledgebases are supported, no teachers.
 */


#ifndef __libalf_algorithm_biermann_h__
# define __libalf_algorithm_biermann_h__

#include <string>
#include <list>
#include <map>

#include <math.h>

#include <libalf/knowledgebase.h>
#include <libalf/automata.h>
#include <libalf/learning_algorithm.h>

namespace libalf {

using namespace std;

template <class answer>
class basic_biermann : public learning_algorithm<answer> {
	public:	// types
		class node_comparator {
			public:
				bool operator() (typename knowledgebase<answer>::node * a, typename knowledgebase<answer>::node * b)
				{ return a < b; };
		};
		typedef typename knowledgebase<answer>::node* knowledgebase_node_ptr;

		// mapping is the final solution, a mapping from the LFDFAs states to the final mDFAs states
		typedef map<knowledgebase_node_ptr, int, node_comparator> mapping;

		class constraint {
			// very reduced constraint: can only represent the following formulas:
			// l1 != l2
			// and
			// l1 != l2 || l3 == l4
			public:
				knowledgebase_node_ptr l1, l2, l3, l4;
				bool has_second;

				pair<bool, bool> verify(mapping & solution)
				// return <true, true> if all (required) information is already set and the constraint is true.
				// return <true, false> if all information is already set and the constraint is false.
				// return <false, ignore> if information is missing.
				{{{
					pair<bool, bool> ret;
					typename mapping::iterator mi1, mi2;

					if(has_second) {
						mi1 = solution.find(l3);
						if(mi1 == solution.end())
							goto incomplete;

						mi2 = solution.find(l4);
						if(mi2 == solution.end())
							goto incomplete;

						if(mi1->second == mi2->second) {
							ret.first = true;
							ret.second = true;
							return ret;
						}
					}

					mi1 = solution.find(l1);
					if(mi1 == solution.end())
						goto incomplete;

					mi2 = solution.find(l2);
					if(mi2 == solution.end())
						goto incomplete;

					ret.first = true;
					ret.second = (mi1->second != mi2->second);
					return ret;
				incomplete:
					ret.first = false;
					ret.second = false;
					return ret;
				}}}

				void print(ostream &os)
				{{{
					char buf1[64];
					char buf2[64];
					list<int> word;
					string s1,s2;

					word = l1->get_word();
					s1 = word2string(word, '.');
					word = l2->get_word();
					s2 = word2string(word, '.');

					snprintf(buf1, 64, "(%s != %s)", s1.c_str(), s2.c_str());
					if(has_second) {
						word = l3->get_word();
						s1 = word2string(word, '.');
						word = l4->get_word();
						s2 = word2string(word, '.');
						snprintf(buf2, 64, " || (%s == %s)", s1.c_str(), s2.c_str());
					}
					buf1[63] = 0;
					buf2[63] = 0;

					os << buf1;
					if(has_second)
						os << buf2;
				}}}
		};

	protected: // data
		int mdfa_size;
		list<constraint> constraints;
		set<knowledgebase_node_ptr> sources;
		mapping solution;


	public: // methods
		basic_biermann()
		{{{
			// nothing
		}}}
		virtual ~basic_biermann()
		{{{
			// nothing.
		}}}

		virtual void increase_alphabet_size(int new_asize)
		{{{
			this->set_alphabet_size(new_asize);
		}}}

		virtual void set_knowledge_source(teacher<answer> *teach, knowledgebase<answer> *base)
		// throw error to logger if teacher is set
		{{{
			if(teach) {
				(*this->my_logger)(LOGGER_ERROR, "algorithm_biermann does not support teachers, as it is an offline-algorithm. please either use an online-algorithm like angluin or use a knowledgebase.\n");
				this->my_teacher = NULL;
				this->my_knowledge = NULL;
			} else {
				this->my_teacher = NULL;
				this->my_knowledge = base;
			}
		}}}

		virtual void get_memory_statistics(statistics & stats)
		{ /* FIXME: maybe keep some stats from last run? */ }

		virtual bool sync_to_knowledgebase()
		{{{
			return true;
		}}}

		virtual bool supports_sync()
		{{{
			return true;
		}}}

		virtual basic_string<int32_t> serialize()
		{{{
			basic_string<int32_t> ret;

			// we don't have any internal, persistent data
			ret += htonl(1);
			ret += htonl(learning_algorithm<answer>::ALG_BIERMANN);

			return ret;
		}}}
		virtual bool deserialize(basic_string<int32_t>::iterator &it, basic_string<int32_t>::iterator limit)
		{{{
			if(ntohl(*it) != 1)
				return false;
			it++;

			if(ntohl(*it) != learning_algorithm<answer>::ALG_BIERMANN)
				return false;
			it++;

			return true;
		}}}

		virtual void print(ostream &os)
		{{{
			typename set<knowledgebase_node_ptr>::iterator si;
			typename list<constraint>::iterator ci;

			os << "mapping sources {\n";
			for(si = sources.begin(); si != sources.end(); si++) {
				list<int> word;
				string s;
				word = (*si)->get_word();
				s = word2string(word, '.');
				os << "\t" << s << "\n";
			}
			os << "}\n";

			os << "constraints {\n";
			for(ci = constraints.begin(); ci != constraints.end(); ci++) {
				os << "\t";
				ci->print(os);
				os << "\n";
			}
			os << "}\n";
		}}}
		virtual string tostring()
		// FIXME: print constraints etc from last run
		{
			string ret;
			ret = "bla bla";
			return ret;
		}

		// conjecture is always ready if there is a non-empty knowledgebase
		virtual bool conjecture_ready()
		{{{
			return ( (this->my_knowledge != NULL) && (this->my_knowledge->count_answers() > 0) );
		}}}

		// stubs for counterexamples will throw a warning to the logger
		virtual void add_counterexample(list<int>, answer)
		{{{
			(*this->my_logger)(LOGGER_ERROR, "algorithm_biermann does not support counter-examples, as it is an offline-algorithm. please add the counter-example directly to the knowledgebase and rerun the algorithm.\n");
		}}}
		virtual void add_counterexample(list<int>)
		{{{
			(*this->my_logger)(LOGGER_ERROR, "algorithm_biermann does not support counter-examples, as it is an offline-algorithm. please add the counter-example directly to the knowledgebase and rerun the algorithm.\n");
		}}}

	protected:
		virtual bool complete()
		{{{
			// we're offline.
			return true;
		}}}
		// derive an automaton from data structure
		virtual bool derive_automaton(finite_language_automaton * automaton)
		{{{
			mapping old_solution;

			if(this->my_knowledge == NULL) {
				(*this->my_logger)(LOGGER_ERROR, "biermann: no knowledgebase set!\n");
				return false;
			}

			// 1) create constraints from LoopFreeDFA (LFDFA = knowledgebase)
			create_constraints();

			// 2) find CSP solution
			bool solved = false;
			bool failed_before = false;
			bool success_before = false;
			// pick an initial mDFA size [FIXME]
			mdfa_size = (int)sqrtf((float)this->my_knowledge->count_nodes());
			if(mdfa_size < 1)
				mdfa_size = 1;
			// FIXME: use binary search instead
			while(!solved) {
				(*this->my_logger)(LOGGER_INFO, "biermann: trying to solve CSP with %d states.\n", mdfa_size);
				old_solution = solution;

				if( solve_constraints() ) {
					(*this->my_logger)(LOGGER_INFO, "biermann: satisfiable.\n");
					if(failed_before) {
						// we found the minimal solution
						solved = true;
					} else {
						success_before = true;
						if(mdfa_size == 1)
							solved = true;
						else
							mdfa_size--;
					}
				} else {
					(*this->my_logger)(LOGGER_INFO, "biermann: unsatisfiable.\n");
					if(success_before) {
						// we found the minimal solution in the iteration before.
						solution = old_solution;
						solved = true;
					} else {
						failed_before = true;
						if(mdfa_size == (int)sources.size()) {
							(*this->my_logger)(LOGGER_ERROR, "biermann: failed to find mapping with LFDFA size == mDFA size. this will be a serious bug in libalf :-(\n");
							return false;
						} else {
							mdfa_size++;
						}
					}
				}
			}

			// 3) derive automaton from current_solution and mdfa_size
			return create_automaton(automaton);
		}}}

		virtual void create_constraints()
		{{{
			constraint constraint;
			int ccount = 0;
			typename knowledgebase<answer>::iterator ki1, ki2;
			typename knowledgebase<answer>::node *suffix1, *suffix2;

			sources.clear();
			constraints.clear();

			for(ki1 = this->my_knowledge->begin(); ki1 != this->my_knowledge->end(); ki1++) {
				ki2 = ki1;
				++ki2;
				while(ki2 != this->my_knowledge->end()) {
					// C1: O(u) != O(u') => S_u != S_u'
					if(ki1->is_answered() && ki2->is_answered()) {
						if(ki1->different(ki2->get_selfptr())) {
							constraint.l1 = ki1->get_selfptr();
							constraint.l2 = ki2->get_selfptr();
							constraint.has_second = false;

							sources.insert(ki1->get_selfptr());
							sources.insert(ki2->get_selfptr());
							constraints.push_back(constraint);
							ccount++;
						}
					}

					// C2: a \in \Sigma,
					//     ua \in L(knowledgebase),
					//     u'a \in L(knowledgebase)
					//     => ( S_u != S_u' ) \or ( S_{ua} = S_{u'a} )
					for(int s = 0; s < this->get_alphabet_size(); s++) {
						// FIXME: if normalizer, normalize suffixes and
						// find corresponding node!

						suffix1 = ki1->find_child(s);
						if(suffix1 == NULL)
							continue;

						suffix2 = ki2->find_child(s);
						if(suffix2 == NULL)
							continue;

						constraint.l1 = ki1->get_selfptr();
						constraint.l2 = ki2->get_selfptr();
						constraint.has_second = true;
						constraint.l3 = suffix1->get_selfptr();
						constraint.l4 = suffix2->get_selfptr();

						sources.insert(ki1->get_selfptr());
						sources.insert(ki2->get_selfptr());
						sources.insert(suffix1->get_selfptr());
						sources.insert(suffix2->get_selfptr());
						constraints.push_back(constraint);
						ccount++;
					}

					// next pair.
					++ki2;
				}
			}
			(*this->my_logger)(LOGGER_ALGORITHM, "biermann: CSP has %d constraints.\n", ccount);
		}}}

		virtual bool solve_constraints() = 0;

		virtual bool create_automaton(finite_language_automaton * automaton)
		{{{
			list<int> start;
			list<int> final;
			list<transition> transitions;
			typename set<knowledgebase_node_ptr>::iterator si;

			// knowledgebase.begin() always starts at root node (epsilon)
			start.push_back( solution[ this->my_knowledge->begin()->get_selfptr() ] );

			for(si = sources.begin(); si != sources.end(); si++) {
list<int> w;
string s;
w = (*si)->get_word();
s = word2string(w, '.');
printf("source %s:\n", s.c_str());
					// acceptance-status
					if((*si)->get_answer() == true)
						final.push_back( solution[ (*si)->get_selfptr() ] );
					// transitions
					transition tr;
					tr.source = solution[ (*si)->get_selfptr() ];
					for(int s = 0; s < this->get_alphabet_size(); s++) {
						knowledgebase_node_ptr child;
						child = (*si)->find_child(s);
						if(child != NULL) {
							tr.label = s;
							tr.destination = solution[ child->get_selfptr() ];
							transitions.push_back(tr);
printf("transition %d -%d-> %d\n", tr.source, tr.label, tr.destination);
						}
else printf("transition %d -%d-|\n", tr.source, s);
					}
			}

			return automaton->construct(this->get_alphabet_size(), mdfa_size, start, final, transitions);
		}}}

};


// biermann using Dependency Directed Backtracking, as described in
// "Arlindo L. Oliveira and João P.M. Silva - Efficient Algorithms for the Inference of Minimum Size DFAs"
template <class answer>
class DDB_biermann : public basic_biermann<answer> {
	public:
		DDB_biermann(knowledgebase<answer> * base, logger * log, int alphabet_size)
		{{{
			this->set_alphabet_size(alphabet_size);
			this->set_logger(log);
			this->set_knowledge_source(NULL, base);
		}}}
		virtual ~DDB_biermann()
		{{{
			// nothing.
			return;
		}}}

	protected:
		virtual bool solve_constraints()
		{
			// FIXME
			
			return false;
		}
};

}; // end namespace libalf

#endif // __libalf_algorithm_biermann_h__

