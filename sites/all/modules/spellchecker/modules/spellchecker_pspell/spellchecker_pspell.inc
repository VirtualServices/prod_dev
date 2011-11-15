<?php
// $Id: spellchecker_pspell.inc,v 1.1 2010/12/15 16:31:20 kenorb Exp $

/**
 * @file
 * @brief
 *   Include file
 *
 * @package 	spellchecker
 * @subpackage
 * @author
 */

/**
 * Implementation of hook_spellchecker callback
 */
function spellchecker_pspell_op($op, $text, $format = SPELLCHECKER_JSON_FORMAT) {
  switch ($op) {
    case 'check':
      $data = spellchecker_pspell_check($text);
      break;
    case 'suggest':
      $data = spellchecker_pspell_suggest($text, $format);
      break;
    case 'invalid_list':
      module_load_include('inc', 'spellchecker');
      $data = spellchecker_get_invalid_list($text);
      break;
    case 'check_words':
      $data = spellchecker_pspell_check_words($text);
      break;
    default:
      $message = t('%func: Operation %op is not supported!', array('%func' => __FUNCTION__, '%op' => $op));
      drupal_set_message($message, 'error');
  }
  return $data;
}

/**
 * Implementation of hook_spellchecker storage callback
 */
function spellchecker_pspell_storage($op, $word) {
  $word = spellchecker_trim($word);
  switch ($op) {
    case 'add':
      $data = spellchecker_pspell_add_new_word_session($word);
      break;
    case 'remove':
      /* TODO: remove word from personal wordlist */
      break;
    case 'list':
      /* TODO: list all words from personal wordlist */
      break;
    default:
      $message = t('%func: Operation %op is not supported!', array('%func' => __FUNCTION__, '%op' => $op));
      drupal_set_message($message, 'error');
  }
  module_invoke_all('spellchecker_op', $op, $word);
  return $data;
}

/**
 * Initialise dictionary
 */
function spellchecker_pspell_init_dictionary($pspell_lang = NULL, $pspell_spelling = NULL, $reset = FALSE) {

  /* get dictionary link */
  static $pspell_link_arr = array(); // static version (slower)
  // $pspell_link_arr = &$_SESSION['pspell_link']; // read pspell list from the current session (doesn't work - 194 is not a PSPELL result index)

  /* assign default values */
  if (empty($pspell_lang)) {
    $pspell_lang = variable_get('spellchecker_pspell_default_language', PSPELL_DEFAULT_LANGUAGE);
    $pspell_spelling = variable_get('spellchecker_pspell_default_spelling', PSPELL_DEFAULT_SPELLING);
  }
  $pspell_link = &$pspell_link_arr[$pspell_lang][$pspell_spelling]; // point to specified dictionary

  /* reset in necessary */
  if ($pspell_link && $reset) {
    pspell_clear_session($pspell_link_arr[$pspell_lang][$pspell_spelling]); // the current wordlist becomes blank
    unset($pspell_link_arr[$pspell_lang][$pspell_spelling]);
  }

  if (!$pspell_link) {
    $sugg_mode = variable_get('spellchecker_pspell_suggestion_mode_run_together', TRUE) ? (variable_get('spellchecker_pspell_suggestion_mode', PSPELL_DEFAULT_SUGGESTION_MODE) | PSPELL_RUN_TOGETHER) : (variable_get('spellchecker_pspell_suggestion_mode', PSPELL_DEFAULT_SUGGESTION_MODE));
    /* Note: Drupal uses UTF-8 for encoding all its data. This is a Unicode encoding, so it can contain data in any language. */
    $jargon = ''; // $jargon = variable_get('spellchecker_pspell_default_jargon', PSPELL_DEFAULT_JARGON); // activate if necessary
    $pspell_link = pspell_new($pspell_lang, $pspell_spelling, $jargon, "utf-8", $sugg_mode);
    if (!$pspell_link) {
      $message = t('Error initialising new dictionary!');
      drupal_set_message($message, 'error');
    }
    $wordlist = spellchecker_op('list', NULL, 'storage');
    $errorlist = array();
    foreach ($wordlist as $tid => $word) { // one day, we'll use a literal dictionary, not session.
      if (!spellchecker_pspell_add_new_word_session($word, $pspell_link)) {
        $errorlist[] = $word;
      }
    }
    if (!empty($errorlist)) {
      $message = t('Error adding following words into dictionary: %words', array('%words' => implode(',', $errorlist)));
      drupal_set_message($message, 'error');
    }
  }
  return $pspell_link;
}

/**
 * Add new word into pspell session
 */
