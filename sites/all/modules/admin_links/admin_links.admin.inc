<?php
// $Id: admin_links.admin.inc,v 1.1.2.10 2010/02/03 20:28:22 davereid Exp $

/**
 * @file
 * Admin page callbacks for the admin_links module.
 */

/**
 * Administration settings form.
 *
 * @see system_settings_form()
 */
function admin_links_settings_form() {
  $form['admin_links_link_tasks'] = array(
    '#type' => 'checkbox',
    '#title' => t('Move node local tasks into the teaser links.'),
    '#default_value' => variable_get('admin_links_link_tasks', 1),
  );
  $form['admin_links_link_new'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add a link to create a new content of the same type (for example, <em>New Story</em>).'),
    '#default_value' => variable_get('admin_links_link_new', 0),
  );
  $form['admin_links_exclude'] = array(
    '#type' => 'textarea',
    '#title' => t('Exclude the following content local tasks or links from being added to the content teaser links'),
    '#description' => t('One path per line. You can use <em>*</em> as a wildcard.'),
    '#default_value' => variable_get('admin_links_exclude', implode("\n", array('node/*/view', 'node/*/devel/*'))),
    '#wysiwyg' => FALSE,
  );
  $form['admin_links_universaledit'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add support for the <a href="@ueb">Universal Edit Button</a> on editable content.', array('@ueb' => 'http://universaleditbutton.org/')),
    '#default_value' => variable_get('admin_links_universaledit', 0),
  );

  return system_settings_form($form);
}
