# Battle camera editor diagnostics

The retained nbCameraShot_CommandFixPos_Calc2 uses these original header, distance, up, and ID formats.
Each C string contains only its text and terminator; native alignment is
not part of the string objects. The camera handler, jump table, and printf calls are
unchanged.
