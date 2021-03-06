/* 
MPI_Type_create_resized

   Create a datatype with a new lower bound and extent from an existing
   datatype
int MPI_Type_create_resized(
  MPI_Datatype oldtype,
  MPI_Aint lb,
  MPI_Aint extent,
  MPI_Datatype *newtype
);

Parameters

   oldtype
          [in] input datatype (handle)

   lb
          [in] new lower bound of datatype (integer)

   extent
          [in] new extent of datatype (integer)

   newtype
          [out] output datatype (handle)

Remarks

   Returns in newtype a handle to a new datatype that is identical to
   oldtype, except that the lower bound of this new datatype is set to be
   lb, and its upper bound is set to be lb + extent. Any previous lb and
   ub markers are erased, and a new pair of lower bound and upper bound
   markers are put in the positions indicated by the lb and extent
   arguments. This affects the behavior of the datatype when used in
   communication operations, with count >1, and when used in the
   construction of new derived datatypes.

   Advice to users.

   It is strongly recommended that users use this new function, rather
   than the old MPI-1 function to set and access lower bound, upper bound
   and extent of datatypes.

Thread and Interrupt Safety

   This routine is thread-safe. This means that this routine may be safely
   used by multiple threads without the need for any user-provided thread
   locks. However, the routine is not interrupt safe. Typically, this is
   due to the use of memory allocation routines such as malloc or other
   non-MPICH runtime routines that are themselves not interrupt-safe.

Notes for Fortran

   All MPI routines in Fortran (except for MPI_WTIME and MPI_WTICK) have
   an additional argument ierr at the end of the argument list. ierr is an
   integer and has the same meaning as the return value of the routine in
   C. In Fortran, MPI routines are subroutines, and are invoked with the
   call statement.

   All MPI objects (e.g., MPI_Datatype, MPI_Comm) are of type INTEGER in
   Fortran.

Errors

   All MPI routines (except MPI_Wtime and MPI_Wtick) return an error
   value; C routines as the value of the function and Fortran routines in
   the last argument. Before the value is returned, the current MPI error
   handler is called. By default, this error handler aborts the MPI job.
   The error handler may be changed with MPI_Comm_set_errhandler (for
   communicators), MPI_File_set_errhandler (for files), and
   MPI_Win_set_errhandler (for RMA windows). The MPI-1 routine
   MPI_Errhandler_set may be used but its use is deprecated. The
   predefined error handler MPI_ERRORS_RETURN may be used to cause error
   values to be returned. Note that MPI does not guarentee that an MPI
   program can continue past an error; however, MPI implementations will
   attempt to continue whenever possible.

   MPI_SUCCESS
          No error; MPI routine completed successfully.

   MPI_ERR_TYPE
          Invalid datatype argument. May be an uncommitted MPI_Datatype
          (see MPI_Type_commit).

*/
 
// Copyright 2009 Deino Software. All rights reserved.
// Source: http://mpi.deino.net/mpi_functions/index.htm

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

int main( int argc, char *argv[] )
{
    int errs = 0, i;
    int rank, size, source, dest;
    int count;
    int *buf;
    MPI_Comm comm;
    MPI_Status status;
    MPI_Datatype newtype;

    MPI_Init( &argc, &argv );
    comm = MPI_COMM_WORLD;
    /* Determine the sender and receiver */
    MPI_Comm_rank( comm, &rank );
    MPI_Comm_size( comm, &size );
    source = 0;
    dest = size - 1;
    MPI_Type_create_resized( MPI_INT, 0, 3 * sizeof(int), &newtype );
    MPI_Type_commit( &newtype );
    for (count = 1; count < 65000; count = count * 2) {
        buf = (int *)malloc( count * 3 * sizeof(int) );
        if (!buf) {
            MPI_Abort( comm, 1 );
        }
        for (i=0; i<3*count; i++) buf[i] = -1;
        if (rank == source) {
            for (i=0; i<count; i++) buf[3*i] = i;
            MPI_Send( buf, count, newtype, dest, 0, comm );
            MPI_Send( buf, count, newtype, dest, 1, comm );
        }
        else if (rank == dest) {
            MPI_Recv( buf, count, MPI_INT, source, 0, comm, &status );
            for (i=0; i<count; i++) {
                if (buf[i] != i) {
                    errs++;
                    printf( "buf[%d] = %d\n", i, buf[i]
);fflush(stdout);
                }
            }
            for (i=0; i<count*3; i++) buf[i] = -1;
            MPI_Recv( buf, count, newtype, source, 1, comm, &status );
            for (i=0; i<count; i++) {
                if (buf[3*i] != i) {
                    errs++;
                    printf( "buf[3*%d] = %d\n", i, buf[i]
);fflush(stdout);
                }
            }
        }
    }

    MPI_Type_free( &newtype );

    MPI_Finalize();
    return errs;
}

