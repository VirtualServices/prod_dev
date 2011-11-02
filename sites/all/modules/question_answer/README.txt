; $Id: README.txt,v 1.1.2.2 2010/10/28 09:23:22 jcisio Exp $

Project homepage http://drupal.org/project/question_answer
Documentation http://drupal.org/node/955460

= Introduction =

This simple module allows you to create a answer/question section 
without much modification or a large module. It requires only CCK,
Ctools (Ajax-responder), both will be in D7 core.

You can reuse your content type, just add a new field to save the
selected answer. The core comment is used to submit answers.

= Configure =

== Installation ==
- Create a new content type, or reuse an existing one
- Add a new number/integer field for this content type (require Number
module, shipped with CCK)
- Select the format "Selected answer" for this field in the "Display
fields" section. This will display the selected comment right after
the node (you can reorder if there are other fields in this content
type)
- Go to admin/settings/question_answer and select the content type/
field name above.
- That's all! You can modify your theme to differenciate the selected
comment.

== Question expiration ==
If you want users to set an expiration time for their questions, do
the following steps:
- Goto the Q/A content type, create a new field "duration"
  - Type: Integer
  - Widget: Select list
  - In the configure page, enter prefined time in second, for example
    (unlimited, 1 day, 7 days and 30 days):

0
86400
604800
2592000

- Go to the Q/A setting page, select this field.
- Install VotingAPI module and another module, for example, vote_up_down
  (don't forget to enable "vud on comments").
- Go to permissions page, allow anonymous to vote on comments.
