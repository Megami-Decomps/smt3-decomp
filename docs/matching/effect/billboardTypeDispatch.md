# Billboard type dispatch

Normal and P2A billboards use two 12-byte dispatch records: create, draw,
and destroy. The constructors and callbacks have the same C signatures
as the existing call sites. This split uses those six functions directly,
without changing the billboard manager's dispatch. The following zero and
float words do not belong to either callback record and remain raw.
