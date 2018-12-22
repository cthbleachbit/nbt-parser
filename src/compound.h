#ifndef __libnbtp_compound
#define __libnbtp_compound

#include "parser.h"

/*
 * Decompose compound, returned as a complete new tag
 */
tag* nbtp_compound_decomp(parse_info *info);

/*
 * Decompose list, returned as a complete new list tag
 */
tag* nbtp_list_decomp(parse_info *info);

#endif
