# Non-matching drafts: movie/MovieViewer

Conventions and the reason these are kept: [../nonmatching.md](../nonmatching.md).

## MovieViewerProc (movie/MovieViewer, 0x24C)

The legacy guarded draft still emits 576 bytes against native 588, with 6 of
144 emitted words matching. The zero-credit shared-record prerequisite now uses
the actual `sdfMovieCb_t.decodeCb.fps`, the 20-byte initialization descriptor,
and the canonical void player/index APIs; it does not promote this body.

The earlier diagnosis that no C spelling could form the split address was too
broad. The actual nested record plus separate cadence branches supplies that
address form. Native/DDS comparison also identifies successful cursor movement
as mutually exclusive and SMT3 replay as actual global phase updates. These
are body work, separate from the provider prerequisite. See
[the record/API evidence](../../movie-records.md).
