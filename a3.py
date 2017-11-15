#!/usr/bin/python
import Tkinter as tk
import sys
import os
from Tkinter import *

LARGE_FONT = ("Verdana", 12)
SMALL_FONT = ("Verdana", 8)

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
		file_menu.add_command(label="Open", command=lambda:self.show_frame(openView))
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
		
		for F in (startPage, openView, displayView, createCalView):
			frame = F(root,self)
			self.frames[F] = frame
			frame.grid(row=0, column=0, sticky="nsew")
		
		#set the opening page	
		self.show_frame(startPage)
		
		
	def show_frame(self,cont):
		frame = self.frames[cont]		
		frame.tkraise()


class startPage(tk.Frame):
	def __init__(self, parent, controller):
		tk.Frame.__init__(self,parent)
		label = tk.Label(self,text="Hello from start page", font=LARGE_FONT)
		label.pack()
	
class openView(tk.Frame):
	def __init__(self, parent, controller):
		tk.Frame.__init__(self,parent)
		label = tk.Label(self, text="Enter the path of the file you want to open.")
		fileInput = tk.Entry(self)
		openButton = tk.Button(self, text="Open", command=lambda:controller.show_frame(displayView))

		label.grid(row=0, columnspan=2, pady=4,padx=4)
		fileInput.grid(row=1,column=0,sticky=W,padx=5)
		openButton.grid(row =1,column=1, sticky=W)
		
class displayView(tk.Frame):
	def __init__(self, parent, controller):
		tk.Frame.__init__(self,parent)
		label1 = Label(self, text="Product ID:")
		label2 = Label(self, text="Version:")

		label1.grid(row=0)
		label2.grid(row=1)

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
		
		
	def checkInput():
		
		
view = GUI()
view.mainloop()
