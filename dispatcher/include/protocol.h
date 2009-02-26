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

#define DISPATCHER_PROTOCOL_VERSION 0

#define DISPATCHER_DEFAULT_PORT 23005

enum client_message {
	CM_DISCONNECT = 0,
	CM_REQ_CAPA = 1,
	CM_REQ_VERSION = 2,
	CM_STARTTLS = 10,
	CM_REQ_SESSION = 20,
	CM_SES_SET_MODALITIES = 21,
	CM_SES_REQ_STATUS = 22,
	CM_SES_SET_STATUS = 23,
	CM_SES_REQ_KNOWLEDGE = 24,
	CM_SES_SET_KNOWLEDGE = 25,
	CM_SES_CONJECTURE = 26,
	CM_SES_ADVANCE = 27,
	CM_SES_ANSWER_QUERIES = 29,
	CM_SES_GIVE_COUNTEREXAMPLES = 30,
	CM_SES_GIVE_COUNTEREXAMPLES_AND_ANSWERS = 31,
	CM_SES_REQ_ALPHABET_SIZE = 32,
	CM_SES_SET_ALPHABET_SIZE = 33,
	CM_SES_INC_ALPHABET_SIZE = 34,
	CM_SES_REQ_STATS = 35,
	CM_SES_SET_STATS = 36,
	CM_SES_REQ_LOG = 37,
	CM_SES_LOG_TABLE = 100,
	CM_SES_NORMALIZE_WORD = 101
};

enum server_message {
	SM_ACK_DISCONNECT = 0,
	SM_ACK_CAPA = 1,
	SM_ACK_VERSION = 2,
	SM_ACK_STATTLS = 10,
	SM_ACK_SESSION = 20,
	SM_SES_ACK_MODALITIES = 21,
	SM_SES_ACK_REQ_STATUS = 22,
	SM_SES_ACK_SET_STATUS = 23,
	SM_SES_ACK_REQ_KNOWLEDGE = 24,
	SM_SES_ACK_SET_KNOWLEDGE = 25,
	SM_SES_ACK_CONJECTURE = 26,
	SM_SES_ACK_ADVANCE_AUTOMATON = 27,
	SM_SES_ACK_ADVANCE_QUERIES = 28,
	SM_SES_ACK_ANSWER_QUERIES = 29,
	SM_SES_ACK_COUNTEREXAMPLES = 30,
	SM_SES_ACK_COUNTEREXAMPLES_AND_ANSWERS = 31,
	SM_SES_ACK_ALPHABET_SIZE = 32,
	SM_SES_ACK_SET_ALPHABET_SIZE = 33,
	SM_SES_ACK_INC_ALPHABET_SIZE = 34,
	SM_SES_ACK_REQ_STATS = 35,
	SM_SES_ACK_SET_STATS = 36,
	SM_SES_ACK_LOG = 37,
	SM_SES_ACK_LOG_TABLE = 100,
	SM_SES_ACK_NORMALIZE_WORD = 101
};

enum capability_type {
	CAPA_STARTTLS = 0
};

enum modality_type {
	MODALITY_SET_NORMALIZER = 0,
	MODALITY_EXTEND_NORMALIZER = 1,
	MODALITY_SET_LOGLEVEL = 2,
	MODALITY_SET_LOG_ALGORITHM = 3
};

#endif // __libalf_protocol_h__


