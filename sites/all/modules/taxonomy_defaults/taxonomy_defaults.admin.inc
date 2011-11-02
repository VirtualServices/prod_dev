<?php
// $Id: taxonomy_defaults.admin.inc,v 1.2.2.8 2010/12/04 23:12:15 sleepcamel Exp $

/**
 * @file
 * Administration forms for the taxonomy_defaults module
 */

/**
 * Defines the page at admin/content/taxonomy/taxonomy_defaults.
 */
function taxonomy_defaults_form() {
  // For each node type we generate per vocabulary a checkbox & term select.
  $form['#tree'] = TRUE;
  $vocabularies = taxonomy_get_vocabularies();

  // Retrieve the core forum vocabulary id so it can be skipped later
  $forum_vid = variable_get('forum_nav_vocabulary', '');

  $ignored_settings = array();

  foreach (node_get_types() as $type => $name) {
    $type_vocabularies = taxonomy_get_vocabularies($type);
 
    // Loop over all vocabularies
    foreach ($vocabularies as $vid => $vocab) {

      $activevocab = array_key_exists($vid, $type_vocabularies);

      if ($activevocab) {
        $form[$type][$vid]['name'] = array('#value' => t($vocab->name));

        // Ignore the core forum vocabury
        if ($forum_vid == $vid) {
          $form[$type][$vid]['msg'] = array('#value' => t('Defaults cannot be assigned to the Forum vocabulary from Drupal Core.'));
          continue;
        }

        $form[$type][$vid]['hide_vocab'] = array(
          '#type' => 'checkbox',
          '#default_value' => !variable_get("taxdef_{$type}_{$vid}_visible", TRUE),
          '#weight' => -16,
        );

        // If the vocabulary stores tags, add an autocomplete field for it.
        if ($vocab->tags) {
          $stored_terms = variable_get("taxdef_{$type}_{$vid}", array());
          $stored_tags = array();

          foreach ( $stored_terms as $tid ) {
            $term = taxonomy_get_term($tid);
            if ( $term )
              $stored_tags[] = $term->name;
          }
          $stored_tags = drupal_implode_tags($stored_tags);
          $form[$type][$vid]['tags'] = array(
            '#type' => 'textfield',
            '#description' => "Begin typing",
            '#default_value' => $stored_tags,
            '#autocomplete_path' => 'taxonomy/autocomplete/'. $vocab->vid,
            '#maxlength' => 255,
          );
        }
        // Other vocabularies get added as normal selectors.
        else {
          $form[$type][$vid]['select'] = taxonomy_form($vid, variable_get("taxdef_{$type}_{$vid}", 0));
        }
      }
      else {
        if (variable_get("taxdef_{$type}_{$vid}", FALSE)) {
          $ignored_settings[] = $name->name;

          $form[$type][$vid]['name'] = array('#value' => t($vocab->name));
          $form[$type][$vid]['msg'] = array(
            '#value' => t("Defaults exist, but are ignored because the @vocab vocabulary is not assigned to the @type content type in the !settings. ", 
                          array(
                            '@vocab' => $vocab->name,
                            '@type' => $name->name,
                            '!settings' => l(t('vocabulary settings'), 'admin/content/taxonomy/edit/vocabulary/'. $vid), 
                          )
                        ),
          );
        }
      }
    }
  }
  if (count($vocabularies) > 0) {
    $form['buttons']['submit'] = array('#type' => 'submit', '#value' => t('Save configuration') );
    $form['buttons']['reset'] = array('#type' => 'submit', '#value' => t('Reset to defaults') , '#submit' => array('taxonomy_defaults_reset_submit'));
  }
  else {
    $form['text'] = array(
      '#value' => t('Before you can assign default terms to node types, go to !link to create and fill vocabularies.', array('!link' => l(t('add vocabulary'), 'admin/content/taxonomy/add/vocabulary'))),
    );
  }

  if(count($ignored_settings) > 0) {
    $ignored_settings = implode(', ', $ignored_settings);
    drupal_set_message(t("Some default terms are being ignored because their vocabularies are not assigned to the corresponding content type. If you are upgrading from 6.x-1.0 or earlier to 6.x-1.1 or later, you'll need to update your hidden vocabularies by assigning them to the content types. Expand the content types below for more details. Content types that may be affected: @vocab.", array('@vocab' => $ignored_settings), 'warning'));
  }
  return $form;
}

