<?php
// $Id: spellchecker.admin.inc,v 1.1 2010/12/15 15:55:58 kenorb Exp $

/**
 * @file
 * @brief
 * Implements spellchecker for Drupal
 *
 * @package     spellchecker
 * @subpackage
 * @author
 */

/**
 * Menu callback for the settings form.
 */
function spellchecker_get_admin_form() {

  $config = module_invoke_all('spellchecker');

  /* TYPE OF DICTIONARY */
  $options = array();
  foreach ($config['engine'] as $name => $sett) {
    $options[$name] = $sett['description'];
  }
  if (!empty($options)) {
    $form['spellchecker_mode'] = array(
        '#type' => 'radios',
        '#title' => t('Spellchecker search engine'),
        '#default_value' => variable_get('spellchecker_mode', module_exists('spellchecker_pspell') ? 'PSPELL' : 'GOOGLE'),
        '#description' => t("Select spellchecker engine."),
        '#options' => $options,
    );
  } else {
    $message = t('Please activate at least one spellchecker sub-module, i.e. Spell Checker PSpell.');
    drupal_set_message($message, 'error');
  }

  /* TYPE OF STORAGE */
  $options = array();
  foreach ($config['storage'] as $name => $sett) {
        $options[$name] = $sett['description'];
  }
  if (!empty($options)) {
    $form['spellchecker_storage'] = array(
        '#type' => 'radios',
        '#title' => t('Spellchecker storage mode'),
        '#default_value' => variable_get('spellchecker_storage', 'taxonomy'),
        '#description' => t("Choose storage mode for custom words."),
        '#options' => $options,
    );
  }

  $form['spellchecker_ajax_timeout'] = array(
    '#type' => 'textfield',
    '#title' => t('Spell Checker Update Timeout'),
    '#default_value' => variable_get('spellchecker_ajax_timeout', 500),
    '#field_suffix' => t('ms'),
  );

  $form['spellchecker_ignore_length'] = array(
    '#type' => 'textfield',
    '#title' => t('Words less than number of this characters will be skipped by spell checker.'),
    '#default_value' => variable_get('spellchecker_ignore_length', 3),
  );

  $form['spellchecker_case_sensitive'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable case sensitive spelling.'),
    '#default_value' => variable_get('spellchecker_case_sensitive', TRUE),
  );                

  $form['spellchecker_ignoredigits'] = array(
    '#type' => 'checkbox',
    '#title' => t('Ignore any words containing embedded digits.'),
    '#default_value' => variable_get('spellchecker_ignoredigits', TRUE),
  );   

  $form['spellchecker_show_warning'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show warning for invalid words on validation.'),
    '#default_value' => variable_get('spellchecker_show_warning', TRUE),
  );

  return system_settings_form($form);
}

