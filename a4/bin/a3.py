#!/usr/bin/python3
import tkinter as tk
import sys
import os
from tkinter import *
from tkinter import filedialog
from tkinter import messagebox
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
		self.root = tk.Frame(self)
		self.root.grid_columnconfigure(0,weight=1)
		self.root.grid_rowconfigure(0,weight=1)
		self.root.pack(side="top", fill="both")
		self.sharedCal = POINTER(Calendar)
		self.sharedCal = None
		self.currentFile = None
		self.update_idletasks()
		
		# initialize the menu
		menu = tk.Menu(self)
		self.config(menu=menu)

		self.file_menu = tk.Menu(menu)
		menu.add_cascade(label="File",menu=self.file_menu)
		self.file_menu.add_command(label="Open", command=fileSelect)
		self.file_menu.add_command(label="Save", command=calSave)
		self.file_menu.entryconfig(2, state=DISABLED)
		self.file_menu.add_command(label="Save As", command=lambda: self.show_frame(saveAsPage))
		self.file_menu.add_command(label="Exit", command=self.destroy)

		create_menu=Menu(menu)
		menu.add_cascade(label="Create",menu=create_menu)
		create_menu.add_command(label="Create Calendar", command=lambda:self.show_frame(createCalView))
		create_menu.add_command(label="Create Event", command=lambda:self.createEventCheck())

		help_menu=Menu(menu)
		menu.add_cascade(label="Help",menu=help_menu)
		help_menu.add_command(label="iCalGUI", command=aboutPopup)
		
		#key press events
		self.bind('<Control-o>',openEvent)
		self.bind('<Control-x>',exitEvent)
		self.bind('<Control-s>',saveEvent)
		
		
		
		#open up all the frames and add them to the dict
		self.frames = {}
		
		for F in (createCalView,createEventView,saveAsPage):
			frame = F(self.root,self)
			self.frames[F] = frame
			frame.grid_columnconfigure(0,weight=1)
			frame.grid(row=0, column=0, sticky="nsew")
		
		#set the opening page	
		self.show_frame(createCalView)
		
		#make the log view frame
		self.root.update()
		w = self.frames[createCalView].winfo_width()
		self.log = LogView(self.root,self,w)
		self.log.grid(row=1,column=0,sticky="nsew")
		self.log.grid_rowconfigure(0, weight=1)
		self.log.grid_columnconfigure(0, weight=1)
		
		
	
		
		
		
	def setSharedCal(self, value):
		self.sharedCal = value
		
		
	def createEventCheck(self):
		if self.sharedCal is not None:
			self.show_frame(createEventView)
		else:
			messages.append("You must create a calendar before you add an event.")
			self.logUpdate()
		
	def logUpdate(self):
		self.root.update()
		w = self.root.winfo_width()
		newLog = LogView(self.root,self,w)
		newLog.grid(row=1,column=0,sticky="nsew")	
		newLog.tkraise()
		
	def make_display(self):
		if self.sharedCal is None:
			print("none")
		f = displayView(self.root,self)
		f.grid(row=0, column=0, sticky="nsew")
		f.tkraise()
		self.logUpdate()
		
	def show_frame(self,cont):
		frame = self.frames[cont]		
		frame.tkraise()

def openEvent(event):
	fileSelect()
	
def exitEvent(event):
	view.destroy()

def saveEvent(event):
	if view.currentFile is not None:
		calSave()

def aboutPopup():
	messagebox.showinfo("About", "ICalendar Parser\nDeveloper: Jessica Authier\nCompatible with iCalendar v2.0")

class LogView(tk.Frame):
	def __init__(self, parent, controller,w):
		tk.Frame.__init__(self,parent)
		self.config(width=w)
		#make the scrollbar
		scrollb = Scrollbar(self)
		scrollb.grid(row=0,column=1,sticky="ns")
		
		
		#make the canvas
		canvas = Canvas(self, yscrollcommand = scrollb.set, height=150,width=w)
		canvas.grid(row=0,column=0,stick="nsew")
		scrollb.config(command=canvas.yview)
		
		
		
		self.logFrame = Frame(canvas,height=100)
		canvas.create_window(0,0,window=self.logFrame, anchor='nw')
		self.addMessages()
		parent.update()
		canvas.config(scrollregion=canvas.bbox("all"))
		canvas.yview_moveto(1)
		
	def addMessages(self):
		i = 0
		for m in messages:
			label = Label(self.logFrame,text="%s" % m,wraplength=500)
			label.grid(row=i, pady=4, padx=4, sticky="w")
			i=i+1




