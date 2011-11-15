<?php
// $Id: spellchecker.inc,v 1.2 2010/12/16 12:40:01 kenorb Exp $

/**
 * @file
 * @brief
 *   Include file
 *
 * @package 	spellchecker
 * @subpackage
 * @author
 */

define('SPELLCHECKER_VOCAB', 'Spellchecker wordlist'); // Vocabulary used for keeping new words

/**
 * Suggest spellings of a word.
 * The list is returned as an array of closely matching words.
 */
function spellchecker_op($op, $text = '', $type = 'engine', $format = SPELLCHECKER_JSON_FORMAT) {
  $config = module_invoke_all('spellchecker');
  switch ($type) {
    case 'engine':
      $curr_engine = variable_get('spellchecker_mode', module_exists('spellchecker_google') ? 'google' : NULL);
      break;
    case 'storage':
      $curr_engine = variable_get('spellchecker_storage', 'taxonomy');
      break;
  }
  $data = array();
  if ($engine_conf = $config[$type][$curr_engine]) {
    if ($file = $engine_conf['file']) {
      require_once drupal_get_path('module', $config['module'][$curr_engine]) . "/$file";
    }
    $data = call_user_func($engine_conf['callback'], $op, $text, $format);
  }
  return $data;
}

/**
 * Checks the spelling of a word.
 * If the function returns true, then the word matches a spelling in the specified dictionary; if false is returned, then there was no match, suggesting the word may be misspelt.
 */
function spellchecker_check($word) {
  if (drupal_strlen($word) < variable_get('spellchecker_ignore_length', 3)) {
    return TRUE;
  } else {
    return spellchecker_op('check', $word) || spellchecker_op('check', $word, 'storage');
  }
}

/**
 * Get a list of words not in the dictionaries are return them.
 */
function spellchecker_get_invalid_list($words) {
  $pspell_link = spellchecker_pspell_init_dictionary();

  $wordlist = spellchecker_op('list', NULL, 'storage');

  module_load_include('inc', 'spellchecker');
  if (is_string($words)) {
    $words = spellchecker_text2array($words);
  }
  $invalid_words = array();
  foreach ($words as $tid => $word) {
    if (spellchecker_is_word($word) && !spellchecker_check($word)) {
      $invalid_words[] = $word;
    }
  }

  return $invalid_words;
}

/**
 * Suggest spellings of a word.
 * The list is returned as an array of closely matching words.
 */
function spellchecker_suggest($text) {
  return spellchecker_op('suggest', $text);
}

/**
 * Add new word into dictionary
 */
function spellchecker_add($word) {
  return spellchecker_op('add', $word, 'storage');
}

/**
 * Remove the word into dictionary
 */
function spellchecker_remove($word) {
  return spellchecker_op('remove', $word, 'storage');
}

/**
 * Convert vocabulary name into vid
 */
function spellchecker_get_vid_by_name($name) {
  return db_result(db_query('SELECT vid FROM vocabulary WHERE name = "%s"', $name));
}

/**
 * Add default vocabulary
 */
function spellchecker_add_vocab($name = 'Spellchecker wordlist', $save_vid = TRUE) {
  $vocabulary = array(
      'name'  => SPELLCHECKER_VOCAB, //Human readable name of the vocabulary
      'multiple' => 0, //set 1 to allow multiple selection
      'required' => 0, //set 1 to make the terms mandatory to be selected
      'hierarchy' => 0, //set 1 to allow and create hierarchy of the terms within the vocabulary
      'relations' => 0, //set 1 to set and allow relation amongst multiple terms
      'module' => 'spellchecker', //provide the module name in which the vocabulary is defined and which is calling this function
      'nodes' => array('page' => 1), //set the node to which this vocabulary will be attached to
      'weight' => 2, //set the weight to display the vocabulary in the list
  );
  taxonomy_save_vocabulary($vocabulary); //saving the $vocabulary array will create a vocabulary named "Currency". Note the t() in the 'name', this will allow to translate the vocabulary name to different languages.
  if ($save_vid) {
    variable_set('spellchecker_vid', $vocabulary['vid']); // save vid of this vocab
  }
}

/**
 * Add default vocabulary
 */
function spellchecker_del_vocab($name = 'Spellchecker wordlist') {
  $vid = spellchecker_get_vid_by_name($name);
  taxonomy_del_vocabulary($vid);
}

/**
 * Prepare the word before adding
 */
