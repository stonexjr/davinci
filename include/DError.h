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
