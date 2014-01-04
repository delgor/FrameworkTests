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

#include <nuria/callback.hpp>
static bool g_wasCalled = false;
static int g_result = 0;

using namespace Nuria;

class CallbackTest : public QObject {
	Q_OBJECT
	
public:
	CallbackTest ();
	
private Q_SLOTS:
	void initTestCase ();
	void cleanupTestCase ();
	
	void callingVoidStaticNoArguments ();
	void callingIntStaticNoArguments ();
	void callingVoidStaticWithArguments ();
	void callingIntStaticWithArguments ();
	
};

CallbackTest::CallbackTest () {
	
}

void CallbackTest::initTestCase () {
	
}

void CallbackTest::cleanupTestCase () {
	
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

QTEST_APPLESS_MAIN(CallbackTest)

#include "tst_callback.moc"
