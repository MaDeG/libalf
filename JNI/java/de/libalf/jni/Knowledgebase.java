package de.libalf.jni;

public class Knowledgebase extends LibALFObject {

	private final int ACCEPTANCE_TRUE = 2;
	private final int ACCEPTANCE_FALSE = 0;
	private final int ACCEPTANCE_UNKNOWN = 1;

	public enum Acceptance {
		ACCEPT, REJECT, UNKNOWN
	}

	public Knowledgebase() {
		this.pointer = init();
	}

	private native long init();

	public boolean is_answered() {
		return is_answered(this.pointer);
	}

	private native boolean is_answered(long pointer);

	public boolean is_empty() {
		return is_empty(this.pointer);
	}

	/**
	 * Returns all nodes not only the knowledge
	 * 
	 * @return
	 */
	public WordList getKnowledge() {
		return getKnowledge(this.pointer);
	}

	private native WordList getKnowledge(long pointer);

	private native boolean is_empty(long pointer);

	public int count_queries() {
		return count_queries(this.pointer);
	}

	private native int count_queries(long pointer);

	public WordList getQueries() {
		return getQueries(this.pointer);
	}

	private native WordList getQueries(long pointer);

	public Acceptance resolve_query(int[] word) {
		int acceptance = resolve_query(word, this.pointer);
		switch (acceptance) {
		case ACCEPTANCE_FALSE:
			return Acceptance.REJECT;
		case ACCEPTANCE_TRUE:
			return Acceptance.ACCEPT;
		case ACCEPTANCE_UNKNOWN:
			return Acceptance.UNKNOWN;
		default:
			System.err.println("Unknown return value '" + acceptance
					+ "' of native mathod resolve_query! Returning 'UNKNOWN'");
			return Acceptance.UNKNOWN;
		}
	}

	private native int resolve_query(int[] word, long pointer);

	public Acceptance resolve_or_add_query(int[] word) {
		int acceptanceOrExists = resolve_or_add_query(word, this.pointer);
		switch (acceptanceOrExists) {
		case ACCEPTANCE_FALSE:
			return Acceptance.REJECT;
		case ACCEPTANCE_TRUE:
			return Acceptance.ACCEPT;
		case ACCEPTANCE_UNKNOWN:
			return Acceptance.UNKNOWN;
		default:
			System.err
					.println("Unknown return value '"
							+ acceptanceOrExists
							+ "' of native mathod resolve_or_add_query! Returning 'UNKNOWN'");
			return Acceptance.UNKNOWN;
		}
	}

	private native int resolve_or_add_query(int[] word, long pointer);

	public boolean add_knowledge(int[] word, boolean acceptance) {
		return add_knowledge(word, acceptance, this.pointer);
	}

	private native boolean add_knowledge(int[] word, boolean acceptance,
			long pointer);

	public void clear() {
		clear(this.pointer);
	}

	private native void clear(long pointer);

	public void clear_queries() {
		clear_queries(this.pointer);
	}

	private native void clear_queries(long pointer);

	public boolean undo(int count) {
		return undo(count, this.pointer);
	}

	private native boolean undo(int count, long pointer);

	public int get_memory_usage() {
		return get_memory_usage(this.pointer);
	}

	private native int get_memory_usage(long pointer);

	public int get_timestamp() {
		return get_timestamp(this.pointer);
	}

	private native int get_timestamp(long pointer);

	public int count_answers() {
		return count_answers(this.pointer);
	}

	private native int count_answers(long pointer);

	public String generate_dotfile() {
		return generate_dotfile(this.pointer);
	}

	private native String generate_dotfile(long pointer);

	public int[] serialize() {
		return serialize(this.pointer);
	}

	private native int[] serialize(long pointer);

	public boolean deserialize(int[] serialization) {
		return deserialize(serialization, this.pointer);
	}

	private native boolean deserialize(int[] serialization, long pointer);

	public String toString() {
		return tostring(this.pointer);
	}

	private native String tostring(long pointer);

	// //////////////////////////////////////////////////////////
	// //////////////////////////////////////////////////////////

	public static void main(String[] args) {
		// System.loadLibrary("alf_jni");
		Knowledgebase base = new Knowledgebase();

		base.add_knowledge(new int[] {}, true);
		base.add_knowledge(new int[] { 1, 2 }, false);
		base.add_knowledge(new int[] { 2 }, true);

		System.out.println(base.getKnowledge());
		System.out.println(base.resolve_or_add_query(new int[] { 1, 2 }));
		System.out.println(base.getKnowledge());
	}

}
