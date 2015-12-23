/** @file

   Author : ChiChen
   Date   : 2012-06-14

**/

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiRuntimeServicesTableLib.h>


EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{

  gRT->ResetSystem (EfiResetWarm, EFI_SUCCESS, 0, NULL);

  return EFI_SUCCESS;
}
