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

#define NURIA_NO_VARIANT_C
#include <nuria/metaobject.hpp>

#include <QtTest/QtTest>
#include <QObject>
#include <QDate>

// Header processed by Tria
#include "tst_header.hpp"

class TriaTest : public QObject {
	Q_OBJECT
public:
	TriaTest ();
	
private slots:
	
	void checkThatTypesAreRegistered ();
	void getAllTypes ();
	void getTypesByAnnotation ();
	void getTypesByBase ();
	
	void verifyTypeA ();
	void verifyTypeB ();
	void fieldsAreSorted ();
	
	void methodsAreSorted ();
	void enumsAreSorted ();
	void enumElementsAreSorted ();
	
	void checkEnumElements ();
	void checkEnumKeyToValue ();
	void checkEnumValueToKey ();
	
	void checkNonExistanceOfIgnoredElements ();
	
	void checkClassAnnotations ();
	void checkMethodAnnotations ();
	void checkEnumAnnotations ();
	void checkFieldAnnotations ();
	
	void checkOverloadedMethod ();
	void verifyMethodsMetaData ();
	void callMemberMethod ();
	void callStaticMethod ();
	void callOverloadedMethod ();
	void createAndDestroyInstance ();
	
	void verifyFieldType ();
	void readField ();
	void writeField ();
	
	void testCtorConversion ();
	void testFromMethodConversion ();
	void testToMethodConversion ();
	void testOperatorConversion ();
	
};

// 
TriaTest::TriaTest () {
	
}

void TriaTest::checkThatTypesAreRegistered () {
	using namespace Nuria;
	
	QVERIFY (MetaObject::byName ("Test::A"));
	QVERIFY (MetaObject::byName ("Test::B"));
	QVERIFY (!MetaObject::byName ("Test::Ignored"));
	
}

void TriaTest::getAllTypes () {
	using namespace Nuria;
	
	MetaObjectMap expected { { "Test::A", MetaObject::byName ("Test::A") },
				 { "Test::B", MetaObject::byName ("Test::B") } };
	MetaObjectMap result = MetaObject::allTypes ();
	
	QCOMPARE(result, expected);
	
}

void TriaTest::getTypesByAnnotation () {
	using namespace Nuria;
	
	MetaObjectMap expected { { "Test::A", MetaObject::byName ("Test::A") } };
	MetaObjectMap result = MetaObject::typesWithAnnotation ("DoesItWork");
	
	QCOMPARE(result, expected);
	
}

void TriaTest::getTypesByBase () {
	using namespace Nuria;
	
	MetaObjectMap expected { { "Test::B", MetaObject::byName ("Test::B") } };
	MetaObjectMap result = MetaObject::typesInheriting ("Test::A");
	
	QCOMPARE(result, expected);
	
}

void TriaTest::verifyTypeA () {
	using namespace Nuria;
	
	MetaObject *a = MetaObject::byName ("Test::A");
	QCOMPARE(a->metaTypeId (), qMetaTypeId< Test::A > ());
	QCOMPARE(a->pointerMetaTypeId (), qMetaTypeId< Test::A * > ());
	QCOMPARE(a->parents ().length (), 0);
	QCOMPARE(a->annotationCount (), 1);
	QCOMPARE(a->methodCount (), 4);
	QCOMPARE(a->fieldCount (), 1);
	QCOMPARE(a->enumCount (), 3);
	
	QVERIFY(a->method ({ "" }).isValid ());
	QVERIFY(a->method ({ "callMe" }).isValid ());
	QVERIFY(a->method ({ "callMe", "int" }).isValid ());
	QVERIFY(a->method ({ "callMeStatic" }).isValid ());
	
}

void TriaTest::verifyTypeB () {
	using namespace Nuria;
	
	MetaObject *b = MetaObject::byName ("Test::B");
	QCOMPARE(b->metaTypeId (), qMetaTypeId< Test::B > ());
	QCOMPARE(b->pointerMetaTypeId (), qMetaTypeId< Test::B * > ());
	QCOMPARE(b->parents ().length (), 1);
	QCOMPARE(b->annotationCount (), 0);
	QCOMPARE(b->methodCount (), 9);
	QCOMPARE(b->fieldCount (), 4);
	QCOMPARE(b->enumCount (), 0);
	
	QVERIFY(b->method ({ "" }).isValid ());
	QVERIFY(b->method ({ "", "QString" }).isValid ());
	QVERIFY(b->method ({ "sum" }).isValid ());
	QVERIFY(b->method ({ "sum", "int" }).isValid ());
	QVERIFY(b->method ({ "sum", "int", "int" }).isValid ());
	QVERIFY(b->method ({ "sum", "int", "int", "int" }).isValid ());
	QVERIFY(b->method ({ "fromInt", "int" }).isValid ());
	QVERIFY(b->method ({ "toByteArray" }).isValid ());
	QVERIFY(b->method ({ "toString" }).isValid ());
	
}

