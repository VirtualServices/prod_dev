<?php
// $Id: validation_api.admin.inc,v 1.22.2.24 2008/12/03 17:12:35 tapocol Exp $

/**
 * @file
 */

/**
 * Admin Page
 */
function validation_api_admin() {
  $content = array(
    array(
      'title' => t('Validators'),
      'href' => 'admin/build/validation_api/validators',
      'localized_options' => array('attributes' => array('title' => t('Create and manage validator rules, messages, etc.'))),
      'description' => t('Create and manage validator rules, messages, etc.'),
    ),
    array(
      'title' => t('Fields'),
      'href' => 'admin/build/validation_api/fields',
      'localized_options' => array('attributes' => array('title' => t('Create and manage a form field\'s validator, argument, etc.'))),
      'description' => t('Create and manage a form field\'s validator, argument, etc.'),
    ),
    array(
      'title' => t('Settings'),
      'href' => 'admin/build/validation_api/settings',
      'localized_options' => array('attributes' => array('title' => t('Modify settings for the Validation API system.'))),
      'description' => t('Modify settings for the Validation API system.'),
    )
  );

  return theme('admin_block_content', $content);
}

/**
 * Settings Form
 */
function validation_api_admin_settings_form() {
  $form['validation_api_links'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add a validator link'),
    '#description' => t('This will show \'add a validator to...\' links on forms (for users that can administer validators).'),
    '#default_value' => variable_get('validation_api_links', 1),
  );
  // Set up the elements array with all possible element names.
  $elements = module_invoke_all('elements');
  foreach ($elements as $name => $element) {
    $elements[$name] = $name;
  }
  asort($elements, SORT_STRING);
  $form['fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => t('Excluded Field Types'),
    'excluded_fields' => array(
      '#type' => 'checkboxes',
      '#options' => $elements,
      '#default_value' => variable_get('excluded_validation_fields', array('item', 'value', 'fieldset', 'hidden', 'button', 'submit', 'image_button', 'markup', 'token')),
    ),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Settings'),
  );

  return $form;
}

/**
 * Submit for Settings Form
 */
function validation_api_admin_settings_form_submit($form, &$form_state) {
  variable_set('validation_api_links', $form_state['values']['validation_api_links']);

  $excluded_fields = array();
  foreach ($form_state['values']['excluded_fields'] as $name => $value) {
    if ($value !== 0) {
      $excluded_fields[] = $name;
    }
  }
  variable_set('excluded_validation_fields', $excluded_fields);

  drupal_set_message(t('Add validator links are %s.', array('%s' => (($form_state['values']['validation_api_links'] == 1) ? t('enabled') : t('disabled')))));
  drupal_set_message(t('%s are all excluded from validation.', array('%s' => implode(', ', $excluded_fields))));
}

/**
 * Lists all validators for the Admin Page
 */
function validation_api_admin_validators() {
  $validators = _validation_api_validators();

  if (count($validators) > 0) {
    $header = array(t('Name'), t('Type'), t('Message'), t('Ops'));
    $rows = array();
    $hook_validators = array();
    $update_str = '';
    foreach ($validators as $key => $validator) {
      unset($code);
      unset($hook_code);
      $links_str = '';
      if ($validator->type != 'php' || user_access('administer php validators')) {
        $links_str .= l(t('edit'), 'admin/build/validation_api/validators/edit/'. $validator->vavid) .' ';
      }
      $links_str .= l(t('delete'), 'admin/build/validation_api/validators/delete/'. $validator->vavid);
      // Add to the links str, if applicable.
      if (!empty($validator->module)) {
        if (module_exists($validator->module) && module_hook($validator->module, HOOK_ADD_VALIDATOR)) {
          if (!isset($hook_validators[$validator->module])) {
            $hook_validators[$validator->module] = module_invoke($validator->module, HOOK_ADD_VALIDATOR);
          }
          if (isset($hook_validators[$validator->module][$validator->delta])) {
            $fields = array('type', 'rule', 'message', 'arguments');
            $code = _validation_api_export(array($validator), $fields);
            $hook_code = '';
            foreach ($fields as $field) {
              if ($field == 'arguments') {
                if (isset($validator->arguments)) {
                  foreach ($hook_validators[$validator->module][$validator->delta]->arguments as $delta => $argument) {
                    $hook_code .= '$validators[0]->arguments['. $delta .']->name = \''. $argument->name ."';\n";
                    $hook_code .= '$validators[0]->arguments['. $delta .']->description = \''. $argument->description ."';\n";
                  }
                }
              }
              else {
                $hook_code .= '$validators[0]->'. $field .' = \''. $hook_validators[$validator->module][$validator->delta]->$field ."';\n";
              }
            }
            $hook_code .= 'return $validators;';
          }
        }
        $links_str .= (!isset($code) || !isset($hook_code) || $code != $hook_code) ? ' '. l(t('update'), 'admin/build/validation_api/validators/update/'. $validator->vavid) : '';
      }
      $rows[] = array($validator->name, $validator->type, $validator->message, $links_str);
    }

    $output = theme('table', $header, $rows);
  }
  else {
    $output = t('There are no validators in the database. !add or !import a validator.', array('!add' => l(t('Add'), 'admin/build/validation_api/validators/add'), '!import' => l(t('Import'), 'admin/build/validation_api/validators/import')));
  }

  return $output;
}

/**
 * The form for Adding or Editing Validators
 */
