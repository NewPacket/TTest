#pragma once
//SHOULD NOT BE INCLUDED DIRECTLY
//INCLUDE only "ttest.h"

#define __T_IS_DEFINED(macro) (#macro[0] == '1' && #macro[1] == 0)
#define _T_IS_DEFINED(macro) __T_IS_DEFINED(macro)

#ifndef TTEST_SUPPRESS_DEBUG_BREAK
#define _TDEBUGGER_BREAK() __debugbreak()
#else
#define _TDEBUGGER_BREAK()() 
#endif

namespace ttest::internal {

	inline constexpr bool report_all_results = _T_IS_DEFINED(ttest_REPORT_ALL);
	 
	inline constexpr const char* no_message = "no message";
}

#define TDECLARE_TEST_SUITE_INTERNAL(x) \
namespace ttest::suits { \
namespace test_##x::internal { \
static test_suite suite(#x); \
int register_test(test_case case_to_add) { return suite.register_test(case_to_add); } \
} \
namespace test_##x::run { \
test_suite_results get_cases() { return ttest::suits::test_##x::internal::suite.run_get_cases(); } \
void report_all() { ttest::suits::test_##x::internal::suite.run_report_all(); } \
void report_fails() { ttest::suits::test_##x::internal::suite.run_report_fails(); } \
void report_level(test_case_result level_to_report) { ttest::suits::test_##x::internal::suite.run_report_level(level_to_report);} \
} \
namespace test_##x { \
void reset() { internal::suite.reset(); } \
} } \

#define TDECLARE_CASE_IN_SUITE_INTERNAL(x, y) \
namespace ttest::suits { \
namespace test_##y::internal { \
void test_##x(test_case& t_case); \
static const char* test_case_##x = #x; \
int val_##x = suite.register_test(test_case(test_##x, test_case_##x, __FILE__, __LINE__)); \
}} \
void ttest::suits::test_##y::internal::test_##x(test_case& t_case)

#define __TEST_INTERNAL_PRE_PART(x, y, op)  \
{ \
	auto&& x_val = x; \
	auto&& y_val = y; \
	bool result = x_val ##op y_val; \
	constexpr int result_print_size = 128; \
	static char result_print[result_print_size]; \
	if (!result || ttest::internal::report_all_results) \
	{ \

#define __TEST_INTERNAL_POST_PART(xy, result_type) \
} \
ttest::internal::test_check(result, xy, result_print, t_case, result_type, __LINE__); \
if constexpr (result_type == ttest::test_case_result::ABORT) { if (!result) { return; } } }

#define __TCHECK_OP_FORMAT_RESULT_TYPE(x, y, op, xy, format, result_type) \
__TEST_INTERNAL_PRE_PART(x, y, op) \
		sprintf_s(result_print, result_print_size, format, x_val, y_val); \
__TEST_INTERNAL_POST_PART(xy, result_type)

#define __TCHECK_OP_FORMAT_CUSTOM_PRINT_RESULT_TYPE(x, y, op, xy, format, custom_print, result_type) \
__TEST_INTERNAL_PRE_PART(x, y, op) \
		int used = sprintf_s(result_print, result_print_size, format, x_val, y_val); \
		if (result_print_size > used) {	custom_print(result_print + used, result_print_size - used); } \
__TEST_INTERNAL_POST_PART(xy, result_type)

#define __TCHECK_OP_FORMAT_CUSTOM_PRINT_BOTH_RESULT_TYPE(x, y, op, xy, format, custom_print_a, custom_print_b, result_type) \
__TEST_INTERNAL_PRE_PART(x, y, op) \
		int print_space_left = result_print_size; \
		int used = sprintf_s(result_print, result_print_size, format, x_val, y_val); \
		if (print_space_left > used) {	used += custom_print_a(result_print + used, print_space_left - used); } \
		if (print_space_left > used) {	custom_print_b(result_print + used, print_space_left - used); } \
__TEST_INTERNAL_POST_PART(xy, result_type)

#define __TCHECK_OP_CUSTOM_PRINT_BOTH_RESULT_TYPE(x, y, op, xy, custom_print_a, custom_print_b, result_type) \
__TEST_INTERNAL_PRE_PART(x, y, op) \
		int print_space_left = result_print_size; \
		int used = custom_print_a(result_print, print_space_left); \
		if (print_space_left > used) { custom_print_b(result_print + used, print_space_left - used); } \
__TEST_INTERNAL_POST_PART(xy, result_type)

#define __TTO_STRING(x) #x
#define __TTO_STRING_M(x) __TTO_STRING(x)

#define __TCONCAT(x,y) x##y
#define __TCONCAT_M(x,y) __TCONCAT(x,y)

#define __TCONCAT_WITH_COMMA(x,y) x,##y
#define __TCONCAT_WITH_COMMA_M(x,y) __TCONCAT_WITH_COMMA(x,y)

#define TCHECK_OP_FORMAT_CUSTOM_PRINT_BOTH(x, y, op, format, print_a, print_b, result_type) __TCHECK_OP_FORMAT_CUSTOM_PRINT_BOTH_RESULT_TYPE(x, y, op, __TTO_STRING_M(__TCONCAT_M(__TCONCAT_M(x,##op), y)), format, print_a, print_b, result_type)
#define TCHECK_OP_FORMAT_CUSTOM_PRINT(x, y, op, format, print, result_type) __TCHECK_OP_FORMAT_CUSTOM_PRINT_RESULT_TYPE(x, y, op, __TTO_STRING_M(__TCONCAT_M(__TCONCAT_M(x,##op), y)), format, print, result_type)
#define TCHECK_OP_FORMAT(x, y, op, format, result_type) __TCHECK_OP_FORMAT_RESULT_TYPE(x, y, op, __TTO_STRING_M(__TCONCAT_M(__TCONCAT_M(x,##op), y)), format, result_type)
#define TCHECK_OP(x, y, op, fx, fy, result_type) TCHECK_OP_FORMAT(x, y, op, __TTO_STRING_M(__TCONCAT_M(__TCONCAT_M(__TCONCAT_M(__TCONCAT_M(x,##op), fx), \n),__TCONCAT_M(__TCONCAT_M(y,=), fy))), result_type)
#define TCHECK_OP_CUSTOM_PRINT_BOTH(x, y, op, print_a, print_b, result_type) __TCHECK_OP_CUSTOM_PRINT_BOTH_RESULT_TYPE(x, y, op, __TTO_STRING_M(__TCONCAT_M(__TCONCAT_M(x, ##op), y)), print_a, print_b, result_type)

#define TASSERT_OP(x, y, op, fx, fy) TCHECK_OP(x, y, op, fx, fy, ttest::test_case_result::ABORT)
#define TASSERT_OP_FORMAT(x, y, op, format) TCHECK_OP_FORMAT(x,y, op, format, ttest::test_case_result::ABORT)
#define TASSERT_OP_FORMAT_CUSTOM(x, y, op, format, print) TCHECK_OP_FORMAT_CUSTOM_PRINT(x, y, op, format, print, ttest::test_case_result::ABORT)
#define TASSERT_OP_FORMAT_CUSTOM_BOTH(x, y, op, format, print_a, print_b) TCHECK_OP_FORMAT_CUSTOM_PRINT_BOTH(x, y, op, format, print_a, print_b, ttest::test_case_result::ABORT)
#define TASSERT_OP_CUSTOM_BOTH(x, y, op, print_a, print_b) TCHECK_OP_CUSTOM_PRINT_BOTH(x, y, op, print_a, print_b, ttest::test_case_result::ABORT)

#define TWARN_OP(x, y, op, fx, fy) TCHECK_OP(x, y, op, fx, fy, ttest::test_case_result::WARN)
#define TWARN_OP_FORMAT(x, y, op, format) TCHECK_OP_FORMAT(x, y, op, format, ttest::test_case_result::WARN)
#define TWARN_OP_FORMAT_CUSTOM(x, y, op, format, print) TCHECK_OP_FORMAT_CUSTOM_PRINT(x, y, op, format, print, ttest::test_case_result::WARN)
#define TWARN_OP_FORMAT_CUSTOM_BOTH(x, y, op, format, print_a, print_b) TCHECK_OP_FORMAT_CUSTOM_PRINT_BOTH(x, y, op, format, print_a, print_b, ttest::test_case_result::WARN)
#define TWARN_OP_CUSTOM_BOTH(x, y, op, print_a, print_b) TCHECK_OP_CUSTOM_PRINT_BOTH(x, y, op, print_a, print_b, ttest::test_case_result::WARN)

namespace ttest
{
	class test_case;
	
