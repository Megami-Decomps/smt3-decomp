# Movie manager inline text

The movie directory scan compares the two existing file suffixes, and
the debug browser passes the third string to its row formatter. The
three strings occupy 15 content bytes with their terminators and nine
zero fill bytes in the original three eight-byte slots.

Leave the four-byte `PlayingNo` prefix in the movie manager. GCC aligns
these arrays to eight bytes in both stock modes, even with an explicit
four-byte alignment request. A separate small-data unit keeps that
alignment at the native string boundary without shifting `PlayingNo`
or altering the manager's text. No compiler constraint is needed.
