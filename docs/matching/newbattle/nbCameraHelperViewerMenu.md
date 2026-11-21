# Camera Helper menu descriptor

The primary Camera Helper menu owns a 64-byte descriptor. It uses the
existing four menu items and six native pointer slots, including the
shared initializer in the recreate slot. The nested PB menu is separate.
The proxy reads its parent through the normal menu layout, preserving
the original callback argument and return value.
