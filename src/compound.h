#ifndef __libnbtp_compound
#define __libnbtp_compound

/*
 * Decompose compound, returned as a complete new tag
 */
tag* compoundDecomp(const char *buffer, int *offset);

/*
 * Decompose list, returned as a complete new list tag
 */
tag* listDecomp(const char *buffer, int *offset);

#endif
