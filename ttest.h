#pragma once

// Ttest ver 1.0
// Author: Peshkov D.
// 
// Small, simple, easy to use and modify testing framework.
// Not designed to be any sort of GTest or doctest/catch2 competitor,
// just enougth to test home/personal projects.
// 
// Attempt to provide somewhat of a testing framework, without mental load of big code base,
// but keeping most usefull features.
// 
// Currenly have some minor faults in terms of memory efficiency and 
// dependant on Crt for memory buffer writes, which may or not, be solved in further(?) versions.
// Some places may be improved with more of C++(i.e. necessity to pass format strings in checks),
// although requires more time to look into.
//
// Maybe needs TEST_CASE type, that await warning, or abort result,
// however in current state looks more like a sugar.

// Fast start:
// 
// 1. Add
// #define TEST_SUITE_NAME %name%
// TDECLARE_TEST_SUITE(TEST_SUITE_NAME)
// #define ADD_TEST_CASE(x) TDECLARE_CASE_IN_SUITE(x, TEST_SUITE_NAME)
// 
// Register some cases, or none, with ADD_TEST_CASE macro
// 
// 2. At any already called function invoke any of run::* functions
// ttest::suits::test_%name%::run::report_all();

// currently stdio is required for proper work of framework, so just
// #include <stdio.h>

namespace ttest
{
	class test_case;

	using t_u8 = unsigned char;
	using t_i32 = int;
	using t_u32 = unsigned int;

	enum class test_case_result : t_u8
	{
		SUCCESS = 0,
		WARN = 1,
		ABORT = 2,
	};

	struct test_check_info
	{
		const char* check_text = nullptr;
		const char* check_message = nullptr;
		t_i32 check_line = 0;
	};

	struct test_suite_results
	{
		const test_case* cases;
		t_u32 cases_count;
		t_u32 cases_failed;
	};


	template<class T>
	class TestAdapter {};
}

// Any func with same signature will work for custom printer
// int print(char* buffer, int max_size)
#define TPRINT_FUNC(format, ...) \
int print(char* buffer, int max_size) { return snprintf(buffer, max_size, format, __VA_ARGS__); }

#include "ttest_internal.h"

#define TTEST_GET_CASES(suite_name) ttest::suits::test_##suite_name::run::get_cases();
#define TTEST_REPORT_ALL(suite_name) ttest::suits::test_##suite_name::run::report_all();
#define TTEST_REPORT_FAILS(suite_name) ttest::suits::test_##suite_name::run::report_all();
#define TTEST_REPORT_LEVEL(suite_name, report_level) ttest::suits::test_##suite_name::run::report_all();

#define TDECLARE_TEST_SUITE(x) TDECLARE_TEST_SUITE_INTERNAL(x) 
#define TDECLARE_CASE_IN_SUITE(x,y) TDECLARE_CASE_IN_SUITE_INTERNAL(x,y)

#define TWARN_MESSAGE(x,message) ttest::internal::test_check(x, #x, message, t_case, ttest::test_case_result::WARN, __LINE__);
#define TWARN(x) TWARN_MESSAGE(x, ttest::internal::no_message)

#define TWARN_EQ(x, y, format_x, format_y) TWARN_OP(x, y, ==, format_x, format_y)
#define TWARN_EQ_FORMAT(x, y, format) TWARN_OP_FORMAT(x, y, ==, format)
#define TWARN_EQ_FORMAT_CUSTOM(x, y, format, print) TWARN_OP_FORMAT_CUSTOM(x, y, ==, format, print)
#define TWARN_EQ_FORMAT_CUSTOM_BOTH(x, y, format, print_a, print_b) TWARN_OP_FORMAT_CUSTOM_BOTH(x, y, ==, format, print_a, print_b)
#define TWARN_EQ_CUSTOM_BOTH(x, y, print_a, print_b) TWARN_OP_CUSTOM_BOTH(x, y, ==, print_a, print_b)

#define TWARN_NEQ(x, y, format_x, format_y) TWARN_OP(x, y, !=, format_x, format_y)
#define TWARN_NEQ_FORMAT(x, y, format) TWARN_OP_FORMAT(x, y, !=, format)
#define TWARN_NEQ_FORMAT_CUSTOM(x, y, format, print) TWARN_OP_FORMAT_CUSTOM(x, y, !=, format, print)
#define TWARN_NEQ_FORMAT_CUSTOM_BOTH(x, y, format, print_a, print_b) TWARN_OP_FORMAT_CUSTOM_BOTH(x, y, !=, format, print_a, print_b)
#define TWARN_NEQ_CUSTOM_BOTH(x, y, print_a, print_b) TWARN_OP_CUSTOM_BOTH(x, y, !=, print_a, print_b)

