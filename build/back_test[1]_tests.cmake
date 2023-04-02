add_test([=[HelloTest.BasicAssertions]=]  [==[C:/Users/simor/Programming Files/backtester/build/back_test.exe]==] [==[--gtest_filter=HelloTest.BasicAssertions]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[HelloTest.BasicAssertions]=]  PROPERTIES WORKING_DIRECTORY [==[C:/Users/simor/Programming Files/backtester/build]==] SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
set(  back_test_TESTS HelloTest.BasicAssertions)
