#!/usr/bin/python3
import tkinter as tk
import sys
import os
import re
import mysql.connector
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
		self.eventSelected = -1
		self.currentFile = None
		self.update_idletasks()
		self.qWin = False
		self.hWin = False
		
		# Open database connection			
		createDatabases()
		
		# initialize the menu
		menu = tk.Menu(self)
		self.config(menu=menu)

		self.file_menu = tk.Menu(menu)
		menu.add_cascade(label="File",menu=self.file_menu)
		self.file_menu.add_command(label="Open", command=fileSelect)
		self.file_menu.add_command(label="Save", command=calSave)
		self.file_menu.entryconfig(2, state=DISABLED)
		self.file_menu.add_command(label="Save As", command=lambda: self.show_frame(saveAsPage))
		self.file_menu.add_command(label="Exit", command=self.exitRoot)

		create_menu=Menu(menu)
		menu.add_cascade(label="Create",menu=create_menu)
		create_menu.add_command(label="Create Calendar", command=lambda:self.show_frame(createCalView))
		create_menu.add_command(label="Create Event", command=lambda:self.createEventCheck())

		self.dataBase_menu=Menu(menu)
		menu.add_cascade(label="Database",menu=self.dataBase_menu)
		self.dataBase_menu.add_command(label="Store All Events", command=addAllEvents)
		self.dataBase_menu.entryconfig(1, state=DISABLED)
		self.dataBase_menu.add_command(label="Store Current Event", command=addCurrentEvent)
		self.dataBase_menu.entryconfig(2, state=DISABLED)
		self.dataBase_menu.add_command(label="Clear All Data", command=clearData)
		self.dataBase_menu.entryconfig(3, state=DISABLED)
		self.dataBase_menu.add_command(label="Display Database Status", command=showDBStatus)
		self.dataBase_menu.add_command(label="Execute Query", command=self.createQueryView)

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
		
		self.queryWin = None
		self.helpW = None
		
	#helper methods
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
		
	def createQueryView(self):
		if self.qWin is False:
			self.qWin = True
			self.queryWin = Query()
			self.queryWin.mainloop()
			
	def createHelpView(self):
		if self.hWin is False:
			self.hWin = True
			self.helpW = helpWin()
			self.helpW.mainloop()
		
	def exitRoot(self):
		clearData()
		if self.hWin is True:
			self.helpW.closeWin()
		if self.qWin is True:
			self.queryWin.delete()
		self.destroy()

#key press event functions
def openEvent(event):
	fileSelect()
	
def exitEvent(event):
	view.exitRoot()

def saveEvent(event):
	if view.currentFile is not None:
		calSave()

#help about function
def aboutPopup():
	messagebox.showinfo("About", "ICalendar Parser\nDeveloper: Jessica Authier\nCompatible with iCalendar v2.0")

#For viewing the log messages
class LogView(tk.Frame):
	def __init__(self, parent, controller,w):
		tk.Frame.__init__(self,parent)
		self.config(width=w)
		#make the scrollbar
		scrollb = Scrollbar(self)
		scrollb.grid(row=0,column=1,sticky="ns")
		#make the canvas
		canvas = Canvas(self, yscrollcommand = scrollb.set, height=150,width=w-15)
		canvas.grid(row=0,column=0,stick="nsew")
		scrollb.config(command=canvas.yview)
		#make the frame for the labels to go on
		self.logFrame = Frame(canvas,height=100)
		canvas.create_window(0,0,window=self.logFrame, anchor='nw')
		self.addMessages()
		parent.update()
		canvas.config(scrollregion=canvas.bbox("all"))
		canvas.yview_moveto(1)
	#For making adding the messages to the frame
	def addMessages(self):
		i = 0
		for m in messages:
			label = Label(self.logFrame,text="%s" % m,wraplength=500)
			label.grid(row=i, pady=4, padx=4, sticky="w")
			i=i+1

#For getting the filename to save the file as
class saveAsPage(tk.Frame):
	def __init__(self, parent, controller):
		tk.Frame.__init__(self,parent)
		label = tk.Label(self,text="Enter the name file name:")
		label.grid(row=0,column=0)
		fileIn = tk.Entry(self)
		fileIn.grid(row=0,column=1)
		btn = Button(self,text="Save",command=lambda:calSaveAs(fileIn))
		btn.grid(row=1)

