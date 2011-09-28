/*
 * Default messages for the jQuery validation plugin
 * Passed through the Drupal javascript translation engine.
 */
jQuery.extend(jQuery.validator.messages, {
  required: Drupal.t("This field is required."),
  remote: Drupal.t("Please fix this field."),
  email: Drupal.t("Please enter a valid email address."),
  url: Drupal.t("Please enter a valid URL."),
  date: Drupal.t("Please enter a valid date."),
  dateISO: Drupal.t("Please enter a valid date (ISO)."),
  number: Drupal.t("Please enter a valid number."),
  digits: Drupal.t("Please enter only digits."),
  creditcard: Drupal.t("Please enter a valid credit card number."),
  equalTo: Drupal.t("Please enter the same value again."),
  accept: Drupal.t("Please enter a value with a valid extension."),
  maxlength: Drupal.t($.validator.format("Please enter no more than {0} characters.")),
  minlength: Drupal.t($.validator.format("Please enter at least {0} characters.")),
  rangelength: Drupal.t($.validator.format("Please enter a value between {0} and {1} characters long.")),
  range: Drupal.t($.validator.format("Please enter a value between {0} and {1}.")),
  max: Drupal.t($.validator.format("Please enter a value less than or equal to {0}.")),
  min: Drupal.t($.validator.format("Please enter a value greater than or equal to {0}."))
});
