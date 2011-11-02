// $Id: README.txt,v 1.3.2.3 2010/11/29 15:02:56 sleepcamel Exp $

Taxonomy Defaults allows you to assign default terms from any vocabulary to any node-type.

INSTALLATION
------------

Copy all files to /sites/all/modules/taxonomy_defaults/, visit Admin >> Site building >> Modules (admin/build/modules) and enable Taxonomy Defaults. 


UPGRADING FROM 6.x-1.0 to 6.x-2.0
---------------------------------
In order to upgrade your "hidden" vocabularies from 6.x-1.0 or earlier, you'll need to 
1) enable the vocabulary for the content_type at admin/content/taxonomy
2) turn off the associated "Visible" checkbox at admin/content/taxonomy/taxonomy_defaults

More info: 
http://drupal.org/node/559828#comment-2162950

CONFIGURATION
-------------

Configure Taxonomy Defaults via Administer >> Content management >> Taxonomy, tab Defaults (admin/content/taxonomy/taxonomy_defaults).

Each vocabulary is shown in a table for every content type. If you enable the checkbox for a vocabulary, Taxonomy Defaults will add terms from the vocabulary to the content type's submissions. If a vocabulary is activated for a content type (shown by 'active' on the Taxonomy Defaults page), the terms will simply be pre-selected on the add/node page. If not, then the terms will be added to the node without any user interaction.

TROUBLESHOOTING
For most configurations, the installation should work automatically, but if you are using other taxonomy related modules, including Taxonomy Access Control, you may need to alter the weight of the taxonomy_defaults module, using a query similar to the following:

UPDATE system SET weight = 1 WHERE name = 'taxonomy_defaults';

The actual weight you need to assign will depend on your configuration.