void TriaTest::fieldsAreSorted () {
	Nuria::MetaObject *meta = Nuria::MetaObject::byName ("Test::B");
	
	QStringList expected { "a", "b", "c", "value" };
	QStringList result;
	for (int i = 0; i < meta->fieldCount (); i++) {
		result << meta->field (i).name ();
	}
	
	// 
	QCOMPARE(result, expected);
	
}

void TriaTest::methodsAreSorted () {
	Nuria::MetaObject *meta = Nuria::MetaObject::byName ("Test::A");
	
	QStringList expected { "", "callMe", "callMe", "callMeStatic" };
	QStringList result;
	for (int i = 0; i < meta->methodCount (); i++) {
		result << meta->method (i).name ();
	}
	
	// 
	QCOMPARE(result, expected);
	
}

void TriaTest::enumsAreSorted () {
	Nuria::MetaObject *meta = Nuria::MetaObject::byName ("Test::A");
	
	QStringList expected { "First", "Numbers", "Second" };
	QStringList result;
	for (int i = 0; i < meta->enumCount (); i++) {
		result << meta->enumAt (i).name ();
	}
	
	// 
	QCOMPARE(result, expected);
	
}

void TriaTest::enumElementsAreSorted () {
	Nuria::MetaObject *meta = Nuria::MetaObject::byName ("Test::A");
	Nuria::MetaEnum theEnum = meta->enumByName ("Numbers");
	
	QStringList expected { "Hundred", "One", "Ten", "Two" };
	QStringList result;
	for (int i = 0; i < theEnum.elementCount (); i++) {
		result << theEnum.key (i);
	}
	
	// 
	QVERIFY(theEnum.isValid ());
	QCOMPARE(result, expected);
	
}

void TriaTest::checkEnumElements () {
	Nuria::MetaObject *meta = Nuria::MetaObject::byName ("Test::A");
	Nuria::MetaEnum theEnum = meta->enumByName ("Numbers");
	
	QMap< QByteArray, int > expected { { "One", 1 }, { "Two", 2 }, { "Ten", 10 }, { "Hundred", 100 } };
	QMap< QByteArray, int > result;
	for (int i = 0; i < theEnum.elementCount (); i++) {
		result.insert (theEnum.key (i), theEnum.value (i));
	}
	
	// 
	QVERIFY(theEnum.isValid ());
	QCOMPARE(result, expected);
	
}

void TriaTest::checkEnumKeyToValue () {
	Nuria::MetaObject *meta = Nuria::MetaObject::byName ("Test::A");
	Nuria::MetaEnum theEnum = meta->enumByName ("Numbers");
	
	QCOMPARE(theEnum.keyToValue ("One"), 1);
	QCOMPARE(theEnum.keyToValue ("Two"), 2);
	QCOMPARE(theEnum.keyToValue ("Ten"), 10);
	QCOMPARE(theEnum.keyToValue ("Hundred"), 100);
}

void TriaTest::checkEnumValueToKey () {
	Nuria::MetaObject *meta = Nuria::MetaObject::byName ("Test::A");
	Nuria::MetaEnum theEnum = meta->enumByName ("Numbers");
	
	QCOMPARE(theEnum.valueToKey (1), QByteArray ("One"));
	QCOMPARE(theEnum.valueToKey (2), QByteArray ("Two"));
	QCOMPARE(theEnum.valueToKey (10), QByteArray ("Ten"));
	QCOMPARE(theEnum.valueToKey (100), QByteArray ("Hundred"));
}