function validation_api_admin_validators_form($form_state, $vavid = NULL) {
  // Set up the defaults array for the add form (if $vavid is NULL).
  $defaults = array('name' => '', 'type' => '', 'rule' => '', 'message' => '');
  if (!is_null($vavid)) {
    // If you can find the vavid in the database, then set defaults.
    if ($array = db_fetch_array(db_query('SELECT * FROM {validation_api_validators} WHERE vavid = %d', $vavid))) {
      $defaults = $array;
    }
    // Else, then give a 404 error.
    else {
      drupal_not_found();
      exit;
    }
    
    if ($defaults['type'] == 'php' && !user_access('administer php validators')) {
      drupal_access_denied();
      exit;
    }
  }

  if (isset($defaults['vavid'])) {
    $form['#redirect'] = 'admin/build/validation_api/validators';
    $form['vavid'] = array(
      '#type' => 'value',
      '#value' => $defaults['vavid'],
    );
  }
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#description' => t('A unique name for this validator.'),
    '#required' => TRUE,
    '#maxlength' => 32,
    '#default_value' => $defaults['name'],
  );
  if (user_access('administer php validators')) {
    $options['php'] = 'PHP';
  }
  $options['regex'] = 'Regular Expression';
  $form['type'] = array(
    '#type' => 'select',
    '#title' => t('Type'),
    '#description' => t('The language to run your code in the rule.'),
    '#options' => $options,
    '#required' => TRUE,
    '#default_value' => $defaults['type'],
  );
  $form['rule'] = array(
    '#type' => 'textarea',
    '#title' => t('Rule'),
    '#description' => t('See the help section above this form.'),
    '#required' => TRUE,
    '#rows' => 3,
    '#default_value' => $defaults['rule'],
  );
  $form['message'] = array(
    '#type' => 'textarea',
    '#title' => t('Default Message'),
    '#description' => t('This is the default message sent to the user on validation error. However, this is overwritable for each field. To use placeholders, see the help section above this form.'),
    '#required' => TRUE,
    '#maxlength' => 255,
    '#rows' => 3,
    '#default_value' => $defaults['message'],
  );
  $form['arguments'] = array(
    '#type' => 'fieldset',
    '#title' => 'Arguments',
    '#tree' => TRUE,
  );
  // Create fieldsets of the already-set arguments, if applicable.
  $new_delta = 0;
  if (isset($defaults['vavid'])) {
    $result = db_query('SELECT * FROM {validation_api_arguments} WHERE vavid = %d', $defaults['vavid']);
    while ($obj = db_fetch_object($result)) {
      $new_delta = (($obj->delta >= $new_delta) ? $obj->delta + 1 : $new_delta);
      $form['arguments'][$obj->delta] = array(
        '#type' => 'fieldset',
        '#title' => t('Argument %delta', array('%delta' => $obj->delta)),
        'arg_delta' => array(
          '#type' => 'item',
          '#value' => t('To implement this argument in the validator, use %argument1 in the message or regex (use %argument2 for PHP).', array('%argument1' => '%arguments['. $obj->delta .']', '%argument2' => '$arguments['. $obj->delta .']')),
        ),
        'arg_name' => array(
          '#type' => 'textfield',
          '#title' => t('Name'),
          '#description' => t('A name for the argument.'),
          '#maxlength' => 32,
          '#default_value' => $obj->name,
        ),
        'arg_description' => array(
          '#type' => 'textarea',
          '#title' => t('Description'),
          '#description' => t('A description for the argument.'),
          '#maxlength' => 255,
          '#default_value' => $obj->description,
        ),
      );
    }
  }
  // Add one more argument fieldset, so admins can add new arguments.
  $form['arguments'][$new_delta] = array(
    '#type' => 'fieldset',
    '#title' => t('New Argument'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    'arg_delta' => array(
      '#type' => 'item',
      '#value' => t('To implement this argument in the validator, use %argument1 in the message or regex (use %argument2 for PHP).', array('%argument1' => '%arguments['. $new_delta .']', '%argument2' => '$arguments['. $new_delta .']')),
    ),
    'arg_name' => array(
      '#type' => 'textfield',
      '#title' => t('Name'),
      '#description' => t('A name for the argument.'),
      '#maxlength' => 32,
    ),
    'arg_description' => array(
      '#type' => 'textarea',
      '#title' => t('Description'),
      '#description' => t('A description for the argument.'),
      '#maxlength' => 255,
    ),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save Validator'),
  );

  return $form;
}

/**
 * The Validate function for the Adding and Editing Validator Form
 */
function validation_api_admin_validators_form_validate($form, &$form_state) {
  $values = $form_state['values'];

  // Make sure the type field is a valid input
  switch ($values['type']) {
    case 'regex':
      if (strpos($values['rule'], '\e') !== FALSE && !user_access('administer php validators')) {
        form_set_error('rule', 'Cannot use the \e regex modifier.');
      }
    break;
    case 'php':
      // Make sure this is valid PHP code
    break;
    default:
      // Send error when the type field does not use one of the above types
      form_set_error('type', 'Must be a valid type: regex or php');
  }
}

/**
 * The Submit function for the Adding and Editing Validator Form
 */
