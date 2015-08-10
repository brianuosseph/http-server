#! /usr/local/bin/python

import cgi
import cgitb; cgitb.enable()
import os

form = cgi.FieldStorage() 
first_name = form.getvalue('first_name')
last_name  = form.getvalue('last_name')

print "Content-type: text/html"
print

print """
<html>
<head><title>Sample CGI Script</title></head>
<body>
  <h1>Sample CGI Script</h1>
"""
print
print "<h2>Getting URL params</h2>"
print "<p>Hello %s %s</p>" % (first_name, last_name)
print

print "<h2>Cookies</h2>"
if os.environ.has_key("HTTP_COOKIE"):
  print "<ul>"
  for cookie in os.environ["HTTP_COOKIE"].split(";"):
    (key, value) = cookie.strip().split("=");
    print "<li>Key: %s,\t" % key
    print "Value: %s</li>" % value
  print "</ul>"
else:
  print "No cookies passed to script."
print

print """
  <h2>Environment variables</h2>
  <ul>
"""
for param in os.environ.keys():
  print "<li>%20s: %s</li>" % (param, os.environ[param])
print "</ul>"
print


print """
</body>
</html>
"""