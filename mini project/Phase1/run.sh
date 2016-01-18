#!/bin/bash
echo "Enter the filepath to input xml file :"
read PATH
./src/parser.py ./bin/stopwords.txt $PATH ./index/output.txt
