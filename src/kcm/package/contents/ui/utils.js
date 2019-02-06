function deviceStatus(device)
{
    var status = device.status;
    var str = "";
    if (status == Bolt.Bolt.Status.Disconnected) {
        str = i18n("Disconnected");
    } else if (status == Bolt.Bolt.Status.Connecting) {
        str = i18n("Connecting");
    } else if (status == Bolt.Bolt.Status.Connected) {
        str = i18n("Connected");
    } else if (status == Bolt.Bolt.Status.AuthError) {
        str = i18n("Authorization Error");
    } else if (status == Bolt.Bolt.Status.Authorizing) {
        str = i18n("Authorizing");
    } else if (status == Bolt.Bolt.Status.Authorized) {
        if (device.authFlags & Bolt.Bolt.Auth.NoPCIE) {
            str = i18n("Reduced Funcionality");
        } else {
            str = i18n("Connected & Authorized");
        }
    }
    if (device.stored) {
        if (str != "") {
            str += ", ";
        }
        str += i18n("Stored");
    }

    return str;
}
