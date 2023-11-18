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
	using t_u8 = unsigned char;
	using t_i32 = int;
	using t_u32 = unsigned int;

	enum class test_case_result : t_u8
	{
		SUCCESS = 0,
		WARN = 1,
		ABORT = 2,
	};

	struct test_check_failed_info
	{
		const char* failed_check = nullptr;
		const char* check_message = nullptr;
		t_i32 check_line = 0;
	};


	template<class T>
	class TestAdapter {};
}

// Any func with same signature will work for custom printer
// int print(char* buffer, int max_size)
#define TPRINT_FUNC(format, ...) \
int print(char* buffer, int max_size) { return sprintf_s(buffer, max_size, format, __VA_ARGS__); }

#include "ttest_internal.h"

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
