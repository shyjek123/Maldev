const print = std.debug.print;
const root = @import("root.zig");
const std = root.std;
pub fn main() !void {
    const stdout = std.io.getStdOut().writer();

    var arena = std.heap.ArenaAllocator.init(std.heap.page_allocator);
    defer arena.deinit();
    const allocator = arena.allocator();

    const wlanHandle = root.OpenWlanHandle(root.wifi.WLAN_API_VERSION_2_0) orelse {
        print("OpenWlanHandle Error: {}", .{root.found.GetLastError()});
        return;
    };

    defer _ = root.wifi.WlanCloseHandle(wlanHandle, null);

    var interfaceP = root.EnumWlanInterfaces(wlanHandle) orelse {
        print("EnumWlanInterfaces Error: {}", .{root.found.GetLastError()});
        return;
    };

    defer _ = root.wifi.WlanFreeMemory(interfaceP);

    const interfacesList: []const root.wifi.WLAN_INTERFACE_INFO = @as([*]const root.wifi.WLAN_INTERFACE_INFO, @ptrCast(&interfaceP.InterfaceInfo[0]))[0..interfaceP.dwNumberOfItems];

    for (interfacesList) |interfaceInfo| {
        var guid: root.core.Guid = interfaceInfo.InterfaceGuid;
        var wlanProfileP = root.GrabInterfaceProfiles(wlanHandle, &guid) orelse {
            print("GrabInterfaceProfiles Error: {}", .{root.found.GetLastError()});
            break;
        };
        defer root.wifi.WlanFreeMemory(wlanProfileP);

        const profileList: []const root.wifi.WLAN_PROFILE_INFO = @as([*]const root.wifi.WLAN_PROFILE_INFO, @ptrCast(&wlanProfileP.ProfileInfo[0]))[0..wlanProfileP.dwNumberOfItems];

        for (profileList) |profile| {
            const name: root.found.PWSTR = @ptrCast(@constCast(profile.strProfileName[0..]));
            const nameString = try root.pwstrToString(allocator, name);

            const xml_bstr = root.GetProfileXml(wlanHandle, &guid, name);
            defer root.found.SysFreeString(xml_bstr);

            const xmlDoc = root.getXmlDoc(xml_bstr);
            defer root.com.CoUninitialize();

            var rootNode: *root.xml.IXMLDOMElement = undefined;
            _ = xmlDoc.get_documentElement(@ptrCast(&rootNode));

            const auth_query = "MSM/security/authEncryption/authentication";
            const auth = root.xpathGetNodeText(allocator, &rootNode.IXMLDOMNode, auth_query).?;

            if (!root.sliceCmp(u8, auth, "open")) {
                const pass_query = "MSM/security/sharedKey/keyMaterial";
                const password = root.xpathGetNodeText(allocator, &rootNode.IXMLDOMNode, pass_query).?;

                stdout.print("Wifi Name: {s}\n\tAuthType: {s} Password: {s}\n\n", .{ nameString, auth, password }) catch |err| {
                    std.debug.print("(password) Failed to write to stdout: {}\n", .{err});
                };
            } else {
                stdout.print("Wifi Name: {s}\n\tAuthType: {s} Password: None\n\n", .{ nameString, "open" }) catch |err| {
                    std.debug.print("(password) Failed to write to stdout: {}\n", .{err});
                };
            }
        }
    }
}
