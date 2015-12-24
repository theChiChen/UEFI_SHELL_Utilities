/** @file

  Author  : ChiChen
  Date  : 2015-12-24

**/

#include "MacAddr.h"

#include <Library/PciExpressLib.h>
#include <Library/IoLib.h>

#define PCI_MAX_BUS     255
#define PCI_MAX_DEVICE  31
#define PCI_MAX_FUNC    7

EFI_HII_HANDLE gStringPackHandle;


EFI_STATUS
Find_MAC_Address()
{

  UINT8 NetworkDeviceCount = 0x00, VendorIdAddress = 0x00, MemBarAddress = 0x10;
  UINT16 VendorID, MemBar, ClassCode;
  UINT8 ClassCodeAddress = 0x0A, MacAddress[5], i;
  UINTN Bus, Device, Func;
  UINTN Addr;
  EFI_STRING            String;

  for (Bus = 0; Bus <= PCI_MAX_BUS; Bus++) {
    //
    // For each devices, enumerate all functions it contains
    //
    for (Device = 0; Device <= PCI_MAX_DEVICE; Device++) {
      //
      // For each function, read its configuration space and print summary
      //
      for (Func = 0; Func <= PCI_MAX_FUNC; Func++) {
        Addr = PCI_EXPRESS_LIB_ADDRESS(Bus, Device, Func, VendorIdAddress);
        VendorID = PciExpressRead16(Addr);

        if (VendorID == 0xffff && Func == 0) {
          break;
        }

        Addr = PCI_EXPRESS_LIB_ADDRESS(Bus, Device, Func, ClassCodeAddress);
        ClassCode = PciExpressRead16(Addr);
        if (ClassCode == 0x0200){
          NetworkDeviceCount++;
          Addr = PCI_EXPRESS_LIB_ADDRESS(Bus, Device, Func, MemBarAddress);
          MemBar = PciExpressRead16(Addr) & 0xFFFC;
          for (i = 0; i < 6; i++) {
            MacAddress[i] = IoRead8(MemBar + i);
          }
          String = HiiGetString (gStringPackHandle, STRING_TOKEN (STR_MAC_ADDRESS_STRING), NULL);
          Print (String, NetworkDeviceCount, MacAddress[0],MacAddress[1],MacAddress[2],MacAddress[3],MacAddress[4],MacAddress[5]);
        }

      }
    }
  }

  return EFI_SUCCESS;
}

EFI_STATUS
help_string(void)
{
  EFI_STRING            String;

  String = HiiGetString (gStringPackHandle, STRING_TOKEN (STR_HELP), NULL);
  Print (String);
  return EFI_SUCCESS;
}

EFI_STATUS
get_args(
  IN EFI_HANDLE                 ImageHandle,
  IN EFI_SYSTEM_TABLE           *SystemTable,
  OUT CHAR16                    ***argv,
  OUT UINTN                     *argc
  )
{
  EFI_STATUS            Status;
  EFI_SHELL_INTERFACE   *ShellInterface;
  EFI_GUID              ShellInterfaceProtocolGuid = SHELL_INTERFACE_PROTOCOL_GUID;

  Status = SystemTable->BootServices->HandleProtocol (
                    ImageHandle,
                    &ShellInterfaceProtocolGuid,
                    &ShellInterface
                    );

  *argc = ShellInterface->Argc;
  *argv = ShellInterface->Argv;

  return EFI_SUCCESS;
}


EFI_STATUS
EFIAPI
MacAddrEntry (
  IN EFI_HANDLE                            ImageHandle,
  IN EFI_SYSTEM_TABLE                      *SystemTable
  )
{
  UINTN argc;
  CHAR16 **argv;
  EFI_STRING            String;

  get_args(ImageHandle, SystemTable, &argv, &argc);

  gStringPackHandle = HiiAddPackages (
                         &gEfiCallerIdGuid,
                         ImageHandle,
                         MacAddrStrings,
                         NULL
                         );
  ASSERT (gStringPackHandle != NULL);



  switch (argc) {
    case 1:
      Find_MAC_Address();
      break;
    case 2:
      if (StrCmp(argv[1], L"-h") == 0 || StrCmp(argv[1], L"/h") == 0 || StrCmp(argv[1], L"-?") == 0 || StrCmp(argv[1], L"/?") == 0 || StrCmp(argv[1], L"?") == 0) {
        help_string();
      } else {
        String = HiiGetString (gStringPackHandle, STRING_TOKEN (STR_PARAMETER_ERROR_STRING), NULL);
        Print (String);
      }
      break;
    default:
        String = HiiGetString (gStringPackHandle, STRING_TOKEN (STR_PARAMETER_ERROR_STRING), NULL);
        Print (String);
      break;

  }
  

  HiiRemovePackages (gStringPackHandle);
  return EFI_SUCCESS;
  
}
