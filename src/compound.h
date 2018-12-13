#ifndef __libnbtp_compound
#define __libnbtp_compound

#include "parser.h"

/*
 * Decompose compound, returned as a complete new tag
 */
tag* compoundDecomp(parse_info *info);

/*
 * Decompose list, returned as a complete new list tag
 */
tag* listDecomp(parse_info *info);

#endif