class saveAsPage(tk.Frame):
	def __init__(self, parent, controller):
		tk.Frame.__init__(self,parent)
		label = tk.Label(self,text="Enter the name file name:")
		label.grid(row=0,column=0)
		fileIn = tk.Entry(self)
		fileIn.grid(row=0,column=1)
		btn = Button(self,text="Save",command=lambda:calSaveAs(fileIn))
		btn.grid(row=1)

def fileSelect():
	filename = filedialog.askopenfilename(filetypes = (("iCal files", "*.ics")
                                                      ,("All files", "*.*") ))
	
	#get ready to call the create cal function
	if filename == ():
		return
	createCal = callib.createCalendar
	fStr = filename.encode('utf-8')
	createCal.argtypes = [c_char_p,POINTER(POINTER(Calendar))]
	createCal.restype = c_int
	calPtr = POINTER(Calendar)()
	check = createCal(fStr,byref(calPtr))
	print(check)
	if check == 0:
		view.setSharedCal(calPtr)
		
		messages.append("Calendar successfully opened.")
		view.make_display()
		view.title(filename)
		view.currentFile = filename
		view.file_menu.entryconfig(2, state=NORMAL)
	else:
		messages.append(getErrorMessage(check))
	view.logUpdate()
	
	
	
def calSave():
	if view.sharedCal is None:
		messages.append("There is no calendar to save.")
		view.logUpdate()
		return
	save = callib.writeToFileWrapper
	save.resType = c_int
	retMsg = save(byref(view.sharedCal),view.currentFile.encode("utf-8"))
	if retMsg is 0:
		messages.append("The calendar was saved!")
	else:
		messages.append(getErrorMessage(retMsg))
	view.logUpdate()
	
def calSaveAs(fileIn):
	if view.sharedCal is None:
		messages.append("There is no calendar to save.")
		view.logUpdate()
		return
	save = callib.writeToFileWrapper
	save.resType = c_char_p
	filename = fileIn.get().encode("utf-8")
	retMsg = save(byref(view.sharedCal),filename)
	if retMsg is 0:
		messages.append("The calendar was saved!")
	else:
		messages.append(getErrorMessage(retMsg))
	view.logUpdate()
	
	

def getErrorMessage(num):
	
	if num == 1:
		return "Invalid File!"
	if num == 2:
		return "Invalid Calendar!"
	if num == 3:
		return "Invalid Version!"
	if num == 4:
		return "Duplicate Version!"
	if num == 5:
		return "Invalid Product ID!"
	if num == 6:
		return "Duplicate Product ID!"
	if num == 7:
		return "Invalid Event!"
	if num == 8:
		return "Invalid Creation Date-Time!"
	if num == 9:
		return "Invalid Alarm!"
	if num == 10:
		return "Write Error!"
	if num == 11:
		return "Other Error!"

