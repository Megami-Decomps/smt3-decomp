# Combine camera point name

`cmbCameraPointNames` at ROM 0x23059C is the last pointer in the native
combine marker-name group. The first three name the model markers; the
one-entry initial-camera view is used for the scene's camera-point lookup.
It relocates against `cmbAkumaposName04` with zero addend. Keep the two
views in one aligned object to preserve the native fourth-word address.
