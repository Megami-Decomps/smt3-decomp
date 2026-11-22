# Field AI diagnostics

The retained fldRunAiCng prints these ten native mode labels in their
original order. Each C array contains thirteen bytes including its NUL;
the three bytes before the next aligned address are storage
padding, not part of the string. Only the labels leave the assembly
fallback. The processing function and its call sites remain untouched.