function spellchecker_pspell_add_new_word_session($word, $pspell_link = NULL) {
  if (!$pspell_link) {
    $pspell_link = spellchecker_pspell_init_dictionary();
  }
  $word = spellchecker_trim($word);
  $word = preg_replace("/[^a-zA-Zs]/", "", $word); // remove invalid characters (like '_', '/', digits, etc.)
  if (!variable_get('spellchecker_case_sensitive', TRUE)) {
    $word = drupal_strtolower($word); // convert to lower case, if we have case insensitive
  }
  return pspell_add_to_session($pspell_link, $word);
}

/**
 * Checks the spelling of a word.
 * If the function returns true, then the word matches a spelling in the specified dictionary; if false is returned, then there was no match, suggesting the word may be misspelt.
 */
function spellchecker_pspell_check($word, $pspell_link = NULL) {
  if (!$pspell_link) {
    $pspell_link = spellchecker_pspell_init_dictionary();
  }
  return pspell_check($pspell_link, preg_replace("/[^a-zA-Zs\-']/", "", $word));
}

/**
 * Checks the spellings of an array of words
 *
 * Returns an array of words that do not match a spelling in the specified dictionary
 *
 * @param $words
 */
function spellchecker_pspell_check_words($words) {
  $ret = array();
  foreach ($words as $word) {
    if (!spellchecker_pspell_check($word) && spellchecker_is_word($word)) {
      $ret[] = $word;
    }
  }

  return $ret;
}

/**
 * Suggest spellings of a word.
 * The list is returned as an array of closely matching words.
 */
function spellchecker_pspell_suggest($text, $format = SPELLCHECKER_JSON_FORMAT) {
  $pspell_link = spellchecker_pspell_init_dictionary();
  // Replace special chars with spaces
  $ignore_digits = variable_get('spellchecker_ignoredigits', TRUE) ? '[0-9]|' : '';
  $text_n = preg_replace('/(' . $ignore_digits . '[' . SPELLCHECKER_SEPARATORS . '])/', ' ', $text);

  if ($format == SPELLCHECKER_XML_FORMAT) {
    $text_arr = explode(' ', $text_n);
    $suggestions = array();
    foreach(explode(' ', $text_n) as $word) {
      // Search for a spelling suggestion if:
      //   - the word is greater than length 2
      //   - the word does not exist in the dictionary
      if (spellchecker_is_word($word) && !spellchecker_check($word)) { 
        $suggestions[$word] = pspell_suggest($pspell_link, $word);
        _spellchecker_pspell_limit_suggestions($suggestions[$word],
                                               variable_get('spellchecker_pspell_max_suggestions', PSPELL_MAX_SUGGESTIONS));
      } else {
        unset($suggestions[$word]);
      }
    }
    module_load_include('inc', 'spellchecker'); // used for spellchecker_generate_suggestions_xml()
    $res = spellchecker_generate_suggestions_xml($suggestions, $text);
  } else {
    $res = pspell_suggest($pspell_link, $text_n);
    _spellchecker_pspell_limit_suggestions($res,
                                           variable_get('spellchecker_pspell_max_suggestions', PSPELL_MAX_SUGGESTIONS));
  }
  return $res;
}

/**
 * Limit the number of suggestions
 *
 * @param $suggestions The array of suggestions for a word
 * @param $limit The max number of suggestions to limit the input array to
 */
function _spellchecker_pspell_limit_suggestions(&$suggestions, $limit = PSPELL_MAX_SUGGESTIONS) {
  if (sizeof($suggestions) > $limit) {
    array_splice($suggestions, $limit);
  }
}

/**
 * Check the passed quote for spelling errors.
 */
function spellchecker_pspell_init_config($pspell_link, $sugg_mode = PSPELL_DEFAULT_SUGGESTION_MODE) {
  if (!$sugg_mode) {
    $sugg_mode = variable_get('spellchecker_pspell_suggestion_mode_run_together', TRUE) ? (variable_get('spellchecker_pspell_suggestion_mode', PSPELL_DEFAULT_SUGGESTION_MODE) | PSPELL_RUN_TOGETHER) : (variable_get('spellchecker_pspell_suggestion_mode', PSPELL_DEFAULT_SUGGESTION_MODE));
  }
  pspell_config_mode($pspell_link, $mode);
/*
  $pspell_link = spellchecker_pspell_init_dictionary();
  $pspell_config = pspell_config_create("en", 'british', );
  pspell_config_mode($pspell_config, PSPELL_FAST);
  $pspell_link = pspell_new_config($pspell_config);
  pspell_check($pspell_link, "thecat");*/
}