#For selecting the file to be opened
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
		view.eventSelected = -1
		view.dataBase_menu.entryconfig(2, state=DISABLED)
	else:
		messages.append(getErrorMessage(check))
	view.logUpdate()
	
#For saving the current Calendar
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
	
#Turn the error int returned from c into a readable string
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

#For displaying the current calendar
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
		
		#Events - grid header
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
		
		#Create the grid
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
		
		if numEvents > 0:
			view.dataBase_menu.entryconfig(1, state=NORMAL)
		else: 
			view.dataBase_menu.entryconfig(1, state=DISABLED)
		view.setSharedCal(temp)
	
	#get the number of properties
	def displayProperties(self, cal, evNum):
		view.eventSelected = evNum
		view.dataBase_menu.entryconfig(2, state=NORMAL)
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

	#get the number of alarms
	def displayAlarms(self, cal, evNum):
		view.eventSelected = evNum
		view.dataBase_menu.entryconfig(2, state=NORMAL)
		display = callib.displayA
		display.restype = c_char_p
		alarms = display(byref(cal), evNum).decode("utf-8")
		if alarms != "NULL":
			toprint = "Displaying alarms of event %d" % evNum +":\n"+alarms
			messages.append(toprint)
		else:
			messages.append("There are no alarms to display for event %d."%evNum)
		view.logUpdate()

#For letting the user create a calendar
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
	
	#check if a string can be turned into a float
	def isfloat(self,value):
		try:
			float(value)
			return True
		except:
			return False
			
	#For checking the input of the create calendar
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
			
#For letting the user create a calendar		
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
			

#database functions
def createDatabases():
	organizerTbl = " CREATE TABLE ORGANIZER (org_id int AUTO_INCREMENT NOT NULL, name varchar(60) NOT NULL, contact varchar(60),PRIMARY KEY(org_id))"
	eventTbl = " CREATE TABLE EVENT (event_id int AUTO_INCREMENT, summary varchar(60) NOT NULL, start_time DATETIME NOT NULL, location varchar(60), organizer int ,num_alarms int, PRIMARY KEY(event_id), FOREIGN KEY (organizer) REFERENCES organizers(org_id) ON DELETE CASCADE)"

	cursor = conn.cursor()
	cursor.execute("DROP TABLE IF EXISTS EVENT")
	cursor.execute("DROP TABLE IF EXISTS ORGANIZER")
	
    
	#try creating
	try:
		cursor.execute(organizerTbl)
	except mysql.connector.Error as err:
		print("Something went wrong creating: {}".format(err))
		
	try:
		cursor.execute(eventTbl)
	except mysql.connector.Error as err:
		print("Something went wrong creating: {}".format(err))
	cursor.close()



