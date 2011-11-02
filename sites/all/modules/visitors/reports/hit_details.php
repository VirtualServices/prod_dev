<?php

/**
 * @file
 * Hit details report for the visitors module.
 */

/**
 * Menu callback; Displays recent page accesses.
 *
 * @param visitors_id
 *   int visitors id from visitors table
 *
 * @return
 *   string hit details report or 404 error if visitors_id not found
 */
function visitors_hit_details($visitors_id) {
  $result = db_query('SELECT a.*, u.name, u.uid
                      FROM {visitors} a
                      LEFT JOIN {users} u
                      ON a.visitors_uid = u.uid
                      WHERE visitors_id = %d',
                      $visitors_id
                    );

  if ($hit_details = db_fetch_object($result)) {

    $rows[] = array(
      array('data' => t('URL'), 'header' => TRUE),
      l(
        urldecode($hit_details->visitors_url),
        urldecode($hit_details->visitors_url)
      )
    );

    $rows[] = array(
      array('data' => t('Title'), 'header' => TRUE),
      check_plain($hit_details->visitors_title)
    );

    $rows[] = array(
      array('data' => t('Referer'), 'header' => TRUE),
      ($hit_details->visitors_referer ?
        l($hit_details->visitors_referer, $hit_details->visitors_referer) : '')
    );

    $rows[] = array(
      array('data' => t('Date'), 'header' => TRUE),
      format_date($hit_details->visitors_date_time, 'large')
    );

    $rows[] = array(
      array('data' => t('User'), 'header' => TRUE),
      theme('username', $hit_details)
    );

    $whois_enable = module_exists('whois');
    $attr = array(
      'attributes' =>
      array(
        'target' => '_blank',
        'title' => t('Whois lookup')
      )
    );
    $ip = long2ip($hit_details->visitors_ip);

    $rows[] = array(
      array('data' => t('Hostname'), 'header' => TRUE),
      $whois_enable ? l($ip, 'whois/'. $ip, $attr) : check_plain($ip)
    );

    $rows[] = array(
      array('data' => t('User Agent'), 'header' => TRUE),
      check_plain($hit_details->visitors_user_agent)
    );

    return theme('table', array(), $rows);
  }
  else {
    drupal_not_found();
  }
}