function spellchecker_trim($word) {
  return variable_get('spellchecker_case_sensitive', TRUE) ? trim($word) : drupal_strtolower(trim($word));
}

/**
 * Prepare the word before adding
 */
function spellchecker_text2array($text) {
  $words = array();
  preg_match_all("/\S+/", $text, $words);
  return $words[0];
}


/**
 * Implementation of menu callback
 */
function spellchecker_ajax_callback() {
  $text = urldecode($_REQUEST['text']);
  // Clean the string. Eg. if it contains £ or other ascii encoded chars
  $text = function_exists('mb_convert_encoding') ? mb_convert_encoding($text,'ASCII') : spellchecker_mb_convert_encoding($text,'ASCII', 'UTF-8');
  $op = $_REQUEST['op'] ? urldecode($_REQUEST['op']) : 'suggest';
  $data = spellchecker_op($op, $text, 'engine', SPELLCHECKER_XML_FORMAT);

  print $data;
  exit;
}

/**
 * PHP implementation of mb_convert_encoding
 * It only suports utf7, 8 e iso 8859-1: 
 */
function spellchecker_mb_convert_encoding($string, $to, $from) { 
  // Convert string to ISO_8859-1 
  if ($from == "UTF-8") {
    $iso_string = utf8_decode($string); 
  } else {
    if ($from == "UTF7-IMAP") {
      $iso_string = imap_utf7_decode($string); 
    } else {
      $iso_string = $string; 
    }
  }
  // Convert ISO_8859-1 string to result coding 
  if ($to == "UTF-8") {
          return(utf8_encode($iso_string)); 
  } else {
    if ($to == "UTF7-IMAP") {
      return(imap_utf7_encode($iso_string)); 
    } else {
      return($iso_string); 
    }
  }
}

/**
 * Filter html tags and tokens from string
 */
function spellchecker_filter($text, $html = TRUE, $token = TRUE) {
  if ($token && module_exists('token')) {
    $text = str_replace(array('[',']'), array('<','>'), $text);
  }
  /* Be aware that tags constitute visual whitespace, so stripping may leave the resulting text looking misjoined. */
  $text = preg_replace('/</',' <', $text); // add space before each tag
  $text = preg_replace('/>/','> ', $text); // add space after each tag
  $text = html_entity_decode(strip_tags($text));
  $text = preg_replace('/[\n\r\t]/',' ', $text);
  $text = preg_replace('/  /',' ', $text);
  return $text;
}

/**
 * Return TRUE if word is a valid word based on settings
 */
function spellchecker_is_word($word) {
  $res = TRUE;
  $pattern = sprintf("^[%sA-Za-z\-]*$", (variable_get('spellchecker_ignoredigits', TRUE) ? '0-9' : ''));
  $res = $res & (bool)ereg($pattern, $word); // check for invalid characters
  $res = $res & (bool)(strlen($word) >= variable_get('spellchecker_ignore_length', 3)); // check if length is correct
  return (bool)$res;
}

/**
 * Implementation of menu callback
 */
function spellchecker_confirm($fields) {
  module_load_include('php', 'spellchecker', 'classes/JSON');

  foreach($fields as $key => $value) {
    $data = spellchecker_op('invalid_list', $value, 'engine', SPELLCHECKER_XML_FORMAT);
    if ($data) {
      $text .= 'Invalid words found in ' . $key . ':<br/>' . implode(', ', $data) . '<br/><br/>';
    }
  }
  return $text;

}

/**
 * Implementation of menu callback
 */