void TriaTest::checkNonExistanceOfIgnoredElements () {
	Nuria::MetaObject *meta = Nuria::MetaObject::byName ("Test::A");
	
	QVERIFY(!meta->fieldByName ("ignoredField").isValid ());
	QVERIFY(!meta->enumByName ("ignoredEnum").isValid ());
	QVERIFY(!meta->method ({ "ignoredMethod" }).isValid ());
	QVERIFY(!meta->method ({ "ignoredStatic" }).isValid ());
	QVERIFY(!meta->method ({ "", "int" }).isValid ()); // Constructor.
	
}

void TriaTest::checkOverloadedMethod () {
	Nuria::MetaObject *meta = Nuria::MetaObject::byName ("Test::B");
	
	int lower = meta->methodLowerBound ("sum");
	int upper = meta->methodUpperBound ("sum");
	QCOMPARE(upper - lower + 1, 4);
	
	// Sorted by argument count in ascending manner.
	for (int i = 0; lower <= upper; i++, lower++) {
		QCOMPARE(meta->method (lower).argumentTypes ().length (), i);
	}
	
}

template< typename ... Args >
static inline QVector< QByteArray > getList (Args ... args) {
	return QVector< QByteArray > { args ... };
}

void TriaTest::verifyMethodsMetaData () {
	using namespace Nuria;
	MetaObject *meta = Nuria::MetaObject::byName ("Test::B");
	
	// Implicitly verifies MetaMethod::argumentTypes().
	MetaMethod ctorDefault = meta->method({ "" });
	MetaMethod ctorString = meta->method({ "", "QString" });
	MetaMethod sumZero = meta->method({ "sum" });
	MetaMethod sumOne = meta->method({ "sum", "int" });
	MetaMethod sumTwo = meta->method({ "sum", "int", "int" });
	MetaMethod sumThree = meta->method({ "sum", "int", "int", "int" });
	MetaMethod fromInt = meta->method({ "fromInt", "int" });
	MetaMethod toByteArray = meta->method({ "toByteArray" });
	MetaMethod toString = meta->method({ "toString" });
	
	// 
	QVERIFY(!ctorDefault.annotationCount ());
	QVERIFY(!ctorString.annotationCount ());
	QVERIFY(!sumZero.annotationCount ());
	QVERIFY(!sumOne.annotationCount ());
	QVERIFY(!sumTwo.annotationCount ());
	QVERIFY(!sumThree.annotationCount ());
	QVERIFY(!fromInt.annotationCount ());
	QVERIFY(!toByteArray.annotationCount ());
	QVERIFY(!toString.annotationCount ());
	
	// 
	QCOMPARE(ctorDefault.type (), MetaMethod::Constructor);
	QCOMPARE(ctorString.type (), MetaMethod::Constructor);
	QCOMPARE(sumZero.type (), MetaMethod::Method);
	QCOMPARE(sumOne.type (), MetaMethod::Method);
	QCOMPARE(sumTwo.type (), MetaMethod::Method);
	QCOMPARE(sumThree.type (), MetaMethod::Method);
	QCOMPARE(fromInt.type (), MetaMethod::Static);
	QCOMPARE(toByteArray.type (), MetaMethod::Method);
	QCOMPARE(toString.type (), MetaMethod::Method);
	
	// 
	QCOMPARE(ctorDefault.returnType (), QByteArray ("Test::B"));
	QCOMPARE(ctorString.returnType (), QByteArray ("Test::B"));
	QCOMPARE(sumZero.returnType (), QByteArray ("int"));
	QCOMPARE(sumOne.returnType (), QByteArray ("int"));
	QCOMPARE(sumTwo.returnType (), QByteArray ("int"));
	QCOMPARE(sumThree.returnType (), QByteArray ("int"));
	QCOMPARE(fromInt.returnType (), QByteArray ("Test::B"));
	QCOMPARE(toByteArray.returnType (), QByteArray ("QByteArray"));
	QCOMPARE(toString.returnType (), QByteArray ("QString"));
	
	// 
	QCOMPARE(ctorDefault.argumentNames (), getList ());
	QCOMPARE(ctorString.argumentNames (), getList ("s"));
	QCOMPARE(sumZero.argumentNames (), getList ());
	QCOMPARE(sumOne.argumentNames (), getList ("a"));
	QCOMPARE(sumTwo.argumentNames (), getList ("a", "b"));
	QCOMPARE(sumThree.argumentNames (), getList ("a", "b", "c"));
	QCOMPARE(fromInt.argumentNames (), getList ("i"));
	QCOMPARE(toByteArray.argumentNames (), getList ());
	QCOMPARE(toString.argumentNames (), getList ());
	
}