def addAllEvents():
	#use the current calendar to get all the events
	#loop through all the events and add them to the table
	
	#get the number of events
	length = callib.getEventLength
	length.argtypes = [c_void_p]
	length.restype = c_int
	numEvents = length(cast(view.sharedCal,c_void_p))
	
	cursor = conn.cursor()
	view.dataBase_menu.entryconfig(3, state=NORMAL)
	for i in range(numEvents):
		#get the organizer
		getOrganizer = callib.getOrganizer
		getOrganizer.argtypes = [POINTER(POINTER(Calendar)),c_int]
		getOrganizer.restype = c_char_p
		orgStr = getOrganizer(byref(view.sharedCal),i).decode('utf-8')
		
		#get the summary
		getSummary = callib.getSummary
		getSummary.argtypes = [c_void_p,c_int]
		getSummary.restype = c_char_p
		sumStr = getSummary(cast(view.sharedCal,c_void_p),i).decode('utf-8')
		
		#get the location
		getLocation = callib.getLocation
		getLocation.argtypes = [POINTER(POINTER(Calendar)),c_int]
		getLocation.restype = c_char_p
		locStr = getLocation(byref(view.sharedCal),i).decode('utf-8')
		
		#get the startDT
		getStartDT = callib.getStartDT
		getStartDT.argtypes = [POINTER(POINTER(Calendar)),c_int]
		getStartDT.restype = c_char_p
		sDTStr = getStartDT(byref(view.sharedCal),i).decode('utf-8')
		#format the string
		startDT = "%s-%s-%s %s:%s:%s" % (sDTStr[0:4],sDTStr[4:6],sDTStr[6:8],sDTStr[10:12],sDTStr[12:14],sDTStr[14:16])
		#print(startDT)
		
		#get the number of Alarms
		getNumAlarms = callib.getEventAlarmLength
		getNumAlarms.argtypes = [c_void_p,c_int]
		getNumAlarms.restype = c_int
		numAlarms = getNumAlarms(cast(view.sharedCal,c_void_p),i)
		
		if sumStr == "NULL":
			sumStr = ""
		else:
			if len(sumStr) > 60:
				sumStr = sumStr[0:60]
	
		if locStr != "NULL":
			locStr = "'%s'" % locStr

		#check if the event already exists
		query = "SELECT * FROM EVENT WHERE start_time = '%s' AND summary = '%s'" % (startDT, sumStr)
		cursor.execute(query)
		result_set = cursor.fetchall()
		if len(result_set) > 0:
			messages.append("The event already exists in the database.")
			view.logUpdate()
			continue	

		#check if there is an organizer
		if orgStr != "NULL":
			#start by adding the organizer to the organizers table
			orgList = re.split("=|:",orgStr)
			#then get the org_id
			query = "\n".join(["IF NOT EXISTS(SELECT org_id FROM ORGANIZER WHERE name = '%s') THEN" % (orgList[2]),
					"INSERT INTO ORGANIZER (name, contact) VALUES ('%s', '%s');" % (orgList[2], orgList[4]),
					"END IF;"])
					
			cursor.execute(query)
			conn.commit()
			cursor.execute("SELECT org_id FROM ORGANIZER WHERE name = '%s'" % (orgList[2]))
			result_set = cursor.fetchall()
			for row in result_set:
				orgID = row[0]
				
			#print("%s %s " % (orgList[2], orgList[4]) + "org_id: %d" % orgID)
			#then add the event to the events table
			
			query = "\n".join(["INSERT INTO EVENT (summary, start_time, location, organizer, num_alarms)",
							"SELECT '%s', '%s', %s , org_id, %d" % (sumStr, startDT,locStr, numAlarms),
							"FROM ORGANIZER WHERE org_id = %d" % orgID])
			cursor.execute(query)
			conn.commit()
			
		else:
			query = "\n".join(["INSERT INTO EVENT (summary, start_time, location, organizer, num_alarms)",
							"VALUES ( '%s', '%s', %s , NULL, %d)" % (sumStr, startDT,locStr, numAlarms)])
			cursor.execute(query)
			conn.commit()
	
	cursor.close()
	

