#include <QString>
#include <QtTest>
#include <nuria/session.hpp>
#include <nuria/abstractsessionmanager.hpp>
#include <nuria/sessionmanager.hpp>

class SessionManagerTest : public QObject {
	Q_OBJECT	
public:
	SessionManagerTest ();
	
private slots:
	void storageAndPersistence ();
	void sessionRemoval ();
	void maxSessions ();
};

SessionManagerTest::SessionManagerTest () {
}

void SessionManagerTest::storageAndPersistence () {
	Nuria::AbstractSessionManager *mgr = new Nuria::SessionManager ();
	Nuria::Session sessionFoo = mgr->get (QByteArray ("Foo"));
	
	QVERIFY(sessionFoo.isValid ());
	QCOMPARE(sessionFoo.id (), QByteArray ("Foo"));
	QCOMPARE(sessionFoo.manager (), mgr);
	QCOMPARE(sessionFoo, mgr->get (QByteArray ("Foo")));
	
	Nuria::Session sessionBar = mgr->get (QByteArray ("Bar"));
	QVERIFY(sessionFoo != sessionBar);
	delete mgr;
}

void SessionManagerTest::sessionRemoval () {
	Nuria::AbstractSessionManager *mgr = new Nuria::SessionManager ();
	Nuria::Session sessionBar = mgr->get (QByteArray ("Bar"));
	sessionBar.remove ();
	QVERIFY(sessionBar != mgr->get (QByteArray ("Bar")));
	delete mgr;
}

void SessionManagerTest::maxSessions () {
	Nuria::SessionManager *mgr = new Nuria::SessionManager ();
	mgr->setMaxSessions (1);
	QCOMPARE(mgr->maxSessions (), 1);

	Nuria::Session sessionFoo = mgr->get (QByteArray ("Foo"));
	Nuria::Session sessionBar = mgr->get (QByteArray ("Bar"));
	QVERIFY(sessionBar == mgr->get (QByteArray ("Bar")));
	QVERIFY(sessionFoo != mgr->get (QByteArray ("Foo")));
	delete mgr;
}

QTEST_MAIN(SessionManagerTest)

#include "tst_sessionmanagertest.moc"
