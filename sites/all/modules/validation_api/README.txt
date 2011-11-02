$Id: README.txt,v 1.3.2.3 2008/10/20 14:53:01 tapocol Exp $
Validation API Module
Author: Craig Jackson <tapocol [at] gmail [dot] com>
This module is apart of the 2008 Google Summer of Code.
Validation API allows admins to create validation rules on any form field with a UI.

How to Use:
1.) Install like any other Drupal module.
    Place validation_api in your modules folder (recommended: /sites/all/modules/).
    Goto admin/build/modules, check enabled on Validation API, and click submit.
2.) Validators.
    There are two options to create valitions on your site:
      Add them (admin/build/validation_api/validators/add).
      Import them (admin/build/validation_api/validators/import). Modules using hook_add_validator or you can use code from an export.
    You can create php or regex types.
    The rule for php types uses $value for the value entered and $arguments[x], with x being replaced by the delta of the argument.
    The rule for regex types automatically tests the value inputted. But, for arguments use %arguments[x], with x being replaced by the delta of the argument.
    For Drupal developers: #element_validate is the function used in the FAPI. So, you can use $form_state to use in conjunction with a PHP validation rule on a different field.
3.) Fields
    There are two options for creating the relationship between a field and one of your site's validators.
      Recommended: Find the field. Make sure that you have enabled 'Add a validator link' in settings (admin/build/validation_api/settings). Then, you can go to the field you want to use a validator on, and use the link directly below the field.
      Drupal Masters: Add form (admin/build/validation_api/fields/add). You need to already know the form_id and field location to set it up properly.
    The second step of creating a relationship is filling out arguments (if applicable), if the field can be empty, and you can override the message from the validator.
4.) You should be able to go to that form and test the validator for pass/fail.

Validator Example:
Fill out the form at admin/build/validation_api/validators/add
Name: pin
Type: Regular Expression
Rule: /^[0-9]{4,4}$/
Message: %field needs to be 4 numeric digits.

Validator + Field Example:
Make sure 'Add a validator link' is enabled in settings (admin/build/validation_api/settings).
Create a node with comments enabled (if you already have a node with comments enabled, you do not need to create one).
After creation, goto the add comment form for that node, and click 'Add a validator to subject' link.
This should send you to the fields add page with the following values:
Form ID: comment_form
Field Name: subject
Select pin in the 'Validator' select box, and click Next Step.
Check the box for 'Allow this field to be empty?'.
Message: Leave as is (%field needs to be 4 numeric digits.).
Save Field.
Go back to the comment form for that node, and try to following tests. You can use the Preview button to not accidentally submit a comment.
First:
  Subject:
  Comment: asdf
Should NOT return an error because the subject is allowed to be empty if you checked it on the previous form.

Second:
  Subject: 123a
  Comment: asdf
Should return an error because all characters are not 0-9.

Third:
  Subject: 123
  Comment: asdf
Should return an error because it is less than 4 characters.

Fourth:
  Subject: 12345
  Comment: asdf
Should return an error because it is more than 4 characters.

Fifth:
  Subject: 1234
  Comment: asdf
Should NOT return an error because all characters are 0-9, and it is 4 characters long.

You can keep testing to make sure it works properly for maximum length as well.

Good luck and remember: Validating is fun!
