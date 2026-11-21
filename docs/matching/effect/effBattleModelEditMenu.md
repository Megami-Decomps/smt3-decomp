# Battle model editor menu

The native model editor descriptor is a standalone 64-byte writable menu
record. Its existing title and 24-item table are already C objects. The
create and update pointers use the original model-color snapshot and light
update functions, whose prototypes match the shared menu's dispatcher.
The unused callback, handle, parent and copy slots remain zero.
