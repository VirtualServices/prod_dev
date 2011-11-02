<?php

/**
 * @file
 * Monthly history report for the visitors module.
 */

/**
 * Display monthly history report.
 *
 * @return
 *   string monthly history report html source
 */
function visitors_monthly_history() {
  $items_per_page = variable_get('visitors_lines_per_page', 10);

  $header = array(
    array('data' => t('#')),
    array('data' => t('Month'), 'field' => 'm', 'sort' => 'asc'),
    array('data' => t('Pages'), 'field' => 'count'),
  );

  $sql = sprintf('select count(*) as count,
                  %s as m,
                  %s as s
                  from {visitors}
                  where %s
                  group by m %s',
                  visitors_date_format_sql('visitors_date_time', '%Y%m'),
                  visitors_date_format_sql('MIN(visitors_date_time)', '%Y %M'),
                  visitors_date_filter_sql_condition(),
                  tablesort_sql($header)
                );
  $sql_count = sprintf('SELECT count(DISTINCT %s)
                        FROM {visitors}
                        WHERE %s',
                        visitors_date_format_sql('visitors_date_time', '%Y %M'),
                        visitors_date_filter_sql_condition()
                      );

  $results = pager_query($sql, $items_per_page, 0, $sql_count);

  $rows = array();

  $page = isset($_GET['page']) ? (int) $_GET['page'] : '';
  $i = 0 + ($page  * $items_per_page);


  while ($data = db_fetch_object($results)) {
    $rows[] = array(
      ++$i,
      $data->s,
      $data->count
    );
  }

  $output  = visitors_date_filter();

  if (count($rows) > 1) {
    $output .= sprintf(
        '<img src="%s" alt="%s" width="%d" height="%d">',
        url('visitors/monthly_history/graph'),
        t('Monthly history'),
        visitors_get_chart_width(),
        visitors_get_chart_height()
    );
  }

  $output .= theme('table', $header, $rows);
  $output .= theme('pager', NULL, $items_per_page, 0);

  return $output;
}

/**
 * Display monthly history chart report.
 */
function graph_visitors_monthly_history() {
  $sql = sprintf('select count(*) as count,
                  %s as m,
                  %s as s
                  from {visitors}
                  where %s
                  group by m
                  order by m ASC',
                  visitors_date_format_sql('visitors_date_time', '%Y%m'),
                  visitors_date_format_sql('MIN(visitors_date_time)', '%Y %M'),
                  visitors_date_filter_sql_condition()
                );

  $rows = array();
  $dates = array();

  $result = db_query($sql);

  while ($data = db_fetch_object($result)) {
    $rows[$data->s] = (int) $data->count;
    $dates[] = $data->s;
  }

  if (count($rows) > 1) {
    visitors_graph($rows, $dates);
  }
}

