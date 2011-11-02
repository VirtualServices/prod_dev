<?php
// $Id: dhtml_menu.admin.inc,v 1.3 2008/06/30 15:08:57 arancaytar Exp $

/**
 * @file dhtml_menu.admin.inc
 * Configuration forms
 */

/**
 * DHTML Menu Settings page.
 *
 * @ingroup form
 */
function dhtml_menu_settings() {
  $form['dhtml_menu_menus_'] = array(
    '#type'           => 'radios',
    '#title'          => t('Use DHTML Effect'),
    '#options'        => array('enable' => t('Enable on all menus.'), 0 => t('Keep current configuration.'), 'disable' => t('Disable on all menus.')),
    '#default_value'  => 0,
  );
  $form['#submit'][] = 'dhtml_menu_settings_submit';

  $form['dhtml_menu_use_effects'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('jQuery slide effect'),
    '#description'    => t('Menu slides when it opens and closes.'),
    '#default_value'  => variable_get('dhtml_menu_use_effects', 0),
  );
  
  $form['dhtml_menu_hide_siblings'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Hide Siblings effect'),
    '#description'    => t('Close all other menu items when one opens.'),
    '#default_value'  => variable_get('dhtml_menu_hide_siblings', 0),
  );

  $form['dhtml_menu_duplicated'] = array(
    '#type'           => 'textarea',
    '#title'          => t('Duplicated menu items'),
    '#description'    => t('You can specify a list of menu items that should be displayed twice: Once as a parent item that expands and collapses the sub-menu and again as its own child item, which points directly to the page (the parent items still links on double-click). Enter one internal path (like "admin") on each line. To specify a custom link text, put it after the path, separated by a space.'),
    '#rows'           => 4,
    '#default_value'  => variable_get('dhtml_menu_duplicated', DHTML_MENU_DUPLICATE_DEFAULT),
  );

  return system_settings_form($form);
}

function dhtml_menu_settings_submit($form, &$form_state) {
  variable_del('dhtml_menu_menus_');
  $state = $form_state['values']['dhtml_menu_menus_'];

  if ($state == 'enable') {
    $dhtml['user'][1] = 1;
    foreach (module_invoke('menu', 'block', 'list') as $delta => $block) {
      $dhtml[$delta] = 1;
    }
    variable_set('dhtml_menu_menus', $dhtml);
    drupal_set_message(t('DHTML has been enabled for all menus.'));
  }
  else if ($state == 'disable') {
    variable_set('dhtml_menu_menus', array());
    drupal_set_message(t('DHTML has been disabled for all menus.'));
  }
}
