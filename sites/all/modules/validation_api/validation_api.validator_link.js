// $Id: validation_api.validator_link.js,v 1.1.2.2 2008/08/08 03:24:26 tapocol Exp $

$(document).ready(function() {
  $('ul.add_validator').before('<a class="show-add-links" href="#show-add-links">Add a validator to...</a>');
  $('ul.add_validator').hide();
  $('a.show-add-links').click(function() {
    $(this).siblings('ul.add_validator').toggle();
  });
});
