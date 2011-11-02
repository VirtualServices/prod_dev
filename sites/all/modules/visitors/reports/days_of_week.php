<?php

/**
 * @file
 * Days of week report for the visitors module.
 */

/**
 * Create days of week array, using date_first_day parameter,
 * using keys as day of week.
 *
 * @return array
 */
function visitors_get_days_of_week() {
  $days = array('Sun', 'Mon', 'Tue', 'Wed', 'Thu', 'Fri', 'Sat');
  $sort_days = array();
  $date_first_day = (int) variable_get('date_first_day', 0);
  $n = 1;
  for ($i = $date_first_day; $i < 7; $i++) {
    $sort_days[$days[$i]] = $n;
    $n++;
  }
  for ($i = 0; $i < $date_first_day; $i++) {
    $sort_days[$days[$i]] = $n;
    $n++;
  }

  return $sort_days;
}

/**
 * Get data for days of week report.
 *
 * @return
 *   database query result resource
 */
function visitors_days_of_week_data() {
  $sql = sprintf('select count(*) as count,
                  %s as d,
                  %s as n
                  from {visitors}
                  where %s
                  group by d
                  order by n',
                  visitors_date_format_sql('visitors_date_time', '%a'),
                  visitors_date_format_sql('min(visitors_date_time)', '%w'),
                  visitors_date_filter_sql_condition()
                );

  return db_query($sql);
}

/**
 * Display days of week report.
 *
 * @return
 *   string days of week report html source
 */
function visitors_days_of_week() {
  $header = array(t('#'), t('Day'), t('Pages'));

  $result = visitors_days_of_week_data();
  $tmp_rows = array();
  $count = 0;

  while ($data = db_fetch_object($result)) {
    $tmp_rows[$data->n] = array(
      $data->d,
      $data->count,
      $data->n
    );

    $count += $data->count;
  }
  $rows = array();
  $sort_days = visitors_get_days_of_week();

  foreach ($sort_days as $day => $value) {
    $rows[$value] = array($value, t($day), 0);
  }

  foreach ($tmp_rows as $tmp_item) {
    $rows[$sort_days[drupal_ucfirst(drupal_strtolower($tmp_item[0]))]][2] = $tmp_item[1];
  }

  $output  = visitors_date_filter();

  if ($count > 0) {
    $output .= sprintf(
        '<img src="%s" alt="%s" width="%d" height="%d">',
        url('visitors/days_of_week/graph'),
        t('Days of week'),
        visitors_get_chart_width(),
        visitors_get_chart_height()
    );
  }
  $output .= theme('table', $header, $rows);
  $output .= theme('pager', NULL, $items_per_page, 0);

  return $output;
}

/**
 * Display days of week chart report.
 */
function graph_visitors_days_of_week() {
  $result = visitors_days_of_week_data();
  $tmp_rows = array();

  while ($data = db_fetch_object($result)) {
    $tmp_rows[$data->n] = array(
      $data->d,
      $data->count,
      $data->n
    );
  }

  $rows = array();
  for ($i = 0; $i < 7; $i++) {
    $rows[$i] = 0;
  }

  $sort_days = visitors_get_days_of_week();
  foreach ($tmp_rows as $key => $tmp_item) {
    $rows[$sort_days[drupal_ucfirst(drupal_strtolower($tmp_item[0]))] - 1] = (int) $tmp_item[1];
  }

 // build dates series
  $dates = array();
  foreach ($sort_days as $day => $value) {
      $dates[] = t($day);
  }

  visitors_graph($rows, $dates);
}

