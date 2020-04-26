#include <PiDxe.h>

#include <Library/DebugLib.h>
#include <Library/ResetSystemLib.h>
#include <Library/EfiResetSystemLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include <Library/LKEnvLib.h>
#include <Library/IoLib.h>
#include <Library/dload_util.h>
#include <Protocol/QcomPm8x41.h>
#include <Platform/iomap.h>
#include <Chipset/reboot.h>

STATIC QCOM_PM8X41_PROTOCOL *mPm8x41 = NULL;

STATIC VOID shutdown_device(VOID)
{
  /* Configure PMIC for shutdown. */
  mPm8x41->pm8x41_v2_reset_configure(PON_PSHOLD_SHUTDOWN);

  /* Drop PS_HOLD for MSM */
  writel(0x00, MPM2_MPM_PS_HOLD);

  mdelay(5000);

  DEBUG((EFI_D_ERROR, "Shutdown failed\n"));

  ASSERT(0);
}

STATIC VOID reboot_device(UINTN reboot_reason)
{
  UINT8 reset_type = 0;
  UINT32 restart_reason_addr;

  restart_reason_addr = RESTART_REASON_ADDR;

  /* Write the reboot reason */
  writel(reboot_reason, restart_reason_addr);

  if((reboot_reason == FASTBOOT_MODE) || (reboot_reason == DLOAD) || (reboot_reason == RECOVERY_MODE))
    reset_type = PON_PSHOLD_WARM_RESET;
  else
    reset_type = PON_PSHOLD_HARD_RESET;

  mPm8x41->pm8x41_v2_reset_configure(reset_type);

  /* Drop PS_HOLD for MSM */
  writel(0x00, MPM2_MPM_PS_HOLD);

  mdelay(5000);

  DEBUG((EFI_D_ERROR, "Rebooting failed\n"));
}

VOID
EFIAPI
ResetCold (
  VOID
  )
{
  reboot_device(0);
}

VOID
EFIAPI
ResetWarm (
  VOID
  )
{
  ResetCold ();
}

VOID
EFIAPI
ResetShutdown (
  VOID
  )
{
  shutdown_device();
}

VOID
EFIAPI
EnterS3WithImmediateWake (
  VOID
  )
{
  // not implemented
}

VOID
EFIAPI
ResetPlatformSpecific (
  IN UINTN   DataSize,
  IN VOID    *ResetData
  )
{
  ResetCold ();
}

RETURN_STATUS
EFIAPI
ResetLibConstructor (
  VOID
  )
{
  EFI_STATUS Status;

  Status = gBS->LocateProtocol (&gQcomPm8x41ProtocolGuid, NULL, (VOID **)&mPm8x41);
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
LibResetSystem (
  IN EFI_RESET_TYPE   ResetType,
  IN EFI_STATUS       ResetStatus,
  IN UINTN            DataSize,
  IN CHAR16           *ResetData OPTIONAL
  )
{
  switch (ResetType) {
  case EfiResetPlatformSpecific:
    //
  case EfiResetWarm:
    ResetWarm();
  case EfiResetCold:
    ResetCold();
  case EfiResetShutdown:
    ResetShutdown();
  default:
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }


  // We should never be here
  DEBUG ((EFI_D_ERROR, "%a: Reset failed\n", __FUNCTION__));
  CpuDeadLoop ();
  return EFI_UNSUPPORTED;
}

EFI_STATUS
EFIAPI
LibInitializeResetSystem (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  return EFI_SUCCESS;
}