class displayView(tk.Frame):
	def __init__(self, parent, controller):
		if view.sharedCal is None:
			print("none")
		temp = view.sharedCal
		tk.Frame.__init__(self,parent)
		#Calendar information
		label1 = Label(self, text="Product ID:")
		label2 = Label(self, text="Version:")
		label1.grid(row=0, sticky=W)
		label2.grid(row=1, sticky=W, pady=8)
		calHold = temp.contents
		label1 = Label(self, text="%s" % calHold.prodID.decode("utf-8"))
		label2 = Label(self, text="%.1f" % calHold.version)
		label1.grid(row=0, sticky=W, column=1, columnspan=3)
		label2.grid(row=1, sticky=W, column=1, columnspan=3,pady=8)
		
		#Events
		evNumLabel = tk.Label(self, text="Event No.",relief="solid",width=11)
		numPropLabel = tk.Label(self, text="Properties",relief="solid",width=12)
		numAlarmLabel = tk.Label(self, text="Alarms",relief="solid",width=8)
		summaryLabel = tk.Label(self, text="Summary",relief="solid",width=36)
		evNumLabel.grid(row=3,column=0,sticky="ew")
		numPropLabel.grid(row=3,column=1,sticky="ew")
		numAlarmLabel.grid(row=3,column=2,sticky="ew")
		summaryLabel.grid(row=3,column=3, columnspan=3,sticky="ew")
		
		#get ready to call the create cal function
		
		length = CDLL(calLibPath).getEventLength
		length.argtypes = [c_void_p]
		length.restype = c_int
		
		numEvents = length(cast(temp,c_void_p)) ##get the number of events from the c functions
		
		
		
		for n in range(numEvents):
			pLen = CDLL(calLibPath).getEventPropLength
			aLen = CDLL(calLibPath).getEventAlarmLength
			pLen.restype = c_int
			aLen.restype = c_int
			numProp = pLen(cast(temp,c_void_p),n)
			numAlarm = aLen(cast(temp,c_void_p),n)
			getSum = CDLL(calLibPath).getSummary
			getSum.restype = c_char_p
			summary = getSum(cast(temp,c_void_p),n).decode("utf-8")
			high = int(len(summary)/35)
			
			label1 = tk.Label(self,text="%d" % n, relief="raised",width=11,height=high)
			label2 = tk.Button(self,text="%d" % numProp,width=11,height=high,command=lambda n=n:self.displayProperties(view.sharedCal,n))
			label3 = tk.Button(self,text="%d" % numAlarm,width=11,height=high,command=lambda n=n:self.displayAlarms(view.sharedCal,n))
			label4 = tk.Label(self,text="%s" % summary,width=36,wraplength=290,justify="left",relief="raised")
			
			label1.grid(row=n+4,column=0,sticky="nsew")
			label2.grid(row=n+4,column=1)
			label3.grid(row=n+4,column=2)
			label4.grid(row=n+4,column=3, columnspan=3,sticky="nsew")
		
		view.setSharedCal(temp)	
		
	def displayProperties(self, cal, evNum):
		display = callib.displayP
		display.restype = c_char_p
		props = display(byref(cal), evNum).decode("utf-8")
		if props != "NULL":
			toprint = "Displaying properties of event %d" % evNum +":\nPRODID:"+ cal.contents.prodID.decode("utf-8")+"\nVERSION:%.1f" % cal.contents.version+"\n"+props
			messages.append(toprint)
		else:
			toprint = "Displayingproperties for event %d"%evNum +":\nPRODID:"+ cal.contents.prodID.decode("utf-8")+"\nVERSION:%.1f" % cal.contents.version+"\n"
			messages.append(toprint)
		view.logUpdate()
		
	def displayAlarms(self, cal, evNum):
		display = callib.displayA
		display.restype = c_char_p
		alarms = display(byref(cal), evNum).decode("utf-8")
		if alarms != "NULL":
			toprint = "Displaying alarms of event %d" % evNum +":\n"+alarms
			messages.append(toprint)
		else:
			messages.append("There are no alarms to display for event %d."%evNum)
		view.logUpdate()

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
		pIDL.grid(row=2, column=0, pady=4,padx=4,sticky=W)
		prodIDInput.grid(row=2,column=1,sticky=E,padx=5)
		#version
		verL = tk.Label(self, text="*Enter the version:")
		versionInput = tk.Entry(self)
		verL.grid(row=3, column=0, pady=4,padx=4,sticky=W)
		versionInput.grid(row=3,column=1,sticky=E,padx=5)
		
		#Events
		evLabel = tk.Label(self,text="Add anEvent")
		evLabel.grid(row=4,columnspan=2,sticky="ew",padx=5)
		#UID
		uidL = tk.Label(self, text="Enter the UID:")
		uidInput = tk.Entry(self)
		uidL.grid(row=5, column=0, pady=4,padx=4,sticky=W)
		uidInput.grid(row=5,column=1,sticky=E,padx=5)
		#dtstamp
		dtL = tk.Label(self, text="Enter the creation date-time:")
		dtInput = tk.Entry(self)
		dtL.grid(row=6, column=0, pady=4,padx=4,sticky=W)
		dtInput.grid(row=6,column=1,sticky=E,padx=5)
		#dtStart
		dtsL = tk.Label(self, text="Enter the start date-time:")
		dtsInput = tk.Entry(self)
		dtsL.grid(row=7, column=0, pady=4,padx=4,sticky=W)
		dtsInput.grid(row=7,column=1,sticky=E,padx=5)
		#Enter button
		bttn = Button(self, text="Enter", command=lambda:self.checkInput(prodIDInput,
		 versionInput,uidInput,dtInput,dtsInput))
		bttn.grid(row=8,column=1,sticky=E)
	
	def isfloat(self,value):
		try:
			float(value)
			return True
		except:
			return False
		
	def checkInput(self,prodIDI, versionI, uidI, cdtI, sdtI):
		prodID = prodIDI.get()
		vTemp = versionI.get()
		calPtr = POINTER(Calendar)()
		if not prodID or not vTemp:
			messages.append("Fields with * are required.")
			view.logUpdate()
			return
		else:
			if self.isfloat(vTemp) == False:
				messages.append("The version must be 2.0.")
				view.logUpdate()
				versionI.delete(0,END)
				uidI.delete(0, END)
				cdtI.delete(0, END)
				sdtI.delete(0, END)
				return
			ver = float(vTemp)
			if ver != 2:
				messages.append("The version must be 2.0.")
				view.logUpdate()
				versionI.delete(0,END)
				uidI.delete(0, END)
				cdtI.delete(0, END)
				sdtI.delete(0, END)
				return
			createCal = callib.initCalWrapper
			createCal.argtypes = [POINTER(POINTER(Calendar)),c_char_p,c_float]
			createCal.restype = c_char_p	
			ret = createCal(byref(calPtr),prodID.encode('utf-8'),ver)
			calMesg = ret.decode('utf-8')
			print(calMesg)
			if calMesg == "OK":
				messages.append("Calendar was created!")
				view.logUpdate()
			else:
				messages.append(calMesg)
				view.logUpdate()
			uid = uidI.get()
			cdt = cdtI.get()
			if not uid or not cdt:
				pass
			else:
				createEv = callib.createAddEvent
				createEv.argtypes = [POINTER(POINTER(Calendar)),c_char_p,c_char_p,c_char_p]
				createEv.restype = c_char_p
				sdt = sdtI.get()
				if not sdt:
					sdt = cdt
				ret = createEv(byref(calPtr),uid.encode('utf-8'),cdt.encode('utf-8'),sdt.encode('utf-8'))
				evMesg = ret.decode('utf-8')
				print(evMesg)
				if evMesg == "OK":
					messages.append("An event was added to the calendar.")
					view.logUpdate()
				else:
					messages.append("Make sure you add events to your calendar.")
					view.logUpdate()
			if calMesg == "OK":
				if calPtr is None:
					print("bad")
				view.setSharedCal(calPtr)
				if view.sharedCal is None:
					print("bad")
				view.make_display()
				
			prodIDI.delete(0, END)
			versionI.delete(0, END)
			uidI.delete(0, END)
			cdtI.delete(0, END)
			sdtI.delete(0, END)
				
		
