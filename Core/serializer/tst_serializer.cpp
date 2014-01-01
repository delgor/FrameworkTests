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

#include <nuria/serializer.hpp>

#include <QtTest/QtTest>
#include <QObject>

class SerializerTest : public QObject {
	Q_OBJECT
public:
	SerializerTest ();
	
private slots:
	void initTestCase();
	
	// Serializer::objectFromMap
	void emptyMapCreatesDefaultInstance ();
	void useCorrectCtorAndSetProperties ();
	void createWithExcludedProperty ();
	void createWithFailedProperty ();
	void createWithFailedPropertyNoList ();
	void objectFromNestedMap ();
	void objectFromNestedMapNotInCtor ();
	
	// Serializer::objectToMap
	void qObjectReturnsEmptyMap ();
	void serializeTestClass ();
	void serializeTestClassExcludeFoo ();
	void nestedObjectFromMap ();
	
};

class TestClass : public QObject {
	Q_OBJECT
	
	Q_CLASSINFO("objectToMap.exclude.usedDefaultCtor", "true")
	Q_PROPERTY(bool usedDefaultCtor READ usedDefaultCtor)
	Q_PROPERTY(int someInt READ someInt WRITE setSomeInt)
	Q_PROPERTY(TestClass *child READ child WRITE setChild)
	Q_PROPERTY(QString foo READ foo)
	
public:
	
	Q_INVOKABLE TestClass ()
		: m_usedDefaultCtor (true), m_someInt (0), m_child (nullptr)
	{}
	
	Q_INVOKABLE TestClass (const QString &foo)
		: m_usedDefaultCtor (false), m_someInt (0), m_foo (foo), m_child (nullptr)
	{}
	
	Q_INVOKABLE TestClass (const QString &foo, TestClass *child)
		: m_usedDefaultCtor (false), m_someInt (0), m_foo (foo), m_child (child)
	{}
	
	bool usedDefaultCtor () const { return this->m_usedDefaultCtor; }
	int someInt () const { return this->m_someInt; }
	void setSomeInt (int i) { this->m_someInt = i; }
	QString foo () const { return this->m_foo; }
	TestClass *child () const { return this->m_child; }
	void setChild (TestClass *child) { this->m_child = child; }
	
	bool compareTo (const TestClass *other) {
		return (this->m_someInt == other->m_someInt &&
			this->m_foo == other->m_foo &&
			((!this->m_child && !other->m_child) ||
			 (this->m_child && other->m_child &&
			  this->m_child->compareTo (other->m_child))));
	}
	
private:
	bool m_usedDefaultCtor;
	int m_someInt;
	QString m_foo;
	TestClass *m_child;
};

class AnotherClass : public QObject {
	Q_OBJECT
	Q_PROPERTY(TestClass *child READ child WRITE setChild)
public:
	
	Q_INVOKABLE
	AnotherClass () : QObject () {}
	
	TestClass *child () const { return this->m_child; }
	void setChild (TestClass *c) { this->m_child = c; }
	
private:
	TestClass *m_child;
	
};

Q_DECLARE_METATYPE(TestClass*)
Q_DECLARE_METATYPE(AnotherClass*)

// 
static QVariantMap getTestClassMap () {
	QVariantMap map;
	
	map.insert ("someInt", 123);
	map.insert ("foo", "Foo");
	
	return map;
}

static QVariantMap getNestedTestClassMap () {
	QVariantMap child = getTestClassMap ();
	QVariantMap parent = getTestClassMap ();
	
	child.insert ("foo", "Child");
	parent.insert ("child", child);
	
	return parent;
}

SerializerTest::SerializerTest () {
	
}

void SerializerTest::initTestCase () {
	qRegisterMetaType< TestClass * > ();
	qRegisterMetaType< AnotherClass * > ();
}

void SerializerTest::qObjectReturnsEmptyMap () {
	QVariantMap expected;
	
	QVariantMap result = Nuria::Serializer::objectToMap (new QObject);
	QCOMPARE(result, expected);
}

void SerializerTest::serializeTestClass () {
	QVariantMap expected = getTestClassMap ();
	
	TestClass testClass ("Foo");
	testClass.setSomeInt (123);
	QVariantMap result = Nuria::Serializer::objectToMap (&testClass);
	
	QCOMPARE(result, expected);
}

