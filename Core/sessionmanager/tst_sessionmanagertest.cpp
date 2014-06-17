#include <QString>
#include <QtTest>
#include <nuria/session.hpp>
#include <nuria/abstractsessionmanager.hpp>
#include <nuria/sessionmanager.hpp>

class SessionManagerTest : public QObject {
	Q_OBJECT
public:
	
private slots:
	void storageAndPersistence ();
	void sessionRemoval ();
	void maxSessions ();
};

void SessionManagerTest::storageAndPersistence () {
	Nuria::SessionManager mgr;
	Nuria::Session sessionFoo = mgr.get (QByteArray ("Foo"));
	
	QVERIFY(sessionFoo.isValid ());
	QCOMPARE(sessionFoo.id (), QByteArray ("Foo"));
	QCOMPARE(sessionFoo.manager (), &mgr);
	QCOMPARE(sessionFoo, mgr.get (QByteArray ("Foo")));
	
	Nuria::Session sessionBar = mgr.get (QByteArray ("Bar"));
	QVERIFY(sessionFoo != sessionBar);
}

void SessionManagerTest::sessionRemoval () {
	Nuria::SessionManager mgr;
	Nuria::Session sessionBar = mgr.get (QByteArray ("Bar"));
	sessionBar.remove ();
	QVERIFY(sessionBar != mgr.get (QByteArray ("Bar")));
}

void SessionManagerTest::maxSessions () {
	Nuria::SessionManager mgr;
	mgr.setMaxSessions (1);
	QCOMPARE(mgr.maxSessions (), 1);

	Nuria::Session sessionFoo = mgr.get (QByteArray ("Foo"));
	Nuria::Session sessionBar = mgr.get (QByteArray ("Bar"));
	QCOMPARE(sessionBar, mgr.get (QByteArray ("Bar")));
	QVERIFY(sessionFoo != mgr.get (QByteArray ("Foo")));
}

QTEST_MAIN(SessionManagerTest)

#include "tst_sessionmanagertest.moc"
