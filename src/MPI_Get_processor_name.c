/* 
MPI_Get_processor_name

   Gets the name of the processor
int MPI_Get_processor_name(
  char *name,
  int *resultlen
);

int MPI_Get_processor_name(
  wchar_t *name,
  int *resultlen
);

Parameters

   name
          [out] A unique specifier for the actual (as opposed to virtual)
          node. This must be an array of size at least
          MPI_MAX_PROCESSOR_NAME.

   resultlen
          [out] Length (in characters) of the name

Remarks

   The name returned should identify a particular piece of hardware; the
   exact format is implementation defined. This name may or may not be the
   same as might be returned by gethostname, uname, or sysinfo.

   This routine returns the name of the processor on which it was called
   at the moment of the call. The name is a character string for maximum
   flexibility. From this value it must be possible to identify a specific
   piece of hardware; possible values include "processor 9 in rack 4 of
   mpp.cs.org" and "231" (where 231 is the actual processor number in the
   running homogeneous system). The argument name must represent storage
   that is at least MPI_MAX_PROCESSOR_NAME characters long.
   MPI_GET_PROCESSOR_NAME may write up to this many characters into name.

   The number of characters actually written is returned in the output
   argument, resultlen.

   Rationale.

   This function allows MPI implementations that do process migration to
   return the current processor. Note that nothing in MPI requires or
   defines process migration; this definition of MPI_GET_PROCESSOR_NAME
   simply allows such an implementation.
   Advice to users.

   The user must provide at least MPI_MAX_PROCESSOR_NAME space to write
   the processor name --- processor names can be this long. The user
   should examine the ouput argument, resultlen, to determine the actual
   length of the name.

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

   In Fortran, the character argument should be declared as a character
   string of MPI_MAX_PROCESSOR_NAME rather than an array of dimension
   MPI_MAX_PROCESSOR_NAME. That is,
   character*(MPI_MAX_PROCESSOR_NAME) name

   rather than
   character name(MPI_MAX_PROCESSOR_NAME)

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

*/
 
// Copyright 2009 Deino Software. All rights reserved.
// Source: http://mpi.deino.net/mpi_functions/index.htm


#include "mpi.h"
#include <stdio.h>
int main(int argc, char *argv[])
{
    int rank, nprocs, len;
    char name[MPI_MAX_PROCESSOR_NAME];
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Get_processor_name(name, &len);
    printf("Hello, world.  I am %d of %d on %s\n", rank, nprocs,
name);fflush(stdout);
    MPI_Finalize();
    return 0;
}

