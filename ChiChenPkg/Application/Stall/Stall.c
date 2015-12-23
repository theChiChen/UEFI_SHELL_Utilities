/** @file

   Author : ChiChen
   Date   : 2012-06-14

**/

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h> 


EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{

  Print(L"Wait 10s!!\n");

  gBS->Stall(10000000);

  Print(L"Hi!\n");

  return EFI_SUCCESS;
}
