#!/usr/local/bin/python

import cgi
import cgitb; cgitb.enable()
from os import environ

form = cgi.FieldStorage()

# Get cookies
num_questions_answered = 0;
num_correct_answers = 0;
if environ.has_key("HTTP_COOKIE"):
  print "Found cookies: " + environ["HTTP_COOKIE"]
  for cookie in environ["HTTP_COOKIE"].split(";"):
    cookie = cookie.strip()
    # print cookie
    (key, value) = cookie.split("=");
    if key == "get_survey_correct_answers":
      num_correct_answers = int(value)
    elif key == "get_survey_questions_answered":
      num_questions_answered = int(value)

# Check if this is a question submission
if form.getvalue('submission') and form.getvalue('answer'):
  submitted_for_question = form.getvalue('submission')
  sumbitted_answer = form.getvalue('answer')
  # Check if question is correct and set cookie correct_answers
  if (int(submitted_for_question) == 1 and sumbitted_answer == "They're") \
  or (int(submitted_for_question) == 2 and sumbitted_answer == "There") \
  or (int(submitted_for_question) == 3 and sumbitted_answer == "Their") \
  or (int(submitted_for_question) == 4 and sumbitted_answer == "There") \
  or (int(submitted_for_question) == 5 and sumbitted_answer == "There") :
    print "Set-Cookie: get_survey_correct_answers=%s" % str(num_correct_answers + 1)
  print "Set-Cookie: get_survey_questions_answered=%s" % str(num_questions_answered + 1)
  print
  # If on last question go to results page
  if int(submitted_for_question) == 5:
    print """
      <html>
        <head>
          <meta http-equiv="REFRESH" content="0;url=/cgi-bin/survey.cgi">
        </head>
      </html>
      """
  # Else redirect to next question
  else:
    print """
      <html>
        <head>
          <meta http-equiv="REFRESH" content="0;url=/cgi-bin/survey.cgi?question=%s">
        </head>
      </html>
      """ % str(int(submitted_for_question) + 1)
# Else just print the question
else:
  # Print HTTP headers
  print "Content-type: text/html"
  # HTML header
  html_header = """
    <html>
      <head>
        <title>CGI Survey</title>
        <link rel="stylesheet" href="../../css/style.css">
      </head>
      <body>
        <header class="header">
          <h1>CGI Survey</h1>
          <a href="/cgi-bin/survey.cgi" id="survey-quit">X</a>
        </header>
        <section class="content">
    """
  # If requesting a question page
  if form.getvalue('question') \
  and 0 < int(form.getvalue('question')) < 6:
    # End HTTP header
    print
    current_question = int(form.getvalue('question'))
    # Question 1
    if current_question == 1:
      print html_header
      print "<h3>Question 1</h3>"
      print """
        <p>_____ going to drive to Toronto?</p>
        <form action="/cgi-bin/survey.cgi" method="GET">
          <input type="text" hidden name="submission" value="1"/>
          <input type="radio" name="answer" value="They're">They're
          <br>
          <input type="radio" name="answer" value="Their">Their
          <input type="submit" value="Submit"/>
        </form>
      """
    # Question 2
    elif current_question == 2:
      print html_header
      print "<h3>Question 2</h3>"
      print """
        <p>Yes, we are getting _____ in a Tesla.</p>
        <form action="/cgi-bin/survey.cgi" method="GET">
          <input type="text" hidden name="submission" value="2"/>
          <input type="radio" name="answer" value="There">There
          <br>
          <input type="radio" name="answer" value="Their">Their
          <input type="submit" value="Submit"/>
        </form>
      """
    # Question 3
    elif current_question == 3:
      print html_header
      print "<h3>Question 3</h3>"
      print """
        <p>_____ cars are pretty good, but I prefer a motorcycle.</p>
        <form action="/cgi-bin/survey.cgi" method="GET">
          <input type="text" hidden name="submission" value="3"/>
          <input type="radio" name="answer" value="Their">Their
          <br>
          <input type="radio" name="answer" value="They're">They're
          <input type="submit" value="Submit"/>
        </form>
      """
    # Question 4
    elif current_question == 4:
      print html_header
      print "<h3>Question 4</h3>"
      print """
        <p>Well, you can't get three people _____ in a motorcycle.</p>
        <form action="/cgi-bin/survey.cgi" method="GET">
          <input type="text" hidden name="submission" value="4"/>
          <input type="radio" name="answer" value="They're">They're
          <br>
          <input type="radio" name="answer" value="There">There
          <input type="submit" value="Submit"/>
        </form>
      """
    # Question 5
    elif current_question == 5:
      print html_header
      print "<h3>Question 5</h3>"
      print """
        <p>Well, maybe we'll get _____ some day.</p>
        <form action="/cgi-bin/survey.cgi" method="GET">
          <input type="text" hidden name="submission" value="5"/>
          <input type="radio" name="answer" value="There">There
          <br>
          <input type="radio" name="answer" value="Their">Their
          <input type="submit" value="Submit"/>
        </form>
      """
  # Survey start or completion page
  else:
    # Reset cookies and print html_header
    print "Set-Cookie: get_survey_questions_answered=0"
    print "Set-Cookie: get_survey_correct_answers=0"
    print
    print html_header
    # Check if user completed the survey
    if num_questions_answered == 5:
      print "<h3>Results</h3>"
      print "<p>You've completed the survey with a sore of %s out of 5</p>" \
      % num_correct_answers
    # Else just print the start page
    else:
      print "<h3>Welcome to my survey</h3>"
      print "<p>This is a short five question survey to see if you know how to answer basic questions.</p>"
      print """
        <form action="/cgi-bin/survey.cgi" method="GET">
          <input type="text" hidden name="question" value="1"/>
          <input type="submit" value="Begin"/>
        </form>
      """
  # Print remaining html
  print """
      </section>
    </body>
  </html>
  """
