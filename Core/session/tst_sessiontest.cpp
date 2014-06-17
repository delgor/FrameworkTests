#include <QString>
#include <QtTest>

#include <nuria/session.hpp>
#include <nuria/abstractsessionmanager.hpp>
#include <nuria/sessionmanager.hpp>

class SessionTest : public QObject
{
	Q_OBJECT
	
public:
	SessionTest ();
	
private Q_SLOTS:
	void defaultConstructed ();
	void dataStorage ();
	void managedSessions ();
	void sessionRemoval ();
	void sessionLimit ();

private:
	Nuria::SessionManager* mgr;
};

SessionTest::SessionTest ()
	: mgr (new Nuria::SessionManager (2))
{
}

void SessionTest::defaultConstructed ()
{
	Nuria::Session session;
	
	QVERIFY(session.isValid () == false);
	QVERIFY(session.isDirty () == false);
	QCOMPARE(session.id (), QByteArray ());
	QVERIFY(session.manager () == nullptr);
}

void SessionTest::dataStorage ()
{
	Nuria::Session session;
	session["a"] = 1;
	session["b"] = QString ("Works");
	
	QVERIFY(session.isDirty ());
	QCOMPARE(session["a"].toInt (), 1);
	QCOMPARE(session.value ("b").toString (), QString ("Works"));
	
	session.markClean();
	QVERIFY(session.isDirty () == false);
	session.markDirty();
	QVERIFY(session.isDirty ());
}

void SessionTest::managedSessions ()
{
	Nuria::Session sessionFoo = mgr->get (QByteArray ("Foo"));
	
	QVERIFY(sessionFoo.isValid ());
	QCOMPARE(sessionFoo.id (), QByteArray ("Foo"));
	QCOMPARE(sessionFoo.manager (), mgr);
	QCOMPARE(sessionFoo, mgr->get (QByteArray ("Foo")));
	
	Nuria::Session sessionBar = mgr->get (QByteArray ("Bar"));
	QVERIFY(sessionFoo != sessionBar);
}

void SessionTest::sessionRemoval ()
{
	Nuria::Session sessionBar = mgr->get (QByteArray ("Bar"));
	sessionBar.remove ();
	QVERIFY(sessionBar != mgr->get (QByteArray ("Bar")));
}

void SessionTest::sessionLimit ()
{
	QCOMPARE(mgr->maxSessions (), 2);
	mgr->setMaxSessions (1);
	QCOMPARE(mgr->maxSessions (), 1);

	Nuria::Session sessionFoo = mgr->get (QByteArray ("Foo"));
	Nuria::Session sessionBar = mgr->get (QByteArray ("Bar"));
	QVERIFY(sessionFoo != mgr->get (QByteArray ("Foo")));
}

QTEST_MAIN(SessionTest)

#include "tst_sessiontest.moc"