#define TWARN_LEQ(x, y, format_x, format_y) TWARN_OP(x, y, <=, format_x, format_y)
#define TWARN_LEQ_FORMAT(x, y, format) TWARN_OP_FORMAT(x, y, <=, format)
#define TWARN_LEQ_FORMAT_CUSTOM(x, y, format, print) TWARN_OP_FORMAT_CUSTOM(x, y, <=, format, print)
#define TWARN_LEQ_FORMAT_CUSTOM_BOTH(x, y, format, print_a, print_b) TWARN_OP_FORMAT_CUSTOM_BOTH(x, y, <=, format, print_a, print_b)
#define TWARN_LEQ_CUSTOM_BOTH(x, y, print_a, print_b) TWARN_OP_CUSTOM_BOTH(x, y, <=, print_a, print_b)

#define TWARN_GEQ(x, y, format_x, format_y) TWARN_OP(x, y, >=, format_x, format_y)
#define TWARN_GEQ_FORMAT(x, y, format) TWARN_OP_FORMAT(x, y, >=, format)
#define TWARN_GEQ_FORMAT_CUSTOM(x, y, format, print) TWARN_OP_FORMAT_CUSTOM(x, y, >=, format, print)
#define TWARN_GEQ_FORMAT_CUSTOM_BOTH(x, y, format, print_a, print_b) TWARN_OP_FORMAT_CUSTOM_BOTH(x, y, >=, format, print_a, print_b)
#define TWARN_GEQ_CUSTOM_BOTH(x, y, print_a, print_b) TWARN_OP_CUSTOM_BOTH(x, y, >=, print_a, print_b)

#define TWARN_GR(x, y, format_x, format_y) TWARN_OP(x, y, >, format_x, format_y)
#define TWARN_GR_FORMAT(x, y, format) TWARN_OP_FORMAT(x, y, >, format)
#define TWARN_GR_FORMAT_CUSTOM(x, y, format, print) TWARN_OP_FORMAT_CUSTOM(x, y, >, format, print)
#define TWARN_GR_FORMAT_CUSTOM_BOTH(x, y, format, print_a, print_b) TWARN_OP_FORMAT_CUSTOM_BOTH(x, y, >, format, print_a, print_b)
#define TWARN_GR_CUSTOM_BOTH(x, y, print_a, print_b) TWARN_OP_CUSTOM_BOTH(x, y, >, print_a, print_b)
		 
#define TWARN_LE(x, y, format_x, format_y) TWARN_OP(x, y, <, format_x, format_y)
#define TWARN_LE_FORMAT(x, y, format) TWARN_OP_FORMAT(x, y, <, format)
#define TWARN_LE_FORMAT_CUSTOM(x, y, format, print) TWARN_OP_FORMAT_CUSTOM(x, y, <, format, print)
#define TWARN_LE_FORMAT_CUSTOM_BOTH(x, y, format, print_a, print_b) TWARN_OP_FORMAT_CUSTOM_BOTH(x, y, <, format, print_a, print_b)
#define TWARN_LE_CUSTOM_BOTH(x, y, print_a, print_b) TWARN_OP_CUSTOM_BOTH(x, y, <, print_a, print_b)

#define TASSERT_MESSAGE(x, message) if (!ttest::internal::test_check(x, #x, message, t_case, ttest::test_case_result::ABORT, __LINE__)) { return; }
#define TASSERT(x) TASSERT_MESSAGE(x, ttest::internal::no_message)

#define TASSERT_EQ(x, y, format_x, format_y) TASSERT_OP(x, y, ==, format_x, format_y)
#define TASSERT_EQ_FORMAT(x, y, format) TASSERT_OP_FORMAT(x, y, ==, format)
#define TASSERT_EQ_FORMAT_CUSTOM(x, y, format, print) TASSERT_OP_FORMAT_CUSTOM(x, y, ==, format, print)
#define TASSERT_EQ_FORMAT_CUSTOM_BOTH(x, y, format, print_a, print_b) TASSERT_OP_FORMAT_CUSTOM_BOTH(x, y, ==, format, print_a, print_b)
#define TASSERT_EQ_CUSTOM_BOTH(x, y, print_a, print_b) TASSERT_OP_CUSTOM_BOTH(x, y, ==, print_a, print_b)

#define TASSERT_NEQ(x, y, format_x, format_y) TASSERT_OP(x, y, !=, format_x, format_y)
#define TASSERT_NEQ_FORMAT(x, y, format) TASSERT_OP_FORMAT(x, y, !=, format)
#define TASSERT_NEQ_FORMAT_CUSTOM(x, y, format, print) TASSERT_OP_FORMAT_CUSTOM(x, y, !=, format, print)
#define TASSERT_NEQ_FORMAT_CUSTOM_BOTH(x, y, format, print_a, print_b) TASSERT_OP_FORMAT_CUSTOM_BOTH(x, y, !=, format, print_a, print_b)
#define TASSERT_NEQ_CUSTOM_BOTH(x, y, print_a, print_b) TASSERT_OP_CUSTOM_BOTH(x, y, !=, print_a, print_b)

