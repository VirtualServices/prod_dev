<?php
// $Id: spellchecker_google.inc,v 1.1 2010/12/15 16:31:20 kenorb Exp $

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
function spellchecker_google_op($op, $text, $format = SPELLCHECKER_XML_FORMAT) {
    switch ($op) {
        case 'check':
            $data = spellchecker_google_check_words(array($text));
        break;
        case 'suggest':
            $data = spellchecker_google_suggest($text, $format);
        break;
        case 'check_words':
            $data = spellchecker_google_check_words($text);
        break;
        default:
          $message = t('%func: Operation %op is not supported!', array('%func' => __FUNCTION__, '%op' => $op));
          drupal_set_message($message, 'error');
    }
    return $data;
}

/**
 * Suggest word spellings of text
 * The list is returned as an array of closely matching words.
 */
function spellchecker_google_suggest($text, $format = SPELLCHECKER_XML_FORMAT) {

  $data = '';
  switch ($format) {
    case SPELLCHECKER_XML_FORMAT:
      $data = spellchecker_google_suggest_xml($text);
    break;
    case SPELLCHECKER_ARRAY_FORMAT:
      $data = spellchecker_google_suggest_arr($text);
    break;
    case SPELLCHECKER_JSON_FORMAT:
    default:
      $data = spellchecker_google_suggest_json($text);
    break;
  }

  return $data;
}

/**
 * Suggest word spellings of text
 * The list of closely matching words is returned in XML format
 */
function spellchecker_google_suggest_xml($text, $format = SPELLCHECKER_XML_FORMAT) {

    $url = GOOGLE_SPELL_URL  . "?lang=" . $_GET['lang'];
    $lang = $_REQUEST['lang'] ? $_REQUEST['lang'] : variable_get('spellchecker_pspell_default_language', PSPELL_DEFAULT_LANGUAGE);
    $ignoredups = $_REQUEST['ignoredups'];
    $ignoredigits = $_REQUEST['ignoredigits'] ? $_REQUEST['ignoredigits'] : variable_get('spellchecker_ignoredigits', TRUE);
    $ignoreallcaps = $_REQUEST['ignoreallcaps'] ? $_REQUEST['ignoreallcaps'] : !(int)variable_get('spellchecker_case_sensitive', TRUE);


    $body = '<?xml version="1.0" encoding="utf-8" ?>';
    $body .= '<spellrequest textalreadyclipped="0" ignoredups="1" ignoredigits="' . (int)$ignoredigits . '" ignoreallcaps="' . (int)$ignoreallcaps . '">';
    $body .= '<text>' . $text . '</text>';
    $body .= '</spellrequest>';
    // Use curl if it exists
    if (function_exists('curl_init')) {
      // Use curl
      $ch = curl_init();
      curl_setopt($ch, CURLOPT_URL, $url);
      curl_setopt($ch, CURLOPT_POST, TRUE);
      curl_setopt($ch, CURLOPT_HTTPHEADER, array('Content-length: ' . drupal_strlen($body)));
      curl_setopt($ch, CURLOPT_POSTFIELDS, $body);
      curl_setopt($ch, CURLOPT_RETURNTRANSFER, TRUE);
      curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);
      $xml = curl_exec($ch);
      curl_close($ch);
    } else {
      // Use raw sockets
      $fp = fsockopen("ssl://" . $server, $port, $errno, $errstr, 30);
      if ($fp) {
        // Send request
        fwrite($fp, $header);

        // Read response
        $xml = "";
        while (!feof($fp))
          $xml .= fgets($fp, 128);

        fclose($fp);
      } else
        echo "Could not open SSL connection to google.";
    }
    return $xml;
}

/**
 * Suggest word spellings of text
 * The list is returned as an array of closely matching words.
 */
function spellchecker_google_suggest_arr($text) {

    $lang = $_REQUEST['lang'] ? $_REQUEST['lang'] : variable_get('spellchecker_pspell_default_language', PSPELL_DEFAULT_LANGUAGE);
    $ignoredups = $_REQUEST['ignoredups'];
    $ignoredigits = $_REQUEST['ignoredigits'] ? $_REQUEST['ignoredigits'] : variable_get('spellchecker_ignoredigits', TRUE);
    $ignoreallcaps = $_REQUEST['ignoreallcaps'] ? $_REQUEST['ignoreallcaps'] : !(int)variable_get('spellchecker_case_sensitive', TRUE);
//    $url = GOOGLE_SPELL_URL; //  . "?lang=" . $_GET['lang'];

    /* load classes */
    module_load_include('php', 'spellchecker', 'classes/SpellChecker');
    module_load_include('php', 'spellchecker', 'classes/GoogleSpell');

    /* send request */
    $config = NULL;
    $spell = new GoogleSpell($config);
    $matches_preg_output = $spell->_getMatches($lang, $text, $ignoredigits, $ignoreallcaps, $ignoredups);

    // Get the contents of the <c> tag
    $matches = array_pop($matches_preg_output[0]);

    // Strip line feeds returned from google
    $matches = preg_replace('/[\t]|[\n]|[\r]|[\f]/', ' ', $matches);

    return array($text => explode(' ', $matches));
}

/**
 * Suggest word spellings of text
 * The list of closely matching words is returned as an array that can be converted by JSON
 */
function spellchecker_google_suggest_json($text) {
  $data = spellchecker_google_suggest_arr($text);
  return $data[$text];
}

/**
 * Checks the spelling of a word.
 */
function spellchecker_google_check_words($text) {

    /* load classes */
    module_load_include('php', 'spellchecker', 'classes/SpellChecker');
    module_load_include('php', 'spellchecker', 'classes/GoogleSpell');

    /* send request */
    $config = NULL;
    $spell = new GoogleSpell($config);
    $matches = $spell->checkWords(variable_get('spellchecker_pspell_default_language', PSPELL_DEFAULT_LANGUAGE), $text);

    return $matches;
}