template< typename T >
static QVariantMap allAnnotations (T &from) {
	QVariantMap map;
	for (int i = 0; i < from.annotationCount (); i++) {
		Nuria::MetaAnnotation a = from.annotation (i);
		map.insert (a.name (), a.value ());
	}
	
	return map;
}

void TriaTest::checkClassAnnotations () {
	Nuria::MetaObject *meta = Nuria::MetaObject::byName ("Test::A");
	
	QVariantMap expect { { "DoesItWork", 1 + 2 } };
	QCOMPARE(allAnnotations (*meta), expect);
	
}

void TriaTest::checkMethodAnnotations () {
	Nuria::MetaObject *meta = Nuria::MetaObject::byName ("Test::A");
	Nuria::MetaMethod ctor = meta->method ({ "" });
	Nuria::MetaMethod member = meta->method ({ "callMe" });
	Nuria::MetaMethod memberTwo = meta->method ({ "callMe", "int" });
	Nuria::MetaMethod staticMethod = meta->method ({ "callMeStatic" });
	
	QVariantMap expectCtor { { "IsACtor", true } };
	QVERIFY(ctor.isValid ());
	QCOMPARE(allAnnotations (ctor), expectCtor);
	
	QVariantMap expectMember { { "IsAMember", true } };
	QVERIFY(member.isValid ());
	QCOMPARE(allAnnotations (member), expectMember);
	
	QVariantMap expectMemberTwo;
	QVERIFY(memberTwo.isValid ());
	QCOMPARE(allAnnotations (memberTwo), expectMemberTwo);
	
	QVariantMap expectStatic { { "IsAStatic", true } };
	QVERIFY(staticMethod.isValid ());
	QCOMPARE(allAnnotations (staticMethod), expectStatic);
	
}

void TriaTest::checkEnumAnnotations () {
	Nuria::MetaObject *meta = Nuria::MetaObject::byName ("Test::A");
	
	Nuria::MetaEnum first = meta->enumByName ("First");
	Nuria::MetaEnum second = meta->enumByName ("Second");
	Nuria::MetaEnum numbers = meta->enumByName ("Numbers");
	
	QVariantMap expectEmpty;
	QVariantMap expectNumbers { { "First", 0 }, { "Second", 1 } };
	
	QCOMPARE(allAnnotations (first), expectEmpty);
	QCOMPARE(allAnnotations (second), expectEmpty);
	QCOMPARE(allAnnotations (numbers), expectNumbers);
	
}

void TriaTest::checkFieldAnnotations () {
	Nuria::MetaObject *meta = Nuria::MetaObject::byName ("Test::A");
	Nuria::MetaField field = meta->fieldByName ("field");
	
	QVariantMap expect { { "IsAField", true } };
	QCOMPARE(allAnnotations (field), expect);
	
}

void TriaTest::callMemberMethod () {
	Nuria::MetaObject *meta = Nuria::MetaObject::byName ("Test::A");
	
	// Supress debug output
	QTest::ignoreMessage (QtDebugMsg, "ctor");
	QTest::ignoreMessage (QtDebugMsg, "dtor");
	Test::A a;
	
	QTest::ignoreMessage (QtDebugMsg, "member");
	meta->method ({ "callMe" }).callback (&a) ();
	
	QTest::ignoreMessage (QtDebugMsg, "a=5");
	meta->method ({ "callMe", "int" }).callback (&a) (5);
	
}

void TriaTest::callStaticMethod () {
	Nuria::MetaObject *meta = Nuria::MetaObject::byName ("Test::A");
	
	QTest::ignoreMessage (QtDebugMsg, "static");
	meta->method ({ "callMeStatic" }).callback () ();
	
}

void TriaTest::callOverloadedMethod () {
	Nuria::MetaObject *meta = Nuria::MetaObject::byName ("Test::B");
	
	QTest::ignoreMessage (QtDebugMsg, "ctor");
	QTest::ignoreMessage (QtDebugMsg, "dtor");
	Test::B b;
	
	QCOMPARE(meta->method ({ "sum" }).callback (&b) ().toInt (), 5 + 4 + 3);
	QCOMPARE(meta->method ({ "sum", "int" }).callback (&b) (7).toInt (), 7 + 4 + 3);
	QCOMPARE(meta->method ({ "sum", "int", "int" }).callback (&b) (7, 9).toInt (), 7 + 9 + 3);
	QCOMPARE(meta->method ({ "sum", "int", "int", "int" }).callback (&b) (7, 9, 11).toInt (), 7 + 9 + 11);
	
}

