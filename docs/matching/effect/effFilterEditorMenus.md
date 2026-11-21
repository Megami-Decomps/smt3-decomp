# Filter editor menus

The filter and partial multi-filter editors each own a 64-byte menu
descriptor. The pointer slots, callbacks and existing creation arguments
match the retail tables. Callbacks conform to the dispatcher’s single
object-pointer interface and retain their concrete effect state within
the provider. No additional handle storage or alignment bytes are counted.