def addCurrentEvent():
	cursor = conn.cursor()
	view.dataBase_menu.entryconfig(3, state=NORMAL)
	#get the organizer
	getOrganizer = callib.getOrganizer
	getOrganizer.argtypes = [POINTER(POINTER(Calendar)),c_int]
	getOrganizer.restype = c_char_p
	orgStr = getOrganizer(byref(view.sharedCal),view.eventSelected).decode('utf-8')
		
	#get the summary
	getSummary = callib.getSummary
	getSummary.argtypes = [c_void_p,c_int]
	getSummary.restype = c_char_p
	sumStr = getSummary(cast(view.sharedCal,c_void_p),view.eventSelected).decode('utf-8')
	
	#get the location
	getLocation = callib.getLocation
	getLocation.argtypes = [POINTER(POINTER(Calendar)),c_int]
	getLocation.restype = c_char_p
	locStr = getLocation(byref(view.sharedCal),view.eventSelected).decode('utf-8')
	
	#get the startDT
	getStartDT = callib.getStartDT
	getStartDT.argtypes = [POINTER(POINTER(Calendar)),c_int]
	getStartDT.restype = c_char_p
	sDTStr = getStartDT(byref(view.sharedCal),view.eventSelected).decode('utf-8')
	#format the string
	startDT = "%s-%s-%s %s:%s:%s" % (sDTStr[0:4],sDTStr[4:6],sDTStr[6:8],sDTStr[10:12],sDTStr[12:14],sDTStr[14:16])
	print(startDT)
		
	#get the number of Alarms
	getNumAlarms = callib.getEventAlarmLength
	getNumAlarms.argtypes = [c_void_p,c_int]
	getNumAlarms.restype = c_int
	numAlarms = getNumAlarms(cast(view.sharedCal,c_void_p),view.eventSelected)
		
	if sumStr == "NULL":
		sumStr = ""
	else:
		if len(sumStr) > 60:
			sumStr = sumStr[0:60]

	if locStr != "NULL":
		locStr = "'%s'" % locStr
		
	#check if the event already exists
	query = "SELECT * FROM EVENT WHERE start_time = '%s' AND summary = '%s'" % (startDT, sumStr)
	cursor.execute(query)
	result_set = cursor.fetchall()
	if len(result_set) > 0:
		messages.append("The event already exists in the databse.")
		view.logUpdate()
		return	
	
	#check if there is an organizer
	if orgStr != "NULL":
		#start by adding the organizer to the organizers table
		orgList = re.split("=|:",orgStr)
		#then get the org_id
		query = "\n".join(["IF NOT EXISTS(SELECT org_id FROM ORGANIZER WHERE name = '%s') THEN" % (orgList[2]),
				"INSERT INTO ORGANIZER (name, contact) VALUES ('%s', '%s');" % (orgList[2], orgList[4]),
				"END IF;"])
					
		cursor.execute(query)
		conn.commit()
		cursor.execute("SELECT org_id FROM ORGANIZER WHERE name = '%s'" % (orgList[2]))
		result_set = cursor.fetchall()
		for row in result_set:
			orgID = row[0]
				
		#print("%s %s " % (orgList[2], orgList[4]) + "org_id: %d" % orgID)
		#then add the event to the events table
			
		query = "\n".join(["INSERT INTO EVENT (summary, start_time, location, organizer, num_alarms)",
				"SELECT '%s', '%s', %s , org_id, %d" % (sumStr, startDT,locStr, numAlarms),
				"FROM ORGANIZER WHERE org_id = %d" % orgID])
	else:
		query = "\n".join(["INSERT INTO EVENT (summary, start_time, location, organizer, num_alarms)",
				"VALUES ( '%s', '%s', %s , NULL, %d)" % (sumStr, startDT,locStr, numAlarms)])
	
	cursor.execute(query)
	conn.commit()
	cursor.close()

def clearData():
	cursor = conn.cursor()
	query = "DELETE FROM ORGANIZER"
	cursor.execute(query)
	conn.commit()
	query = "DELETE FROM EVENT"
	cursor.execute(query)
	conn.commit()
	cursor.execute("SELECT * FROM ORGANIZER")
	for row in cursor.fetchall() :
		print(row)
	cursor.execute("SELECT * FROM EVENT")
	
	for row in cursor.fetchall() :
		print(row)
	cursor.close()
	view.dataBase_menu.entryconfig(3, state=DISABLED)
	
def showDBStatus():
	cursor = conn.cursor()
	query = "SELECT * FROM ORGANIZER"
	cursor.execute(query)
	orgList = cursor.fetchall()
	query = "SELECT * FROM EVENT"
	cursor.execute(query)
	evList = cursor.fetchall()
	mes = "Database has %d Organizers and %d Events" % (len(orgList), len(evList))
	messages.append(mes)
	view.logUpdate()
	cursor.close()