	namespace internal
	{
		bool test_check(bool predicate, const char* failed_check, const char* message, test_case& t_case, test_case_result result_on_fail, t_i32 line);
	}

	struct test_suite_results
	{
		const test_case* cases;
		t_u32 cases_count;
	};

	class test_case
	{
		constexpr static size_t MAX_CHECKS = 32;
		using test_fn = void (*)(test_case& t_case);
		friend class test_suite;
		friend bool ttest::internal::test_check(bool predicate, const char* failed_check, const char* message, test_case& t_case, test_case_result result_on_fail, t_i32 line);
	public:

		void run() { func(*this); };

		explicit test_case() = default;

		explicit test_case(test_fn func_, const char* test_case_name, const char* file_name, int line) :
			func(func_),
			test_case_name(test_case_name),
			file_name(file_name),
			line(line)
		{};

		void report();
		test_case_result GetResult() const { return result; };
		const char* GetResultString() const
		{
			static const char* results[3] =
			{
				"SUCCESS", "WARN", "ABORT"
			};
			return results[(t_u8)GetResult()];
		}
	private:

		void report_failed_check(test_check_failed_info& info);

		test_fn func = nullptr;
		const char* test_case_name = nullptr;
		const char* file_name = nullptr;
		t_i32 line = 0;

