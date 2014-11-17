Cegta
=====

A tiny DSL for TDD/BDD written in C.

> `Cegta`'s based on [C blocks][0], so I'm not sure it will work on anything except for OS X and iOS.


### Why?
`Cegta` is just a PoC: one day I saw [Specta](https://github.com/specta/specta) and wondered if I could use the same DSL for testing my projects written in plain C. As it turns out, I can.

### What's BDD, BTW?
Check out the [Wikipedia article](http://en.wikipedia.org/wiki/Behavior-driven_development) (yes, I'm *that* rude), discover [RSpec](http://tutorials.jumpstartlab.com/topics/internal_testing/rspec_and_bdd.html) or [Specta](https://github.com/specta/specta) iteself.

### What it looks like?
Well, consider the snippet below:

```c
#include "Cegta.h"

CegtaRun();

SpecBegin(CegtaDemo)

	describe("Cegta's expect()", ^{
		beforeEach(^(const char *current_it) {
			// fprintf(stdout, "Prepare the environment for <%s>\n", current_it);
		});

		afterEach(^(const char *current_it) {
			// fprintf(stdout, "Clean up the environment for <%s>\n", current_it);
		});

		it("should work well with integers and floating point numbers", ^{
			int demo_int = 42;
			expectInt(demo_int, toBe(42));
			// ...Like() for integers does exactly the same thing
			expectInt(demo_int, toBeLike(42));
			expectInt(demo_int, notToBe(90));
			expectInt(demo_int, notToBeLike(90));

			double demo_double = 38.121;
			expectDouble(demo_double, toBe(38.121));
			// ...Like() for floating point numbers is verifying
			// whether fabs(a-b) is less or equal to EPS
			expectDouble(demo_double, toBeLike(demo_double + CEGTA_EPSILON));
			expectDouble(demo_double, toBeLike(demo_double - CEGTA_EPSILON));
			expectDouble(demo_double, notToBeLike(demo_double + CEGTA_EPSILON*2));
			expectDouble(demo_double, notToBe(0.444444));
		});
	});

	describe("Cegta's expect()", ^{
		it("should work well with strings (char*) as well", ^{
			char* demo_str = "i am a demo string";

			// if requreX() fails, Cegta skips all of the tests below
			requireString(demo_str, notToBe(NULL));
			// toBe() for strings is a strcmp()-based comparison
			expectString(demo_str, toBe("i am a demo string"));
			// ...Like() for strings is a case insensitive comparison
			expectString(demo_str, toBeLike("i Am a DEmO strING"));
			expectString(demo_str, notToBeLike("another string"));
			// NULLs are also welcome
			expectString(NULL, toBe(NULL));
			expectString(NULL, notToBe(demo_str));
		});
	});

SpecEnd()
```

Sample output from the spec above:

```
Begin spec <CegtaDemo>
	✓ Cegta's expect() should work well with integers and floating point numbers
	✓ Cegta's expect() should work well with strings (char*) as well
Done spec: 15 of 15 tests passed
```

and from a failing spec:

```
Begin spec <FailingCegtaDemo>
	* [demo.c, L49]
	|	expected 1 toBe(9) -> 9
	|	     got 1 is(1)
	𝙓 This one may actually fail
Done spec: 0 of 1 tests passed
```

### Usage
Check out [`demo.c`](./demo/demo.c) file: it contains a demo test suit. To build&run it:

```bash
$ cd ./demo
$ rake
```

> NOTE: you don't need to write `main()` routine in your tests: just replace it with a single `CegtaRun();` line.

---------

If you found any bug(s) or something, please open an issue or a pull request — I'd appreciate your help! (^,,^)

Dmitry Rodionov, 2014
i.am.rodionovd@gmail.com


   [0]: http://en.wikipedia.org/wiki/Blocks_(C_language_extension)
