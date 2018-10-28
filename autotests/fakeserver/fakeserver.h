#ifndef FAKESERVER_H
#define FAKESERVER_H

#include <QString>
#include <QScopedPointer>

class FakeServerException : public std::runtime_error
{
public:
    FakeServerException(const char *what)
        : std::runtime_error(what) {}
    FakeServerException(const QString &what)
        : std::runtime_error(what.toStdString()) {}
};

class FakeManager;
class FakeServer
{
public:
    explicit FakeServer(const QString &file);
    explicit FakeServer();
    ~FakeServer();

    static void enableFakeEnv();

    FakeManager *manager() const;
private:
    QScopedPointer<FakeManager> mManager;
};

#endif
