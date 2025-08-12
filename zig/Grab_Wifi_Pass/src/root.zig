pub const std = @import("std");
pub const wifi = @import("zigwin32/win32/network_management/wi_fi.zig");
pub const core = @import("zigwin32/win32/zig.zig");
const winrt = @import("zigwin32/win32/system/win_rt.zig");
pub const found = @import("zigwin32/win32/foundation.zig");
const global = @import("zigwin32/win32/globalization.zig");
pub const xml = @import("zigwin32/win32/data/xml/ms_xml.zig");
pub const com = @import("zigwin32/win32/system/com.zig");

pub fn OpenWlanHandle(api_version: u32) ?found.HANDLE {
    var negotiated_version: u32 = 0;
    var wlan_handle: ?found.HANDLE = found.INVALID_HANDLE_VALUE;
    _ = wifi.WlanOpenHandle(api_version, null, &negotiated_version, &wlan_handle);
    return wlan_handle;
}

pub fn EnumWlanInterfaces(handle: found.HANDLE) ?*wifi.WLAN_INTERFACE_INFO_LIST {
    var interfaceP: ?*wifi.WLAN_INTERFACE_INFO_LIST = null;
    _ = wifi.WlanEnumInterfaces(handle, null, &interfaceP);
    return interfaceP;
}

pub fn GrabInterfaceProfiles(handle: found.HANDLE, interfaceGuid: *core.Guid) ?*wifi.WLAN_PROFILE_INFO_LIST {
    var profilesP: ?*wifi.WLAN_PROFILE_INFO_LIST = null;
    _ = wifi.WlanGetProfileList(handle, interfaceGuid, null, &profilesP);
    return profilesP;
}

pub fn GetProfileXml(handle: found.HANDLE, guid: *core.Guid, name: found.PWSTR) ?found.BSTR {
    var profileXmlData: ?found.PWSTR = null;
    var profileGetFlag = wifi.WLAN_PROFILE_GET_PLAINTEXT_KEY;
    const nameC = name;
    _ = wifi.WlanGetProfile(handle, guid, nameC, null, &profileXmlData, &profileGetFlag, null);
    const bstr = found.SysAllocString(profileXmlData);
    _ = wifi.WlanFreeMemory(profileXmlData);
    return bstr;
}

pub fn getXmlDoc(bstrXml: ?found.BSTR) *xml.IXMLDOMDocument {
    var instance = com.CoInitialize(null);
    var xmlDoc: *xml.IXMLDOMDocument = undefined;
    var isSuccessful: i16 = 0;
    instance = com.CoCreateInstance(xml.CLSID_DOMDocument, null, com.CLSCTX_ALL, xml.IID_IXMLDOMDocument, @ptrCast(&xmlDoc));
    _ = xmlDoc.loadXML(bstrXml, &isSuccessful);
    return xmlDoc;
}

pub fn xpathGetNodeText(allocator: std.mem.Allocator, rootNode: *xml.IXMLDOMNode, query: []const u8) ?[]const u8 {
    const array = getArrayU16(allocator, query);
    const query_bstr = found.SysAllocString(array);
    defer found.SysFreeString(query_bstr);

    var resultNode: *xml.IXMLDOMNode = undefined;
    _ = rootNode.selectSingleNode(query_bstr, @ptrCast(&resultNode));

    var result_bstr: ?found.BSTR = undefined;
    _ = resultNode.get_text(&result_bstr);

    const r = bstrToString(allocator, result_bstr);
    return r;
}

pub fn pwstrToString(allocator: std.mem.Allocator, pwstr: found.PWSTR) ![]const u8 {
    const len: i32 = @intCast(std.mem.len(pwstr));
    const size_needed = global.WideCharToMultiByte(global.CP_UTF8, 0, @ptrCast(&pwstr[0]), len, null, 0, null, null);
    var buffer = try allocator.alloc(u8, @intCast(size_needed));
    _ = global.WideCharToMultiByte(global.CP_UTF8, 0, @ptrCast(&pwstr[0]), len, @ptrCast(&buffer[0]), size_needed, null, null);
    return buffer;
}

pub fn getArrayU16(allocator: std.mem.Allocator, array: []const u8) ?[*:0]const u16 {
    const result: []u16 = allocator.alloc(u16, array.len) catch {
        return null;
    };
    var i: usize = 0;

    for (array) |char| {
        result[i] = char;
        i += 1;
    }
    return @ptrCast(result[0..result.len]);
}

pub fn sliceCmp(comptime T: type, slice1: []const T, slice2: []const T) bool {
    if (slice1.len == slice2.len) {
        for (slice1, slice2) |a, b| {
            if (a == b) continue;
            return false;
        }
    } else {
        return false;
    }

    return true;
}

pub fn bstrToString(allocator: std.mem.Allocator, bstr: ?found.BSTR) ?[]const u8 {
    const pwstr: found.PWSTR = @ptrCast(bstr);
    const string = pwstrToString(allocator, pwstr) catch {
        return null;
    };
    return string;
}