function validation_api_admin_validators_form_submit($form, &$form_state) {
  $values = $form_state['values'];

  // If vavid is present, then update it. Else, insert a new record
  if (isset($values['vavid'])) {
    // Run SQL query to update the vavid. Send message on success. Else, send error.
    if (db_query('UPDATE {validation_api_validators} SET name = \'%s\', type = \'%s\', rule = \'%s\', message = \'%s\' WHERE vavid = %d', array($values['name'], $values['type'], $values['rule'], $values['message'], $values['vavid']))) {
      drupal_set_message(t('%name has been updated', array('%name' => $values['name'])));
      // Find arguments and add new arguments, or update existing arguments.
      if (isset($values['arguments']) && is_array($values['arguments'])) {
        $result = db_query('SELECT * FROM {validation_api_arguments} WHERE vavid = %d', $values['vavid']);
        while ($obj = db_fetch_object($result)) {
          $arguments[$obj->delta] = $obj;
        }
        foreach ($values['arguments'] as $delta => $arg_values) {
          if (!empty($arg_values['arg_name'])) {
            if (isset($arguments[$delta])) {
              if ($arguments[$delta]->name != $arg_values['arg_name'] || $arguments[$delta]->description != $arg_values['arg_description']) {
                if (!db_query('UPDATE {validation_api_arguments} SET name = \'%s\', description = \'%s\' WHERE vavid = %d AND delta = %d', array($arg_values['arg_name'], $arg_values['arg_description'], $values['vavid'], $delta))) {
                  drupal_set_message(t('%name (argument) was not able to be updated.', array('%name' => $arg_values['arg_name'])), 'error');
                }
              }
            }
            else {
              if (!db_query('INSERT INTO {validation_api_arguments} (vavid, delta, name, description) VALUES (%d, %d, \'%s\', \'%s\')', array($values['vavid'], $delta, $arg_values['arg_name'], $arg_values['arg_description']))) {
                drupal_set_message(t('%name (argument) was not able to be inserted.', array('%name' => $arg_values['arg_name'])), 'error');
              }
            }
          }
        }
      }
    }
    else {
      drupal_set_message(t('%name could not be updated', array('%name' => $values['name'])), 'error');
    }
  }
  else {
    // Run SQL query to insert a new validator. Send message on success. Else, send error.
    if (db_query('INSERT INTO {validation_api_validators} (name, type, rule, message) VALUES (\'%s\', \'%s\', \'%s\', \'%s\')', array($values['name'], $values['type'], $values['rule'], $values['message']))) {
      drupal_set_message(t('New Validator Added'));
      // Insert the arguments as well.
      if (isset($values['arguments']) && is_array($values['arguments'])) {
        $vavid = db_result(db_query('SELECT vavid FROM {validation_api_validators} WHERE name = \'%s\'', $values['name']));
        foreach ($values['arguments'] as $delta => $arg_values) {
          if (!empty($arg_values['arg_name'])) {
            if (!db_query('INSERT INTO {validation_api_arguments} (vavid, delta, name, description) VALUES (%d, %d, \'%s\', \'%s\')', array($vavid, $delta, $arg_values['arg_name'], $arg_values['arg_description']))) {
              drupal_set_message(t('%name (argument) was not able to be inserted.', array('%name' => $arg_values['arg_name'])), 'error');
            }
          }
        }
      }
    }
    else {
      drupal_set_message(t('Could not add validator'), 'error');
    }
  }
}

/**
 * The form for Deleting Validators
 */
function validation_api_admin_validators_delete($form_state, $vavid) {
  if (!$obj = db_fetch_object(db_query('SELECT name FROM {validation_api_validators} WHERE vavid = %d', $vavid))) {
    drupal_not_found();
    exit;
  }
  $count = db_result(db_query('SELECT COUNT(f.vafid) FROM {validation_api_fields} AS f RIGHT JOIN {validation_api_validators} AS v ON v.vavid = f.vavid WHERE v.vavid = %d', $vavid));
  drupal_set_breadcrumb(array_merge(drupal_get_breadcrumb(), array(l(t('Validators'), 'admin/build/validation_api/validators'))));

  $form['#redirect'] = 'admin/build/validation_api/validators';
  $form['vavid'] = array(
    '#type' => 'value',
    '#value' => $vavid,
  );
  $form['name'] = array(
    '#type' => 'value',
    '#value' => $obj->name,
  );
  // Notifies the admin to the field relationships that will be deleted, as well.
  if ($count > 0) {
    $form['fields'] = array(
      '#type' => 'item',
      '#value' => t('Submitting will also delete the %count field relationship(s).', array('%count' => $count)),
    );
  }

  return confirm_form($form, t('Are you sure you want to delete the validator %name?', array('%name' => $obj->name)), 'admin/build/validation_api/validators', '', t('Delete'));
}

/**
 * The Submit function for the Validator Delete form
 */
function validation_api_admin_validators_delete_submit($form, &$form_state) {
  $values = $form_state['values'];

  // Delete the validator and it's arguments, as well as the field relationships and their argument values.
  if (db_query('DELETE FROM v, a, f, av USING {validation_api_validators} AS v LEFT JOIN {validation_api_arguments} AS a ON v.vavid = a.vavid LEFT JOIN {validation_api_fields} AS f ON v.vavid = f.vavid LEFT JOIN {validation_api_arguments_values} AS av ON a.vaaid = av.vaaid WHERE v.vavid = %d', $values['vavid'])) {
    drupal_set_message(t('%name was deleted', array('%name' => $values['name'])));
  }
  else {
    drupal_set_message(t('%name could not be deleted', array('%name' => $values['name'])));
  }
}

/**
 * Lists all fields for the Admin Page
 */
