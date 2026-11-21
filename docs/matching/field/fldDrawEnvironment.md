# Field draw environment state

The mutable environment handle uses its original four-byte storage and
initial value. It is now defined in C alongside its existing users. This
partial small-data conversion does not add matching report credit.
