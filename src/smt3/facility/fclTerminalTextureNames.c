extern const char trmTmxPath[40];

/* fclLoadTexture reads the path without modifying it. */
char *trmTextureNames[1] __attribute__((section(".data"))) = {
    (char *)trmTmxPath,
};
