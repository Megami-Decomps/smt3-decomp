# Particle callback tables

The live initialization and scale tables each own thirteen callback
pointers. All twenty-six relocations target their original functions.
Those functions accept the dispatchers' real context argument and use the
original concrete state views internally. Both four-byte alignment words
and the preceding callback and offset storage stay in the raw input.
