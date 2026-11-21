#include "sdk_libc_alloc.h"

#define MEMBER_OFFSET(type, member) ((size_t)&((type *)0)->member)

SIZE_CHECK(mallocChunkPrefixSize, mallocChunkPrefix_t, 0x10);
SIZE_CHECK(mallocBinLinksSize, mallocBinLinks_t, 0x8);
SIZE_CHECK(mallocArenaSize, mallocArenaPrefix_t, 0x408);

/* The allocator uses each bin's own link pair as the empty-list sentinel.
 * Its chunk view backs up from the link pair to the forward member's offset.
 * The sentinel values point within the arena; they are not allocated chunks. */
#define BIN_SENTINEL(index)                                                 \
    ((mallocChunkPrefix_t *)((u8 *)&__malloc_av_ +                           \
        MEMBER_OFFSET(mallocArenaPrefix_t, bins) +                               \
        (index) * sizeof(mallocBinLinks_t) -                                \
        MEMBER_OFFSET(mallocChunkPrefix_t, forward)))
#define EMPTY_BIN(index) { BIN_SENTINEL(index), BIN_SENTINEL(index) }
#define EIGHT_EMPTY_BINS(first)                                             \
    EMPTY_BIN(first), EMPTY_BIN((first) + 1),                               \
    EMPTY_BIN((first) + 2), EMPTY_BIN((first) + 3),                         \
    EMPTY_BIN((first) + 4), EMPTY_BIN((first) + 5),                         \
    EMPTY_BIN((first) + 6), EMPTY_BIN((first) + 7)

mallocArenaPrefix_t __malloc_av_ = {
    0,
    0,
    (mallocChunkPrefix_t *)&__malloc_av_,
    (mallocChunkPrefix_t *)&__malloc_av_,
    {
        EIGHT_EMPTY_BINS(0), EIGHT_EMPTY_BINS(8),
        EIGHT_EMPTY_BINS(16), EIGHT_EMPTY_BINS(24),
        EIGHT_EMPTY_BINS(32), EIGHT_EMPTY_BINS(40),
        EIGHT_EMPTY_BINS(48), EIGHT_EMPTY_BINS(56),
        EIGHT_EMPTY_BINS(64), EIGHT_EMPTY_BINS(72),
        EIGHT_EMPTY_BINS(80), EIGHT_EMPTY_BINS(88),
        EIGHT_EMPTY_BINS(96), EIGHT_EMPTY_BINS(104),
        EIGHT_EMPTY_BINS(112),
        EMPTY_BIN(120), EMPTY_BIN(121), EMPTY_BIN(122), EMPTY_BIN(123),
        EMPTY_BIN(124), EMPTY_BIN(125), EMPTY_BIN(126),
    },
};
