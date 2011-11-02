$Id: README.txt,v 1.3 2008/05/12 07:11:53 arancaytar Exp $

DESCRIPTION
-----------

DHTML Menus uses Javascript DHTML to reduce the number of page loads when using
nested menus; this is particularly useful with Drupal's administration system.

Ordinarily in Drupal, to expand a menu with sub-items, you need to click a 
static link and load a full page with the expanded menu in it.
With DHTML Menus, instead the sub-items are expanded dynamically when you click 
on each item. Additionally, the module uses a cookie to remember which menus are 
open and whih are closed, so as you navigate around the site your menus remain
consistent.


INSTALLATION
------------

1) Copy the dhtml_menu directory into your sites/all/modules directory.
2) Enable the module at administer >> site building >> modules
3) All menu blocks will be converted automatically to DHTML menus.
4) On administer >> site building >> blocks, you can enable and disable
   this for each block separately as the menu blocks now have an additional
   DHTML option in their settings.
5) Disabling the module will remove DHTML behavior from all your blocks immediately.

Enjoy!


CREDITS
-------

Arancaytar                     <http://drupal.org/user/28680>
Bruno Massa - "brmassa"        <http://drupal.org/user/67164>
Earl Miles  - "merlinofchaos"  <http://drupal.org/user/26979>
