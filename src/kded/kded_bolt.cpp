#include "kded_bolt.h"
#include "kded_bolt_debug.h"

#include "lib/manager.h"
#include "lib/device.h"
#include "lib/exceptions.h"

#include <QPointer>

#include <KNotification>
#include <KLocalizedString>

KDEDBolt::KDEDBolt(QObject *parent, const QVariantList &)
    : KDEDModule(parent)
{
    try {
        mManager.reset(new Bolt::Manager);
    } catch (const Bolt::DBusException &e) {
        qCInfo(log_kded_bolt, "Couldn't connect to Bolt DBus daemon: %s", e.what());
        return;
    }

    connect(mManager.get(), &Bolt::Manager::deviceAdded,
            this, [this](Bolt::Device *device) {
        // Already authorized, nothing else to do here
        if (device->status() == Bolt::Status::Authorized) {
            return;
        }

        auto ntf = KNotification::event(
                device->uid(),
                i18n("Unauthorized Thunderbolt device <b>%1</b> was connected. Do you want to authorize it?"),
                /*icon*/ {}, /* widget */ nullptr,
                KNotification::CloseOnTimeout);
        ntf->setActions({
                i18n("Authorize"),
                i18n("Block")
        });
        ntf->setTitle(i18n("New Thunderbolt Device Detected"));
        connect(ntf, &KNotification::action1Activated,
                this, [this, dev = QPointer<Bolt::Device>(device)]() {
            if (!dev) {
                return;
            }

            mManager->enrollDevice(dev->uid(), Bolt::Policy::Auto,
                    Bolt::Auth::Secure | Bolt::Auth::Boot);
        });
    });
}

KDEDBolt::~KDEDBolt()
{
}