function validation_api_admin_fields() {
  $result = db_query('SELECT * FROM {validation_api_fields} ORDER BY form_id, form_field');
  $validators = _validation_api_validators();

  $header = array(t('Form ID'), t('Form Field'), t('Validator'), t('Ops'));
  while ($obj = db_fetch_object($result)) {
    $forms[$obj->form_id][$obj->form_field][] = $obj;
  }

  $output = '';
  if (isset($forms) && is_array($forms)) {
    $rows = array();
    foreach ($forms as $form_id => $fields) {
      foreach ($fields as $field_name => $field) {
        foreach ($field as $validator) {
          $rows[] = array($form_id, $field_name, $validators[$validator->vavid]->name, l(t('edit'), 'admin/build/validation_api/fields/edit/'. $validator->vafid) .' '. l(t('delete'), 'admin/build/validation_api/fields/delete/'. $validator->vafid));
        }
      }
    }
    $output .= theme('table', $header, $rows, array());
  }
  else {
    $output .= t('There are no fields being validated. !link.', array('!link' => l(t('Add a field'), 'admin/build/validation_api/fields/add')));
  }

  return $output;
}

/**
 * The form for Adding or Editing Fields
 */
function validation_api_admin_fields_form($form_state, $vafid = NULL) {
  $validators = _validation_api_validators();

  // Any validators in the database
  if (count($validators) == 0) {
    $form['validators'] = array(
      '#type' => 'item',
      '#value' => t('There are no validators in the database. !link.', array('!link' => l(t('Add a validator'), 'admin/build/validation_api/validators/add'))),
    );
  }

  // Add Field Step 1
  elseif ((!isset($form_state['rebuild']) || $form_state['rebuild'] !== TRUE) && is_null($vafid)) {
    // Set defaults
    $defaults['form_id'] = isset($_GET['form_id']) ? $_GET['form_id'] : '';
    $defaults['form_field'] = isset($_GET['form_field']) ? $_GET['form_field'] : '';
    $defaults['validator'] = isset($_GET['validator']) ? $_GET['validator'] : '';

    $form['field_form_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Form ID'),
      '#description' => t('The ID of the field\'s form (e.g. comment_form). See the help section above this form.'),
      '#required' => TRUE,
      '#maxlength' => 64,
      '#default_value' => $defaults['form_id'],
    );
    $form['form_field'] = array(
      '#type' => 'textfield',
      '#title' => t('Field Name'),
      '#description' => t('The name of the field you want to be validated. See the help section above this form.'),
      '#required' => TRUE,
      '#maxlength' => 64,
      '#default_value' => $defaults['form_field'],
    );
    foreach ($validators as $key => $validator) {
      $options[$key] = $validator->name;
    }
    $form['validator'] = array(
      '#type' => 'select',
      '#title' => t('Validator'),
      '#description' => t('The Validator to test on the field.'),
      '#required' => TRUE,
      '#options' => $options,
      '#default_value' => $defaults['validator'],
    );
    $form['continue'] = array(
      '#type' => 'submit',
      '#value' => t('Next Step'),
    );
  }
  else {
    $form_state['rebuild'] = FALSE;

    // Add Field Step 2 Defaults
    if (is_null($vafid)) {
      drupal_set_message(t('Not saved! Finish the form below to complete the addition.'), 'warning');
      $form['#redirect'] = 'admin/build/validation_api/fields/add';
      $defaults = $form_state['values'];
      if (isset($validators[$defaults['validator']]->arguments)) {
        foreach ($validators[$defaults['validator']]->arguments as $vaaid => $argument) {
          $defaults['arguments'][$vaaid] = '';
        }
      }
      $defaults['allow_empty'] = '';
      $defaults['message'] = $validators[$defaults['validator']]->message;

      $form['step_back'] = array(
        '#type' => 'item',
        '#value' => l(t('Go Back to the previous step'), 'admin/build/validation_api/fields/add', array('query' => array('form_id' => $form_state['storage']['field_form_id'], 'form_field' => $form_state['storage']['form_field'], 'validator' => $form_state['storage']['validator']))),
      );
    }
    // Edit Field Defaults
    else {
      $result = db_query('SELECT * FROM {validation_api_fields} AS f LEFT JOIN {validation_api_arguments_values} AS av ON f.vafid = av.vafid WHERE f.vafid = %d', $vafid);
      if ($array = db_fetch_array($result)) {
        $defaults = $array;
        $defaults['validator'] = $defaults['vavid'];
        $defaults['field_form_id'] = $defaults['form_id'];
        if (isset($array['vaaid'])) {
          do {
            $defaults['arguments'][$array['vaaid']] = $array['value'];
          } while ($array = db_fetch_array($result));
        }

        $form['#redirect'] = 'admin/build/validation_api/fields';
        $form['vafid'] = array(
          '#type' => 'hidden',
          '#value' => $vafid,
        );
        $form['another'] = array(
          '#type' => 'item',
          '#value' => l(t('Attach another validator to this field'), 'admin/build/validation_api/fields/add', array('query' => array('form_id' => $defaults['field_form_id'], 'form_field' => $defaults['form_field']))),
        );
      }
      else {
        drupal_not_found();
        exit;
      }
    }

    $form['field_form_id'] = array(
      '#type' => 'item',
      '#title' => t('Form ID'),
      '#value' => $defaults['field_form_id'],
    );
    $form['form_field'] = array(
      '#type' => 'item',
      '#title' => t('Field Name'),
      '#value' => $defaults['form_field'],
    );
    $form['validator'] = array(
      '#type' => 'item',
      '#title' => t('Validator'),
      '#value' => $validators[$defaults['validator']]->name,
    );
    if (isset($validators[$defaults['validator']]->arguments)) {
      $form['arguments'] = array(
        '#type' => 'fieldset',
        '#title' => t('Arguments'),
        '#tree' => TRUE,
      );
      foreach ($validators[$defaults['validator']]->arguments as $vaaid => $argument) {
        $form['arguments'][$vaaid] = array(
          '#type' => 'textfield',
          '#title' => $argument->name,
          '#description' => $argument->description,
          '#maxlength' => 32,
          '#default_value' => $defaults['arguments'][$vaaid],
        );
      }
    }
    $form['allow_empty'] = array(
      '#type' => 'checkbox',
      '#title' => t('Allow this field to be empty?'),
      '#description' => t('The validator will not be run if the field is empty. If the field is marked as required, then this option will not affect anything as it will be required either way.'),
      '#default_value' => $defaults['allow_empty'],
    );
    $form['message'] = array(
      '#type' => 'textfield',
      '#title' => t('Message'),
      '#description' => t('If this validator throws an error, then this will be the message returned.'),
      '#maxlength' => 255,
      '#default_value' => $defaults['message'],
    );
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save Field'),
    );
  }

  return $form;
}

