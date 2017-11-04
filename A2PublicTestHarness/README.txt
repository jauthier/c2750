*** Test harness instructions ***

The harness tests calendar creation/printing/deletion, as well as the printError function. It also tests error handling.  It does not test for memory leaks - those scripts are separate (see below).  As a result, deletion tests only really test for segfaults and other crashes during deletion. 

We don’t want to rely on potentially broken makefiles/static libs, so the test harness only uses your .h and .c files.

The directory structure is:
- bin - will contain list and calendar tests.
- src - contains test cases.  Do to modify these in any way.
- include - contains test harness headers and CalendarParser.h.  Do not modify these in any way.
- studentCode - all your .c files go here.  There should only be 2: calendarParser.c and LinkedListAPI.c
- studentInclude - all student .h files go here.  There should be at least 1 - LinkedListAPI.h - but there can be more.  DO NOT use your CalendarParser.h!
- testFiles - contains various broken and valid calendar files


To run:
- compile by typing  "make calTests"
- run bin/calTests
