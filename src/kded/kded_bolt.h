#ifndef KDED_BOLT_H
#define KDED_BOLT_H

#include <kdedmodule.h>

#include <QScopedPointer>
#include <QSharedPointer>
#include <QVector>
#include <QTimer>

namespace Bolt {
class Manager;
class Device;
}

class Q_DECL_EXPORT KDEDBolt : public KDEDModule
{
    Q_OBJECT

public:
    KDEDBolt(QObject *parent, const QVariantList &args);
    ~KDEDBolt() override;

protected:
    virtual void notify();

    QVector<QSharedPointer<Bolt::Device>> sortDevices(const QVector<QSharedPointer<Bolt::Device>> &devices);

private:
    enum AuthMode {
        Enroll,
        Authorize
    };
    void authorizeDevices(QVector<QSharedPointer<Bolt::Device>> devices, AuthMode mode);

protected:
    QScopedPointer<Bolt::Manager> mManager;
    QVector<QSharedPointer<Bolt::Device>> mPendingDevices;
    QTimer mPendingDeviceTimer;
};

#endif // KDED_BOLT_H
