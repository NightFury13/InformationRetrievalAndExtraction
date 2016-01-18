Mohit Jain
201202164

Work:

Used HashMap to map words to their categorized pages and frequency on each page. The frequency and page are stored in an ordered map. Used SAX parser in python to parse the XML data. Then used content based extraction of data from the XML file to categorize the data into 6 categories. 
1. Title
2. Body Text
3. Infobox
4. Categories
5. External Links (outlinks)
6. References




Extra:

1. Analysed run-time of Trie and Unordered_Map(HashTable).

				Trie				HashMap
	Index construction: 	12 seconds			28 seconds
	Estimated Query Time	O(lenght of word)    		O(1) if no collisions

Since, our job is to minimize query time. It is better to use to a HashMap.

2. The indexing has been categorized into the 6 categories and the another category which has all the words. Hence, the current indexing can handle both types of queries, i.e. category based and uncategorized queries.

3. Used OOPS.

4. Multilevel Indexing

5. Merge Sort

6. Storing File offsets

7. Used tdf-idf
	
