<?php

/**
 * Administration form.
 */
function ting_local_bibliography_admin_settings_form() {
  $form = array();

  $form['ting_local_bibliography_recipient_email'] = array(
    '#type' => 'textfield',
    '#title' => t('Library staff email'),
    '#description' => t('This is the email address to which local bibliography order notifications will be sent. A copy of the notification will also be sent to the user.'),
    '#required' => TRUE,
    '#default_value' => variable_get('ting_local_bibliography_recipient_email',  variable_get('site_mail', ini_get('sendmail_from'))),
  );

  $form['ting_local_bibliography_mailtext'] = array(
    '#type' => 'textarea',
    '#title' => t('Mail text for sending notifications of local bibliography orders. This will be sent to library staff and the user.'),
    '#description' => t(
      'You can use the following variables to be replaced:
      <ul>
      <li>!name = the name of the user placing the order</li>
      <li>!address = the address of the user placing the order</li>
      <li>!city = the address of the user placing the order</li>
      <li>!birthday = the address of the user placing the order</li>
      <li>!email = the email address of the user placing the order</li>
      <li>!phone = the phone number of the user placing the order</li>
      <li>!material_title = the title of the item which the user wants to order</li>
      <li>!material_url = the url for the item which the user wants to order</li>
      <li>!library = the name of the library the user would like the item sent to</li>
      </ul>'
    ),
    '#default_value' => variable_get('ting_local_bibliography_mailtext', t(TING_LOCAL_BIBLIOGRAPHY_DEFAULT_MAILTEXT)),
    '#cols' => 80,
    '#rows' => 15
  );

  return system_settings_form($form);
}