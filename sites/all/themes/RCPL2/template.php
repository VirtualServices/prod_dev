<?php

jq_add('cluetip');
drupal_add_js('sites/all/themes/RCPL2/js/cluetip.js');
drupal_add_js('sites/all/themes/RCPL2/js/jquery.url.js');
drupal_add_js('sites/all/themes/RCPL2/js/summer-reading.js');

function RCPL2_links($links, $attributes = array()) {
  if (isset($links['blog_usernames_blog'])) {
    unset($links['blog_usernames_blog']);
  }
  return theme_links($links, $attributes);
}
function custom_login_block() {
  $form = array(
    '#action' => url($_GET['q'], array('query' => drupal_get_destination())),
    '#id' => 'user-login-form',
    '#validate' => user_login_default_validators(),
    '#submit' => array('user_login_submit'),
  );
  $form['name'] = array('#type' => 'textfield',
    '#title' => t(''),
    '#maxlength' => USERNAME_MAX_LENGTH,
    '#size' => 15,
    '#required' => TRUE,
  );
  $form['pass'] = array('#type' => 'password',
    '#title' => t(''),
    '#maxlength' => 60,
    '#size' => 15,
    '#required' => TRUE,
  );
  $form['submit'] = array('#type' => 'submit', '#value' => t('Log in'), );
  
  $items = array();
  if (variable_get('user_register', 1)) {
    $items[] = l(t('Create new account'), 'user/register', array('title' => t('Create a new user account.')));
  }
  $items[] = l(t('Lost password?'), 'user/password', array('title' => t('Request new password via e-mail.')));
  $form['links'] = array('#value' => theme('item_list', $items));
  return $form;
}

function theme023_user_bar() {
 global $user;                                                               
 $output = '';

 if (!$user->uid) {                                                          
   $output .= drupal_get_form('custom_login_block');                           
 }                                                                           
 else {                                                                      
   $output .= t('<p class="user-info">Hi !user, welcome back.</p>', array('!user' => theme('username', $user)));

   $output .= theme('item_list', array(
     l(t('Your account'), 'user/'.$user->uid, array('title' => t('Edit your account'))),
     l(t('Sign out'), 'logout')));
 }
  
 $output = '<div id="user-bar">'.$output.'</div>';
    
 return $output;
}
// split out taxonomy terms by vocabulary
function RCPL2_taxonomy_links($node, $vid) {
  if (count($node->taxonomy)){
    $tags = array();
    foreach ($node->taxonomy as $term) {
       if ($term->vid == $vid){
          $tags[] = array('title' => $term->name, 'href' => taxonomy_term_path($term), 'attributes' => array('rel' => 'tag'));
       }
}
    if ($tags){
      return theme_links($tags, array('class'=>'links inline'));
    }
  }
}


?>