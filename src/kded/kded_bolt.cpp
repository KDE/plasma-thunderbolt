#include "kded_bolt.h"
#include "kded_bolt_debug.h"

#include "lib/manager.h"
#include "lib/device.h"

#include <QPointer>

#include <KNotification>
#include <KLocalizedString>
#include <KPluginFactory>

#include <chrono>

using namespace std::chrono_literals;

K_PLUGIN_CLASS_WITH_JSON(KDEDBolt, "kded_bolt.json")

KDEDBolt::KDEDBolt(QObject *parent, const QVariantList &)
    : KDEDModule(parent)
    , mManager(new Bolt::Manager)
{
    if (!mManager->isAvailable()) {
        qCInfo(log_kded_bolt, "Couldn't connect to Bolt DBus daemon");
        return;
    }

    mPendingDeviceTimer.setSingleShot(true);
    mPendingDeviceTimer.setInterval(500ms);
    connect(&mPendingDeviceTimer, &QTimer::timeout, this, &KDEDBolt::notify);

    connect(mManager.get(), &Bolt::Manager::deviceAdded,
            this, [this](const QSharedPointer<Bolt::Device> &device) {
        // Already authorized, nothing else to do here
        if (device->status() == Bolt::Status::Authorized) {
            return;
        }

        mPendingDevices.append(device);
        mPendingDeviceTimer.start();
    });
}

KDEDBolt::~KDEDBolt()
{
}

void KDEDBolt::notify()
{
    auto ntf = KNotification::event(
            QStringLiteral("unauthorizedDeviceConnected"),
            i18n("New Thunderbolt Device Detected"),
            mPendingDevices.size() == 1
                ? i18n("Unauthorized Thunderbolt device <b>%1</b> was detected. Do you want to authorize it?", mPendingDevices.front()->name())
                : i18np("%1 unauthorized Thunderbolt device was detected. Do you want to authorize it?",
                        "%1 unauthorized Thunderbolt devices were detected. Do you want to authorize them?",
                        mPendingDevices.size()),
            /*icon*/ QPixmap{}, /* widget */ nullptr,
            KNotification::Persistent,
            QStringLiteral("kded_bolt"));
    ntf->setActions({
            i18n("Authorize"),
            i18n("Block")
    });
    connect(ntf, &KNotification::action1Activated,
            this, [this, devices = mPendingDevices]() {
                authorizeDevices(sortDevices(devices));
            });
   connect(ntf, &KNotification::action2Activated,
            this, [this, devices = mPendingDevices]() {
                blockDevices(sortDevices(devices));
            });

    mPendingDevices.clear();
}

QVector<QSharedPointer<Bolt::Device>> KDEDBolt::sortDevices(const QVector<QSharedPointer<Bolt::Device>> &devices)
{
    QVector<QSharedPointer<Bolt::Device>> sorted;
    // Sort the devices so that parents go before their children. Probably
    // fairly inefficient but there's rarely more than a couple of items.
    for (const auto &device : devices) {
        auto child = std::find_if(sorted.begin(), sorted.end(), [device](const auto &d) { return d->parent() == device->uid(); });
        auto parent = std::find_if(sorted.begin(), child, [device](const auto &d) { return device->parent() == d->uid(); });
        if (parent != sorted.end()) {
            ++parent;
        }
        sorted.insert(parent, device);
    }

    return sorted;
}

void KDEDBolt::authorizeDevices(QVector<QSharedPointer<Bolt::Device>> devices)
{
    if (devices.empty()) {
        return;
    }

    const auto device = devices.takeFirst();;
    mManager->enrollDevice(device->uid(), Bolt::Policy::Auto, Bolt::Auth::Boot | Bolt::Auth::NoKey,
            [this, devices]() {
                authorizeDevices(std::move(devices));
            },
            [device](const QString &error) {
                KNotification::event(
                    QStringLiteral("deviceAuthError"),
                    i18n("Thunderbolt Device Authorization Error"),
                    i18n("Failed to authorize Thunderbolt device <b>%1</b>: %2", device->name(), error),
                    /* icon */ QPixmap{}, /* parent */ nullptr,
                    KNotification::CloseOnTimeout,
                    QStringLiteral("kded_bolt"));
            });
}

void KDEDBolt::blockDevices(QVector<QSharedPointer<Bolt::Device>> devices)
{
    if (devices.empty()) {
        return;
    }

    const auto device = devices.takeLast();
    mManager->forgetDevice(device->uid(),
            [this, devices]() {
                blockDevices(std::move(devices));
            },
            [device](const QString &error) {
                KNotification::event(
                    QStringLiteral("deviceAuthError"),
                    i18n("Thunderbolt Device Blocking Error"),
                    i18n("Failed to block Thunderbolt device <b>%1</b>: %2", device->name(), error),
                    /* icon */ QPixmap{}, /* parent */ nullptr,
                    KNotification::CloseOnTimeout,
                    QStringLiteral("kded_bolt"));
            });
}

#include "kded_bolt.moc"
