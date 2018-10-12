#ifndef KDED_BOLT_H
#define KDED_BOLT_H

#include <kdedmodule.h>

#include <QScopedPointer>

namespace Bolt {
class Manager;
}

class Q_DECL_EXPORT KDEDBolt : public KDEDModule
{
    Q_OBJECT

public:
    KDEDBolt(QObject *parent, const QVariantList &args);
    ~KDEDBolt() override;

private:
    QScopedPointer<Bolt::Manager> mManager;
};

#endif // KDED_BOLT_H