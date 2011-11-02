<?php

/**
 * @file
 * Admin page callbacks for the visitors module.
 */

define('VISITORS_MODULE_DIR'        , dirname(__FILE__));
define('VISITORS_MODULE_CHART_DIR'  , VISITORS_MODULE_DIR . '/pchart');
define('VISITORS_MODULE_FORMS_DIR'  , VISITORS_MODULE_DIR . '/forms');
define('VISITORS_MODULE_REPORTS_DIR', VISITORS_MODULE_DIR . '/reports');

require_once VISITORS_MODULE_DIR . '/visitors.chart';

require_once VISITORS_MODULE_FORMS_DIR . '/date_filter.php';
require_once VISITORS_MODULE_FORMS_DIR . '/referers.php';

require_once VISITORS_MODULE_REPORTS_DIR . '/days_of_month.php';
require_once VISITORS_MODULE_REPORTS_DIR . '/days_of_week.php';
require_once VISITORS_MODULE_REPORTS_DIR . '/hit_details.php';
require_once VISITORS_MODULE_REPORTS_DIR . '/hosts.php';
require_once VISITORS_MODULE_REPORTS_DIR . '/hours.php';
require_once VISITORS_MODULE_REPORTS_DIR . '/monthly_history.php';
require_once VISITORS_MODULE_REPORTS_DIR . '/recent_hits.php';
require_once VISITORS_MODULE_REPORTS_DIR . '/referers.php';
require_once VISITORS_MODULE_REPORTS_DIR . '/top_pages.php';
require_once VISITORS_MODULE_REPORTS_DIR . '/user_activity.php';

