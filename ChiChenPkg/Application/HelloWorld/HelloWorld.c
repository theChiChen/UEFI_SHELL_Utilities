/** @file

  Author  : ChiChen
  Date  : 2015-12-22

**/

#include "HelloWorld.h"


EFI_HII_HANDLE gStringPackHandle;


EFI_STATUS
help_string(void)
{
  EFI_STRING            String;
  EFI_STRING_ID         StringID;
  StringID = STRING_TOKEN (STR_HELP);
  String = HiiGetString (gStringPackHandle, StringID, NULL);
  Print (L"%s", String);
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
HelloWorldEntry (
  IN EFI_HANDLE                            ImageHandle,
  IN EFI_SYSTEM_TABLE                      *SystemTable
  )
{
  UINTN argc;
  CHAR16 **argv;
  EFI_STRING            String;
  EFI_STRING_ID         StringID;

  get_args(ImageHandle, SystemTable, &argv, &argc);

  gStringPackHandle = HiiAddPackages (
                         &gEfiCallerIdGuid,
                         ImageHandle,
                         HelloWorldStrings,
                         NULL
                         );
  ASSERT (gStringPackHandle != NULL);



  switch (argc) {
    case 1:
      StringID = STRING_TOKEN (STR_HELLO_WORLD_STRING);
      String = HiiGetString (gStringPackHandle, StringID, NULL);
      Print (L"%s", String);
      break;
    case 2:
      if (StrCmp(argv[1], L"-h") == 0) {
        help_string();
      } else {
        StringID = STRING_TOKEN (STR_PARAMETER_ERROR_STRING);
        String = HiiGetString (gStringPackHandle, StringID, NULL);
        Print (L"%s", String);
      }
      break;
    default:
        StringID = STRING_TOKEN (STR_PARAMETER_ERROR_STRING);
        String = HiiGetString (gStringPackHandle, StringID, NULL);
        Print (L"%s", String);
      break;

  }
  

  HiiRemovePackages (gStringPackHandle);
  return EFI_SUCCESS;
  
}