		test_case_result result = test_case_result::SUCCESS;
		t_u32 checks_runned = 0;
		t_u32 checks_failed = 0;
		test_check_failed_info failed_checks[MAX_CHECKS];
	};

	void test_case::report()
	{
		printf("Test: %s on line %d, result: %s\n", test_case_name, line, GetResultString());
		if (checks_failed || internal::report_all_results)
		{
			printf("Checks runned: %d, failed: %d \n", checks_runned, checks_failed);
			printf("Failed checks:\n");
			for (t_u32 i = 0; i < checks_failed; ++i)
			{
				report_failed_check(failed_checks[i]);
			}
		}
		printf("\n");
	}

	void test_case::report_failed_check(test_check_failed_info& info)
	{
		printf("Check: %s, on line %d: with message:\n%s\n", info.failed_check, info.check_line, info.check_message);
	}

	class test_suite
	{

	public:

		constexpr static size_t MAX_CASES = 64;

		test_suite(const char* test_suite_name) : test_suite_name(test_suite_name) {};


		void reset() { suite_run = false; };
		test_suite_results run_get_cases();

		void run_report_all();
		void run_report_fails();
		void run_report_level(test_case_result level_to_report);

		t_i32 register_test(test_case case_to_add);

	protected:

		void print_name() { printf("Test suite: %s\n", test_suite_name); };

		const char* test_suite_name = nullptr;
		t_u32 cases_registered = 0;
		bool suite_run = false;
		test_case cases[MAX_CASES];

	};

	test_suite_results test_suite::run_get_cases()
	{
		if (!suite_run)
		{
			for (t_u32 i = 0; i < MAX_CASES && i < cases_registered; ++i)
			{
				cases[i].run();
			}
			suite_run = true;
		}
		return { cases, cases_registered };
	}

	void test_suite::run_report_all()
	{
		suite_run ? (void)0 : print_name();
		for (t_u32 i = 0; i < MAX_CASES && i < cases_registered; ++i)
		{
			suite_run ? (void)0 : cases[i].run();
			cases[i].report();
		}
		suite_run = true;
	}

	void test_suite::run_report_fails()
	{
		suite_run ? (void)0 : print_name();
		for (t_u32 i = 0; i < MAX_CASES && i < cases_registered; ++i)
		{
			suite_run ? (void)0 : cases[i].run();
			if (cases[i].GetResult() != test_case_result::SUCCESS)
			{
				cases[i].report();
			}
		}
		suite_run = true;
	}

	void test_suite::run_report_level(test_case_result level_to_report)
	{
		suite_run ? (void)0 : print_name();
		for (t_u32 i = 0; i < MAX_CASES && i < cases_registered; ++i)
		{
			suite_run ? (void)0 : cases[i].run();
			if (cases[i].GetResult() == level_to_report)
			{
				cases[i].report();
			}
		}
		suite_run = true;
	}