/**
 * The Submit function for the Adding and Editing Field Form
 */
function validation_api_admin_fields_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  if (isset($values['submit'])) {
    // If vafid is present, then update it. Else, insert a new record
    if (isset($values['vafid'])) {
      // Run SQL query to update the vavid. Send message on success. Else, send error.
      if (db_query('UPDATE {validation_api_fields} SET allow_empty = %d, message = \'%s\' WHERE vafid = %d', array($values['allow_empty'], $values['message'], $values['vafid']))) {
        drupal_set_message(t('The field has been updated.'));
        // Update the values of the arguments, as well.
        $vavid = db_result(db_query('SELECT vavid FROM {validation_api_fields} WHERE vafid = %d', $values['vafid']));
        $validators = _validation_api_validators();
        if (isset($validators[$vavid]->arguments)) {
          foreach ($validators[$vavid]->arguments as $vaaid => $argument) {
            if (!db_query('UPDATE {validation_api_arguments_values} SET value = \'%s\' WHERE vafid = %d AND vaaid = %d', array($values['arguments'][$vaaid], $values['vafid'], $vaaid))) {
              drupal_set_message(t('Could not update an argument.'), 'error');
            }
          }
        }
      }
      else {
        drupal_set_message(t('The field could not be updated.'), 'error');
      }
    }
    else {
      // Get values that were stored during the multi-step form.
      $values['form_id'] = $form_state['storage']['field_form_id'];
      $values['form_field'] = $form_state['storage']['form_field'];
      $values['vavid'] = $form_state['storage']['validator'];
      // Run SQL query to insert a new validator. Send message on success. Else, send error.
      if (db_query('INSERT INTO {validation_api_fields} (form_id, form_field, vavid, allow_empty, message) VALUES (\'%s\', \'%s\', %d, %d, \'%s\')', array($values['form_id'], $values['form_field'], $values['vavid'], $values['allow_empty'], $values['message']))) {
        drupal_set_message(t('New field added.'));
        // Insert the values of the arguments, as well.
        $validators = _validation_api_validators();
        if (isset($validators[$values['vavid']]->arguments)) {
          foreach ($validators[$values['vavid']]->arguments as $vaaid => $argument) {
            if (!db_query('INSERT INTO {validation_api_arguments_values} (vafid, vaaid, value) VALUES (%d, %d, \'%s\')', array(db_last_insert_id('validation_api_fields', 'vafid'), $vaaid, $values['arguments'][$vaaid]))) {
              drupal_set_message(t('Could not update an argument.'), 'error');
            }
          }
        }
      }
      else {
        drupal_set_message(t('Could not add field.'), 'error');
      }
    }
  }
  else {
    // Change the form_state for adding fields, as the admin will be using a two-step process.
    $form_state['storage'] = $form_state['values'];
    $form_state['rebuild'] = TRUE;
  }
}

/**
 * The form for Deleting Fields
 */
function validation_api_admin_fields_delete($form_state, $vafid) {
  if (!$obj = db_fetch_object(db_query('SELECT f.form_field AS form_field, v.name AS name FROM {validation_api_fields} AS f JOIN {validation_api_validators} AS v ON f.vavid = v.vavid WHERE vafid = %d', $vafid))) {
    drupal_not_found();
    exit;
  }
  drupal_set_breadcrumb(array_merge(drupal_get_breadcrumb(), array(l(t('Fields'), 'admin/build/validation_api/fields'))));

  $form['#redirect'] = 'admin/build/validation_api/fields';
  $form['vafid'] = array(
    '#type' => 'value',
    '#value' => $vafid,
  );
  $form['form_field'] = array(
    '#type' => 'value',
    '#value' => $obj->form_field,
  );

  return confirm_form($form, t('Are you sure you want to delete the validator %validator from being used on the field %field?', array('%validator' => $obj->name, '%field' => $obj->form_field)), 'admin/build/validation_api/fields', '', t('Delete'));
}

/**
 * The Submit function for the Field Delete form
 */
function validation_api_admin_fields_delete_submit($form, &$form_state) {
  $values = $form_state['values'];

  // Delete the field and it's argument values.
  if (db_query('DELETE FROM f, av USING {validation_api_fields} AS f LEFT JOIN {validation_api_arguments_values} AS av ON f.vafid = av.vafid WHERE f.vafid = %d', $values['vafid'])) {
    drupal_set_message(t('%field was deleted', array('%field' => $values['form_field'])));
  }
  else {
    drupal_set_message(t('%field could not be deleted', array('%field' => $values['form_field'])));
  }
}

