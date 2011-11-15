<?php
// $Id: spellchecker.taxonomy.inc,v 1.1 2010/12/15 15:55:58 kenorb Exp $

/**
 * @file
 * @brief
 *   Include file for taxonomy storage engine
 *
 * @package     spellchecker
 * @subpackage
 * @author
 */

/**
 * Implementation of hook_spellchecker storage callback
 */
function spellchecker_taxonomy_storage($op, $word) {
    $word = spellchecker_trim($word);
    switch ($op) {
        case 'check':
            $data = spellchecker_find_term_by_name($word);
        break;
        case 'add':
            if (!spellchecker_check($word)) { // don't add duplicates
                $data = spellchecker_add_new_word_to_vocab($word);
            }
        break;
        case 'remove':
            $data = spellchecker_remove_word_from_vocab($word);
        break;
        case 'list':
            $data = spellchecker_get_wordlist_from_taxonomy();
        break;
        default:
          $message = t('%func: Operation %op is not supported!', array('%func' => __FUNCTION__, '%op' => $op));
          drupal_set_message($message, 'error');
    }
    module_invoke_all('spellchecker_op', $op, $word);
    return $data;
}

/**
 * Add new word into taxonomy vocabulary
 */
function spellchecker_add_new_word_to_vocab($word) {
    $vid = variable_get('spellchecker_vid', spellchecker_get_vid_by_name(SPELLCHECKER_VOCAB));
    $term = array(
        'vid' => $vid, // Voacabulary ID
        'name' => $word, // Term Name 
        // 'synonyms' => 'Druplet', // TODO: (Optional) Synonym of this term
        // 'relations' => array(15), // TODO: (Optional) Related Term IDs 
    );
    return taxonomy_save_term($term); // save a word into taxonomy vocabulary
}

/**
 * Remove a word into taxonomy vocabulary
 */
function spellchecker_remove_word_from_vocab($word) {
    // $vid = variable_get('spellchecker_vid', spellchecker_get_vid_by_name(SPELLCHECKER_VOCAB));
    $tid = spellchecker_tid_by_name($word);
    taxonomy_del_term($tid); // save a word into taxonomy vocabulary
}

/**
 * Get list of words from personal dictionary
 */
function spellchecker_get_wordlist_from_taxonomy() {
    $term_list = array();
    $result = db_query('SELECT tid, name FROM {term_data} WHERE vid = %d', variable_get('spellchecker_vid', spellchecker_get_vid_by_name(SPELLCHECKER_VOCAB)));
    while ($term = db_fetch_object($result)) {
        $term_list[$term->tid] = $term->name;
    }
    return $term_list;
}

/**
 * Get list of words from personal dictionary
 */
function spellchecker_find_term_by_name($word) {
    $case_sens = variable_get('spellchecker_case_sensitive', TRUE);
    return (bool)db_result(db_query('SELECT TRUE FROM {term_data} WHERE vid = %d AND %sname = "%s"', variable_get('spellchecker_vid', spellchecker_get_vid_by_name(SPELLCHECKER_VOCAB)), ($case_sens ? 'BINARY ' : ''), $word));
}

