#!/usr/bin/python

# e.g. call "AC_LOG_VERBOSITY=DEBUG ./setVerbosity.py"
# or AC_LOG_MODULE_VERBOSITY=DEBUG/Window.cpp/102/202:TRACE/Shape.cpp ./setVerbosity.py

import os
import xml.dom.minidom 

env_file = "Environment.plist";

AC_LOG_VERBOSITY = os.getenv("AC_LOG_VERBOSITY")
AC_LOG_MODULE_VERBOSITY = os.getenv("AC_LOG_MODULE_VERBOSITY")

dom = xml.dom.minidom.parse(env_file)
keys = dom.getElementsByTagName("key");
for key in keys:
    text = key.firstChild.wholeText;
    if AC_LOG_VERBOSITY and text == "AC_LOG_VERBOSITY":
        valueNode = key.nextSibling.nextSibling;
        if valueNode.firstChild and valueNode.firstChild.nodeName == "#text":
            valueNode.firstChild.replaceWholeText(AC_LOG_VERBOSITY);
        else:
            newText = dom.createTextNode(AC_LOG_VERBOSITY);
            valueNode.appendChild(newText);
    if AC_LOG_MODULE_VERBOSITY and text == "AC_LOG_MODULE_VERBOSITY":
        valueNode = key.nextSibling.nextSibling;
        if valueNode.firstChild and valueNode.firstChild.nodeName == "#text":
            valueNode.firstChild.replaceWholeText(AC_LOG_MODULE_VERBOSITY);
        else:
            newText = dom.createTextNode(AC_LOG_MODULE_VERBOSITY);
            valueNode.appendChild(newText);


#print dom.toxml();

f = open(env_file, 'w');
dom.writexml(f);
f.close();


