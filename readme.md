# Ting Local Bibliography
Ting local bibliography is Drupal module which adds support for local bibliography objects to the [Ding.Ting project](http://ting.dk).

## Use
The module identifies Ting objects as local bibliography objects if their `ac.source` contains the value `Lokalbibliografi`.

The module does two things for these objects:

### Identification
Identify them as local bibliography objects in the user interface through a label.

### Ordering
Acquiring these objects is different from other materials. A new *Order material* button replaces the standard reserve and cart buttons. Clicking this button allows any user (even people who are not logged in) to fill out a form and thereby request the object by email.

![Ting local bibliography order form](https://github.com/downloads/helsbib/ting-local-bibliography/ting_local_bibliography_order.png)

The recipient and wording of this email can be configured in the administration interface uder `/admin/settings/ting/local_bibliography`

## Developers
The module adds a new ting object class, `local_bibliography` which can been checked using `ting_object_is()`.

## Credits
Ting local bibliography module has been developed for [Helsingør libraries](http://helsbib.dk) by [Reload!](http://reload.dk).
