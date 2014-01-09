/* This file is part of the NuriaProject Framework. Copyright 2012 NuriaProject
 * The NuriaProject Framework is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the License,
 * or (at your option) any later version.
 * The NuriaProject Framework is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General Public License
 * along with the library.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QString>
#include <QtTest>
#include <QDate>

#define NURIA_NO_VARIANT_COMPARISON
#include <nuria/callback.hpp>
static bool g_wasCalled = false;
static int g_result = 0;

using namespace Nuria;

class CallbackTest : public QObject {
	Q_OBJECT
	
public:
	CallbackTest ();
	
private slots:
	void callingVoidStaticNoArguments ();
	void callingIntStaticNoArguments ();
	void callingVoidStaticWithArguments ();
	void callingIntStaticWithArguments ();
	
	void callingVoidMemberNoArguments ();
	void callingIntMemberNoArguments ();
	void callingVoidMemberWithArguments ();
	void callingIntMemberWithArguments ();
	
	void callingVoidSlotNoArguments ();
	void callingIntSlotNoArguments ();
	void callingVoidSlotWithArguments ();
	void callingIntSlotWithArguments ();
	
	void implicitArgumentConversion ();
	void implicitArgumentConversionCustomType ();
};


CallbackTest::CallbackTest () {
	
}

// 
static void staticVoidNoArgs () { g_wasCalled = true; }
void CallbackTest::callingVoidStaticNoArguments () {
	g_wasCalled = false;
	
	Callback cb (staticVoidNoArgs);
	cb ();
	QVERIFY(g_wasCalled);
}

// 
static int staticIntNoArgs () { return 42; }
void CallbackTest::callingIntStaticNoArguments () {
	Callback cb (staticIntNoArgs);
	int result = cb ().toInt ();
	QCOMPARE(result, 42);
}

// 
static void staticVoidWithArgs (int a, int b) { g_result = a + b; }
void CallbackTest::callingVoidStaticWithArguments () {
	Callback cb (staticVoidWithArgs);
	cb (3, 4);
	QCOMPARE(g_result, 7);
}

// 
static int staticIntWithArgs (int a, int b) { return a + b; }
void CallbackTest::callingIntStaticWithArguments () {
	Callback cb (staticIntWithArgs);
	int result = cb (3, 4).toInt ();
	QCOMPARE(result, 7);
}

// 
class Members : public QObject {
	Q_OBJECT
public:
	int i;
	
	Members () : i (123) {}
	
public slots:
	void voidNoArgs () {
		qDebug("Members::voidNoArgs called");
	}
	
	int intNoArgs () {
		return this->i;
	}
	
	void voidWithArgs (int a, int b) {
		qDebug("Members::voidWithArgs %i %i", a, b);
	}
	
	int intWithArgs (int a, int b) {
		return a + b;
	}
	
};

void CallbackTest::callingVoidMemberNoArguments () {
	Members m;
	QTest::ignoreMessage (QtDebugMsg, "Members::voidNoArgs called");
	Callback (&m, &Members::voidNoArgs)();
}

void CallbackTest::callingIntMemberNoArguments () {
	Members m;
	QCOMPARE(Callback (&m, &Members::intNoArgs)(), QVariant (m.i));
}

void CallbackTest::callingVoidMemberWithArguments () {
	Members m;
	QTest::ignoreMessage (QtDebugMsg, "Members::voidWithArgs 4 5");
	Callback (&m, &Members::voidWithArgs)(4, 5);
}

void CallbackTest::callingIntMemberWithArguments () {
	Members m;
	QCOMPARE(Callback (&m, &Members::intWithArgs)(4, 5), QVariant (4 + 5));
}

void CallbackTest::callingVoidSlotNoArguments () {
	Members m;
	QTest::ignoreMessage (QtDebugMsg, "Members::voidNoArgs called");
	Callback (&m, SLOT(voidNoArgs()))();
}

void CallbackTest::callingIntSlotNoArguments () {
	Members m;
	QCOMPARE(Callback (&m, SLOT(intNoArgs()))(), QVariant (m.i));
}

void CallbackTest::callingVoidSlotWithArguments () {
	Members m;
	QTest::ignoreMessage (QtDebugMsg, "Members::voidWithArgs 4 5");
	Callback (&m, SLOT(voidWithArgs(int,int)))(4, 5);
}

void CallbackTest::callingIntSlotWithArguments () {
	Members m;
	QCOMPARE(Callback (&m, SLOT(intWithArgs(int,int)))(4, 5), QVariant (4 + 5));
}

// 
void CallbackTest::implicitArgumentConversion () {
	QString a ("5");
	QString b ("4");
	
	// Will convert QString to int
	QCOMPARE(Callback (&staticIntWithArgs)(a, b), QVariant (5 + 4));
}

static void printString (const QString &string) { qDebug("%s", qPrintable(string)); }
void CallbackTest::implicitArgumentConversionCustomType () {
	Variant::registerConversion< QDate, QString > ([](const QDate &date) {
		return new QString (date.toString (Qt::ISODate));
	});
	
	QDate date (2010, 1, 2);
	QTest::ignoreMessage (QtDebugMsg, "2010-01-02");
	Callback cb (&printString);
	cb (date);
}

QTEST_APPLESS_MAIN(CallbackTest)

#include "tst_callback.moc"
