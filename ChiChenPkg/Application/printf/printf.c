/** @file

   Author : ChiChen
   Date   : 2012-06-14

**/

#include <stdio.h>

/**
  The user Entry Point for Application. The user code starts with this function
  as the real entry point for the application.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.  
  @param[in] SystemTable    A pointer to the EFI System Table.
  
  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
int
EFIAPI
main (
  IN int Argc,
  IN char **Argv
  )
{

  printf("HelloWorld");

  return EFI_SUCCESS;
}