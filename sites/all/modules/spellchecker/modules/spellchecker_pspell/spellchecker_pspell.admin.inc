<?php
// $Id: spellchecker_pspell.admin.inc,v 1.1 2010/12/15 16:31:20 kenorb Exp $

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
function spellchecker_pspell_get_admin_form() {
  /* SUGGESTION MODE OF PSPELL */
  $options = array();
  $options[PSPELL_FAST] = t('Fast mode (least number of suggestions).');
  $options[PSPELL_NORMAL] = t('Normal mode (more suggestions).');
  $options[PSPELL_BAD_SPELLERS] = t('Slow mode (a lot of suggestions).');
  $form['spellchecker_pspell_suggestion']['spellchecker_pspell_suggestion_mode'] = array(
    '#type' => 'radios',
    '#title' => t('Spelling mode of suggestions returned'),
    '#default_value' => variable_get('spellchecker_pspell_suggestion_mode', PSPELL_DEFAULT_SUGGESTION_MODE),
    '#description' => t("Determines how many suggestions will be returned by pspell_suggest()."),
    '#options' => $options,
  );
  /* ENABLE RUN_TOGETHER option - PSPELL_RUN_TOGETHER */
  $form['spellchecker_pspell_suggestion']['spellchecker_pspell_suggestion_mode_run_together'] = array(
    '#type' => 'checkbox',
    '#title' => t('Consider run-together words as legal compounds.'),
    '#default_value' => variable_get('spellchecker_pspell_suggestion_mode_run_together', TRUE),
    '#description' => t('That is, "thecat" will be a legal compound, although there should be a space between the two words. Changing this setting only affects the results returned by pspell_check(); pspell_suggest() will still return suggestions.'),
  );
  /* DEFAULT LANGUAGE */
  $form['spellchecker_pspell_suggestion']['spellchecker_pspell_default_language'] = array(
    '#type' => 'textfield',
    '#title' => t('Language'),
    '#default_value' => variable_get('spellchecker_pspell_default_language', PSPELL_DEFAULT_LANGUAGE),
    '#description' => t('The language parameter is the language code which consists of the two letter ISO 639 language code (ie "en" or "da") or en_US for American, en_GB for British'),
  );
  /* DEFAULT SPELLING */
  $form['spellchecker_pspell_suggestion']['spellchecker_pspell_default_spelling'] = array(
    '#type' => 'textfield',
    '#title' => t('Spelling'),
    '#default_value' => variable_get('spellchecker_pspell_default_spelling', PSPELL_DEFAULT_SPELLING),
    '#description' => t("The spelling parameter is the requested spelling for languages with more than one spelling such as English. Known values are 'american', 'british', and 'canadian'."),
  );
  /* DEFAULT JARGON */
  $form['spellchecker_pspell_suggestion']['spellchecker_pspell_default_jargon'] = array(
    '#type' => 'textfield',
    '#title' => t('Jargon'),
    '#default_value' => variable_get('spellchecker_pspell_default_jargon', PSPELL_DEFAULT_JARGON),
    '#description' => t("The jargon parameter contains extra information to distinguish two different words lists that have the same language and spelling parameters."),
  );
  /* MAX SUGGESTIONS PER WORD*/
  $form['spellchecker_pspell_suggestion']['spellchecker_pspell_max_suggestions'] = array(
    '#type' => 'textfield',
    '#title' => t('Max suggestions per word'),
    '#default_value' => variable_get('spellchecker_pspell_max_suggestions', PSPELL_MAX_SUGGESTIONS),
    '#description' => t("The maximum number of spelling suggestions to show per misspelt word, in case the suggestions list exceeds the max value."),
  );
  return system_settings_form($form);
}