class Query(tk.Tk):
	def __init__(self, *args, **kwargs):
		tk.Tk.__init__(self, *args, **kwargs)
		self.title("Query")	
		#set up the window
		self.root = tk.Frame(self)
		self.root.pack(side="top", fill="both")
		self.root.grid_rowconfigure(0, weight=1, uniform="x")
		self.root.grid_rowconfigure(1, weight=1, uniform="x")
		
		#make the query view
		self.qView = QueryView(self.root,self)
		self.qView.grid(row=0,column=0,sticky="nsew")
		self.qView.grid_rowconfigure(0, weight=1)
		self.qView.grid_columnconfigure(0, weight=1)
		
		#make the results view frame
		self.root.update()
		w = self.qView.winfo_width()
		self.res = ResultsView(self.root,self,w)
		self.res.grid(row=1,column=0,sticky="nsew")
		self.res.grid_rowconfigure(0, weight=1)
		self.res.grid_columnconfigure(0, weight=1)
		
		
		
		self.protocol("WM_DELETE_WINDOW", self.on_closing)
		
	def updateRes(self):
		self.root.update()
		w = self.winfo_width()
		newLog = ResultsView(self.root,self,w)
		newLog.grid(row=1,column=0,sticky="nsew")	
		newLog.tkraise()
		
	def on_closing(self):
		view.qWin = False
		self.destroy()
		
	def delete(self):
		self.destroy()

