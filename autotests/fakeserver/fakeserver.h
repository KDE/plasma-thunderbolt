#ifndef FAKESERVER_H
#define FAKESERVER_H

#include <QString>
#include <QScopedPointer>

class FakeManager;
class FakeServer
{
public:
    explicit FakeServer(const QString &file);
    explicit FakeServer();
    ~FakeServer();

    bool wait() const;

    FakeManager *manager() const;
private:
    QScopedPointer<FakeManager> mManager;
};

#endif
