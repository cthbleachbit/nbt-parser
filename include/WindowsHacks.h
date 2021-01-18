//
// Created by cth451 on 2020/05/18.
//

#ifndef NBTP_WINDOWSHACKS_H
#define NBTP_WINDOWSHACKS_H

#if defined(_MSC_VER)
#include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#endif

#ifndef LINE_MAX
#define LINE_MAX 2048
#endif

#endif //NBTP_WINDOWSHACKS_H