#For viewing the query results
class QueryView(tk.Frame):
	def __init__(self, parent,controller):
		tk.Frame.__init__(self,parent)
		self.var = IntVar()
		self.v = 0
		R1 = Radiobutton(self, variable=self.var, value=1,command=self.set1)
		R1.grid(row=0,column=0)
		queryEntry = Entry(self, width=30)
		queryEntry.insert(0,"SELECT")
		queryEntry.grid(row=0,column=1,sticky=W,columnspan=3)
		
		
		R2 = Radiobutton(self, variable=self.var, value=2,command=self.set2)
		R2.grid(row=1,column=0)
		L2 = Label(self, text="Display all events sorted by date.")
		L2.grid(row=1,column=1, columnspan=3,stick=W)
		
		R3 = Radiobutton(self, variable=self.var, value=3,command=self.set3)
		R3.grid(row=2,column=0)
		L3 = Label(self, text="Display events oganized by:")
		L3.grid(row=2, column=1,sticky=W,columnspan=3)
		orgEntry = Entry(self)
		orgEntry.grid(row=2,column=4,sticky=W)
		
		R4 = Radiobutton(self, variable=self.var, value=4,command=self.set4)
		R4.grid(row=3,column=0)
		L4 = Label(self,text="Display where I have to be on:")
		L4.grid(row=3,column=1,sticky=W,columnspan=3)
		dateEntry = Entry(self)
		dateEntry.grid(row=3,column=4)
		
		R5 = Radiobutton(self, variable=self.var, value=5,command=self.set5)
		R5.grid(row=4,column=0)
		L5 = Label(self, text="Display contact information and events of:")
		L5.grid(row=4,column=1,sticky=W,columnspan=3)
		nameEntry = Entry(self)
		nameEntry.grid(row=4,column=4)
		
		R6 = Radiobutton(self, variable=self.var, value=6,command=self.set6)
		R6.grid(row=5,column=0)
		L6 = Label(self, text="Display all events with")
		L6.grid(row=5,column=1,sticky=W)
		alarmEntry = Entry(self,width=3)
		alarmEntry.grid(row=5,column=2,sticky=W)
		L6 = Label(self, text="Alarms")
		L6.grid(row=5,column=3, sticky=W)
		
		submit = Button(self, text="Submit", command=lambda:self.submitAction(controller, self.v ,queryEntry,orgEntry,dateEntry,nameEntry,alarmEntry))
		submit.grid(row=6,column=0,sticky=W,columnspan=2,padx=8,pady=4)
		
		helpBtn = Button(self, text="Help",command=self.helpAction)
		helpBtn.grid(row=6,column=2,sticky=W,columnspan=2,padx=8,pady=4)
		
	def set1(self):
		self.v = 1
	
	def set2(self):
		self.v = 2
		
	def set3(self):
		self.v = 3
		
	def set4(self):
		self.v = 4
		
	def set5(self):
		self.v = 5	
		
	def set6(self):
		self.v = 6
		
	def helpAction(self):
		view.createHelpView()
		
	def submitAction(self, parent, var, queryE, orgE, dateE, nameE, alarmE):
		
		cursor = conn.cursor()
		if var == 0:
			results.append("You must choose an option.")
			results.append("---------------------------------------------------------------------------------------------------")
			parent.updateRes()
			
		if var == 1:
			query = queryE.get()
			results.append("Executing query: %s"%query)
			try:
				cursor.execute(query)
			except mysql.connector.Error as err:
				results.append("Something went wrong connecting: {}".format(err))
				results.append("---------------------------------------------------------------------------------------------------")
				parent.updateRes()
				cursor.close()
				return
			res = cursor.fetchall()
			if res == []:
				results.append("Query returned nothing."%orgName)
				results.append("---------------------------------------------------------------------------------------------------")
				parent.updateRes()
				cursor.close()
				return
			for r in res:
				results.append("Event No.: %s\nSummary: %s\nStart date-time: %s\nLocation:%s\nOrganizer ID: %s\nNumber of Alarms:%s"%(r[0],r[1],r[2],r[3],r[4],r[5]))
				
			results.append("---------------------------------------------------------------------------------------------------")				
			parent.updateRes()
			
		if var == 2:
			query = "SELECT * FROM EVENT ORDER BY start_time"
			cursor.execute(query)
			res = cursor.fetchall()
			for r in res:
				results.append("Event No.: %s\nSummary: %s\nStart date-time: %s\nLocation:%s\nOrganizer ID: %s\nNumber of Alarms:%s"%(r[0],r[1],r[2],r[3],r[4],r[5]))
				results.append("---------------------------------------------------------------------------------------------------")
			parent.updateRes()
			
		if var == 3:
			orgName = orgE.get().strip()
			if orgName == "":
				results.append("You must enter the name of an organizer.")
				results.append("---------------------------------------------------------------------------------------------------")
				parent.updateRes()
				cursor.close()
				return
			query = "SELECT org_id FROM ORGANIZER WHERE name = '%s'"%orgName
			cursor.execute(query)
			res = cursor.fetchall()
			if res == []:
				results.append("No organizers with the name '%s' were found."%orgName)
				results.append("---------------------------------------------------------------------------------------------------")
				parent.updateRes()
				cursor.close()
				return
			for r in res:
				orgID = r[0]
			query = "SELECT * FROM EVENT WHERE organizer = %s"%orgID
			cursor.execute(query)
			res = cursor.fetchall()
			for r in res:
				results.append("Event No.: %s\nSummary: %s\nStart date-time: %s\nLocation:%s\nOrganizer ID: %s\nNumber of Alarms:%s"%(r[0],r[1],r[2],r[3],r[4],r[5]))
			results.append("---------------------------------------------------------------------------------------------------")
			parent.updateRes()
			
		if var == 4:
			date = dateE.get().strip()
			if date == "":
				results.append("You must enter the date in the format year-month-day.")
				results.append("---------------------------------------------------------------------------------------------------")
				parent.updateRes()
				cursor.close()
				return
			query = "SELECT * FROM EVENT"
			cursor.execute(query)
			res = cursor.fetchall()
			
			for r in res:
				dt = "%s" % r[2]
				dt = dt[0:10]
				if date == dt.strip():
					results.append("Event No.: %s\nSummary: %s\nStart date-time: %s\nLocation:%s\nOrganizer ID: %s\nNumber of Alarms:%s"%(r[0],r[1],r[2],r[3],r[4],r[5]))
			results.append("---------------------------------------------------------------------------------------------------")
			parent.updateRes()
		
		if var == 5:
			name = nameE.get()
			if name == "":
				results.append("You must enter the name of the organiver you wish to contact.")
				results.append("---------------------------------------------------------------------------------------------------")
				parent.updateRes()
				cursor.close()
				return
			query = "SELECT org_id,contact FROM ORGANIZER WHERE name = '%s'"%name
			cursor.execute(query)
			res = cursor.fetchall()
			if res == []:
				results.append("No one by the name %s was found."%name)
				results.append("---------------------------------------------------------------------------------------------------")
				parent.updateRes()
				cursor.close()
				return
			for r in res:
				orgID = r[0]
				contact = r[1]
			
			results.append("%s's contact is: %s\nAnd they have organized the following events:" % (name,contact))
			query = "SELECT * FROM EVENT WHERE organizer = %s"%orgID
			cursor.execute(query)
			res = cursor.fetchall()
			for r in res:
				results.append("Event No.: %s\nSummary: %s\nStart date-time: %s\nLocation:%s\nOrganizer ID: %s\nNumber of Alarms:%s"%(r[0],r[1],r[2],r[3],r[4],r[5]))
			
			results.append("---------------------------------------------------------------------------------------------------")
			parent.updateRes()
			
		if var == 6:
			aStr = alarmE.get()
			if aStr == "":
				results.append("You must enter the number of alarms.")
				results.append("---------------------------------------------------------------------------------------------------")
				parent.updateRes()
				cursor.close()
				return
			query="SELECT * FROM EVENT WHERE num_alarms = %s"%aStr
			cursor.execute(query)
			res = cursor.fetchall()			
			if res == []:
				results.append("No events with %s alarms were found."%aStr)
				results.append("---------------------------------------------------------------------------------------------------")
				parent.updateRes()
				cursor.close()
				return
			for r in res:
				results.append("Event No.: %s\nSummary: %s\nStart date-time: %s\nLocation:%s\nOrganizer ID: %s\nNumber of Alarms:%s"%(r[0],r[1],r[2],r[3],r[4],r[5]))
			
			
			results.append("---------------------------------------------------------------------------------------------------")
			parent.updateRes()
			
		cursor.close()