function spellchecker_ajax_wysiwyg_callback() {
  /**
   * Get input data
   */
  $raw = "";

  // Try param
  if (isset($_POST["json_data"]))
  $raw = getRequestParam("json_data");

  // Try globals array
  if (!$raw && isset($_GLOBALS) && isset($_GLOBALS["HTTP_RAW_POST_DATA"]))
  $raw = $_GLOBALS["HTTP_RAW_POST_DATA"];

  // Try globals variable
  if (!$raw && isset($HTTP_RAW_POST_DATA))
  $raw = $HTTP_RAW_POST_DATA;

  // Try stream
  if (!$raw) {
    if (!function_exists('file_get_contents')) {
      $fp = fopen("php://input", "r");
      if ($fp) {
        $raw = "";
        while (!feof($fp))
        $raw = fread($fp, 1024);
        fclose($fp);
      }
    } else
    $raw = "" . file_get_contents("php://input");
  }

  // No input data
  if (!$raw)
  die('{"result":null,"id":null,"error":{"errstr":"Could not get raw post data.","errfile":"","errline":null,"errcontext":"","level":"FATAL"}}');

  // Get JSON data
  // module_load_include('inc', 'spellchecker');
  module_load_include('php', 'spellchecker', 'classes/JSON');
  $json = new Moxiecode_JSON();
  $input = $json->decode($raw);

  $data = '';
  $input_text = $input['params'][1];
  switch ($input['method']) {
    case 'checkWords':
      $data = spellchecker_op('check_words', $input_text);
      break;
    case 'getSuggestions':
      $data = spellchecker_op('suggest', $input_text, 'engine', SPELLCHECKER_JSON_FORMAT);
      break;
    case 'learnWord':
      $data = spellchecker_op('add', $input_text, 'storage');
      break;
  }

  // Request and response id should always be the same
  $output = array(
        "id" => $input->id,
        "result" => $data,
        "error" => ($data === FALSE),
  );

  // Return JSON encoded string
  print $json->encode($output);
  exit;
}

/**
 * Implementation of menu callback
 */
function spellchecker_ajax_op_callback() {
  $op = arg(1);
  $word = urldecode($_REQUEST['word']);
  $data = spellchecker_op($op, $word, 'storage', SPELLCHECKER_XML_FORMAT);
  print json_encode((bool)$data);
  exit;
}

/**
 * Render pspell suggestions as google api xml format
 *
 * @param $suggestions Array of suggestions returned from pspell
 * @param $text The complete input text field value
 */
function spellchecker_generate_suggestions_xml($suggestions, $text) {
  /* Sample output;
   *
   * <?xml version="1.0" encoding="UTF-8"?>
   * <spellresult error="0" clipped="0" charschecked="27">
   *  <c o="13" l="8" s="1">teachers teaches techies teacher\'s techs</c>
   *  <c o="22" l="5" s="1">treat  Tera  Tresa treas Terra Trey</c>
   * </spellresult>;
   */
  $writer = new XMLWriter();
  $writer = xmlwriter_open_memory();
  xmlwriter_start_document($writer, '1.0', 'UTF-8');

  // <spellresult> top element
  xmlwriter_start_element($writer, 'spellresult');
  xmlwriter_write_attribute($writer, 'error', 0);
  xmlwriter_write_attribute($writer, 'clipped', 0);
  xmlwriter_write_attribute($writer, 'charschecked', strlen($text));

  // Suggestions <c>
  $remaining_text = 0;
  foreach ($suggestions as $word => $word_suggestions) {
    xmlwriter_start_element($writer, 'c');
    $offset = stripos($text, $word, $remaining_text);
    $length = strlen($word);
    $remaining_text += $length;
    xmlwriter_write_attribute($writer, 'o', $offset); // offset of the word in the text
    xmlwriter_write_attribute($writer, 'l', $length); // length of the word
    xmlwriter_write_attribute($writer, 's', 1); // confidence level of the suggestion, unused but kept to preserve format
    xmlwriter_text($writer, implode(' ', $word_suggestions));
    xmlwriter_end_element($writer); // c
  }
  xmlwriter_end_element($writer); // spellresult

  xmlwriter_end_document($writer);

  return xmlwriter_output_memory($writer);
}

/**
 * Return id of field by name
 */
function spellchecker_get_id_of_field($form, $field_name) {
  $id = 'edit-' . str_replace('_', '-', $field_name);
  /* TODO: find more appropriate way if default id is changed - see spellchecker_array_find_r() and '#id' within form element */
  return $id;
}

/**
 * Array search help function for finding the right field path in the form array
 *
 * (array_search with recursive searching, optional partial matches and optional search by key)
 */
function &spellchecker_array_find_r($needle, &$haystack, $partial_matches = FALSE, $search_keys = TRUE) {
    $res = FALSE;
    if (!is_array($haystack)) {
        return FALSE;
    }
    foreach ($haystack as $key => $value) {
        $what = ($search_keys) ? $key : $value;
        if ($needle === $what) {
            return $haystack[$key];
        } elseif ($partial_matches && @strpos($what, $needle) !== FALSE) {
            return $haystack[$key];
        } elseif (is_array($value)) {
            $obj = &spellchecker_array_find_r($needle, $haystack[$key], $partial_matches, $search_keys);
            if ($obj) {
                return $obj;
            }
        }
    }
    return $res;
}