class createEventView(tk.Frame):
	def __init__(self, parent, controller):
		tk.Frame.__init__(self,parent)
		#titles
		intro = tk.Label(self,text="Create an Event",font=LARGE_FONT)
		intro.grid(row=0,columnspan=2,sticky="ew",padx=5)
		warning = tk.Label(self,text="All fields with * are required.",font=SMALL_FONT)
		warning.grid(row=1,columnspan=2,sticky="ew",padx=5)
		#UID
		uidL = tk.Label(self, text="*Enter the UID:")
		uidInput = tk.Entry(self)
		uidL.grid(row=5, column=0, pady=4,padx=4,sticky=W)
		uidInput.grid(row=5,column=1,sticky=E,padx=5)
		#dtstamp
		dtL = tk.Label(self, text="*Enter the creation date-time:")
		dtInput = tk.Entry(self)
		dtL.grid(row=6, column=0, pady=4,padx=4,sticky=W)
		dtInput.grid(row=6,column=1,sticky=E,padx=5)
		#dtStart
		dtsL = tk.Label(self, text="Enter the start date-time:")
		dtsInput = tk.Entry(self)
		dtsL.grid(row=7, column=0, pady=4,padx=4,sticky=W)
		dtsInput.grid(row=7,column=1,sticky=E,padx=5)
		#Enter button
		bttn = Button(self, text="Enter", command=lambda:self.checkInput(uidInput,dtInput,dtsInput))
		bttn.grid(row=8,column=1,sticky=E)

	def checkInput(self,uidI, cdtI, sdtI):
		uid = uidI.get()
		cdt = cdtI.get()
		if not uid or not cdt:
			messages.append("Fields with * are required.")
			view.logUpdate()
		else:
			createEv = callib.createAddEvent
			createEv.argtypes = [POINTER(POINTER(Calendar)),c_char_p,c_char_p,c_char_p]
			createEv.restype = c_char_p
			sdt = sdtI.get()
			if not sdt:
				sdt = cdt
			ret = createEv(byref(view.sharedCal),uid.encode('utf-8'),cdt.encode('utf-8'),sdt.encode('utf-8'))
			evMesg = ret.decode('utf-8')
			print(evMesg)
			if evMesg == "OK":
				messages.append("An event was added to the calendar.")
				view.logUpdate()
			else:
				messages.append("Invalid Input\nThe event was not added to the calendar.")
				view.logUpdate()
		if evMesg == "OK":
			view.make_display()
				
		uidI.delete(0, END)
		cdtI.delete(0, END)
		sdtI.delete(0, END)
			



#save the path
calLibPath = './libcal.so'
#load the module
callib = CDLL(calLibPath)
#list to hold the log messages
messages = ["Welcome User!"]

view = GUI()
view.mainloop()
