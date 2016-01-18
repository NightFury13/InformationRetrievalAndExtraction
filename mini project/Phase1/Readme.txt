#################################
#				#
# Author : Mohit Jain		#
# RollNo : 201202164		#
#				#
#################################

This package is a creates an inverted-index of the Wikipedia dataset for efficient search and extraction of data. 
 

*****How To Run*****
`````````````````````
(1) Run the python script named 'run.sh' and give in the input as the path to sample.xml file.

	>>> $ ./run.sh

(2) Wait for the script to complete its run.

(3) The output is stored a .txt file in the same folder.


*****Work Details*****
``````````````````````
> The script has been written using OOPs, the major reason for doing so is increased elegance of code and simplification of changing modules of code.

> I have written my own parser using regular-expressions (for the time-being) as I want to test its performance in comparison to SAX parser at a later stage. Using OOPs helps me make these changes easily.

> To create the index, all non-alphanumeric characters, numbers and stopwords removed. Also, stemming converted similar meaning words to one word and all the words were converted to lower case characters.

> I have also given the option of creating the index with also storing the positions of the found words in each page (in-case required for in further work). However, this greatly increases the index file size. (see file - https://drive.google.com/file/d/0BypKJ8kYolnDRU5fTkFEQVJ3MGs/view?usp=sharing)

> Python has a bug in its garbage collection. Appending to lists becomes progressively slower as the loop progresses. To avoid this issue, I disable the gc while appending to my lists and re-enable it once the task is done to optimize on my speed.
	# Refer :  http://stackoverflow.com/questions/2473783/is-there-a-way-to-circumvent-python-list-append-becoming-progressively-slower

> The index saving has been done in the format :- "word|page-id:freq;page-id:freq..." so that retrieval becomes easier when using this index.

*****Packages Used*****
```````````````````````
> PorterStemmer from nltk
> gc (garbage collector)
> defaultdict from collections
> re (regular expressions)
