<?php
/**
 * @file
 * Page callbacks for Ding reservation module.
 */

/**
 * Reserve an item immediately.
 *
 * @param $item
 * 	A string containing either an object id or an
 *  object title;
 */
function ting_local_bibliography_page_order_item($item) {
  global $user;

  // If the item is a object id then determine title from the
  // object. Else we assume it is a title
  $title = $item;

  module_load_include('client.inc', 'ting');
  if ($object = ting_get_object_by_id($item)) {
    $title = $object->title;
  }

  $form = drupal_get_form('ting_local_bibliography_object_order_form', $item);

  // If client wants JSON (usually AJAX requests), let's give it to them.
  if (ting_local_bibliography_detect_ahah()) {
    $reply['status'] = 'success';
    $reply['title'] = t('Order %title', array('%title' => $title));
    $reply['message'] = $form;

    // We are returning JavaScript, so tell the browser.
    drupal_set_header('Content-Type: application/json');
    echo json_encode($reply);
    return;
  }

  return $form;
}

/**
 * Form to order a local bibliography object
 */
function ting_local_bibliography_object_order_form(&$form_state, $item) {
  global $user;

  $form = array();

  $form['item'] = array(
    '#type' => 'hidden',
    '#value' => $item,
  );

  $library_options = array();
  $default_library = NULL;

  // Fetch all library nodes defined as nodes
  // Retrieving branches from the library system yields a lot of irrelevant
  // locations as there is no way to retrieve reservation branches only.
  // This means that we cannot select the users' preferred branch as default
  // as the names may differ.
  $result = db_query('SELECT nid, title FROM {node} WHERE type = "library"');
  while ($library = db_fetch_array($result)) {
    $library_options[$library['title']] = $library['title'];
  }

  $form['library'] = array(
    '#type' => 'select',
    '#options' => array_merge(array(t('Choose a library branch')) , $library_options),
    '#title' => t('Library'),
    '#description' => t('The library where you want to pick up the material'),
    '#required' => TRUE,
  );

  $form['user'] = array(
    '#type' => 'fieldset',
    '#title' => t('Personal information'),
  );

  $form['user']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#required' => TRUE,
    '#default_value' => (isset($user->display_name) ? $user->display_name : ''),
  );

  $form['user']['address'] = array(
    '#type' => 'textfield',
    '#title' => t('Address'),
    '#required' => TRUE,
    '#default_value' => '',
  );

  $form['user']['city'] = array(
    '#type' => 'textfield',
    '#title' => t('City'),
    '#required' => TRUE,
    '#default_value' => '',
  );

  $form['user']['date_of_birth'] = array(
    '#type' => 'textfield',
    '#title' => t('Date of birth'),
    '#description' => t('Format: dd/mm/yyyy'),
    '#required' => TRUE,
    '#default_value' => '',
  );

  $form['user']['email'] = array(
    '#type' => 'textfield',
    '#title' => t('Email'),
    '#required' => TRUE,
    '#default_value' => (isset($user->mail) ? $user->mail : ''),
  );

  $form['user']['phone'] = array(
    '#type' => 'textfield',
    '#title' => t('Telephone number'),
    '#required' => TRUE,
  );

  $form['buttons']['order'] = array(
    '#type' => 'submit',
    '#op' => 'order',
    '#value' => t('Order item'),
  );

  return $form;
}

/**
 * Validate handler for order form.
 */

function ting_local_bibliography_object_order_form_validate($form, &$form_state) {
  module_load_include('client.inc', 'ting');

  if (!$form_state['values']['item']) {
    form_set_error('item', t('You have tried to order an object which is not a local bibliography.'));
  } else {
    $object = ting_get_object_by_id($form_state['values']['item']);
    // If the item is a valid object then it must be a local bibliography one.
    if ($object && !ting_object_is($object, 'local_bibliography')) {
      form_set_error('item', t('You have tried to order an object which is not a local bibliography.'));
    }
  }

  // Make sure the library selected is not the default with value 0
  if (!$form_state['values']['library']) {
    form_set_error('library', t('Please select a library.'));
  }

  if (!filter_var($form_state['values']['email'], FILTER_VALIDATE_EMAIL)) {
    form_set_error('email', t('Please enter a valid email address.'));
  }

  // Date must follow format dd/mm/yyyy
  $date = explode('/', $form_state['values']['date_of_birth'], 3);
  if (!checkdate($date[1], $date[0], $date[2])) {
    form_set_error('date_of_birth', t('Please enter a valid date of birth.'));
  }

  // Phone number must follow format
  if (!preg_match('/^(\+45)?(\s?\d{2}){4}$/', $form_state['values']['phone'])) {
    form_set_error('phone', t('Please enter a valid telephone number.'));
  }
}

/**
 * Submit handler for order form.
 */
function ting_local_bibliography_object_order_form_submit($form, &$form_state) {
  // Fetch the object which is being ordered
  $title = $form_state['values']['item'];
  $url = '';
  if ($object = ting_get_object_by_id($form_state['values']['item'])) {
    $title = $object->title;
    $url = url('ting/object/' . $object->id, array('absolute' => true));
  }

  // Determine library from and to emails
  $email_from = variable_get('site_mail', ini_get('sendmail_from'));
  $email_to = variable_get('ting_local_bibliography_recipient_email', variable_get('site_mail', ini_get('sendmail_from')));

  // Email will also be sent to the requesting user
  $email_to . ', ' . $form_state['values']['email'];

  // Compose email
  $message['subject'] = t('!site: New local bibliography order', array('!site' => variable_get('site_name', 'Ding!')));
  $message['body'] = t(variable_get('ting_local_bibliography_mailtext', TING_LOCAL_BIBLIOGRAPHY_DEFAULT_MAILTEXT),
        array(
          '!name' => $form_state['values']['name'],
          '!address' => $form_state['values']['address'],
          '!city' => $form_state['values']['city'],
          '!date_of_birth' => $form_state['values']['date_of_birth'],
        	'!email' => $form_state['values']['email'],
          '!phone' => $form_state['values']['phone'],
          '!library' => $form_state['values']['library'],
          '!material_title' => $title,
          '!material_url' => $url,
        )
      );


  // Send email
  drupal_mail('ting_local_bibliography',
              'local_bibliography_order',
              $email_to,
              language_default(),
              $message,
              variable_get('site_mail', ini_get('sendmail_from')));

  // Set a message to notify the user that the order is succesful.
  drupal_set_message(t('You have ordered %title. The library has been notified and a reciept has been emailed to you.', array('%title' => $title)));

  // If we have an object then redirect to the object page. If not bail to the front page.
  $result_uri = ($object) ? 'ting/object/' . $object->id : '<front>';
  // If we have an referrer and it is not the order page then redirect to it instead of the object/front page
  if (referer_uri() && !strpos(referer_uri(), 'ting/local_bibliography/order')) {
    $result_uri = referer_uri();
  }
  drupal_goto($result_uri);
}

/**
 * Detect if a request wants a JSON-response, usually for AHAH.
 */
function ting_local_bibliography_detect_ahah() {
  return (isset($_SERVER['HTTP_ACCEPT']) && (strpos($_SERVER['HTTP_ACCEPT'], 'application/json') !== FALSE) || (strpos($_SERVER['HTTP_ACCEPT'], 'text/javascript')) !== FALSE);
}
