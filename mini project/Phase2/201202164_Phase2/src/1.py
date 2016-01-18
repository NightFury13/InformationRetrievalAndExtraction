import xml.sax
import codecs,re
import sys
f = open('out.txt', 'r+')


class WikiContentHandler(xml.sax.ContentHandler):
  title=0
  text=0
  text_flag=0
  link=0
  ref=0
  pid=0
  paged=0
  infobox_flag=0

  def __init__(self):
    xml.sax.ContentHandler.__init__(self)
    self.title=0
    self.text=0
    self.text_flag=0
    self.link=0
    self.ref=0
    self.pid=0
    self.infobox_flag=0
    self.paged=0

  def startElement(self, name, attrs):
    if name=="id" and self.paged==0:
        self.paged=1
        self.pid=1
    else:
        self.pid=0

    if name=="title":
        self.paged=0
        self.title=1
    else:
        self.title=0

    if name=="text":
        self.text=1
    else:
        self.text=0


  def endElement(self, name):
    if name.find("title") != -1:
        self.link=0
        self.ref=0
    if name.find("text") != -1:
        self.text=0
  def characters(self, content):

      if self.pid==1:
          self.title=0
          f.write("\n&&pid&&:\n ")
          f.write( content.encode('utf-8', 'ignore'))
          f.write("\n&&pid&&:\n ")
          self.pid=0
          return
      if self.title==1 and self.pid==0:
          if(content[0]!='\n' and content[0]!=' '):
            f.write("\n&&title&&:\n ")
            content = re.sub(r'[^a-zA-Z ]+', ' ', content)
            f.write( content.encode('utf-8', 'ignore'))
            f.write("\n&&title&&:\n ")
          return


      if self.text==1:
          if content.find("{{Infobox") != -1:
              self.infobox_flag=1
              f.write("\n&&infobox&&:\n ")
          if content.find("'''") != -1 and self.infobox_flag==1:
              f.write("\n&&infobox&&:\n ")
              self.text_flag=1
              self.infobox_flag=0
              f.write("\n&&text&&:\n ")
          elif content.find("'''") != -1 and self.infobox_flag==0:
              f.write("\n&&text&&:\n ")
              self.text_flag=1
              self.infobox_flag=0


          if content.find("==References==")!=-1 and self.text_flag==1:
              f.write("\n&&text&&:\n ")
              self.ref=1
          if self.ref==1:
              if content.find("*")!=-1:
                  f.write("\n&&reference&&:\n ")
                  content = re.sub(r'[^a-zA-Z ]+', ' ', content)
                  f.write( content.encode('utf-8', 'ignore'))
                  f.write("\n&&reference&&:\n ")
                  return
          if content.find("==External")!=-1:
              self.link=1
              self.ref=0
          if self.link==1:
              if content.find("*")!=-1:
                  f.write("\n&&link&&:\n ")
                  content = content[content.find("www.")+4:]
                  content = content[:content.find(".")]
                  content = re.sub(r'[^a-zA-Z ]+', ' ', content)
                  f.write( content.encode('utf-8', 'ignore'))
                  f.write("\n&&link&&:\n ")
                  return
          if content.find("[[Category")!=-1:
              self.link=0
              content = content[content.find("[[Category"):]
              f.write("\n&&category&&:\n ")
              content = re.sub(r'[^a-zA-Z ]+', ' ', content)
              f.write( content.encode('utf-8', 'ignore'))
              f.write("\n&&category&&:\n ")
              return




      content = re.sub(r'[^a-zA-Z ]+', ' ', content)
      f.write( content.encode('utf-8', 'ignore'))



def main(sourceFileName):
  source = open(sourceFileName)
  xml.sax.parse(source, WikiContentHandler())

if __name__ == "__main__":
  main(str(sys.argv[1]))
f.close()
