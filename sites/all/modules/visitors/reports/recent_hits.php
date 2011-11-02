<?php

/**
 * @file
 * Recent hits report for the visitors module.
 */

/**
 * Display recent hits report.
 *
 * @return
 *   string recent hits report html source
 */
function visitors_recent_hits() {
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
                  WHERE %s". tablesort_sql($header),
                  visitors_date_filter_sql_condition()
                );

  $sql_count = sprintf('SELECT COUNT(*)
                        FROM {visitors}
                        WHERE %s',
                        visitors_date_filter_sql_condition()
                      );

  $results = pager_query($sql, $items_per_page, 0, $sql_count);

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
      check_plain($data->visitors_title) .'<br/>'. l($data->visitors_path,
      $data->visitors_url),
      $user_page,
      l(t('details'), 'visitors/hits/'. $data->visitors_id)
    );
  }

  $output  = visitors_date_filter();
  $output .= theme('table', $header, $rows);
  $output .= theme('pager', NULL, $items_per_page, 0);

  return $output;
}

