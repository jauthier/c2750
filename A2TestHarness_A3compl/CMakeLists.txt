cmake_minimum_required(VERSION 3.8)
project(A1TestHarness)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        include/CalendarParser.h
        include/calTestCases.h
        include/testcases.h
        include/testharness.h
        src/calMemTest.c
        src/calMemTestPrint.c
        src/calTest.c
        src/calTestCases.c
        src/listTest.c
        src/testCases.c
        src/testharness.c
        testFiles/invCalendar/lineEnder.c
        testFiles/validCalendar/lineEnder.c)

add_executable(A1TestHarness ${SOURCE_FILES})