#define TASSERT_GR(x, y, format_x, format_y) TASSERT_OP(x, y, >, format_x, format_y)
#define TASSERT_GR_FORMAT(x, y, format) TASSERT_OP_FORMAT(x, y, >, format)
#define TASSERT_GR_FORMAT_CUSTOM(x, y, format, print) TASSERT_OP_FORMAT_CUSTOM(x, y, >, format, print)
#define TASSERT_GR_FORMAT_CUSTOM_BOTH(x, y, format, print_a, print_b) TASSERT_OP_FORMAT_CUSTOM_BOTH(x, y, >, format, print_a, print_b)
#define TASSERT_GR_CUSTOM_BOTH(x, y, print_a, print_b) TASSERT_OP_CUSTOM_BOTH(x, y, >, print_a, print_b)

#define TASSERT_LE(x, y, format_x, format_y) TASSERT_OP(x, y, <, format_x, format_y)
#define TASSERT_LE_FORMAT(x, y, format) TASSERT_OP_FORMAT(x, y, <, format)
#define TASSERT_LE_FORMAT_CUSTOM(x, y, format, print) TASSERT_OP_FORMAT_CUSTOM(x, y, <, format, print)
#define TASSERT_LE_FORMAT_CUSTOM_BOTH(x, y, format, print_a, print_b) TASSERT_OP_FORMAT_CUSTOM_BOTH(x, y, <, format, print_a, print_b)
#define TASSERT_LE_CUSTOM_BOTH(x, y, print_a, print_b) TASSERT_OP_CUSTOM_BOTH(x, y, <, print_a, print_b)

#define TASSERT_LEQ(x, y, format_x, format_y) TASSERT_OP(x, y, <=, format_x, format_y)
#define TASSERT_LEQ_FORMAT(x, y, format) TASSERT_OP_FORMAT(x, y, <=, format)
#define TASSERT_LEQ_FORMAT_CUSTOM(x, y, format, print) TASSERT_OP_FORMAT_CUSTOM(x, y, <=, format, print)
#define TASSERT_LEQ_FORMAT_CUSTOM_BOTH(x, y, format, print_a, print_b) TASSERT_OP_FORMAT_CUSTOM_BOTH(x, y, <=, format, print_a, print_b)
#define TASSERT_LEQ_CUSTOM_BOTH(x, y, print_a, print_b) TASSERT_OP_CUSTOM_BOTH(x, y, <=, print_a, print_b)

#define TASSERT_GEQ(x, y, format_x, format_y) TASSERT_OP(x, y, >=, format_x, format_y)
#define TASSERT_GEQ_FORMAT(x, y, format) TASSERT_OP_FORMAT(x, y, >=, format)
#define TASSERT_GEQ_FORMAT_CUSTOM(x, y, format, print) TASSERT_OP_FORMAT_CUSTOM(x, y, >=, format, print)
#define TASSERT_GEQ_CUSTOM_BOTH(x, y, print_a, print_b) TASSERT_OP_CUSTOM_BOTH(x, y, >=, print_a, print_b)

// Some implementation details 

// Ttest have 3-level structure:
// - Test suite
// -- Test case
// --- Test check
//
// To start test suite add:
// 
// #define TEST_SUITE_NAME %name%
// TDECLARE_TEST_SUITE(TEST_SUITE_NAME)
// 
// place at the end of the test suite, or file if using unity build
// #undef TEST_SUITE_NAME
// 
// Added test suite will be accessable via namespace
// ttest::suits::test_%name% 
// 
// For easier addition of test cases its possible to use next macro
// 
// #define ADD_TEST_CASE(x) TDECLARE_CASE_IN_SUITE(x, TEST_SUITE_NAME)
// 
// Same as for the test suite
// #undef ADD_TEST_CASE
// 
// or you can just use DECLARE_TEST_IN_SUITE(%test_case_name%, TEST_SUITE_NAME)
// 
// Checks are separated in two categories:
// - WARN, save results and continue further
// - ASSERT, immidiately stop test case execution.

// Small behaviour customization possibile via additional defines
// - Show all results
// #define TTEST_REPORT_ALL 1
// 
// - No debugger break on failed checks
// #define TTEST_NO_BREAKS 1

// TestAdapter template class provided in case you want to
// bring out protected members, write test specific print and
// dont want to modify original class at all. 

// TPRINT_FUNC macro is the shortcut for buffer write function

// Examples of framework usage are present at the end of ttest_internal.h

//TBD:
// - store cases and checks in single monotonic buffer instead of static amount of cases/checks info 
//	 + no static sprintf_s buffers for all checks or template on needed size, but will still waste memory and code is uglier
// 
// - libfmt instead of printf and sprintf

// Examples of usage

//#include <stdio.h>
//#define TTEST_SHOW_ALL_CHECKS_RESULTS 0
//#define TTEST_NO_BREAKS 0
//#include "ttest.h"
//
//#define DEFINES_TESTS defines
//#define TEST_SUITE_NAME DEFINES_TESTS
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
//int main()
//{
//	int cases_failed_defines = TTEST_REPORT_ALL(defines);
//	int cases_failed_class = TTEST_REPORT_ALL(class_tests);
//	int cases_failed_class_operators = TTEST_REPORT_ALL(class_operators);
//	return TTEST_REPORT_LEVEL(defines, ttest::test_case_result::ABORT);
//}