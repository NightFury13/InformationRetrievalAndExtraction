#!/usr/bin/python

#################################
#								#
# Author : Mohit Jain			#
# RollNo : 201202164			#
#								#
#################################


import os
import re
from collections import defaultdict
from nltk import PorterStemmer
from array import array
import gc
import sys

porter = PorterStemmer()

class CreateIndex:
	
	def __init__(self):
		'''Gives the Inverted Index to the class instance'''
		self.index = defaultdict(list)

	def GetInputParams(self):
		'''Take user input for (1)stopwords-file, (2)xml-input-file, (3)index-output-file.'''
		self.stopWordsFilename = sys.argv[1]
		self.inputXMLFilename = sys.argv[2]
		self.outputIndexFilename = sys.argv[3]

	def GetStopWords(self):
		'''Extract the stopwords from the given stopwords file (like 'a','an','the', etc...)'''
		stopwordsFile = open(self.stopWordsFilename, 'r')
		stopwords = [line.rstrip() for line in stopwordsFile]
		self.stopWords = dict.fromkeys(stopwords)
		stopwordsFile.close()

	def ParseTextRegion(self, text):
		textString = text.group(1)
		textString = re.sub(r'&lt;!--(.*?)--&gt;',' ', textString)  # Remove all that is in HTML comments.
		

	def ParseInputXMLFile(self):
		'''Returns all the tags of the next page in XML file'''
		attributeDump = []
		ctr = 0
		for line in self.inputXMLFile:
			ctr+=1
			if line.strip()=='</page>':
				break
			attributeDump.append(line.strip())

		currentPage = ''.join(attributeDump)
		pageTitle = re.search('<title>(.*?)</title>', currentPage, re.DOTALL)
		pageId = re.search('<id>(.*?)</id>', currentPage, re.DOTALL)
		revision = re.search('<revision>(.*?)</revision>', currentPage, re.DOTALL)
		
		try:
			# Now revision is the major tag here, so search in that.
			revText = revision.group(1)
			text = re.search('<text xml:space="preserve">(.*?)</text>', revText, re.DOTALL)  #This is where the shit lies!

#			revId = re.search('<id>(.*?)</id>', revText, re.DOTALL)
#			timstamp = re.search('<timestamp>(.*?)</timestamp>', revText, re.DOTALL)
#			comment = re.search('<comment>(.*?)</comment>', revText, re.DOTALL)

##########################################################################################
# USE THIS IF YOU WANNA PARSE THE CONTRIBUTOR ID AND NAME.
#
#		contributor = re.search('<contributor>(.*?)</contributor>', revText, re.DOTALL)
#
#		# Search for username and id in contributor.
#		contribText = contributor.group(1)
#
#		username = re.search('<username>(.*?)</username>', contribText, re.DOTALL)
#		userId = re.search('<id>(.*?)</id>', contribText, re.DOTALL)
#													
##########################################################################################
	
		except:
			text = ''

		# Add all these attributes to a dictionary.
		try:
			pageAttributes = {}

			pageAttributes['title'] = pageTitle.group(1)
			pageAttributes['id'] = pageId.group(1)
#		pageAttributes['rev-id'] = revId.group(1)
#		pageAttributes['timestamp'] = timestamp.group(1)    no point taking this :/
#		pageAttributes['comment'] = comment.group(1)

####################################################
# To-Do : Make a text-parser for this specific tag.#
####################################################
			pageAttributes['text'] = text.group(1)
		except:
			print "Page has no revision or title."
#		pageAttributes['text'] = self.ParseTextRegion(text)
####################################################
		
#		pageAttributes['username'] = username.group(1)
#		pageAttributes['user-id'] = userId.group(1)

		return pageAttributes

	def GetTerms(self, line):
		'''Removes all unnecessary characters from the text'''
		line = line.lower() # convert all to lower case.
		line = re.sub(r'[^a-z0-9^ ]',' ', line) # replace all non-alphanumeric characters with spaces.
		line = line.split() # seperate each word
		line = [word for word in line if word not in self.stopWords] # Remove the stop words.
		line = [porter.stem(word) for word in line]
		return line

	def WriteIndexToFile(self):
		'''Write the contents of the Inverted Index to the output-file'''
		outFile = open(self.outputIndexFilename, 'w')
		for term in self.index.iterkeys():
			postingList = []
			for args in self.index[term]:
				docId = args[0]
				count = args[1]
				postingList.append(':'.join([str(docId), str(count)]))
			print >> outFile, ''.join((term,'|',';'.join(postingList)))
		
		outFile.close()

	def CreateInvertedIndex(self):
		'''This is the main function that drives all the other class-functions'''
		self.GetInputParams() 
		self.inputXMLFile = open(self.inputXMLFilename, 'r')
		self.GetStopWords()

		# BUG IN PYTHON : appending to list becomes progressively slower due to bug in garbage collector.
		# Ref : http://stackoverflow.com/questions/2473783/is-there-a-way-to-circumvent-python-list-append-becoming-progressively-slower
		# Soln : disable the garbage collector while appending and then enable it back once done.

		gc.disable()

		pageDict = {}
		pageDict = self.ParseInputXMLFile()

		# Convert the lines of text to words.
		while pageDict != {}:
			lines = '\n'.join((pageDict['title'], pageDict['text']))
			pageId = int(pageDict['id'])
			terms = self.GetTerms(lines)
			
			# Make the Inverted Index.
			pageIndexDict = {}
			for position, term in enumerate(terms):
				if (any(char.isdigit() for char in term)): # Removing the numbers from index.
					continue
				try:
					pageIndexDict[term][1]+=1
				except:
					pageIndexDict[term] = [pageId, 1]

			# Merge the page index with the overall index.
			for termPage, postingPage in pageIndexDict.iteritems():
				self.index[termPage].append(postingPage)
			
			# Go-to next page.
			pageDict = self.ParseInputXMLFile()
			
		# Re-enable the garbage collector.
		gc.enable()

		self.WriteIndexToFile()

if __name__ == '__main__':
	index = CreateIndex()
	index.CreateInvertedIndex()
