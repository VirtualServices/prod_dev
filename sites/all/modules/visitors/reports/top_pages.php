<?php

/**
 * @file
 * Top pages report for the visitors module.
 */

/**
 * Display top pages report.
 *
 * @return
 *   string top pages report html source
 */
function visitors_top_pages() {
  $date_format = variable_get('date_format_short_custom', 'Y-m-d H:i:s');
  $items_per_page = variable_get('visitors_lines_per_page', 10);

  $header = array(
    array('data' => t('#')),
    array('data' => t('URL'), 'field' => 'visitors_url'),
    array('data' => t('Count'), 'field' => 'count', 'sort' => 'desc'),
  );

  $sql = sprintf("SELECT COUNT(visitors_id) AS count,
                  visitors_path,
                  MIN(visitors_title) AS visitors_title,
                  MIN(visitors_url) AS visitors_url
                  FROM {visitors}
                  WHERE %s
                  GROUP BY visitors_path" . tablesort_sql($header),
                  visitors_date_filter_sql_condition()
                );

  $sql_count = sprintf('SELECT COUNT(DISTINCT visitors_path)
                        FROM {visitors}
                        WHERE %s',
                        visitors_date_filter_sql_condition()
                      );

  $results = pager_query($sql, $items_per_page, 0, $sql_count);

  $rows = array();

  $page = isset($_GET['page']) ? $_GET['page'] : '';
  $i = 0 + ($page  * $items_per_page);

  while ($data = db_fetch_object($results)) {
    $rows[] = array(
      ++$i,
      check_plain($data->visitors_title) .'<br/>'.
      l($data->visitors_path, $data->visitors_url),
      $data->count,
    );
  }

  $output  = visitors_date_filter();
  $output .= theme('table', $header, $rows);
  $output .= theme('pager', NULL, $items_per_page, 0);

  return $output;
}

