<?php
// $Id: views-view-fields.tpl.php,v 1.6 2008/09/24 22:48:21 merlinofchaos Exp $
/**
 * @file views-view-fields.tpl.php
 * Default simple view template to all the fields as a row.
 *
 * - $view: The view in use.
 * - $fields: an array of $field objects. Each one contains:
 *   - $field->content: The output of the field.
 *   - $field->raw: The raw data for the field, if it exists. This is NOT output safe.
 *   - $field->class: The safe class id to use.
 *   - $field->handler: The Views field handler object controlling this field. Do not use
 *     var_export to dump this object, as it can't handle the recursion.
 *   - $field->inline: Whether or not the field should be inline.
 *   - $field->inline_html: either div or span based on the above flag.
 *   - $field->separator: an optional separator that may appear before a field.
 * - $row: The raw result object from the query, with all data it fetched.
 *
 * @ingroup views_templates
 */


?>

<?php print '<br><h4>' . $fields['title']->content . '</h4>'; ?>
<?php print $fields['body']->content; ?>
<?php print '<br><br>'; ?>
<?php print '<b>When: </b>' . $fields['field_evanced_event_date_value']->content . '  '
                            . '<b>Age Group: </b>' . $fields['field_evanced_event_agegroup_value']->content; ?>
<?php print '<br>'; ?>

<?php
if($fields['field_evanced_event_location_value']->content == 'Main Library'){
$event_location = '<a target="_blank" href="http://maps.google.com/maps?saddr=&daddr=1431 Assembly Street, Columbia, SC 29201">Main Library</a>';
}

if($fields['field_evanced_event_location_value']->content == 'The Link, Ballentine'){
$event_location = '<a target="_blank" href="http://maps.google.com/maps?saddr=&daddr=218 McNulty Rd., Blythewood, SC 29016">Blythewood Branch</a>';
}

if($fields['field_evanced_event_location_value']->content == 'Blythewood Branch'){
$event_location = '<a target="_blank" href="http://maps.google.com/maps?saddr=&daddr=5317 North Trenhold Rd., Columbia, SC 29206">Blythewood Branch</a>';
}

if($fields['field_evanced_event_location_value']->content == 'John Hughes Cooper Branch'){
$event_location = '<a target="_blank" href="http://maps.google.com/maps?saddr=&daddr=5317 North Trenhold Rd., Columbia, SC 29206">John Hughes Cooper Branch</a>';
}

if($fields['field_evanced_event_location_value']->content == 'Eastover Branch'){
$event_location = '<a target="_blank" href="http://maps.google.com/maps?saddr=&daddr=608 Main St., Eastover, SC 29044">Eastover Branch</a>';
}

if($fields['field_evanced_event_location_value']->content == 'North Main Branch'){
$event_location = '<a target="_blank" href="http://maps.google.com/maps?saddr=&daddr=5306 North Main St., Columbia, SC 29203">North Main Branch</a>';
}

if($fields['field_evanced_event_location_value']->content == 'Northeast Regional Branch'){
$event_location = '<a target="_blank" href="http://maps.google.com/maps?saddr=&daddr=7490 Parklane Rd., columbia, SC 29223">Northeast Regional Branch</a>';
}

if($fields['field_evanced_event_location_value']->content == 'Sandhills Branch'){
$event_location = '<a target="_blank" href="http://maps.google.com/maps?saddr=&daddr=1 Summit Parkway at Clemson Rd., Columbia, SC 29229">Sandhills Branch</a>';
}

if($fields['field_evanced_event_location_value']->content == 'Southeast Regional Branch'){
$event_location = '<a target="_blank" href="http://maps.google.com/maps?saddr=&daddr=7421 Garners Ferry Rd., Columbia, SC 29209">Southeast Regional Branch</a>';
}

if($fields['field_evanced_event_location_value']->content == 'St. Andrews Regional Branch'){
$event_location = '<a target="_blank" href="http://maps.google.com/maps?saddr=&daddr=2916 Broad River Rd., Columbia, SC 29210">St. Andrews Regional Branch</a>';
}

if($fields['field_evanced_event_location_value']->content == 'Wheatley Branch'){
$event_location = '<a target="_blank" href="http://maps.google.com/maps?saddr=&daddr=931 Woodrow St., Columbia, SC 29205">Wheatley Branch</a>';
}

if($fields['field_evanced_event_location_value']->content == 'RCPL Operations Center'){
$event_location = '<a target="_blank" href="http://maps.google.com/maps?saddr=&daddr=130 Lancewood Rd., Columbia, SC 29210">RCPL Operations Center</a>';
}
?>



<?php print '<b>Location: </b>' . $event_location; ?>

