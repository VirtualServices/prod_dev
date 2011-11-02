
This module allows you to display an image in a block where the selection
of the image is based on conditions per image.

Each image, included in a node, can be shown based on node id, path,
taxonomy, book or PHP code. Headerimage has an arbitrary node type to work
with.

Multiple images (nodes) can be displayed in one block, with each image
having its own conditions. Using a weight per node the order of selection
can be controlled.


-- INSTALLATION --

* Copy header image module to your modules directory and enable it on the
  admin modules page.

* Set access rights for admin and view on the access control page.

* If required create a node type to put your image in.
  Set node type(s) and conditions types on the header image settings page
  Site configuration > Header image > Settings
  
* Create one or more blocks for header image at the add block page.
  Site configuration > Header image > Add

* Using the selected node type, upload one image into the node using a CCK
  image field.
  Image header is designed to work with a node containing only one image.
  However with custom theming you can display any node content and any
  part of the node content. By default the node content without the
  title, taxonomy or links is shown in teaser view.

* Enter the display conditions to the node.
  Different type of conditions are available: node id, URL, taxonomy, etc. 
  Select the conditions types you will use (site wide) on the header image 
  settings page: Site configuration > Header image > Settings

  The header image node will be displayed when one of it's display conditions 
  are met (evaluated to TRUE). Empty or not selected conditions are evaluated 
  false.

  For testing purpose it may be convenient to show a block on all pages. 
  See Troubleshooting below.

* Change the weight of the display condition to influence the order in which 
  display conditions are evaluated from one node to another. For example, 
  a header image node with weight 0 is evaluated before a header image with 
  weight 1.

* Assign the block to the region of your choice in the Blocks administration
  page.
  Site building > Blocks


-- TROUBLESHOOTING --

* Use the condition weight to control the order in which the conditions are
  evaluated. The node with the smallest weight number will be evaluated
  first.
  Tip: To use one image as default give it a high weight (10) and a
  condition always evaluating to true.
  For example: path: '*' or PHP: <?php return true; ?>

* If the block is (partly) not visible, check the Display settings of your
  node type. Header image uses the teaser view or full view depending
  on the Teaser setting.
  Administer > Content management > Content types > MyHeaderImageContentType > Display fields


-- THEMING --

* By default the block will show the content of the node, without the
  title, taxonomy or links. The node is shown in teaser view or full view
  depending on the Teaser setting.

* To show the full node with title, taxonomy, etc. use the theme function
  included at the bottom of this README file.


-- AUTHOR --

Erik Stielstra
For contact: http://drupal.org/user/73854


-- THEMING SNIPPET --
To customize the theming of the Header Image block, copy the snippet below in your 
theme template.php file and modify the code to your needs.

/**
 * Custom implementation of theme_headerimage_block()
 *
 * To show the full node with title, taxonomy, etc. 
 */
function phptemplate_headerimage_block($node, $teaser = true) {
  if (!$node->status) {
    $output  = '<div class="node-unpublished">';
  }

  if (module_exists('taxonomy')) {
    $terms = taxonomy_link('taxonomy terms', $node);
  }

  $output .= t('!title by !name', array('!title' => '<h2 class="title">'. check_plain($node->title) .'</h2>', '!name' => theme('username', $node)));

  if (count($terms)) {
    $output .= ' <small>('. theme('links', $terms) .')</small><br />';
  }

  if ($teaser && $node->teaser) {
    $output .= $node->teaser;
  }
  else {
    $output .= $node->body;
  }  

  if ($node->links) {
    $output .= '<div class="links">'. theme('links', $node->links) .'</div>';
  }

  if (!$node->status) {
    $output .= '</div>';
  }

  return $output;
}
