<?php

/**
 * @file
 * Referers report for the visitors module.
 */

/**
 * Display referers report.
 *
 * @return
 *   string referers report html source
 */
function visitors_referer_list() {
  $items_per_page = variable_get('visitors_lines_per_page', 10);

  $header = array(
    array('data' => t('#')),
    array('data' => t('Referer'), 'field' => 'visitors_referer'),
    array('data' => t('Count'), 'field' => 'count', 'sort' => 'desc'),
  );

  $sql = sprintf("SELECT count( * ) as count, visitors_referer
                  FROM {visitors}
                  WHERE %s %s
                  GROUP BY visitors_referer" . tablesort_sql($header),
                  visitors_date_filter_sql_condition(),
                  visitors_referers_condition()
                );
  $sql_count = 'SELECT count(DISTINCT visitors_referer) FROM {visitors} WHERE '.
                visitors_date_filter_sql_condition() .' '.
                visitors_referers_condition();
  $results = pager_query($sql, $items_per_page, 0, $sql_count, $_SERVER['HTTP_HOST']);
  $rows = array();

  $page = isset($_GET['page']) ? (int) $_GET['page'] : '';
  $i = 0 + ($page * $items_per_page);
  while ( $data = db_fetch_object($results) ) {
    $rows[] = array(
      ++$i,
      l($data->visitors_referer, $data->visitors_referer),
      $data->count,
    );
  }

  $output  = drupal_get_form('visitors_referers_form');
  $output .= theme('table', $header, $rows);
  $output .= theme('pager', NULL, $items_per_page, 0);

  return $output;
}

