*** Test harness instructions ***

The directory structure is the following:
- include/ contains the headers.  Do not modify them in any way.
- src/ contains the test harness.  You can make some changes to testCases.c to temporarily comment out test sub-cases. See instructions below.
- studentCode/ is where you put LinkedListAPI.c.  Do not put any other files there.  LinkedListAPI.c must not contain a main().
- bin/ will contain the resulting test suite.

To run:
- compile using make
- run bin/tests
- The test harness leaks memory, since I have to test your memory deallocators and I cannot rely on the integrity of your pointers.  I recommend you run your code inside valgrind to contain the leaks, i.e. valgrind bin/tests.  You can ignore the all the valgrind output, and just look at the output of the autograder.


*** Removing test cases ***

There are 7 tests in the test suite.  The file listTests.c runs the test suite and testCases.c defines 7 test case functions.  Each test case consists of sub-cases.

The test harness clearly reports passed and failed sub-tests.  However, if the code segfaults during testing, the entire test case fails.  You will want to isolate the specific sub-test that resulted in a segfault.  If you know, for example, that test case 5 (Testing sorted insertion) results inn a crash (segfault), you can figure out which sub-test fails, and fix the bug in your list that was uncovered by this test case. 

Doing this is a bit fiddly, but here are the instructions:

In testCases.c, each test case function has the variable numSubs, which is the number of the sub-tests in that function.  It must be set correctly, or the test harness will report a failed test even if all sub-tests pass.  For example, you know that your list fails test case 5 (Testing insertion into a sorted list).  You can modify insertSortedTest() function inside testCases.c to isolate the sub-test that results in a crash.  A good strategy would be to do this one subtest at a time.

At first you want to run only subtest 1 (insert to middle of sorted list).  To isolate only subtest 1, comment out test cases 2-5 inside insertSortedTest() AND change its numSubs to 1 instead of 5.  Then recompile using make and run bin/tests.  If the test harness reports a segfault in test case 5, you know it must caused by insertion into the middle of a sorted list, so you know what part of insertSorted() function in your list you need to fix.  If the test case passes, you know sub-test 1 wasn't the source of the crash.  You can then uncomment sub-test 2 (leave sub-tests 3-5 commented out) AND change numSubs to 2. Re-compile and re-run the test suite.  Repeat until you uncomment the sub-test that results in a segfault, then go into your list and fix the bug.

If you break the test harness beyond repair, re-download it and start again.


*** Fixing crashes/segfaults ***

In general, the sources of the crashes are:
- Trying to insert/clear/print/delete using a NULL list. 
- Incorrectly linking list elements when you insert into or delete from the list.  For example, make sure that the previous and next pointers of the new list element are set correctly, as well as the previous and next elements of its neighbour(s).  Similarly, when removing an element, make sure that its old neighbours are relinked correctly.

