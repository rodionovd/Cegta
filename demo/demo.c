#include "../Cegta.h"

CegtaRun();

SpecBegin(CegtaDemo)

	describe("Cegta's expect()", ^{
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
			// toBe() for strings is a strcmp()-based comparison
			expectString(demo_str, toBe("i am a demo string"));
			// ...Like() for strings is a case insensitive comparison
			expectString(demo_str, toBeLike("i Am a DEmO strING"));
			expectString(demo_str, notToBeLike("another string"));
			// NULLs are also welcome
			expectString(demo_str, notToBe(NULL));
			expectString(NULL, toBe(NULL));
			expectString(NULL, notToBe(demo_str));
		});
	});

SpecEnd()


SpecBegin(AnotherCegtaDemo)

	describe("This one", ^{
		it("may actually fail", ^{
			expectInt(1, toBe(9));
		});
	});

SpecEnd()
