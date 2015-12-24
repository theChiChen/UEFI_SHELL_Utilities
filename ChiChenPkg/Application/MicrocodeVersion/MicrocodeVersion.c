/** @file

  Author  : ChiChen
  Date  : 2015-12-23

**/

#include "MicrocodeVersion.h"

EFI_HII_HANDLE gStringPackHandle;

UINTN 
ReadMsr(
IN UINT32 Msr
  )
{
    return AsmReadMsr64(Msr);
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
MicrocodeVersionEntry (
  IN EFI_HANDLE                            ImageHandle,
  IN EFI_SYSTEM_TABLE                      *SystemTable
  )
{
  UINTN                 argc;
  CHAR16                **argv;
  EFI_STRING            String;
  UINT64                MSRData = 0;

  get_args(ImageHandle, SystemTable, &argv, &argc);

  gStringPackHandle = HiiAddPackages (
                         &gEfiCallerIdGuid,
                         ImageHandle,
                         MicrocodeVersionStrings,
                         NULL
                         );
  ASSERT (gStringPackHandle != NULL);



  switch (argc) {
    case 1:
      MSRData = ReadMsr(0x8B);
      String = HiiGetString (gStringPackHandle, STRING_TOKEN (STR_MICROCODE_VERSION_STRING), NULL);
      Print (String, MSRData >> 32);
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
