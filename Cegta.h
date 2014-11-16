// Cegta.h
// Copyright (c) 2014 Dmitry Rodionov
//
// This software may be modified and distributed under the terms
// of the MIT license.  See the LICENSE file for details.
//
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#ifndef CEGTA_H
#define CEGTA_H

/**
 * The epsilon value used for comparing floating point numbers via [not]ToBeLike().
 * One can change it to any other value.
 */
#define CEGTA_EPSILON 0.000000001

 /**
  * Since a single Cegta spec is just a C function, it's clear that we have to store
  * information about whether it failed or not *somewhere outside* this function
  * (i.e. in a global scope).
  *
  * Also, since Cegta is just a single-header-based thing and global variables are so
  * mainstream, I decided to modify |argv| array in order to store the whole test suit
  * resuls: it can be actualy modified by any spec so why not?
  *
  * Initially (during the first spec execution) we set argv[x] to EXIT_SUCCESS, then if some
  * spec fails we set argv[x] to EXIT_FAILURE.
  * Eventually the program starts executing main() function after completion of all Cegta
  * specs. But |argv| array is aleady modified so we just need to return argv[x].
  */
#define CEGTA_ARGV_TEST_RESULTS_INDEX 1

/**
 * Creates main() routine for the test suit.
 * (thus it should be called only once).
 */
#define CegtaRun() \
	int main(int argc, char const *argv[]) \
	{ \
		return (int)argv[CEGTA_ARGV_TEST_RESULTS_INDEX]; \
	}

/**
 * Defines a single spec.
 * [Spec is a plain C function marked as a constructor].
 */
#define SpecBegin(specname) \
	__attribute__((constructor)) \
	void Cegta_spec_ ##specname (int argc, char **argv, char **env) { \
		if ((int)argv[CEGTA_ARGV_TEST_RESULTS_INDEX] != EXIT_FAILURE) {\
			argv[CEGTA_ARGV_TEST_RESULTS_INDEX] = (char *)EXIT_SUCCESS; \
		} \
		const char *__context = NULL; \
		__block const char *__it = NULL; \
		__block int __spec_tests = 0, __spec_test_passed = 0;\
		fprintf(stdout, "Begin spec <%s>\n", #specname);
/**
 * Completes a single spec.
 *
 * Notifies a user about the number of passed tests and modifies |argv| array
 * in case there were some failing test in this spec.
 */
#define SpecEnd() \
		fprintf(stdout, "Done spec: %d of %d tests passed\n\n",\
			__spec_test_passed, __spec_tests); \
		if (argv[CEGTA_ARGV_TEST_RESULTS_INDEX] == EXIT_SUCCESS && \
			(__spec_tests - __spec_test_passed > 0)) \
		{ \
			argv[CEGTA_ARGV_TEST_RESULTS_INDEX] = (char *)EXIT_FAILURE; \
		}\
	}

#define describe(label, block) \
		__context = label; \
		block();

#define it(label, block) { \
			__it = label; \
			__block int __it_test_count = 0, __it_test_passed = 0; \
			block(); \
			if (__it_test_count == __it_test_passed) { \
				fprintf(stdout, "\t✓ %s %s\n", __context, __it); \
			} else { \
				fprintf(stdout, "\t𝙓 %s %s\n", __context, __it); \
			} \
			__spec_tests += __it_test_count; \
			__spec_test_passed += __it_test_passed; \
		}

// The names of these macros is the only thing that matter here
#define toBe(value) (value)
#define notToBe(value) (value)
#define toBeLike(value) (value)
#define notToBeLike(value) (value)

// Print an error if the given expectation was not fulfilled
#define CegtaExpectation(that, this, this_as_str, format_str, fulfillTo, fulfillNotTo) \
		++__it_test_count; \
		if ((strstr(this_as_str, "notTo") == this_as_str) && !fulfillNotTo) { \
			fprintf(stdout, "\t* [%s, L%d]\n\t|\texpected %s %s\n", \
				__FILE__, __LINE__, #that, this_as_str); \
		} else \
		if ((strstr(this_as_str, "to") == this_as_str) && !fulfillTo) { \
			fprintf(stdout, "\t* [%s, L%d]\n\t|\texpected %s %s -> "format_str \
				"\n\t|\t     got %s is("format_str")\n", \
					__FILE__, __LINE__, #that, this_as_str, this, #that, that); \
		} else { \
			++__it_test_passed; \
		}

#define expectInt(that, this) \
		CegtaExpectation(that, this, #this, "%d",  ((that) == (this)), ((that) != (this)))

#define expectDouble(that, this) \
		CegtaExpectation(that, this, #this, "%lf", ({ \
			int result; \
			if (strstr(#this, "Like") != NULL) { \
				result = (fabs((that)-(this)) <= CEGTA_EPSILON); \
			} else { \
				result = ((that) == (this)); \
			} \
			result; \
		}), ({ \
			int result; \
			if (strstr(#this, "Like") != NULL) { \
				result = (fabs((that)-(this)) > CEGTA_EPSILON); \
			} else { \
				result = ((that) != (this)); \
			} \
			result; \
		}))

#define expectString(that, this) \
		CegtaExpectation(that, this, #this, "%s", ({ \
			int result; \
			if (this == NULL || that == NULL) { \
				result = (that == this); \
			} else { \
				if (strstr(#this, "Like") != NULL) { \
					result = (strcasecmp(that, this) == 0); \
				} else { \
					result = (strcmp(that, this) == 0); \
				} \
			} \
			result; \
		}), ({ \
			int result; \
			if (this == NULL || that == NULL) { \
				result = (that != this); \
			} else {\
				if (strstr(#this, "Like") != NULL) { \
					result = (strcasecmp(that, this) != 0); \
				} else { \
					result = (strcmp(that, this) != 0); \
				} \
			} \
			result; \
		}))

#endif // #ifndef CEGTA_H