/**
 * Clear all settings in the variable table
 */
function taxonomy_defaults_reset_submit($form, &$form_state) {
  foreach (node_get_types() as $type => $name) {
    foreach ($form_state['values'][$type] as $vid => $values) {
      _taxonomy_defaults_clear_settings($type, $vid);
    }
  }
  drupal_set_message(t('The configuration options have been reset to their default values.'));
}

/**
 * Store settings in the variable table.
 */
function taxonomy_defaults_form_submit($form, &$form_state) {
  foreach (node_get_types() as $type => $name) {

    if (!is_array($form_state['values'][$type])) {
      continue;
    }

    foreach ($form_state['values'][$type] as $vid => $values) {
      variable_set("taxdef_{$type}_{$vid}_visible", !$values['hide_vocab']);
      if (isset($values['tags'])) {
        $typed_input = $values['tags'];
        $typed_terms = drupal_explode_tags($typed_input);
        $inserted = array();
        foreach ($typed_terms as $typed_term) {
          // See if the term exists in the chosen vocabulary
          // and return the tid; otherwise, add a new record.
          $possibilities = taxonomy_get_term_by_name($typed_term);
          $typed_term_tid = NULL;  // tid match, if any.
          foreach ($possibilities as $possibility) {
            if ($possibility->vid == $vid) {
              $typed_term_tid = $possibility->tid;
            }
          }
          if ($typed_term_tid) {
            $inserted[] = $typed_term_tid;
          }
          else {
            drupal_set_message(t("'@term' does not exist.", array('@term' => $typed_term, '@vocab' => $vid)), 'warning');
          }
        }
        variable_set("taxdef_{$type}_{$vid}", $inserted);
      }
      else {
        variable_set("taxdef_{$type}_{$vid}", is_array($values['select']) ? $values['select'] : array($values['select']));
      }
    }
  }
  drupal_set_message(t('The configuration options have been saved.'));
}

/**
 * Renders the settings form in a table.
 */
function theme_taxonomy_defaults_form($form) {
  drupal_add_css(drupal_get_path('module', 'taxonomy_defaults') .'/taxonomy_defaults.css', 'module', 'all', FALSE);
  $output = '';

  foreach (node_get_types() as $type => $name) {
    if (isset($form[$type]) ) {
      $rowcount = 0;
      foreach (element_children($form[$type]) as $key) {
        $vocabtable[$rowcount][] = drupal_render($form[$type][$key]['name']);
        // Add the term selector - tags autocomplete field.
        if (isset($form[$type][$key]['tags'])) {
          $vocabtable[$rowcount][] = drupal_render($form[$type][$key]['hide_vocab']);
          $vocabtable[$rowcount][] = drupal_render($form[$type][$key]['tags']);
        }
        // Add the term selector - standard selector.
        elseif (isset($form[$type][$key]['select'])) {
          $vocabtable[$rowcount][] = drupal_render($form[$type][$key]['hide_vocab']);
          $form[$type][$key]['select']['#title']='';
          $vocabtable[$rowcount][] = drupal_render($form[$type][$key]['select']);
        }
        else {
          $vocabtable[$rowcount][] = array(
            "data" => drupal_render($form[$type][$key]['msg']),
            "colspan" => 2
          );
        }

        $rowcount++;
      }
      if ($rowcount) {
        $subtable = theme('table', array(t('Vocabulary'), t('Hide'), t('Default Terms')), $vocabtable);

        $fieldset = array(
          '#title' => $name->name,
          '#collapsible' => TRUE,
          '#collapsed' => TRUE,
          '#value' => $subtable,
        );

        $output .= theme('fieldset', $fieldset);
      }
    }
    unset($vocabtable);
  }

  // Render remaining fields
  $output .= drupal_render($form);
  return $output;
}
