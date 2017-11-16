#!/usr/bin/python3
import tkinter as tk
import sys
import os
from tkinter import *
from tkinter import filedialog
from ctypes import *

LARGE_FONT = ("Verdana", 12)
SMALL_FONT = ("Verdana", 8)

class Calendar(Structure):
    _fields_ = [
        ('version', c_float),
        ('prodID', c_char*1000),
        ('events', c_void_p),
        ('propertiies', c_void_p)]

class GUI(tk.Tk):
	def __init__(self, *args, **kwargs):
		tk.Tk.__init__(self, *args, **kwargs)
		self.title("ICalendar")
		#set up the window
		root = tk.Frame(self)
		root.pack(side="top", fill="both")
		
		# initialize the menu
		menu = tk.Menu(self)
		self.config(menu=menu)

		file_menu = tk.Menu(menu)
		menu.add_cascade(label="File",menu=file_menu)
		file_menu.add_command(label="Open", command=lambda:fileSelect(root))
		file_menu.add_command(label="Save", command=self.show_frame)
		file_menu.add_command(label="Save As", command=self.show_frame)
		file_menu.add_command(label="Exit", command=self.show_frame)

		create_menu=Menu(menu)
		menu.add_cascade(label="Create",menu=create_menu)
		create_menu.add_command(label="Create Calendar", command=lambda:self.show_frame(createCalView))
		create_menu.add_command(label="Create Event", command=self.show_frame)

		help_menu=Menu(menu)
		menu.add_cascade(label="Help",menu=help_menu)
		help_menu.add_command(label="iCalGUI", command=self.show_frame)
		
		#open up all the frames and add them to the dict
		self.frames = {}
		
		for F in (startPage, createCalView):
			frame = F(root,self)
			self.frames[F] = frame
			frame.grid(row=0, column=0, sticky="nsew")
		
		#set the opening page	
		self.show_frame(startPage)
		
	def make_display(self,root,cal):
		f = displayView(root,self,cal)
		f.grid(row=0, column=0, sticky="nsew")
		f.tkraise()
		
		
	def show_frame(self,cont):
		frame = self.frames[cont]		
		frame.tkraise()


class startPage(tk.Frame):
	def __init__(self, parent, controller):
		tk.Frame.__init__(self,parent)
		label = tk.Label(self,text="Hello from start page", font=LARGE_FONT)
		label.pack()
	

		
def fileSelect(root):
	filename = filedialog.askopenfilename(filetypes = (("iCal files", "*.ics")
                                                      ,("All files", "*.*") ))
	createCal = callib.createCalendar
		
	#get ready to call the create cal function
	fStr = filename.encode('utf-8')
	createCal.argtypes = [c_char_p,POINTER(POINTER(Calendar))]
	createCal.restype = c_int
	printCal = callib.printCalendar
	printCal.argtypes = [POINTER(Calendar)]
	printCal.restype = c_char_p
	calPtr = POINTER(Calendar)()	
	
	print('returned = ',createCal(fStr,byref(calPtr)))
	view.make_display(root,calPtr)
	
		
		
class displayView(tk.Frame):
	def __init__(self, parent, controller, cal):
		tk.Frame.__init__(self,parent)
		#Calendar information
		label1 = Label(self, text="Product ID:")
		label2 = Label(self, text="Version:")
		label1.grid(row=0, sticky=W)
		label2.grid(row=1, sticky=W, pady=8)
		calHold = cal.contents
		label1 = Label(self, text="%s" % calHold.prodID)
		label2 = Label(self, text="%f" % calHold.version)
		label1.grid(row=0, sticky=W, column=1, columnspan=3)
		label2.grid(row=1, sticky=W, column=1, columnspan=3,pady=8)

		
		#Events
		evNumLabel = tk.Label(self, text="Event No.",relief="solid",width=11)
		numPropLabel = tk.Label(self, text="Properties",relief="solid",width=12)
		numAlarmLabel = tk.Label(self, text="Alarms",relief="solid",width=8)
		summaryLabel = tk.Label(self, text="Summary",relief="solid",width=36)
		evNumLabel.grid(row=3,column=0)
		numPropLabel.grid(row=3,column=1)
		numAlarmLabel.grid(row=3,column=2)
		summaryLabel.grid(row=3,column=3, columnspan=3)
		
		#get ready to call the create cal function
		
		length = CDLL(calLibPath).getEventLength
		length.argtypes = [c_void_p]
		length.restype = c_int
		
		numEvents = length(cast(cal,c_void_p)) ##get the number of events from the c functions
		
		for n in range(numEvents):
			pLen = CDLL(calLibPath).getEventPropLength
			aLen = CDLL(calLibPath).getEventAlarmLength
			pLen.restype = c_int
			aLen.restype = c_int
			numProp = pLen(cast(cal,c_void_p),n)
			numAlarm = aLen(cast(cal,c_void_p),n)
			getSum = CDLL(calLibPath).getSummary
			getSum.restype = c_char_p
			summary = getSum(cast(cal,c_void_p),n)
			
			label1 = tk.Label(self,text="%d" % n)
			label2 = tk.Label(self,text="%d" % numProp)
			label3 = tk.Label(self,text="%d" % numAlarm)
			label4 = tk.Label(self,text="%s" % summary.decode("utf-8"),width=36,wraplength=290,justify="left")
			
			label1.grid(row=n+4,column=0)
			label2.grid(row=n+4,column=1)
			label3.grid(row=n+4,column=2)
			label4.grid(row=n+4,column=3, columnspan=3, sticky=W)
			
		
		

class createCalView(tk.Frame):
	def __init__(self, parent, controller):
		tk.Frame.__init__(self,parent)
		#titles
		intro = tk.Label(self,text="Create a Calendar",font=LARGE_FONT)
		intro.grid(row=0,columnspan=2,sticky="ew",padx=5)
		warning = tk.Label(self,text="All fields with * are required.",font=SMALL_FONT)
		warning.grid(row=1,columnspan=2,sticky="ew",padx=5)
		#prod ID
		pIDL = tk.Label(self, text="*Enter the product ID:")
		prodIDInput = tk.Entry(self)
		pIDL.grid(row=2, column=0, pady=4,padx=4)
		prodIDInput.grid(row=2,column=1,sticky=E,padx=5)
		#version
		verL = tk.Label(self, text="*Enter the version:")
		versionInput = tk.Entry(self)
		verL.grid(row=3, column=0, pady=4,padx=4)
		versionInput.grid(row=3,column=1,sticky=E,padx=5)
		#Events
		
		
	def checkInput(prodID, version):
		print("checking input")
		
		
		
#save the path
calLibPath = 'libs/libcal.so'

#load the module
callib = CDLL(calLibPath)
view = GUI()
view.mainloop()
