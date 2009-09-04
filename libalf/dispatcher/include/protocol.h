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

#ifndef __libalf_protocol_h__
# define __libalf_protocol_h__

#define DISPATCHER_PROTOCOL_VERSION 1

#define DISPATCHER_DEFAULT_PORT 24940

enum client_command {
	CLCMD_REQ_CAPA = 0,
	CLCMD_REQ_VERSION = 1,

	CLCMD_DISCONNECT = 10,
	CLCMD_STARTTLS = 11,
	CLCMD_AUTH = 12,

	CLCMD_CREATE_OBJECT = 20,
	CLCMD_DELETE_OBJECT = 21,
	CLCMD_GET_OBJECTTYPE = 22,
	CLCMD_OBJECT_COMMAND = 23,

	CLCMD_HELLO_CARSTEN = 99,

	// reserved for extensions: >= 1000
};

inline const char* cmd2string(int cmd)
{{{
	if(cmd == 0)
		return "CLCMD_REQ_CAPA";
	if(cmd == 1)
		return "CLCMD_REQ_VERSION";
	if(cmd == 10)
		return "CLCMD_DISCONNECT";
	if(cmd == 11)
		return "CLCMD_STARTTLS";
	if(cmd == 12)
		return "CLCMD_AUTH";
	if(cmd == 20)
		return "CLCMD_CREATE_OBJECT";
	if(cmd == 21)
		return "CLCMD_DELETE_OBJECT";
	if(cmd == 22)
		return "CLCMD_GET_OBJECTTYPE";
	if(cmd == 23)
		return "CLCMD_OBJECT_COMMAND";
	if(cmd == 99)
		return "CLCMD_HELLO_CARSTEN";

	return "invalid";
}}}

enum object_type {
	OBJ_LOGGER = 0,
	OBJ_KNOWLEDGEBASE = 10,
	OBJ_KNOWLEDGEBASE_ITERATOR = 11,
	OBJ_ALGORITHM = 20,
	OBJ_NORMALIZER = 21,
};

enum logger_command {
	LOGGER_RECEIVE_AND_FLUSH = 0,
	LOGGER_SET_MIN_LOGLEVEL = 1,
	LOGGER_LOG_ALGORITHM = 2,
	LOGGER_NOT_LOG_ALGORITHM = 3,
};

enum knowledgebase_command {
	KNOWLEDGEBASE_SERIALIZE = 0,
	KNOWLEDGEBASE_DESERIALIZE = 1,
	KNOWLEDGEBASE_TO_DOTFILE = 2,
	KNOWLEDGEBASE_TO_STRING = 3,

	KNOWLEDGEBASE_IS_ANSWERED = 10,
	KNOWLEDGEBASE_IS_EMPTY = 11,
	KNOWLEDGEBASE_GET_ALPHABET_SIZE = 12,
	KNOWLEDGEBASE_COUNT_QUERIES = 13,
	KNOWLEDGEBASE_COUNT_ANSWERS = 14,
	KNOWLEDGEBASE_COUNT_RESOLVED_QUERIES = 15,
	KNOWLEDGEBASE_GET_MEMORY_USAGE = 16,

	KNOWLEDGEBASE_RESOLVE_QUERY = 20,
	KNOWLEDGEBASE_RESOLVE_OR_ADD_QUERY = 21,
	KNOWLEDGEBASE_ADD_KNOWLEDGE = 22,

	KNOWLEDGEBASE_GET_QUERY_TREE = 25,
	KNOWLEDGEBASE_MERGE_TREE = 26,

	// iterator:
	KNOWLEDGEBASE_BEGIN = 30,
	KNOWLEDGEBASE_END = 31,
	KNOWLEDGEBASE_QBEGIN = 32,
	KNOWLEDGEBASE_QEND = 33,

	KNOWLEDGEBASE_CLEAR = 40,
	KNOWLEDGEBASE_CLEAR_QUERIES = 41,

	KNOWLEDGEBASE_UNDO = 50,
};

enum knowledgebase_iterator_command {
	KITERATOR_ASSIGN = 0,
	KITERATOR_COMPARE = 1,

	KITERATOR_NEXT = 10,

	KITERATOR_GET_WORD = 20,
	KITERATOR_IS_ANSWERED = 21,
	KITERATOR_GET_ANSWER = 22,
	KITERATOR_ANSWER = 23,
};

enum algorithm_command {
	ALGORITHM_SERIALIZE = 0,
	ALGORITHM_DESERIALIZE = 1,
	ALGORITHM_TO_STRING = 2,

	ALGORITHM_ASSOCIATE_LOGGER = 10,
	ALGORITHM_REMOVE_LOGGER = 11,
	ALGORITHM_SET_KNOWLEDGE_SOURCE = 12,
	ALGORITHM_GET_KNOWLEDGE_SOURCE = 13,
	ALGORITHM_SET_NORMALIZER=14,
	ALGORITHM_GET_NORMALIZER=15,
	ALGORITHM_UNSET_NORMALIZER=16,

	ALGORITHM_GET_MEMORY_STATISTICS = 20,
	ALGORITHM_GET_TIMING_STATISTICS = 21,
	ALGORITHM_ENABLE_TIMING = 22,
	ALGORITHM_DISABLE_TIMING = 23,
	ALGORITHM_RESET_TIMING = 24,

	ALGORITHM_GET_ALPHABET_SIZE = 30,
	ALGORITHM_INCREASE_ALPHABET_SIZE = 31,

	ALGORITHM_CONJECTURE_READY = 40,
	ALGORITHM_ADVANCE = 41,
	ALGORITHM_ADD_COUNTEREXAMPLE = 42,

	ALGORITHM_SUPPORTS_SYNC = 50,
	ALGORITHM_SYNC_TO_KNOWLEDGEBASE = 51,
};

enum normalizer_command {
	NORMALIZER_SERIALIZE = 0,
	NORMALIZER_DESERIALIZE = 1,
	NORMALIZER_DESERIALIZE_EXTENSION = 2,

	NORMALIZER_GET_TYPE = 10,

	NORMALIZER_NORMALIZE_A_WORD_PNF = 20,
	NORMALIZER_NORMALIZE_A_WORD_SNF = 21,
};

#endif // __libalf_protocol_h__


