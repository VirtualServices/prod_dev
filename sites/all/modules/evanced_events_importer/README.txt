/* $Id: README.txt,v 1.1.2.3 2010/09/20 13:17:22 slanger Exp $ */

Evanced Events Importer
=======================
The Evanced Events Importer module integrates features from Evanced 
Solutions' Events calendar product in to your Drupal site.

This module imports data from Evanced Events' built-in EXML feed to 
create nodes for each published event.  When the Evanced Events 
calendar is updated in Evanced Events and cron is run in Drupal, 
the module keeps the events in sync using the ID assigned to each event.  

With this module installed you can continue to use Evanced Events as 
a stand-alone product to create events, manage event registration, 
record attendance statistics, run reports and manage room reservations 
(in conjunction with Room Reserve), but you can also take advantage 
of all the power of CCK and Views to create multiple displays of your 
events across your website.

This module was originally created in 2009 by Worthington Libraries 
for their website (http://www.worthingtonlibraries.org).


Installation notes
==================
Note: This module requires a subscription to Events by Evanced 
Solutions (http://evancedsolutions.com).

Required modules
----------------
•	CCK
•	Date

Basic configuration
-------------------
This module assumes basic familiarity with CCK and Views.  For 
information on creating a content type or configuring views, please 
refer to the documentation for those modules.

1.	Administer > Content management > Content types 
	Create a content type for your events.  Required fields: Date, 
	Evanced ID
	•	Add a new field of type Date for the event's start date/time 
		and end date/time
		•	To date: Required
		•	Granularity: Year + Month + Day + Hour + Minute
		•	Time zone handling: No time zone conversion
	•	Add a new field of type Integer to accommodate the Evanced 
		Events ID
		•	Number of values: 1

2.	Administer > Content management > Taxonomy 
	Create vocabularies that correspond with the Branch Specific Lists 
	and System Wide Lists in Evanced Events.  Suggested vocabularies: 
	Location, Event Type, Age Group
	•	Branches + Locations
		•	Include all of the branches from your System 
			Configuration & Settings
		•	Arranged hierarchically beneath each branch, include all 
			of the rooms from the Location List
	•	Event Type
		Include all the event types from the Event Type List
	•	Age Group
		Include all the age groups from the Age Group List
	Associate each vocabulary with the event content type you created 
	in step 1.

3.	Admin > Site configuration > Evanced Event Importer settings 
	Configure the Evanced Events Importer module.
	•	In the "Main configuration" tab:
		•	Supply the EXML feed URL from your Evanced Events 
			installation.
		•	Under node type, select the content type you created in 
			Step 1.
		•	Under input format, select the input format that best 
			matches your needs.
		•	Check the checkbox if you wish to strip links from the 
			event description in Evanced Events.
	•	The  "XML Mapper" tab allows you to map elements from the 
		Evanced Events EXML feed to the fields of your event content 
		type in Drupal.
		•	Select the Evanced Events element at left and click the 
			button with the right arrow (-->) next to the field you 
			want to map it to.
		•	To see an example of a mapping configuration 
			typically used for event content types, click 
			on the "Suggested Settings" link. 

*IMPORTANT!* Importing events for the very first time
-----------------------------------------------------
When cron is running, the Evanced Events Importer checks to see if 
an existing event in Drupal needs to be updated -- and quickly moves 
past it if it doesn't. The most time-consuming thing the module does 
is create new event nodes. 

With this in mind: the first time you use the Evanced Events Importer, 
you'll likely have *a lot* of new event data to import, which will be 
a time-consuming process. Although Drupal can handle it, most likely, 
if you import all that data at once, you'll hit PHP's Max Execution 
Time limit (i.e. 'max_execution_time' in your php.ini file) before 
cron can finish executing. This can result in cron locking up (if 
this happens, go here to find out how to unlock it: 
http://drupal.org/node/141332#comment-236210). 

To avoid overtaxing the system during your inaugural import, it is 
recommended that you configure your EXML Feed so that it lists only 
a month's worth of data the first time (see the section entitled 
"The EXML Feed" to learn more about using parameters to control 
what the EXML feed displays). After cron has successfully run, expand 
that time period to two months (or three, if you don't have that 
many events scheduled in the future). Manually run cron again. 
Continue to do this until you've imported all your desired data. 
From that point on, you should be able to set the Evanced Events 
Importer to import events up to 365 days in the future without 
causing any problems. NOTE: This may vary depending on how many 
other tasks are hooked into cron on your Drupal installation. 


The EXML Feed
=============
Evanced provides various XML feeds for exporting data from its 
Events software. Their EXML feed includes the most robust event 
data, which is why it is the feed of choice for this module. The 
other feeds offered by Evanced (xml, rss2, atom1, ical) are a bit 
more compact and don't include certain fields, such as event end 
times and/or specific categories (Event Types, Age Groups, etc).

Where is the EXML Feed located?
-------------------------------
Based on your installation of Evanced Events, you should be able 
to find the EXML feed here: 

<url_of_your_evanced_installation>/eventsxml.asp?dm=exml&<additional_parameters>
Example: http://www.example.com/evanced/lib/eventsxml.asp?lib=all&nd=30&alltime=1&dm=exml 

EXML Feed: Parameters
---------------------
One can add parameters to the end of the feed URL to control how 
much event data should be displayed. For example, one important 
parameter (nd) limits the number of days included in the EXML feed 
("nd=30" means "show any events coming up in the next 30 days").

You can find a description of all the parameters in the Evanced 
Events manual. To access it: 

1.	Go here: http://evancedsolutions.com/manuals.asp 

2.	Under the heading Events Manuals, download the PDF entitled 
	"Events Manual – Version 6.0". 

3.	Open the PDF and jump to the 145th page, entitled 
	"APPENDIX E. USING EVENTS XML.ASP". This section will describe 
	all of the options available to you for configuring the feed. 

4.	Consult your Evanced representative if you need help with 
	the EXML feed or Events software. 


Limitations
===========
This module does not (yet!) include functionality related to patron 
registration.


Contact
=======
•	Stefan Langer (slanger) - slanger@worthingtonlibraries.org
•	Kara Reuter (kittysunshine) - kreuter@worthingtonlibraries.org

This module was originally created in 2009 by Worthington Libraries 
for their website (http://www.worthingtonlibraries.org).
