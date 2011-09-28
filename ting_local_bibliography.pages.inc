<?php
/**
 * @file
 * Page callbacks for Ding reservation module.
 */

/**
 * Reserve an item immediately.
 */
function ting_local_bibliography_page_order_item($object) {
  global $user;

  $form = drupal_get_form('ting_local_bibliography_object_order_form', $object);

  // If client wants JSON (usually AJAX requests), let's give it to them.
  if (ting_local_bibliography_detect_ahah()) {
    $reply['status'] = 'success';
    $reply['title'] = t('Order %title', array('%title' => $object->title));
    $reply['message'] = $form;

    // We are returning JavaScript, so tell the browser.
    drupal_set_header('Content-Type: application/json');
    echo json_encode($reply);
    return;
  }

  return $form;
}


/**
 * Detect if a request wants a JSON-response, usually for AHAH.
 */
function ting_local_bibliography_detect_ahah() {
  return (isset($_SERVER['HTTP_ACCEPT']) && (strpos($_SERVER['HTTP_ACCEPT'], 'application/json') !== FALSE) || (strpos($_SERVER['HTTP_ACCEPT'], 'text/javascript')) !== FALSE);
}
