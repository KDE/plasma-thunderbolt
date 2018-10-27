function deviceStatus(device)
{
    var status = device.status;
    if (status == Bolt.Bolt.Status.Disconnected) {
        return i18n("Disconnected");
    } else if (status == Bolt.Bolt.Status.Connecting) {
        return i18n("Connecting");
    } else if (status == Bolt.Bolt.Status.Connected) {
        return i18n("Connected");
    } else if (status == Bolt.Bolt.Status.AuthError) {
        return i18n("Authorization Error");
    } else if (status == Bolt.Bolt.Status.Authorizing) {
        return i18n("Authorizing");
    } else if (status == Bolt.Bolt.Status.Authorized) {
        if (device.authFlags & Bolt.Bolt.Auth.NoPCIE) {
            return i18n("Reduced Funcionality");
        } else {
            return i18n("Connected & Authorized");
        }
    }
    return "";
}
