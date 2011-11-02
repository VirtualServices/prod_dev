<?php

/**
 * @file
 * User activity report for the visitors module.
 */

/**
 * Display user activity report.
 *
 * @return
 *   string user activity report html source
 */
function visitors_user_activity() {
  $date_format    = variable_get('date_format_short_custom', 'Y-m-d H:i:s');
  $items_per_page = variable_get('visitors_lines_per_page', 10);

  $header = array(
    array('data' => t('#')),
    array('data' => t('User'), 'field' => 'name'),
    array('data' => t('Hits'), 'field' => 'hits', 'sort' => 'desc'),
    array('data' => t('Nodes'), 'field' => 'nodes'),
    array('data' => t('Comments'), 'field' => 'comments')
  );

  $from = visitors_get_from_timestamp();
  $to   = visitors_get_to_timestamp();

  $sql = sprintf('SELECT u.name,
                         u.uid,
                         count(DISTINCT v.visitors_id) as hits,
                         count(DISTINCT n.nid) as nodes,
                         count(DISTINCT c.cid) as comments
                  FROM {users} as u
                  LEFT JOIN {visitors} as v
                  ON u.uid=v.visitors_uid
                  LEFT JOIN {node} as n
                  ON u.uid=n.uid
                  AND n.created BETWEEN %s AND %s
                  LEFT JOIN {comments} as c
                  ON u.uid=c.uid
                  AND c.timestamp BETWEEN %s AND %s
                  WHERE %s
                  GROUP BY u.name, u.uid, v.visitors_uid, n.uid, c.uid
                  %s',
                  $from,
                  $to,
                  $from,
                  $to,
                  visitors_date_filter_sql_condition(),
                  tablesort_sql($header)
                );

  $sql_count = sprintf('SELECT COUNT(DISTINCT u.uid)
                        FROM {users} u
                        INNER JOIN {visitors} v
                        ON u.uid=v.visitors_uid
                        WHERE %s',
                        visitors_date_filter_sql_condition()
                      );
  $results = pager_query($sql, $items_per_page, 0, $sql_count);

  $rows = array();

  $page = isset($_GET['page']) ? (int) $_GET['page'] : '';
  $i = 0 + ($page  * $items_per_page);

  while ($data = db_fetch_object($results)) {
    $user = user_load(array('uid' => $data->uid));
    $user_page = theme('username', $data);

    $rows[] = array(
      ++$i,
      $user_page,
      $data->hits,
      $data->nodes,
      $data->comments
    );
  }
  $output  = visitors_date_filter();
  $output .= theme('table', $header, $rows);
  $output .= theme('pager', NULL, $items_per_page, 0);

  return $output;
}

