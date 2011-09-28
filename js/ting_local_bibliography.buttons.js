/**
 * Javascript helpers for Ding reservation interaction buttons.
 */
Drupal.behaviors.tingLocalBibliographyButtons = function () {
  Drupal.tingButtons.dialogButton('.local-bibliography-order a', {});
  
  $('.local-bibliography-order a').ajaxSuccess( function(event, XMLHttpRequest, ajaxOptions) {
    // Add clientside validation when the form has been loaded  
    var form = $('#ting-local-bibliography-object-order-form');
    if (form.size() > 0) {
      form.validate();
      
      // Ensure a valid library has been selected
      form.find('select[name=library]').rules('add', {
        'library': true,
      });

      // Date of birth must follow format
      form.find('input[name=date_of_birth]').rules('add', { 
        'dateITA': true, 
        'messages' : {
          'dateITA': Drupal.t('Please enter a correct date')
        } 
      });

      // Email must be valid
      form.find('input[name=email]').rules('add', { 'email': true });
      
      // Phone should be a valid Danish phone number
      form.find('input[name=phone]').rules('add', { 'phoneDK': true });
    }
  });
};

// Validator for checking Danish phone numbers. Valid examples:
// - +45 12345678
// - 12 34 56 78
jQuery.validator.addMethod('phoneDK', function(phone_number, element) {
  return this.optional(element) || phone_number.length >= 8 &&
  phone_number.match(/^(\+45)?(\s?\d{2}){4}$/);
}, Drupal.t('Invalid number'));

// Validator for ensuring that a library is selected. Default value i 0.
jQuery.validator.addMethod('library', function(library, element) {
  return this.optional(element) || library != 0;
}, Drupal.t('Please select a library'));