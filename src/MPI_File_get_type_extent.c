/* 
MPI_File_get_type_extent

   Returns the extent of datatype in the file
int MPI_File_get_type_extent(
  MPI_File mpi_fh,
  MPI_Datatype datatype,
  MPI_Aint *extent
);

Parameters

   mpi_fh
          [in] file handle (handle)

   datatype
          [in] datatype (handle)

   extent
          [out] extent of the datatype (nonnegative integer)

Remarks

   Returns the extent of datatype in the file fh. This extent will be the
   same for all processes accessing the file fh. If the current view uses
   a user-defined data representation, MPI uses the dtype_file_extent_fn
   callback to calculate the extent.

   If the file data representation is other than "native," care must be
   taken in constructing etypes and filetypes. Any of the datatype
   constructor functions may be used; however, for those functions that
   accept displacements in bytes, the displacements must be specified in
   terms of their values in the file for the file data representation
   being used. MPI will interpret these byte displacements as is; no
   scaling will be done. The function MPI_FILE_GET_TYPE_EXTENT can be used
   to calculate the extents of datatypes in the file. For etypes and
   filetypes that are portable datatypes, MPI will scale any displacements
   in the datatypes to match the file data representation. Datatypes
   passed as arguments to read/write routines specify the data layout in
   memory; therefore, they must always be constructed using displacements
   corresponding to displacements in memory.

   Advice to users.

   One can logically think of the file as if it were stored in the memory
   of a file server. The etype and filetype are interpreted as if they
   were defined at this file server, by the same sequence of calls used to
   define them at the calling process. If the data representation is
   "native", then this logical file server runs on the same architecture
   as the calling process, so that these types define the same data layout
   on the file as they would define in the memory of the calling process.
   If the etype and filetype are portable datatypes, then the data layout
   defined in the file is the same as would be defined in the calling
   process memory, up to a scaling factor. The routine
   MPI_FILE_GET_FILE_EXTENT can be used to calculate this scaling factor.
   Thus, two equivalent, portable datatypes will define the same data
   layout in the file, even in a heterogeneous environment with
   "internal", "external32", or user defined data representations.
   Otherwise, the etype and filetype must be constructed so that their
   typemap and extent are the same on any architecture. This can be
   achieved if the they have an explicit upper bound and lower bound
   (defined either using MPI_LB and MPI_UB markers, or using
   MPI_TYPE_CREATE_RESIZED). This condition must also be fulfilled by any
   datatype that is used in the construction of the etype and filetype, if
   this datatype is replicated contiguously, either explicitly, by a call
   to MPI_TYPE_CONTIGUOUS, or implictly, by a blocklength argument that is
   greater than one. If an etype or filetype is not portable, and has a
   typemap or extent that is architecture dependent, then the data layout
   specified by it on a file is implementation dependent.

   File data representations other than "native" may be different from
   corresponding data representations in memory. Therefore, for these file
   data representations, it is important not to use hardwired byte offsets
   for file positioning, including the initial displacement that specifies
   the view. When a portable datatype is used in a data access operation,
   any holes in the datatype are scaled to match the data representation.
   However, note that this technique only works when all the processes
   that created the file view build their etypes from the same predefined
   datatypes. For example, if one process uses an etype built from MPI_INT
   and another uses an etype built from MPI_FLOAT, the resulting views may
   be nonportable because the relative sizes of these types may differ
   from one data representation to another.

*/
 
// Copyright 2009 Deino Software. All rights reserved.
// Source: http://mpi.deino.net/mpi_functions/index.htm

#include "mpi.h"
#include <stdio.h>

int main( int argc, char *argv[] )
{
    int errs = 0;
    MPI_File fh;
    MPI_Comm comm;
    MPI_Aint extent, nextent;
    MPI_Init( &argc, &argv );
    comm = MPI_COMM_WORLD;
    MPI_File_open( comm, "test.ord", MPI_MODE_RDWR | MPI_MODE_CREATE |
		   MPI_MODE_DELETE_ON_CLOSE, MPI_INFO_NULL, &fh );
    MPI_File_get_type_extent( fh, MPI_INT, &extent );
    MPI_Type_extent( MPI_INT, &nextent );
    if (nextent != extent) {
        errs++;
        fprintf( stderr, "Native extent not the same as the file extent\n" );
	fflush(stderr);
    }
    
    MPI_File_close( &fh );
    MPI_Finalize();
    return errs;
}

