#pragma once
//SHOULD NOT BE INCLUDED DIRECTLY
//INCLUDE only "ttest.h"

#define __T_IS_DEFINED(macro) (#macro[0] == '1' && #macro[1] == 0)
#define _T_IS_DEFINED(macro) __T_IS_DEFINED(macro)

#ifdef _MSC_VER
#define _TDEBUGGER_BREAK() __debugbreak()
#else
#define _TDEBUGGER_BREAK()
#endif

namespace ttest::internal {

	constexpr bool report_all_checks_results = _T_IS_DEFINED(TTEST_SHOW_ALL_CHECKS_RESULTS);
	constexpr int result_print_buffer_size = 256;
	constexpr const char* no_message = "no message";
}

#define TDECLARE_TEST_SUITE_INTERNAL(x) \
namespace ttest::suits { \
namespace test_##x::internal { \
test_suite suite(#x); \
int register_test(test_case case_to_add) { return suite.register_test(case_to_add); } \
} \
namespace test_##x::run { \
test_suite_results get_cases() { return ttest::suits::test_##x::internal::suite.run_get_cases(); } \
int report_all() { return ttest::suits::test_##x::internal::suite.run_report_all(); } \
int report_fails() { return ttest::suits::test_##x::internal::suite.run_report_fails(); } \
int report_level(test_case_result level_to_report) { return ttest::suits::test_##x::internal::suite.run_report_level(level_to_report);} \
} \
namespace test_##x { \
void reset() { internal::suite.reset(); } \
} } \

#define TDECLARE_CASE_IN_SUITE_INTERNAL(x, y) \
namespace ttest::suits { \
namespace test_##y::internal { \
void test_##x(test_case& t_case); \
const char* test_case_##x = #x; \
int val_##x = suite.register_test(test_case(test_##x, test_case_##x, __FILE__, __LINE__)); \
}} \
void ttest::suits::test_##y::internal::test_##x(test_case& t_case)

#define __TEST_INTERNAL_PRE_PART(x, y, op)  \
{ \
	auto&& x_val = x; \
	auto&& y_val = y; \
	bool result = x_val ##op y_val; \
	constexpr int print_buffer_size = ttest::internal::result_print_buffer_size  ; \
	static char result_print_buffer[print_buffer_size]; \
	if (!result || ttest::internal::report_all_checks_results) \
	{ \

#define __TEST_INTERNAL_POST_PART(check_text, result_type) \
} \
ttest::internal::test_check(result, check_text, result_print_buffer, t_case, result_type, __LINE__); \
if constexpr (result_type == ttest::test_case_result::ABORT) { if (!result) { return; } } }

#define __TCHECK_OP_FORMAT_RESULT_TYPE(x, y, op, check_text, format, result_type) \
__TEST_INTERNAL_PRE_PART(x, y, op) \
		snprintf(result_print_buffer, print_buffer_size, format, x_val, y_val); \
__TEST_INTERNAL_POST_PART(check_text, result_type)

#define __TCHECK_OP_FORMAT_CUSTOM_PRINT_RESULT_TYPE(x, y, op, check_text, format, custom_print, result_type) \
__TEST_INTERNAL_PRE_PART(x, y, op) \
		int used = snprintf(result_print_buffer, print_buffer_size, format, x_val, y_val); \
		if (print_buffer_size > used) {	custom_print(result_print_buffer + used, print_buffer_size - used); } \
__TEST_INTERNAL_POST_PART(check_text, result_type)

#define __TCHECK_OP_FORMAT_CUSTOM_PRINT_BOTH_RESULT_TYPE(x, y, op, check_text, format, custom_print_a, custom_print_b, result_type) \
__TEST_INTERNAL_PRE_PART(x, y, op) \
		int print_space_left = print_buffer_size; \
		int used = snprintf(result_print_buffer, print_space_left, format, x_val, y_val); \
		if (print_space_left > used) {	used += custom_print_a(result_print_buffer + used, print_space_left - used); } \
		if (print_space_left > used) {	custom_print_b(result_print_buffer + used, print_space_left - used); } \
__TEST_INTERNAL_POST_PART(check_text, result_type)

#define __TCHECK_OP_CUSTOM_PRINT_BOTH_RESULT_TYPE(x, y, op, check_text, custom_print_a, custom_print_b, result_type) \
__TEST_INTERNAL_PRE_PART(x, y, op) \
		int print_space_left = print_buffer_size; \
		int used = custom_print_a(result_print_buffer, print_space_left); \
		if (print_space_left > used) { custom_print_b(result_print_buffer + used, print_space_left - used); } \
__TEST_INTERNAL_POST_PART(check_text, result_type)

#define __TTO_STRING(x) #x
#define __TTO_STRING_M(x) __TTO_STRING(x)

#define __TCONCAT(x,y) x##y
#define __TCONCAT_M(x,y) __TCONCAT(x,y)

#define __TCONCAT_WITH_COMMA(x,y) x,##y
#define __TCONCAT_WITH_COMMA_M(x,y) __TCONCAT_WITH_COMMA(x,y)

#define __TT_CONCAT(x, y, op) #x " " #op " " #y
#define __TT_FORMAT(x, y, op, fx, fy) #x " " #op " " #fx "\n" #y " " #op " " #fy

#define TCHECK_OP_FORMAT_CUSTOM_PRINT_BOTH(x, y, op, format, print_a, print_b, result_type) __TCHECK_OP_FORMAT_CUSTOM_PRINT_BOTH_RESULT_TYPE(x, y, op, __TT_CONCAT(x, y, op), format, print_a, print_b, result_type)
#define TCHECK_OP_FORMAT_CUSTOM_PRINT(x, y, op, format, print, result_type) __TCHECK_OP_FORMAT_CUSTOM_PRINT_RESULT_TYPE(x, y, op, __TT_CONCAT(x, y, op) , format, print, result_type)
#define TCHECK_OP_FORMAT(x, y, op, format, result_type) __TCHECK_OP_FORMAT_RESULT_TYPE(x, y, op, __TT_CONCAT(x, y, op) ,format, result_type)
#define TCHECK_OP(x, y, op, fx, fy, result_type) TCHECK_OP_FORMAT(x, y, op, __TT_FORMAT(x, y, op, fx, fy), result_type)

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
		static bool test_check(bool predicate, const char* check_text, const char* message, test_case& t_case, test_case_result result_on_fail, t_i32 line);
	}

	class test_case
	{
		using test_fn = void (*)(test_case& t_case);

		constexpr static size_t MAX_CHECKS = 32;

		friend class test_suite;

		friend bool ttest::internal::test_check(bool predicate, const char* check_text, const char* message, test_case& t_case, test_case_result result_on_fail, t_i32 line);

	public:

		test_case_result run() { func(*this); return result; };

		explicit test_case() = default;

		explicit test_case(test_fn func_, const char* test_case_name, const char* file_name, int line) :
			func(func_),
			test_case_name(test_case_name),
			file_name(file_name),
			line(line)
		{};

		void report()
		{
			printf("Test: %s on line %d, result: %s\n", test_case_name, line, GetResultString());
			printf("Checks runned: %d, failed: %d \n", checks_runned, checks_failed);
			if constexpr (internal::report_all_checks_results)
			{
				for (t_u32 i = 0; i < checks_runned; ++i)
				{
					report_check(checks_info[i]);
				}
			}
			else
			{
				if (checks_failed)
				{

					printf("Failed checks:\n");
					for (t_u32 i = 0; i < checks_failed; ++i)
					{
						report_check(checks_info[i]);
					}
				}
			}
			printf("\n");
		}

		test_case_result GetResult() const { return result; };
		const char* GetResultString() const
		{
			constexpr const char* results[3] =
			{
				"SUCCESS", "WARN", "ABORT"
			};
			return results[(t_u8)GetResult()];
		}

	private:

		void report_check(test_check_info& info)
		{
			printf("Check: %s, on line %d: with message:\n%s\n", info.check_text, info.check_line, info.check_message);
		}

		test_fn func = nullptr;
		const char* test_case_name = nullptr;
		const char* file_name = nullptr;
		t_i32 line = 0;

		test_case_result result = test_case_result::SUCCESS;
		t_u32 checks_runned = 0;
		t_u32 checks_failed = 0;
		test_check_info checks_info[MAX_CHECKS];
	};

	class test_suite
	{

	public:

		constexpr static size_t MAX_CASES = 64;

		test_suite(const char* test_suite_name) : test_suite_name(test_suite_name) {};

		void reset() { suite_run = false; cases_failed = 0; };

		test_suite_results run_get_cases()
		{
			if (!suite_run)
			{
				for (t_u32 i = 0; i < MAX_CASES && i < cases_registered; ++i)
				{
					cases[i].run();
				}
				suite_run = true;
			}
			return test_suite_results{ cases, cases_registered, cases_failed };
		}

		t_u32 run_report_all()
		{
			print_name();
			if (suite_run)
			{
				for (t_u32 i = 0; i < MAX_CASES && i < cases_registered; ++i)
				{
					cases[i].report();
				}
			}
			else
			{

				for (t_u32 i = 0; i < MAX_CASES && i < cases_registered; ++i)
				{
					test_case_result result = cases[i].run();
					cases[i].report();
					check_increase_failed_count(result);
				}
				suite_run = true;
			}
			return cases_failed;
		}

		t_u32 run_report_fails()
		{
			print_name();
			if (suite_run)
			{
				for (t_u32 i = 0; i < MAX_CASES && i < cases_registered; ++i)
				{
					if (cases[i].GetResult() != test_case_result::SUCCESS)
					{
						cases[i].report();
					}
				}
			}
			else
			{

				for (t_u32 i = 0; i < MAX_CASES && i < cases_registered; ++i)
				{
					test_case_result result = cases[i].run();
					if (result != test_case_result::SUCCESS)
					{
						cases[i].report();
						check_increase_failed_count(result);
					}
				}
				suite_run = true;
			}

			return cases_failed;
		}

		t_u32 run_report_level(test_case_result level_to_report)
		{
			print_name();
			if (suite_run)
			{
				for (t_u32 i = 0; i < MAX_CASES && i < cases_registered; ++i)
				{
					if (cases[i].GetResult() == level_to_report)
					{
						cases[i].report();
					}
				}
			}
			else
			{
				for (t_u32 i = 0; i < MAX_CASES && i < cases_registered; ++i)
				{
					test_case_result result = cases[i].run();
					if (cases[i].GetResult() == level_to_report)
					{
						cases[i].report();
					}
					check_increase_failed_count(cases[i].GetResult());
				}
			}

			return cases_failed;
		}

		int register_test(test_case case_to_add)
		{
			if (cases_registered >= MAX_CASES) { return -1; }

			cases[cases_registered++] = case_to_add;
			return 0;
		}

	protected:

		void print_name() { printf("Test suite: %s\n", test_suite_name); };

		const char* test_suite_name = nullptr;
		t_u32 cases_registered = 0;
		t_u32 cases_failed = 0;
		test_case cases[MAX_CASES];
		bool suite_run = false;

		void check_increase_failed_count(test_case_result result)
		{
			if (result != test_case_result::SUCCESS)
			{
				cases_failed++;
			}
		}

	};
}

namespace ttest::internal 
{
	bool test_check(bool predicate, const char* check_text, const char* message, test_case& t_case, test_case_result result_on_fail, t_i32 line)
	{
		if constexpr (report_all_checks_results)
		{
			t_case.checks_info[t_case.checks_runned] = {
								check_text,
								message,
								line };
			if (!predicate)
			{
				t_case.result = result_on_fail;
				t_case.checks_failed++;
			}
		}
		else
		{
			if (!predicate)
			{
				t_case.checks_info[t_case.checks_failed++] = {
							check_text,
							message,
							line };

				t_case.result = result_on_fail;
				if constexpr (!_T_IS_DEFINED(TTEST_NO_BREAKS))
				{
					_TDEBUGGER_BREAK();
				}
			}
		}
		t_case.checks_runned++;
		return predicate;
	}
}