/* The four-byte label slots cover decimal floors, basement/floor suffixes,
 * and the roof. The zero-filled tail to the next native object stays raw. */
char fldElvFloorLabels[13][4] __attribute__((section(".data"), aligned(8))) = {
    "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "B", "F", "R",
};
