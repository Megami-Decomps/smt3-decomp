# Field draw debug small data

The decimal and hexadecimal format strings retain their eight- and
four-byte native slots. fldDrawSpriteTex is one texture-handle pointer:
the setup path stores a single pointer and the draw paths read it back
at the same address. The four subsequent zero bytes remain linker fill,
not an invented second pointer or newly matched data.

The aligned small-data window starts at 0x54EC50. Existing draw
environment, blur handle and encounter mode storage keeps its original
types and addresses. Both stock modes produce the same new literal
data. The complete linked fldDraw code and allocated data reproduce
retail without changing any function body or compiler attributes.