void SerializerTest::serializeTestClassExcludeFoo () {
	QVariantMap expected = getTestClassMap ();
	expected.remove ("foo");
	
	TestClass testClass;
	testClass.setSomeInt (123);
	QVariantMap result = Nuria::Serializer::objectToMap (&testClass, 0, QStringList() << "foo");
	
	QCOMPARE(result, expected);
}

void SerializerTest::nestedObjectFromMap () {
	TestClass child ("Child");
	TestClass parent ("Foo", &child);
	child.setSomeInt (123);
	parent.setSomeInt (123);
	
	QVariantMap expected = getNestedTestClassMap ();
	QVariantMap result = Nuria::Serializer::objectToMap (&parent, 100);
	
	QCOMPARE(result, expected);
}

void SerializerTest::emptyMapCreatesDefaultInstance () {
	QObject *result = Nuria::Serializer::objectFromMap (&TestClass::staticMetaObject, QVariantMap ());
	
	QVERIFY(result);
	QCOMPARE(QString(result->metaObject ()->className ()), QString("TestClass"));
	QCOMPARE(result->property ("usedDefaultCtor"), QVariant(true));
	delete result;
}

void SerializerTest::useCorrectCtorAndSetProperties () {
	QObject *result = Nuria::Serializer::objectFromMap (&TestClass::staticMetaObject, getTestClassMap ());
	TestClass *test = qobject_cast< TestClass * > (result);
	
	QVERIFY(test);
	QCOMPARE(test->usedDefaultCtor (), false);
	QCOMPARE(test->someInt (), 123);
	QCOMPARE(test->foo (), QString ("Foo"));
	delete result;
	
}

void SerializerTest::createWithExcludedProperty () {
	QObject *result = Nuria::Serializer::objectFromMap (&TestClass::staticMetaObject, getTestClassMap (),
							    0, QStringList() << "foo");
	TestClass *test = qobject_cast< TestClass * > (result);
	
	QVERIFY(test);
	QCOMPARE(test->usedDefaultCtor (), true);
	QCOMPARE(test->someInt (), 123);
	QCOMPARE(test->foo (), QString ());
	delete result;
	
}

void SerializerTest::createWithFailedProperty () {
	QVariantMap map = getTestClassMap ();
	map.insert ("foo", QByteArray());
	
	QObject *result = Nuria::Serializer::objectFromMap (&TestClass::staticMetaObject, map);
	TestClass *test = qobject_cast< TestClass * > (result);
	
	QVERIFY(test);
	QCOMPARE(test->usedDefaultCtor (), true);
	QCOMPARE(test->someInt (), 123);
	QCOMPARE(test->foo (), QString ());
	delete result;
	
}

void SerializerTest::createWithFailedPropertyNoList () {
	QStringList failed;
	QVariantMap map = getTestClassMap ();
	map.insert ("foo", QByteArray());
	
	QObject *result = Nuria::Serializer::objectFromMap (&TestClass::staticMetaObject, map,
							    0, QStringList(), &failed);
	TestClass *test = qobject_cast< TestClass * > (result);
	
	QVERIFY(test);
	QCOMPARE(test->usedDefaultCtor (), true);
	QCOMPARE(test->someInt (), 123);
	QCOMPARE(test->foo (), QString ());
	QVERIFY(failed.length () == 1);
	QVERIFY(failed.contains ("foo"));
	delete result;
	
}

void SerializerTest::objectFromNestedMap () {
	QVariantMap map = getNestedTestClassMap ();
	QStringList failed;
	
	QObject *obj = Nuria::Serializer::objectFromMap (&TestClass::staticMetaObject, map,
							 1, QStringList(), &failed);
	TestClass *parent = qobject_cast< TestClass * > (obj);
	
	QVERIFY(parent);
	QVERIFY(parent->child ());
	QVERIFY(!parent->child ()->child ());
	QVERIFY(failed.isEmpty ());
	QCOMPARE(parent->foo (), QString("Foo"));
	QCOMPARE(parent->child ()->foo (), QString("Child"));
	
}

void SerializerTest::objectFromNestedMapNotInCtor () {
	QVariantMap map = getNestedTestClassMap ();
	
	QObject *obj = Nuria::Serializer::objectFromMap (&AnotherClass::staticMetaObject, map, 1);
	AnotherClass *another = qobject_cast< AnotherClass * > (obj);
	
	QVERIFY(another);
	QVERIFY(another->child ());
	QVERIFY(!another->child ()->child ());
	QCOMPARE(another->child ()->foo (), QString("Child"));
	
}

QTEST_MAIN(SerializerTest)
#include "tst_serializer.moc"
