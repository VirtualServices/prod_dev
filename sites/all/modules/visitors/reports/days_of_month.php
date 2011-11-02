<?php

/**
 * @file
 * Days of month report for the visitors module.
 */

/**
 * Display days of month report.
 *
 * @return
 *   string days of month report html source.
 */
function visitors_days_of_month() {
  $items_per_page = 31;

  $header = array(
    array('data' => t('#')),
    array('data' => t('Day'), 'field' => 'day', 'sort' => 'asc'),
    array('data' => t('Pages'), 'field' => 'count'),
  );

  global $db_type;
  if ($db_type == 'pgsql') {
    $sql = sprintf('select count(*) as count,
                    %s as day
                    from {visitors}
                    where %s
                    group by day %s',
                    visitors_date_format_sql('visitors_date_time', '%e'),
                    visitors_date_filter_sql_condition(),
                    tablesort_sql($header)
                  );
  }
  else {
    $sql = sprintf('select count(*) count,
                    %s+0 as day
                    from {visitors}
                    where %s
                    group by day %s',
                    visitors_date_format_sql('visitors_date_time', '%e'),
                    visitors_date_filter_sql_condition(),
                    tablesort_sql($header)
                  );
  }
  $result = db_query($sql);

  $rows = array();
  $i = 0;
  $count = 0;

  while ($data = db_fetch_object($result)) {
    $rows[] = array(
      ++$i,
      (int) $data->day,
      $data->count
    );

    $count += $data->count;
  }

  $output  = visitors_date_filter();

  if ($count > 0) {
    $output .= sprintf(
        '<img src="%s" alt="%s" width="%d" height="%d">',
        url('visitors/days_of_month/graph'),
        t('Days of month'),
        visitors_get_chart_width(),
        visitors_get_chart_height()
    );
  }
  $output .= theme('table', $header, $rows);
  $output .= theme('pager', NULL, $items_per_page, 0);

  return $output;
}

/**
 * Display days of month chart report.
 */
function graph_visitors_days_of_month() {
  $sql = sprintf('select count(*) as count,
                  %s as day
                  from {visitors}
                  where %s
                  group by day',
                  visitors_date_format_sql('visitors_date_time', '%e'),
                  visitors_date_filter_sql_condition()
                );

  $results = pager_query($sql, 31, 0, NULL);
  $rows = array();

  for ($i = 1; $i <= 31; $i++) {
    $rows[$i] = 0;
  }

  while ($data = db_fetch_object($results)) {
    $rows[(int)$data->day] = (int)$data->count;
  }

  // build dates series
  $dates = range(1, 31);

  visitors_graph($rows, $dates);
}