	int test_suite::register_test(test_case case_to_add)
	{
		if (cases_registered >= MAX_CASES) { return -1; }

		cases[cases_registered++] = case_to_add;
		return 0;
	}
}

namespace ttest::internal 
{
	bool test_check(bool predicate, const char* failed_check, const char* message, test_case& t_case, test_case_result result_on_fail, t_i32 line)
	{
		if (!predicate)
		{
			t_case.failed_checks[t_case.checks_failed++] = {
						failed_check,
						message,
						line };

			t_case.result = result_on_fail;
			if constexpr (!_T_IS_DEFINED(TTEST_NO_BREAKS))
			{
				_TDEBUGGER_BREAK();
			}
		}
		t_case.checks_runned++;
		return predicate;
	}
}

// Examples of usage

//#define TTEST_REPORT_ALL 1
//#define TTEST_NO_BREAKS 1
//#include "ttest.h"
//#define TEST_SUITE_NAME defines
//TDECLARE_TEST_SUITE(TEST_SUITE_NAME)
//#define ADD_TEST_CASE(x) TDECLARE_CASE_IN_SUITE(x, TEST_SUITE_NAME)
//
//int GetSelf(int x)
//{
//	return x;
//}
//
//ADD_TEST_CASE(Basic_calls)
//{
//	TWARN(true);
//	TASSERT(true);
//}
//
//ADD_TEST_CASE(Basic_calls_with_messages)
//{
//	int x = 2;
//	int y = 2;
//	TWARN_MESSAGE(x == y, "warn message")
//		TASSERT_MESSAGE(true, "assert message")
//}
//
//ADD_TEST_CASE(Calls_with_operation)
//{
//	int x = 1;
//	int y = 2;
//	TWARN_EQ(GetSelf(x) == 1, GetSelf(y) == 2, % d, % d);
//	TASSERT_EQ(2, 2, % d, % d);
//}
//
//ADD_TEST_CASE(Calls_with_operation_FORMAT_CUSTOM_format)
//{
//	int x = 1;
//	int y = 2;
//	TWARN_EQ_FORMAT(GetSelf(x) == 1, GetSelf(y) == 1, "GetSelf() %d %d"); // SHOULD WARN
//	TASSERT_EQ_FORMAT(GetSelf(2), GetSelf(2), "%d %d");
//}
//
//#undef TEST_SUITE_NAME
//#undef ADD_TEST_CASE
//
//#define TEST_SUITE_NAME class_tests
//TDECLARE_TEST_SUITE(TEST_SUITE_NAME)
//#define ADD_TEST_CASE(x) TDECLARE_CASE_IN_SUITE(x, TEST_SUITE_NAME)
//
//class TestClassA
//{
//public:
//
//	explicit TestClassA() = default;
//
//	explicit TestClassA(int int_v, float float_v) :
//		int_var(int_v),
//		float_var(float_v)
//	{};
//
//	int GetInt() const { return int_var; };
//	float GetFloat() const { return float_var; };
//
//protected:
//
//	int int_var = 0;
//	float float_var = 0.f;
//};
//
//class TestClassB
//{
//public:
//
//	explicit TestClassB() = default;
//
//	explicit TestClassB(bool bool_v, char char_v) :
//		bool_var(bool_v),
//		char_var(char_v)
//	{};
//
//	bool GetBool() const { return bool_var; };
//	char GetChar() const { return char_var; };
//
//protected:
//
//	bool bool_var = false;
//	char char_var = 0;
//};
//
//template<>
//class ttest::TestAdapter<TestClassA> : TestClassA
//{
//public:
//
//	using TestClassA::TestClassA;
//	using TestClassA::int_var;
//	using TestClassA::float_var;
//
//	TPRINT_FUNC("Class a: \n\tint_var: %d\n\tfloat_var: %f\n", int_var, float_var)
//};
//
//template<>
//class ttest::TestAdapter<TestClassB> : TestClassB
//{
//public:
//
//	using TestClassB::TestClassB;
//	using TestClassB::bool_var;
//	using TestClassB::char_var;
//
//	int print(char* buffer, int max_size)
//	{
//		return sprintf_s(buffer, max_size, "Class b: \n\tbool_var: %d\n\tchar_var: %c\n", bool_var, char_var);
//	};
//};
//
//
//ADD_TEST_CASE(TestClassA_tests)
//{
//	TestAdapter<TestClassA> a{ 1, 3.0 };
//	TASSERT_EQ(a.int_var, 1, % d, % d);
//	TASSERT_EQ_FORMAT_CUSTOM(a.float_var, 3.1, "%f %f\n", a.print)  // SHOULD ABORT
//}
//
//ADD_TEST_CASE(TestClassB_test)
//{
//	TestAdapter<TestClassB> b{ true, 3 };
//	TASSERT_EQ(b.bool_var, true, % d, % d);
//	TASSERT_EQ_FORMAT_CUSTOM(b.bool_var, (bool)1, "%d %d\n", b.print)
//
//}
//
//ADD_TEST_CASE(A_and_B_tests)
//{
//	TestAdapter<TestClassA> class_a{ 1, 3.0 };
//	int a_int = class_a.int_var;
//
//	TestAdapter<TestClassB> class_b{ true, 3 };
//	bool b_bool = class_b.bool_var;
//
//	TASSERT_EQ_FORMAT_CUSTOM_BOTH((bool)class_a.int_var, class_b.bool_var, "%d %d\n", class_a.print, class_b.print)
//
//}
//
//#undef TEST_SUITE_NAME
//#undef ADD_TEST_CASE
//
//
//#define TEST_SUITE_NAME class_operators
//TDECLARE_TEST_SUITE(TEST_SUITE_NAME)
//#define ADD_TEST_CASE(x) TDECLARE_CASE_IN_SUITE(x, TEST_SUITE_NAME)
//
//class TestClassC
//{
//public:
//
//	explicit TestClassC() = default;
//	explicit TestClassC(TestClassC& other) = default;
//
//	explicit TestClassC(int int_v_1, int int_v_2, float float_v) :
//		int_var_1(int_v_1),
//		int_var_2(int_v_2),
//		float_var(float_v)
//	{};
//
//	int GetIntFirst() const { return int_var_1; };
//	int GetIntSecond() const { return int_var_2; };
//	float GetFloat() const { return float_var; };
//
//	bool operator!=(TestClassC& other)
//	{
//		return int_var_1 != other.int_var_1 || int_var_2 != other.int_var_2 || float_var != other.float_var;
//	}
//
//	bool operator==(TestClassC& other)
//	{
//		return !(*this != other);
//	}
//
//	bool operator>(TestClassC& other)
//	{
//		return int_var_1 > other.int_var_1 && int_var_2 > other.int_var_2 && float_var > other.float_var;
//	}
//
//	bool operator>=(TestClassC& other)
//	{
//		return !(*this < other);
//	}
//
//	bool operator<(TestClassC& other)
//	{
//		return int_var_1 < other.int_var_1&& int_var_2 < other.int_var_2&& float_var < other.float_var;
//	}
//
//	bool operator<=(TestClassC& other)
//	{
//		return !(*this > other);
//	}
//
//protected:
//
//	int int_var_1 = 0;
//	int int_var_2 = 0;
//	float float_var = 0.f;
//
//};
//
//template<>
//class ttest::TestAdapter<TestClassC> : public TestClassC
//{
//public:
//
//	using TestClassC::TestClassC;
//	using TestClassC::int_var_1;
//	using TestClassC::int_var_2;
//	using TestClassC::float_var;
//
//	TPRINT_FUNC("Class C:\n\tint_var_1: %d\n\tint_var_2: %d\n\tfloat_var: %f\n", int_var_1, int_var_2, float_var)
//};
//using Test_TestClassC = ttest::TestAdapter<TestClassC>;
//
//ADD_TEST_CASE(creation_test)
//{
//	Test_TestClassC a{ 1, 2, 3.0f };
//	Test_TestClassC b{ 2, 3, 3.0f };
//	TASSERT_NEQ_CUSTOM_BOTH(a, b, a.print, b.print);
//}
//
//ADD_TEST_CASE(comparisons_with_literals_test)
//{
//	Test_TestClassC a{ 2, 2, 3.0f };
//	Test_TestClassC b{ 2, 2, 3.0f };
//	TASSERT_NEQ(a.int_var_1, 1, % d, % d);
//	TASSERT_EQ_CUSTOM_BOTH(a, b, a.print, b.print);
//}
//
//ADD_TEST_CASE(comparisons_test)
//{
//	Test_TestClassC a{ 1, 2, 3.0f };
//	Test_TestClassC b{ 2, 3, 4.0f };
//	Test_TestClassC c{ 2, 4, 4.0f };
//	Test_TestClassC d{ 1, 3, 3.0f };
//
//	TWARN_EQ_CUSTOM_BOTH(a, b, a.print, b.print); // SHOULD WARN
//	TWARN_NEQ_CUSTOM_BOTH(a, b, a.print, b.print);
//	TWARN_LEQ_CUSTOM_BOTH(a, b, a.print, b.print);
//	TWARN_GEQ_CUSTOM_BOTH(d, c, a.print, b.print); // SHOULD WARN
//	TWARN_GR_CUSTOM_BOTH(a, b, a.print, b.print); // SHOULD WARN
//	TWARN_LE_CUSTOM_BOTH(a, c, a.print, b.print);
//}
//
//ADD_TEST_CASE(custom_print_with_format)
//{
//	Test_TestClassC a{ 1, 2, 3.0f };
//	Test_TestClassC b{ 2, 3, 4.0f };
//	TASSERT_NEQ_FORMAT_CUSTOM_BOTH(a.int_var_1, b.int_var_2, "a: %d, b: %d", a.print, b.print);
//}
//
//#undef TEST_SUITE_NAME
//#undef ADD_TEST_CASE
//
//bool run_tests()
//{
//	ttest::suits::test_defines::run::report_all();
//	ttest::suits::test_class_tests::run::report_all();
//	ttest::suits::test_class_operators::run::report_all();
//	return true;
//}
