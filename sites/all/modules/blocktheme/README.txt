
== Description ==
BlockTheme allows an admin to define tpl files for standard block templates
and provides a select box on the block configure form so the user can select
a tpl file to use as opposed to having to override the templates by block ID.

== Installation ==

1. Enable the module

2. go to admin/settings/blocktheme and add entries like:
customtemplate|My Custom Template
mysupertemplate|My SuperTemplate
Where the first name is the machine-readable name of your template which may contain only
alphanumerical characters, -, or _ . The second name is the user-friendly name that appears
in the selectionbox on the block edit form.

3. choose from either step 4. or 5. 

4. Create tpl files in your theme directory like: (Note: filenames must be preceded by blocktheme- )
 blocktheme-customtemplate.tpl.php
 blocktheme-mysupertemplate.tpl.php

5. Alternatively use the extra provided variable $blocktheme to customize your
 block.tpl.php or block-*.tpl.php files. The $blocktheme will typically be
 used as a css class name in you template and contains the machine-readable name of
 your template.

== Usage ==

Go to the edit form for any block and select the appropriate template.

== Credits ==

Currently maintained by:
 - Andrei Mateescu - http://drupal.org/user/729614

Updated for drupal 6 by:
 - Tony Miller - tony.miller@eightyoptions.com
 - Gertjan Idema - http://drupal.org/user/269240
