package de.libalf.jni;

/**
 * <p>
 * Angluin's L* learning algorithm for regular languages. See
 * <ul>
 * <li>
 * <em>D. Angluin - Learning regular sets from queries and counterexamples</em></li>
 * </ul>
 * </P>
 * <p>
 * Angluin's L* algorithm is an <em>online learning algorithm</em>. The
 * following example illustrates how to use online learning algorithms in
 * LibALF:
 * 
 * <PRE>
 * public BasicAutomaton learn() {
 * 	// Prepare the knowledgebase and the algorithm
 * 	Knowledgebase knowledgebase = new Knowledgebase();
 * 	int alphabetSize = // Set some size
 * 	BufferedLogger logger = new BufferedLogger();
 * 	LearningAlgorithm algorithm = new AlgorithmAngluin(knowledgebase,
 * 			alphabetSize, logger);
 * 
 * 	BasicAutomaton learnedAutomaton = null;
 * 
 * 	do {
 * 		// Advance
 * 		BasicAutomaton conjecture = algorithm.advance();
 * 
 * 		// Process membership query
 * 		if (conjecture == null) {
 * 
 * 			// Process each query
 * 			for (int[] word : knowledgebase.getQueries()) {
 * 
 * 				boolean acceptance = // Perform a membership query here
 * 				knowledgebase.add_knowledge(word, acceptance);
 * 			}
 * 		}
 * 
 * 		// Process equivalence query
 * 		else {
 * 			int[] counterexample = // Perform an equivalence query here
 * 
 * 			// The conjecture is equivalent to the language to learn
 * 			if (counterexample == null) {
 * 				learnedAutomaton = conjecture;
 * 				break;
 * 			}
 * 
 * 			// Found a counter-example
 * 			else {
 * 				algorithm.add_counterexample(counterexample);
 * 			}
 * 		}
 * 
 * 	} while (learnedAutomaton == null);
 * 
 * 	return learnedAutomaton;
 * 
 * }
 * </PRE>
 * 
 * Note that the user needs to
 * <ul>
 * <li>adjust the <code>alphabetSize</code> and</li>
 * <li>provide a teacher for <em>membership</em> and <em>equivalence</em>
 * queries.</li>
 * </ul>
 * </p>
 * 
 * @author Daniel Neider (<a
 *         href="mailto:neider@automata.rwth-aachen.de">neider@automata.
 *         rwth-aachen.de</a>), Chair of Computer Science 7, RWTH Aachen
 *         University
 * @version 1.0
 * 
 */
public class AlgorithmAngluin extends JNIAlgorithm {
	private static final long serialVersionUID = 1L;

	/**
	 * Creates a new object implementing Angluin's learning algorithm.
	 * 
	 * @param knowledgebase
	 *            the source of information for the algorithm.
	 * @param alphabet_size
	 *            the size of the used alphabet
	 */
	public AlgorithmAngluin(Knowledgebase knowledgebase, int alphabet_size) {
		this.knowledgebase = knowledgebase;
		this.pointer = init(knowledgebase.getPointer(), alphabet_size);
	}

	/**
	 * <p>
	 * <em>JNI method call:</em>
	 * </p>
	 * Invokes the JNI interface to initialize a new C++
	 * <code>angluin_simple_observationtable</code> object with the pointer to a
	 * <code>knowledgebase</code> and the size of the alphabet. The pointer to
	 * the new created C++ object is returned.
	 * 
	 * @param knowledgebase_pointer
	 *            a pointer to a knowledgebase C++ object
	 * @param alphabet_size
	 *            the size of the used alphabet
	 * @return a pointer to the memory location of the new C++ object.
	 */
	@Override
	native long init(long knowledgebase_pointer, int alphabet_size);

	/**
	 * Creates a new object implementing Angluin's learning algorithm.
	 * 
	 * @param knowledgebase
	 *            the source of information for the algorithm.
	 * @param alphabet_size
	 *            the size of the used alphabet
	 * @param logger
	 *            a logger
	 */
	public AlgorithmAngluin(Knowledgebase knowledgebase, int alphabet_size,
			BufferedLogger logger) {
		this.knowledgebase = knowledgebase;
		this.logger = logger;
		this.pointer = init(knowledgebase.getPointer(), alphabet_size, logger
				.getPointer());
	}

	/**
	 * <p>
	 * <em>JNI method call:</em>
	 * </p>
	 * Invokes the JNI interface to initialize a new C++
	 * <code>angluin_simple_observationtable</code> object with the pointer to a
	 * <code>knowledgebase</code>, the size of the alphabet and a pointer to a
	 * <code>buffered_logger</code>. The pointer to the new created C++ object
	 * is returned.
	 * 
	 * @param knowledgebase_pointer
	 *            a pointer to a knowledgebase C++ object
	 * @param alphabet_size
	 *            the size of the used alphabet
	 * @param logger_pointer
	 *            a pointer to a buffered_logger C++ object
	 * @return a pointer to the memory location of the new C++ object.
	 */
	@Override
	native long init(long knowledgebase_pointer, int alphabet_size,
			long logger_pointer);
}
