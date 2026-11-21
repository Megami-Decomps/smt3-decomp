# Status camera down vector

The 16-byte record at ROM 0x246F60 is the source of full aligned
quadword copies to the camera's down vector in the Camp status model
initializer and parameter refresh, and in the Encyclopedia model setup.
Its four single-precision lanes are 0, 1, 0, 1. The Camp declaration
now uses the same vector type as the Encyclopedia consumer; the
existing quadword copies are unchanged.
