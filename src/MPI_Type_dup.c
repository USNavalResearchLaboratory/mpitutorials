/* 
MPI_Type_dup

   Duplicate a datatype
int MPI_Type_dup(
  MPI_Datatype datatype,
  MPI_Datatype *newtype
);

Parameters

   type
          [in] datatype (handle)

   newtype
          [out] copy of type (handle)

Remarks

   MPI_TYPE_DUP is a new type constructor which duplicates the existing
   type with associated key values. For each key value, the respective
   copy callback function determines the attribute value associated with
   this key in the new communicator; one particular action that a copy
   callback may take is to delete the attribute from the new datatype.
   Returns in newtype a new datatype with exactly the same properties as
   type and any copied cached information. The newtype has the same
   committed state as the old type.

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

int main(int argc, char *argv[])
{
    int myrank;
    MPI_Status status;
    MPI_Datatype type, type2;
    int buffer[100];

    MPI_Init(&argc, &argv);

    MPI_Type_contiguous( 100, MPI_CHAR, &type );
    MPI_Type_commit(&type);
    MPI_Type_dup(type, &type2);
    MPI_Type_free(&type);

    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    if (myrank == 0)
    {
        MPI_Send(buffer, 1, type2, 1, 123, MPI_COMM_WORLD);
    }
    else if (myrank == 1)
    {
        MPI_Recv(buffer, 1, type2, 0, 123, MPI_COMM_WORLD, &status);
    }

    MPI_Finalize();
    return 0;
}

