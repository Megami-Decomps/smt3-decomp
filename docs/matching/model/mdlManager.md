# Matching notes: model/mdlManager

## Model file and kind registries

The model loader uses seven eight-byte slots in each writable registry.
Slot five is empty in the file and is populated and cleared by HDD
registration. Slot six is live and points to the 352-entry alternate model
paths and their index records. The first path bank contains two native
model/texture/motion sets. The four preceding pointers are instead the
separate puzzle debug menu, which iterates its four entries. The physical
menu split preserves the generated IDA metadata despite its older combined
size estimate.
