<?php
// $Id: question_answer.pages.inc,v 1.1.2.4 2010/10/28 09:23:22 jcisio Exp $

/** @file
 * Extra stuffs for privileged user
 * By jcisio
 */

/**
 * Menu callback; select a comment.
 *
 * @param $cid
 *   The comment do be hidden.
 */
function question_answer_select($cid, $token) {
  if (is_numeric($cid) && drupal_valid_token($token, "question_answer/select/$cid")) {
    $nid = db_result(db_query('SELECT nid FROM {comments} WHERE cid=%d', $cid));
    if (is_numeric($nid)) {
      $node = node_load($nid);
      $qtype = _question_answer_nodetypes();
      $node->{$qtype[$node->type]['answer']}[0]['value'] = $cid;
      node_save($node);
    }
  }

  if (! $node) {
    drupal_set_message(t("Oops! There was an error in selecting your answer!"), 'warning');
  }

  if (isset($_SERVER['HTTP_X_REQUESTED_WITH']) && $_SERVER['HTTP_X_REQUESTED_WITH'] == 'XMLHttpRequest') {
    ctools_include('ajax');
    $commands = array();
    $commands[] = ctools_ajax_command_remove('a[href^='. url('question_answer/select') .']');
    //should be some "addClass" command, not supported now, but will (at least will be D7)
    //$commands[] = ctools_ajax_command_attr('#comment-'. $cid.' + div', 'class', 'question_answer-selected');

    ctools_ajax_render($commands);
  }
  else {
    drupal_goto($_SERVER['HTTP_REFERER']);
  }
}

/**
 * Settings form
 */
function question_answer_settings() {
  $form = array();

  $nodes = array();
  $fields = array();
  foreach (content_fields() as $field) {
    $fields[$field['field_name']] = $field['field_name'];
    if (! in_array($field['type_name'], $nodes)) {
      $nodes[$field['type_name']] = $field['type_name'];
    }
  }
  
  $nodetype = array_shift(array_keys(_question_answer_nodetypes()));
  $options = array_shift(array_values(_question_answer_nodetypes()));

  $form['question_answer_nodetype'] = array(
    '#type' => 'select',
    '#title' => t('Node type for questions'),
    '#required' => true,
    '#options' => $nodes,
    '#default_value' => $nodetype,
  );

  $form['question_answer_fieldtype'] = array(
    '#type' => 'select',
    '#title' => t('Field type for answer'),
    '#required' => true,
    '#options' => $fields,
    '#default_value' => $options['answer'],
  );

  $form['question_answer_duration'] = array(
    '#type' => 'select',
    '#title' => t('Field type for duration'),
    '#required' => true,
    '#options' => $fields,
    '#default_value' => $options['duration'],
  );

  $form['question_answer_maxlength'] = array(
    '#type' => 'textfield',
    '#title' => t('Node title maxlength'),
    '#required' => true,
    '#default_value' => variable_get('question_answer_maxlength', 0),
    '#description' => t('Maxlength of the question title. Default is 0 (unlimited)'),
  );

  return system_settings_form($form);
}