/**
 * The Export form for validators. After submit, this will create a textarea for the export code.
 */
function validation_api_admin_export_form($form_state) {
  $validators = _validation_api_validators();
  if (isset($validators) && is_array($validators) && count($validators) > 0) {
    // Show the Export Code after submit.
    if ($form_state['submitted']) {
      drupal_set_message(t('Use the code filled in the Export Code text-area to import elsewhere.'));
      $form['export_code'] = array(
        '#type' => 'textarea',
        '#title' => t('Export Code'),
        '#default_value' => $form_state['storage'],
        '#rows' => 10,
      );
    }

    foreach ($validators as $key => $validator) {
      $options[$key] = $validator->name;
    }

    $form['validators'] = array(
      '#title' => t('Validators to Export'),
      '#type' => 'checkboxes',
      '#options' => $options,
    );
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => 'Export',
    );
  }
  else {
    $form['validators'] = array(
      '#type' => 'item',
      '#value' => t('There are no validators in the database. !link.', array('!link' => l(t('Add a validator'), 'admin/build/validation_api/validators/add'))),
    );
  }

  return $form;
}

/**
 * The Submit function for the Validator Export form.
 */
function validation_api_admin_export_form_submit($form, &$form_state) {
  // Store the export code for use in the export form.
  $form_state['storage'] = validation_api_export($form_state['values']['validators']);
}

/**
 * The Import form for validators.
 */
function validation_api_admin_import_form() {
  $validators = _validation_api_validators_from_hook();

  if (isset($validators) && is_array($validators) && count($validators) > 0) {
    $form['validators'] = array(
      '#title' => t('Import Validators from Modules'),
      '#type' => 'fieldset',
      '#theme' => 'validation_api_admin_import',
      '#tree' => TRUE,
    );
    foreach ($validators as $key => $validator) {
      $form['validators'][$key] = array(
        '#title' => $validator->name .' from '. $validator->module,
        '#type' => 'checkbox',
        '#parents' => array('validators', $key),
        '#return_value' => $key,
      );
    }
  }
  else {
    $form['validators'] = array(
      '#title' => t('Import Validators from Modules'),
      '#type' => 'item',
      '#value' => t('There are no validators to import from modules.'),
    );
  }

  $form['code'] = array(
    '#type' => 'textarea',
    '#title' => t('Import Code'),
    '#description' => t('The code for validators.'),
    '#rows' => 15,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Import',
  );

  return $form;
}

/**
 * The Submit function for the Validator Import form.
 */
function validation_api_admin_import_form_submit($form, &$form_state) {
  // Run the given, and set it up to be in a valid state.
  $validators = eval($form_state['values']['code']);
  if (isset($validators) && is_array($validators)) {
    foreach ($validators as $key => $validator) {
      if (isset($validator->module)) {
        unset($validators[$key]->module);
      }
      if (isset($validator->delta)) {
        unset($validators[$key]->delta);
      }
    }
  }
  // Import checked hook validators.
  if (isset($form_state['values']['validators']) && is_array($form_state['values']['validators'])) {
    foreach ($form_state['values']['validators'] as $key => $value) {
      if ($value !== 0) {
        $array = explode('-', $key);
        $module = array_shift($array);
        $delta = (int) array_shift($array);
        if (!isset($hook_validators[$module])) {
          $hook_validators[$module] = module_invoke($module, HOOK_ADD_VALIDATOR);
        }
        $validators[$key] = $hook_validators[$module][$delta];
        $validators[$key]->module = $module;
        $validators[$key]->delta = $delta;
      }
    }
  }
  if (isset($validators) && is_array($validators)) {
    _validation_api_import($validators);
  }
  else {
    drupal_set_message(t('Validator(s) were not returned in a proper format. Needs to be an array.'), 'error');
  }
}

/**
 * The Clone form for validators.
 */
function validation_api_admin_clone_form() {
  $validators = _validation_api_validators();

  if (isset($validators) && is_array($validators) && count($validators) > 0) {
    foreach ($validators as $key => $validator) {
      $options[$key] = $validator->name;
    }

    $form['validators'] = array(
      '#title' => t('Validators to Clone'),
      '#type' => 'checkboxes',
      '#options' => $options,
    );
    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => 'Clone',
    );
  }
  else {
    $form['validators'] = array(
      '#type' => 'item',
      '#value' => t('There are no validators in the database. !link.', array('!link' => l(t('Add a validator'), 'admin/build/validation_api/validators/add'))),
    );
  }

  return $form;
}

/**
 * The Submit function for the Validator Clone form.
 */
function validation_api_admin_clone_form_submit($form, &$form_state) {
  _validation_api_clone($form_state['values']['validators']);
}

/**
 * The form for Updating Validators
 */
