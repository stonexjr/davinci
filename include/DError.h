/*
Copyright (c) 2013-2017 Jinrong Xie (jrxie at ucdavis dot edu)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef DERROR_H
#define DERROR_H

#include <stdlib.h>
#include <stdio.h>

#ifdef WIN32
#define  __func__ __FUNCTION__
#endif
//#define BOOL					 bool
#define DTRUE					   1
#define DFALSE					   0
#define DERROR_TRUE                1
#define DERROR_FALSE               0

#define DERROR_SUCCESS             0
#define DERROR_ERROR              -1
#define DERROR_ERR_MEM            -2
#define DERROR_ERR_IO             -4
#define DERROR_ERR_COMM           -3
#define DERROR_ERR_OVERFLOW       -5
#define DERROR_ERR_FUN_INVALID    -6

#define D_ABORT(msg, errno) {\
    D_ERR_MSG(msg);\
    exit(errno);\
}

#define D_ERR_MSG(msg) {\
    fprintf(stderr, "<%s:%s:%d> : %s \n", \
            __FILE__, __func__, __LINE__, (msg));\
    fflush(stderr);\
}

#define D_ASSERT(condition, msg, errno) {\
    if (!(condition)) {\
    D_ABORT(msg, errno);\
}\
}

#define D_P_MSG(id, root, msg) {\
    if (id == root) {\
    fprintf(stderr, "PE%d: %s", id, msg); \
    fflush(stderr);\
    }\
}

#define D_ERR_MSG_P(id, msg) {\
    fprintf(stderr, "Proc %d <%s:%s:%d> : %s \n", \
    (id), __FILE__, __func__, __LINE__, (msg));\
    fflush(stderr);\
}

#ifdef MPI_INCLUDED

#include <mpi.h>

#define D_ABORT_P(id, msg, errno) {\
    D_ERR_MSG_P(id, msg); \
    MPI_Abort(MPI_COMM_WORLD, errno);\
    exit(errno);\
}

#define D_ASSERT_P(id, condition, msg, errno) {\
    if (!(condition)) {\
    D_ABORT_P(id, msg, errno);\
    }\
}
#endif

#endif // DERROR_H
