# Battle panel UV records

## Shared parameter and layout views

**Unit:** newbattle/nbPanelParamTableData. **Size:** 912 bytes.

One C object holds all 57 native four-word texture-coordinate records,
including record 56. The Part UV reader can reach rows 0 through 10 from
its interior head. Party selection independently reads the second such
row through the named Layout view. It also uses another interior anchor
to load four words each at offsets -0x40, zero and +0x80. The analysis
display uses several more overlapping slices of the same records.

The named views do not allocate any data. Every name refers to a real
allocated consumer relocation at its original address; their bounds are
the full envelopes of the reviewed loads. An opted-in record view may
overlap only another compatible view of the same primary. Prefix bytes
describe a native access before an interior anchor without moving that
anchor. All other alias rules still apply.

The enclosing C object owns the entire native span. The consumer's
existing C and assembly continue to use the original relocation names.