function validation_api_admin_update_form($form_state, $vavid) {
  $validators = _validation_api_validators();
  if (!isset($validators[$vavid])) {
    drupal_not_found();
    exit;
  }
  $validator = $validators[$vavid];
  unset($validators);

  // If the validator is not related to a module, then give them a message about it.
  if (empty($validator->module)) {
    $form['error'] = array(
      '#type' => 'item',
      '#value' => t('This validator is not connected to a hook and cannot be updated.'),
    );

    return $form;
  }

  $form['#redirect'] = 'admin/build/validation_api/validators';
  $form['vavid'] = array(
    '#type' => 'value',
    '#value' => $vavid,
  );
  $form['name'] = array(
    '#type' => 'value',
    '#value' => $validator->name,
  );
  $form['module'] = array(
    '#type' => 'value',
    '#value' => $validator->module,
  );
  $form['delta'] = array(
    '#type' => 'value',
    '#value' => $validator->delta,
  );
  $fields = array('type', 'rule', 'message', 'arguments');
  $form['your_code'] = array(
    '#type' => 'textarea',
    '#title' => t('Your code'),
    '#default_value' => _validation_api_export(array($validator), $fields),
    '#rows' => 8,
    '#disabled' => TRUE,
  );
  $form['hook_code'] = array(
    '#type' => 'textarea',
    '#title' => t('Hook\'s code'),
    '#rows' => 8,
    '#disabled' => TRUE,
  );

  // Give them the submit function if the hook module and delta are found, if not give them a message and let them know they can disconnect the validator.
  if (module_exists($validator->module) && module_hook($validator->module, HOOK_ADD_VALIDATOR)) {
    $hook_validators = module_invoke($validator->module, HOOK_ADD_VALIDATOR);
    if (isset($hook_validators[$validator->delta])) {
      $hook_validator = $hook_validators[$validator->delta];
      $default_value = '$validators[0]->type = \''. $hook_validator->type ."';\n";
      $default_value .= '$validators[0]->rule = \''. $hook_validator->rule ."';\n";
      $default_value .= '$validators[0]->message = \''. $hook_validator->message ."';\n";
      if (isset($hook_validator->arguments)) {
        foreach ($hook_validator->arguments as $delta => $argument) {
          $default_value .= '$validators[0]->arguments['. $delta .']->name = \''. $argument->name ."';\n";
          $default_value .= '$validators[0]->arguments['. $delta .']->description = \''. $argument->description ."';\n";
        }
      }
      $form['hook_code']['#default_value'] = $default_value .'return $validators;';
      return confirm_form($form, t('Are you sure you want to update the validator %name?', array('%name' => $validator->name)), 'admin/build/validation_api/validators', '', t('Update'));
    }
    else {
      $form['hook_code']['#default_value'] = t('The delta is no longer available in the module\'s hook.');
    }
  }
  else {
    $form['hook_code']['#default_value'] = t('The module is no longer available or the hook is no longer used by the module.');
  }
  $form['sorry'] = array(
    '#type' => 'item',
    '#value' => t('You cannot update this validator. However, you can disconnect the validator from the hook to get rid of the update notification.') .' '. l(t('Click here'), 'admin/build/validation_api/validators/disconnect/'. $vavid),
  );

  return $form;
}

/**
 * The Submit function for the Validator Update form
 */
function validation_api_admin_update_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  $hook_validators = module_invoke($values['module'], HOOK_ADD_VALIDATOR);
  $validator = $hook_validators[$values['delta']];

  if (db_query('UPDATE {validation_api_validators} SET type = \'%s\', rule = \'%s\', message = \'%s\' WHERE vavid = %d', array($validator->type, $validator->rule, $validator->message, $values['vavid']))) {
    drupal_set_message(t('Updated %validator', array('%validator' => $values['name'])));
    if (isset($validator->arguments)) {
      $result = db_query('SELECT vaaid, delta FROM {validation_api_arguments} WHERE vavid = %d', $values['vavid']);
      while ($obj = db_fetch_object($result)) {
        $vaaids[$obj->delta] = $obj->vaaid;
      }
      foreach ($validator->arguments as $delta => $argument) {
        if (isset($vaaids[$delta])) {
          if (!db_query('UPDATE {validation_api_arguments} SET name = \'%s\', description = \'%s\' WHERE vaaid = %d', array($argument->name, $argument->description, $vaaids[$delta]))) {
            drupal_set_message(t('Problem with updating the argument[%delta] for %name.', array('%delta' => $delta, '%name' => $values['name'])), 'error');
          }
        }
        else {
          if (!db_query('INSERT INTO {validation_api_arguments} (vavid, delta, name, description) VALUES (%d, %d, \'%s\', \'%s\')', array($vavid, $delta, $argument->name, $argument->description))) {
            drupal_set_message(t('Problem with creating the argument[%delta] for %name.', array('%delta' => $delta, '%name' => $values['name'])), 'error');
          }
        }
      }
    }
  }
  else {
    drupal_set_message(t('Could not update %validator', array('%validator' => $values['name'])));
  }

}

/**
 * The form for Disconnecting Validators
 */
function validation_api_admin_disconnect_form($form_state, $vavid) {
  $validators = _validation_api_validators();
  if (!isset($validators[$vavid])) {
    drupal_not_found();
    exit;
  }
  $validator = $validators[$vavid];
  unset($validators);

  if (empty($validator->module)) {
    $form['error'] = array(
      '#type' => 'item',
      '#value' => t('This validator is not connected to a hook and cannot be updated.'),
    );

    return $form;
  }

  $form['#redirect'] = 'admin/build/validation_api/validators';
  $form['vavid'] = array(
    '#type' => 'value',
    '#value' => $vavid,
  );
  $form['name'] = array(
    '#type' => 'value',
    '#value' => $validator->name,
  );

  return confirm_form($form, t('Are you sure you want to disconnect the validator %validator from being tied to it\'s hook?', array('%validator' => $validator->name)), 'admin/build/validation_api/validators', '', t('Disconnect'));
}

/**
 * The Submit function for the Validator Disconnect form
 */
