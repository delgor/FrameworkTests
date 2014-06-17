#include <QString>
#include <QtTest>

#include <nuria/session.hpp>

class SessionTest : public QObject {
	Q_OBJECT	
public:
	SessionTest ();	
	
private slots:
	void defaultConstructed ();
	void dataStorage ();
};

SessionTest::SessionTest () {
}

void SessionTest::defaultConstructed () {
	Nuria::Session session;
	
	QVERIFY(session.isValid () == false);
	QVERIFY(session.isDirty () == false);
	QCOMPARE(session.id (), QByteArray ());
	QVERIFY(session.manager () == nullptr);
}

void SessionTest::dataStorage () {
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

QTEST_MAIN(SessionTest)

#include "tst_sessiontest.moc"