void TriaTest::createAndDestroyInstance () {
	Nuria::MetaObject *meta = Nuria::MetaObject::byName ("Test::A");
	
	// 
	Nuria::MetaMethod ctor = meta->method ({ "" });
	
	QTest::ignoreMessage (QtDebugMsg, "ctor");
	Test::A *a = ctor.callback () ().value< Test::A * > ();
	
	QTest::ignoreMessage (QtDebugMsg, "dtor");
	meta->destroyInstance (a);
	
}

void TriaTest::verifyFieldType () {
	Nuria::MetaObject *meta = Nuria::MetaObject::byName ("Test::A");
	Nuria::MetaField field = meta->fieldByName ("field");
	
	QVERIFY(field.isValid ());
	QCOMPARE(field.typeName (), QByteArray ("int"));
	
}

void TriaTest::readField () {
	Nuria::MetaObject *meta = Nuria::MetaObject::byName ("Test::A");
	Nuria::MetaField field = meta->fieldByName ("field");
	
	QTest::ignoreMessage (QtDebugMsg, "ctor");
	QTest::ignoreMessage (QtDebugMsg, "dtor");
	Test::A a;
	
	a.field = 123;
	QCOMPARE(field.read (&a).toInt (), 123);
}

void TriaTest::writeField () {
	Nuria::MetaObject *meta = Nuria::MetaObject::byName ("Test::A");
	Nuria::MetaField field = meta->fieldByName ("field");
	
	QTest::ignoreMessage (QtDebugMsg, "ctor");
	QTest::ignoreMessage (QtDebugMsg, "dtor");
	Test::A a;
	
	a.field = 0;
	field.write (&a, 123);
	QCOMPARE(a.field, 123);
	
}

void TriaTest::testCtorConversion () {
	QTest::ignoreMessage (QtDebugMsg, "ctor");
	QTest::ignoreMessage (QtDebugMsg, "dtor");
	QTest::ignoreMessage (QtDebugMsg, "dtor");
	QTest::ignoreMessage (QtDebugMsg, "dtor");
	
	QVariant result = Nuria::Variant::convert< Test::B > (QString ("42"));
	QVERIFY(result.userType () == qMetaTypeId< Test::B > ());
	QCOMPARE(result.value< Test::B > ().value, 42);
	
}

void TriaTest::testFromMethodConversion () {
	QTest::ignoreMessage (QtDebugMsg, "ctor");
	QTest::ignoreMessage (QtDebugMsg, "dtor");
	QTest::ignoreMessage (QtDebugMsg, "dtor");
	QTest::ignoreMessage (QtDebugMsg, "dtor");
	
	QVariant result = Nuria::Variant::convert< Test::B > (42);
	QVERIFY(result.userType () == qMetaTypeId< Test::B > ());
	QCOMPARE(result.value< Test::B > ().value, 42);
}

void TriaTest::testToMethodConversion () {
	QTest::ignoreMessage (QtDebugMsg, "ctor");
	QTest::ignoreMessage (QtDebugMsg, "dtor");
	QTest::ignoreMessage (QtDebugMsg, "dtor");
	
	Test::B b;
	b.value = 1337;
	
	QVariant result = Nuria::Variant::convert< QString > (QVariant::fromValue (b));
	QVERIFY(result.userType () == qMetaTypeId< QString > ());
	QCOMPARE(result.toString ().toInt (), 1337);
	
}


void TriaTest::testOperatorConversion () {
	QTest::ignoreMessage (QtDebugMsg, "ctor");
	QTest::ignoreMessage (QtDebugMsg, "dtor");
	QTest::ignoreMessage (QtDebugMsg, "dtor");
	QTest::ignoreMessage (QtDebugMsg, "dtor");
	
	Test::A a;
	a.field = Test::A::Hundred;
	
	QVariant result = Nuria::Variant::convert< Test::A::Numbers > (QVariant::fromValue (a));
	QVERIFY(result.userType () == qMetaTypeId< Test::A::Numbers > ());
	QCOMPARE(result.value< Test::A::Numbers > (), Test::A::Hundred);
}

QTEST_MAIN(TriaTest)
#include "tst_tria.moc"
