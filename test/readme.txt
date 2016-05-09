
------------------------------------------------------------------------------------
googletest articles:
http://stackoverflow.com/questions/13513905/how-to-setup-googletest-as-a-shared-library-on-linux
http://www.ibm.com/developerworks/aix/library/au-googletestingframework.html
http://www.yolinux.com/TUTORIALS/Cpp-GoogleTest.html
https://github.com/google/googletest/blob/master/googletest/docs/FAQ.md#why-is-it-not-recommended-to-install-a-pre-compiled-copy-of-google-test-for-example-into-usrlocal

------------------------------------------------------------------------------------
To run a subset of the test cases
http://stackoverflow.com/questions/7208070/googletest-how-to-skip-a-test

--gtest_filter
Google Test will only run the tests whose full names (in the form of TestCaseName.TestName) 
match the filter.

The format of a filter is a ':'-separated list of wildcard patterns (called the positive 
patterns) optionally followed by a '-' and another ':'-separated pattern list (called the 
negative patterns). A test matches the filter if and only if it matches any of the positive 
patterns but does not match any of the negative patterns.

A pattern may contain '*' (matches any string) or '?' (matches any single character). For 
convenience, the filter '*-NegativePatterns' can be also written as '-NegativePatterns'.

For example:
./foo_test Has no flag, and thus runs all its tests.
./foo_test --gtest_filter=* Also runs everything, due to the single match-everything * value.
./foo_test --gtest_filter=FooTest.* Runs everything in test case FooTest.
./foo_test --gtest_filter=*Null*:*Constructor* Runs any test whose full name contains either "Null" or "Constructor".
./foo_test --gtest_filter=-*DeathTest.* Runs all non-death tests.
./foo_test --gtest_filter=FooTest.*-FooTest.Bar Runs everything in test case FooTest except FooTest.Bar.

------------------------------------------------------------------------------------
To run the disabled tests
--gtest_also_run_disabled_tests

To catch error in the debugger - stop at the wrong test
--getst_break_on_failure

------------------------------------------------------------------------------------
To set breakpoint at a test case
$ break TestcaseName_TestName_Test::TestBody
i.e.
TEST(algorithm, gridsearch) {}
$ break algorithm_gridsearch_Test::TestBody


