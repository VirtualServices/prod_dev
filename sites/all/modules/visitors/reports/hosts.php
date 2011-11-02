<?php

/**
 * @file
 * Hosts report and hits from host report for the visitors module.
 */

/**
 * Menu callback; presents the "hits from" page.
 *
 * @param ip
 *   A string containing an ip address.
 *
 * @return
 *   string hits from host report html source or 404 error if hits not found.
 */
function visitors_host_hits($ip) {
  if (!visitors_is_ip_valid($ip) && ($ip != '0.0.0.0')) {
    return drupal_not_found();
  }

  $date_format    = variable_get('date_format_short_custom', 'Y-m-d H:i:s');
  $items_per_page = variable_get('visitors_lines_per_page', 10);

  $header = array(
    array('data' => t('#')),
    array('data' => t('ID'), 'field' => 'visitors_id', 'sort' => 'desc'),
    array('data' => t('Date'), 'field' => 'visitors_date_time'),
    array('data' => t('URL'), 'field' => 'visitors_url'),
    array('data' => t('User'), 'field' => 'u.name'),
    array('data' => t('Operations'))
  );

  $sql = sprintf("SELECT v.*, u.name, u.uid
                  FROM {visitors} v
                  LEFT JOIN {users} u
                  ON u.uid = v.visitors_uid
                  WHERE v.visitors_ip='%u' AND %s" . tablesort_sql($header),
                  ip2long($ip),
                  visitors_date_filter_sql_condition()
                );
  $count_sql = sprintf("SELECT COUNT(*) AS count
                        FROM {visitors}
                        WHERE visitors_ip='%u'
                        AND %s",
                        ip2long($ip),
                        visitors_date_filter_sql_condition()
                      );

  $query = db_query($count_sql);
  $data = db_fetch_object($query);

  if ($data->count == 0) {
    return drupal_not_found();
  }

  $results = pager_query($sql, $items_per_page, 0, $count_sql);

  $rows = array();

  $page = isset($_GET['page']) ? (int) $_GET['page'] : '';
  $i = 0 + ($page  * $items_per_page);

  while ($data = db_fetch_object($results)) {
    $user = user_load(array('uid' => $data->visitors_uid));
    $user_page = theme('username', $data);
    $rows[] = array(
      ++$i,
      $data->visitors_id,
      format_date($data->visitors_date_time, 'custom', $date_format),
      check_plain($data->visitors_title) .'<br/>'.
        l($data->visitors_path, $data->visitors_url),
      $user_page,
      l(t('details'), 'visitors/hits/'. $data->visitors_id)
    );
  }

  $output  = visitors_date_filter();
  $output .= theme('table', $header, $rows);
  $output .= theme('pager', NULL, $items_per_page, 0);

  drupal_set_title(t('Hits from') .' '. check_plain($ip));

  return $output;
}

/**
 * Display hosts report.
 *
 * @return
 *   string hosts report html source.
 */
function visitors_hosts() {
  $items_per_page = variable_get('visitors_lines_per_page', 10);

  $header = array(
    array('data' => t('#')),
    array('data' => t('Host'), 'field' => 'visitors_ip'),
    array('data' => t('Pages'), 'field' => 'count', 'sort' => 'desc'),
    array('data' => t('Operations'))
  );

  $sql = sprintf("SELECT count( * ) as count, visitors_ip
                  FROM {visitors}
                  WHERE %s
                  GROUP BY visitors_ip" . tablesort_sql($header),
                  visitors_date_filter_sql_condition()
                );


  $sql_count = "SELECT count(DISTINCT visitors_ip) FROM {visitors} WHERE ".
    visitors_date_filter_sql_condition();
  $results = pager_query($sql, $items_per_page, 0, $sql_count);

  $rows = array();

  $page = isset($_GET['page']) ? (int) $_GET['page'] : '';
  $i = 0 + ($page * $items_per_page);
  $whois_enable = module_exists('whois');
  $attr = array(
    'attributes' => array('target' => '_blank', 'title' => t('Whois lookup'))
  );

  while ( $data = db_fetch_object($results) ) {
    $ip = long2ip($data->visitors_ip);
    $rows[] = array(
      ++$i,
      $whois_enable ? l($ip, 'whois/'. $ip, $attr) : check_plain($ip),
      $data->count,
      l(t('hits'), 'visitors/hosts/'. $ip)
    );
  }

  $output  = visitors_date_filter();
  $output .= theme('table', $header, $rows);
  $output .= theme('pager', NULL, $items_per_page, 0);

  return $output;
}

