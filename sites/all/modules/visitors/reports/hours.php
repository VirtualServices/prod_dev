<?php

/**
 * @file
 * Hours report for the visitors module.
 */

/**
 * Get data for hours report order the query based on a header array.
 *
 * @param header
 *   Table header array. If header is NULL - data is not sorted.
 *
 * @return
 *   database query result resource
 */
function visitors_hours_data($header) {
  $sql = sprintf('SELECT count(*) as count,
                  %s as hour
                  FROM {visitors}
                  WHERE %s
                  group by hour %s',
                  visitors_date_format_sql('visitors_date_time', '%H'),
                  visitors_date_filter_sql_condition(),
                  ($header != NULL) ? tablesort_sql($header) : ''
                );
  return db_query($sql);
}

/**
 * Display hours report.
 *
 * @return
 *   string hours report html source
 */
function visitors_hours() {
  $items_per_page = 24;

  $header = array(
    array('data' => t('#')),
    array('data' => t('Hour'), 'field' => 'hour', 'sort' => 'asc'),
    array('data' => t('Pages'), 'field' => 'count'),
  );

  $results = visitors_hours_data($header);
  $rows = array();

  $page = isset($_GET['page']) ? (int) $_GET['page'] : '';
  $i = 0 + $page * $items_per_page;
  $count = 0;

  while ($data = db_fetch_object($results)) {
    $rows[] = array(
      ++$i,
      $data->hour,
      $data->count
    );
    $count += $data->count;
  }

  $output  = visitors_date_filter();

  if ($count > 0) {
    $output .= sprintf(
        '<img src="%s" alt="%s" width="%d" height="%d">',
        url('visitors/hours/graph'),
        t('Hours'),
        visitors_get_chart_width(),
        visitors_get_chart_height()
    );
  }
  $output .= theme('table', $header, $rows);
  $output .= theme('pager', NULL, $items_per_page, 0);

  return $output;
}

/**
 * Display hours chart report.
 */
function graph_visitors_hours() {
  $result = visitors_hours_data(NULL);
  $tmp_rows = array();
  $rows = array();
  for ($i = 0; $i < 24; $i++) {
    $rows[$i] = 0;
  }

  while ( $data = db_fetch_object($result) ) {
    $rows[(int)$data->hour] = $data->count;
  }

  $hours = range(0, 23);

  visitors_graph($rows, $hours);
}

