<?php
// $Id: spellchecker_enchant.inc,v 1.1 2010/12/15 16:31:20 kenorb Exp $

/**
 * @file
 * @brief
 *   Include file
 *
 * @package 	spellchecker
 * @subpackage
 * @author    Rafal Wieczorek
 */

/**
 * Implementation of hook_spellchecker callback
 */
function spellchecker_enchant_op($op, $word, $format = SPELLCHECKER_JSON_FORMAT) {
  switch ($op) {
    case 'check':
      $data = spellchecker_enchant_check($word);
      break;
    case 'suggest':
      $data = spellchecker_enchant_suggest($word, $format);
      break;
    case 'invalid_list':
      module_load_include('inc', 'spellchecker');
      $data = spellchecker_get_invalid_list($text);
      break;
    case 'check_words':
      // TODO: $data = spellchecker_enchant_check_words($text);
      break;
    default:
      $message = t('%func: Operation %op is not supported!', array('%func' => __FUNCTION__, '%op' => $op));
      drupal_set_message($message, 'error');
  }
  return $data;
}

/**
 * Initialise dictionary
 */
function spellchecker_enchant_init_dictionary($enchant_lang = NULL, $enchant_spelling = NULL, $reset = FALSE) {

  /* get dictionary link */
  static $enchant_link_arr = array(); // static version (slower)

  /* assign default values */
  if (empty($enchant_lang)) {
    $enchant_lang = variable_get('spellchecker_pspell_default_language', PSPELL_DEFAULT_LANGUAGE);
    $enchant_spelling = variable_get('spellchecker_pspell_default_spelling', PSPELL_DEFAULT_SPELLING);
  }
  $enchant_link = &$enchant_link_arr[$enchant_lang][$enchant_spelling]; // point to specified dictionary

  /* reset in necessary */
  if ($enchant_link && $reset) {
    enchant_broker_free_dict($enchant_link_arr[$enchant_lang][$enchant_spelling]); // the current wordlist becomes blank
    unset($enchant_link_arr[$enchant_lang][$enchant_spelling]);
  }

  if (!$enchant_link) {
    $sugg_mode = variable_get('spellchecker_enchant_suggestion_mode_run_together', TRUE) ? (variable_get('spellchecker_enchant_suggestion_mode', PSPELL_DEFAULT_SUGGESTION_MODE) | PSPELL_RUN_TOGETHER) : (variable_get('spellchecker_enchant_suggestion_mode', PSPELL_DEFAULT_SUGGESTION_MODE));
    /* Note: Drupal uses UTF-8 for encoding all its data. This is a Unicode encoding, so it can contain data in any language. */
    $jargon = ''; // $jargon = variable_get('spellchecker_enchant_default_jargon', PSPELL_DEFAULT_JARGON); // activate if necessary
    $enchant_link = enchant_broker_init();
    if (!enchant_broker_dict_exists($enchant_link, $enchant_lang)) {
      $message = t('Error initialising new dictionary!');
      drupal_set_message($message, 'error');
    }
    $wordlist = spellchecker_op('list', NULL, 'storage');
    $errorlist = array();
    foreach ($wordlist as $tid => $word) { // one day, we'll use a literal dictionary, not session.
      if (!enchant_dict_add_to_session($enchant_link, spellchecker_trim($word))) {
        $errorlist[] = $word;
      }
    }
    if (!empty($errorlist)) {
      $message = t('Error adding following words into dictionary: %words', array('%words' => implode(',', $errorlist)));
      drupal_set_message($message, 'error');
    }
  }
  return $enchant_link;
}

/**
 * Add new word into taxonomy vocabulary
 */
function spellchecker_enchant_add_new_word($word) {
  $enchant_link = spellchecker_enchant_init_dictionary();
  return enchant_dict_add_to_session($enchant_link, $word);
}

/**
 * Checks the spelling of a word.
 * If the function returns true, then the word matches a spelling in the specified dictionary; if false is returned, then there was no match, suggesting the word may be misspelt.
 */
function spellchecker_enchant_check($word) {
  $enchant_link = spellchecker_enchant_init_dictionary();
  return enchant_dict_check($enchant_link, $word);
}

/**
 * Suggest spellings of a word.
 * The list is returned as an array of closely matching words.
 */
function spellchecker_enchant_suggest($text, $format = SPELLCHECKER_XML_FORMAT) {

  $enchant_link = spellchecker_enchant_init_dictionary();

  $data = '';
  switch ($format) {
    case SPELLCHECKER_XML_FORMAT:
      $data = spellchecker_enchant_suggest_xml($text);
    break;
    case SPELLCHECKER_ARRAY_FORMAT:
      $data = spellchecker_enchant_suggest_arr($text);
    break;
    case SPELLCHECKER_JSON_FORMAT:
    default:
      $data = spellchecker_enchant_suggest_json($text);
    break;
  }

  return $data;
}

/**
 * Suggest word spellings of text
 * The list is returned as an array of closely matching words.
 */
function spellchecker_enchant_suggest_arr($text) {
  /* FIXME: this function is not tested */

    $lang = $_REQUEST['lang'] ? $_REQUEST['lang'] : $language->language;

    /* load classes */
    module_load_include('php', 'spellchecker', 'classes/SpellChecker');
    module_load_include('php', 'spellchecker', 'classes/EnchantSpell');

    /* send request */
    $spell = new EnchantSpell;
    $matches = $spell->getSuggestions($lang, $text);

    return $matches;
}

/**
 * Suggest word spellings of text
 * The list of closely matching words is returned in XML format
 */
function spellchecker_enchant_suggest_xml($text) {
  /* FIXME: this function is not tested */

  module_load_include('inc', 'spellchecker'); // used for spellchecker_generate_suggestions_xml()
  $suggestions = spellchecker_enchant_suggest_arr($text);
  $res = spellchecker_generate_suggestions_xml($suggestions, $text);
  return $res;
}

/**
 * Suggest word spellings of text
 * The list of closely matching words is returned in JSON format
 */
function spellchecker_enchant_suggest_json($text) {
  $data = spellchecker_enchant_suggest_arr($text);
  module_load_include('php', 'spellchecker', 'classes/JSON');
  $json = new Moxiecode_JSON();
  $output = $json->encode($data);

  print "TODO: Enchant for TinyMCE doesn't supported yet!"; $output = FALSE;

  return $output;
}