function validation_api_admin_disconnect_form_submit($form, &$form_state) {
  $values = $form_state['values'];

  if (db_query('UPDATE {validation_api_validators} SET module = NULL WHERE vavid = %d', array($values['vavid']))) {
    drupal_set_message(t('Disconnected %validator', array('%validator' => $values['name'])));
  }
  else {
    drupal_set_message(t('Could not disconnect %validator', array('%validator' => $values['name'])));
  }

}

/**
 * Export function for vavids. Sets up the validator objects, and then sends it to the helper function.
 *
 * @param $vavids
 *   An array of validator IDs.
 *
 * @return
 *   Returns export code that can be used to import the validator in other applications.
 */
function validation_api_export($vavids) {
  $validators = _validation_api_validators();
  foreach ($validators as $vavid => $validator) {
    if (!in_array($vavid, $vavids)) {
      unset($validators[$vavid]);
    }
  }

  return _validation_api_export($validators);
}

/**
 * Export helper function that creates and returns the export code from validator objects.
 *
 * @param $validators
 *   An array of validator objects to be exported.
 * @param $fields
 *   An array of fields that are included in the export code.
 *
 * @return
 *   Returns export code that can be used to import the validator in other applications.
 */
function _validation_api_export($validators, $fields = array('name', 'type', 'rule', 'message', 'arguments')) {
  $output = '';
  $i = 0;
  foreach ($validators as $validator) {
    foreach ($fields as $field) {
      // Arguments need to be set up with their own objects.
      if ($field == 'arguments') {
        if (isset($validator->arguments)) {
          foreach ($validator->arguments as $argument) {
            $output .= '$validators['. $i .']->arguments['. $argument->delta .']->name = \''. $argument->name ."';\n";
            $output .= '$validators['. $i .']->arguments['. $argument->delta .']->description = \''. $argument->description ."';\n";
          }
        }
      }
      else {
        $output .= '$validators['. $i .']->'. $field .' = \''. str_replace("'", "\\'", $validator->$field) ."';\n";
      }
    }
    $i++;
  }
  $output .= 'return $validators;';

  return $output;
}

/**
 * Helper function that inserts validators into the database from an array of validators.
 *
 * @param $validators
 *   An array of validators.
 *
 * @return
 *   Nothing is returned.
 */
function _validation_api_import($validators) {
  // Set up validator names already used in the database.
  $db_validators = _validation_api_validators();
  $validator_names = array();
  foreach ($db_validators as $validator) {
    $validator_names[] = $validator->name;
  }
  unset($db_validators);

  foreach ($validators as $validator) {
    if (isset($validator->name) && isset($validator->type) && isset($validator->rule) && isset($validator->message)) {
      $args = array();
      // If the current name is already used by another validator, then change the name.
      if (in_array($validator->name, $validator_names)) {
        $i = 1;
        $open_validator = FALSE;
        do {
          if (!in_array($validator->name . $i, $validator_names)) {
            $validator->name .= $i;
            $open_validator = TRUE;
          }
          else {
            $i++;
          }
        } while (!$open_validator);
      }
      $validator_names[] = $validator->name;

      array_push($args, $validator->name, $validator->type, $validator->rule, $validator->message);
      if (isset($validator->module) && isset($validator->delta)) {
        array_push($args, $validator->module, $validator->delta);
        $field_str = '(name, type, rule, message, module, delta)';
        $value_str = '("%s", "%s", "%s", "%s", "%s", %d)';
      }
      else {
        $field_str = '(name, type, rule, message)';
        $value_str = '("%s", "%s", "%s", "%s")';
      }

      if (db_query('INSERT INTO {validation_api_validators} '. $field_str .' VALUES '. $value_str, $args)) {
        drupal_set_message(t('Imported %name.', array('%name' => $validator->name)));
        // Insert validator arguments, as well.
        if (isset($validator->arguments) && is_array($validator->arguments)) {
          $vavid = db_last_insert_id('validation_api_validators', 'vavid');
          foreach ($validator->arguments as $delta => $argument) {
            if (!db_query('INSERT INTO {validation_api_arguments} (vavid, delta, name, description) VALUES (%d, %d, \'%s\', \'%s\')', array($vavid, $delta, $argument->name, $argument->description))) {
              drupal_set_message(t('Problem with creating an argument for %name.', array('%name' => $validator->name)), 'error');
            }
          }
        }
      }
      else {
        drupal_set_message(t('Could not import %name.', array('%name' => $validator->name)), 'error');
      }
    }
  }
}

/**
 * Helper function for cloning validators. It runs export, then runs import on the returned export code.
 *
 * @param $vavids
 *   An array of validator IDs.
 *
 * @return
 *   Nothing is returned.
 */
function _validation_api_clone($vavids) {
  $validators = eval(validation_api_export($vavids));
  _validation_api_import($validators);
}

/**
 * Theme function for the import form.
 * Sets up a table to easier readability.
 *
 * @param $form
 *   An array of the passed form element.
 *
 * @return
 *   A themed table, or a notification for empty.
 */
function theme_validation_api_admin_import($form) {
  if (is_array($form)) {
    $header = array('', t('Name'), t('Module'));
    $rows = array();
    foreach ($form as $key => $option) {
      if (strpos($key, '#') === FALSE) {
        $split = explode(' from ', $option['#title']);
        $name = array_shift($split);
        $module = array_shift($split);
        $option['#title'] = '';
        $rows[] = array(drupal_render($option), $name, $module);
      }
    }

    return theme('table', $header, $rows);
  }
  else {
    return t('Empty');
  }
}
