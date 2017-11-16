#!/usr/bin/python3
from ctypes import *


class Calendar(Structure):
    _fields_ = [
        ('version', c_float),
        ('prodID', c_byte*1000),
        ('events', c_void_p),
        ('propertiies', c_void_p)]


filename = 'megacal1.ics'
fStr = filename.encode('utf-8')


#save the path
calLibPath = './libcal.so'

#load the module
callib = CDLL(calLibPath)

#create an alias for callib.createCalendar() 
createCal = callib.createCalendar

# Help the Python compiler figure out the types for the function  
#********** these can be commented out - Python can infer them **********
createCal.argtypes = [c_char_p,POINTER(POINTER(Calendar))]
createCal.restype = c_int
#**********************************************************************


#create an alias for callib.printCalendar() 
printCal = callib.printCalendar

# Help the Python compiler figure out the types for the function  
printCal.argtypes = [POINTER(Calendar)] #this can also be commented out
printCal.restype = c_char_p             #this CANNOT be commented out! Otherwise Python will decide that printCal returns an int!

# create a variable that will store the pointer to the calendar
# if we don't do this, Python will be unhappy
calPtr = POINTER(Calendar)()

#call the library function createCalendar() using our alias createCal
print('returned = ',createCal(fStr,byref(calPtr))) #notice the type

testCal = calPtr.contents
print('version = ', testCal.version)

#testCal.prodID is an array of chars.  First we convert it to a char pointer
stuff = cast(testCal.prodID, c_char_p)

#Then we decode (convert) the char pointer to a Python string
print('',stuff.value.decode("utf-8"))

#call printCal
calStr = printCal(calPtr)

#and display the result
print(calStr.decode("utf-8"))