#For viewing the query results
class ResultsView(tk.Frame):
	def __init__(self, parent,controller,w):
		tk.Frame.__init__(self,parent)
		self.config(width=w)
		#make the scrollbar
		scrollb = Scrollbar(self)
		scrollb.grid(row=0,column=1,sticky="ns")
		#make the canvas
		canvas = Canvas(self, yscrollcommand = scrollb.set, height=150,width=w-15)
		canvas.grid(row=0,column=0,stick="nsew")
		scrollb.config(command=canvas.yview)
		#make the frame for the labels to go on
		self.logFrame = Frame(canvas,height=100)
		canvas.create_window(0,0,window=self.logFrame, anchor='nw')
		self.addMessages()
		parent.update()
		canvas.config(scrollregion=canvas.bbox("all"))
		canvas.yview_moveto(1)
	#For making adding the messages to the frame
	def addMessages(self):
		i = 0
		for m in results:
			label = Label(self.logFrame,text="%s" % m,wraplength=500,justify=LEFT)
			label.grid(row=i, pady=4, padx=4, sticky="w")
			i=i+1

class helpWin(tk.Tk):
	def __init__(self, *args, **kwargs):
		tk.Tk.__init__(self, *args, **kwargs)
		self.title("Query Help")	
		#set up the window
		self.root = tk.Frame(self)
		self.root.pack(side="top", fill="both")
		self.root.grid_rowconfigure(0, weight=1, uniform="x")
		self.root.grid_rowconfigure(1, weight=1, uniform="x")
		display = Frame(self.root)
		display.pack(fill="both")
		self.protocol("WM_DELETE_WINDOW", self.on_closing)
		
		#get sql info
		cursor = conn.cursor()
		query = "DESCRIBE ORGANIZER"
		cursor.execute(query)
		res = cursor.fetchall()
		toPrint = ""
		for r in res:
			toPrint += "%s\n"%str(r) 
		print(toPrint)
		l1 = Label(display, text="ORGANIZER\n%s"%toPrint)
		l1.pack()
		
		query = "DESCRIBE EVENT"
		cursor.execute(query)
		res = cursor.fetchall()
		toPrint = ""
		for r in res:
			toPrint += "%s\n"%str(r) 
		print(toPrint)
		l1 = Label(display, text="EVENT\n%s"%toPrint)
		l1.pack()
		
		cursor.close()
		

		
	def on_closing(self):
		view.hWin = False
		self.destroy()
		
	def closeWin(self):
		self.destroy()	
		

#open database
dbName = "jauthier"
uName = "jauthier"
passwd = "0849720"

try:
	conn = mysql.connector.connect(host="dursley.socs.uoguelph.ca",database=dbName,user=uName, password=passwd)
except mysql.connector.Error as err:
	print("Something went wrong connecting: {}".format(err))
	exit()

#save the path
calLibPath = './libcal.so'
#load the module
callib = CDLL(calLibPath)
#list to hold the log messages
messages = ["Welcome User!"]
results = []

view = GUI()
view.mainloop()
