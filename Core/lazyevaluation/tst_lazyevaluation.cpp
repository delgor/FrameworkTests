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

#include <QtTest/QtTest>

#define NURIA_NO_VARIANT_COMPARISON
#include <nuria/lazyevaluation.hpp>

class LazyEvaluationTest : public QObject {
	Q_OBJECT
private slots:
	void fieldHandlesTypesCorrectly ();
	void testFieldToVariant ();
	
	void singleCondition ();
	void basicConditions ();
	void simpleConjunction ();
	void passedArgument ();
	void stlAlgorithm ();
};

// 
void LazyEvaluationTest::fieldHandlesTypesCorrectly () {
	using namespace Nuria;
	
	Field emptyType;
	Field valueType (val (1));
	Field argumentType (arg (2));
	Field callType (test ("aaa", 456));
	Field customType (Field::Custom + 1, QVariant ());
	
	QCOMPARE(emptyType.type (), Field::Empty);
	QCOMPARE(emptyType.customType (), int (Field::Empty));
	QCOMPARE(valueType.type (), Field::Value);
	QCOMPARE(valueType.customType (), int (Field::Value));
	QCOMPARE(argumentType.type (), Field::Argument);
	QCOMPARE(argumentType.customType (), int (Field::Argument));
	QCOMPARE(callType.type (), Field::TestCall);
	QCOMPARE(callType.customType (), int (Field::TestCall));
	QCOMPARE(customType.type (), Field::Custom);
	QCOMPARE(customType.customType (), int (Field::Custom + 1));
}

void LazyEvaluationTest::testFieldToVariant () {
	using namespace Nuria;
	
	QCOMPARE(Field ().toVariant (), QVariant ());
	QCOMPARE(val(1).toVariant (), QVariant (1));
	QCOMPARE(arg(123).toVariant ().userType (), qMetaTypeId< Field > ());
	QCOMPARE(test ("name", 123).toVariant ().userType (), qMetaTypeId< Field > ());
	
}

void LazyEvaluationTest::singleCondition () {
	using namespace Nuria;
	
	QVERIFY(LazyCondition (val(1))());
	QVERIFY(LazyCondition (val(true))());
	QVERIFY(!LazyCondition (val(false))());
}

void LazyEvaluationTest::basicConditions () {
	using namespace Nuria;
	
	QCOMPARE((val(1) == val(2))(), 1 == 2);
	QCOMPARE((val(1) != val(2))(), 1 != 2);
	QCOMPARE((val(1) < val(2))(), 1 < 2);
	QCOMPARE((val(1) <= val(2))(), 1 <= 2);
	QCOMPARE((val(1) > val(2))(), 1 > 2);
	QCOMPARE((val(1) >= val(2))(), 1 >= 2);
	
}

void LazyEvaluationTest::simpleConjunction () {
	using namespace Nuria;
	
	QCOMPARE((val(1) != val(2) && val(3) < val(4))(), (1 != 2 && 3 < 4));
	QCOMPARE((val(1) != val(2) && val(3) > val(4))(), (1 != 2 && 3 > 4));
	QCOMPARE((val(1) != val(2) || val(3) > val(4))(), (1 != 2 || 3 > 4));
	QCOMPARE((val(1) == val(2) && val(3) < val(4))(), (1 == 2 && 3 < 4));
	QCOMPARE((val(1) == val(2) || val(3) > val(4))(), (1 == 2 || 3 > 4));
	QCOMPARE((val(1) == val(2) || val(3) < val(4))(), (1 == 2 || 3 < 4));
	
}

void LazyEvaluationTest::passedArgument () {
	using namespace Nuria;
	
	QString a ("a");
	QString b ("b");
	
	QVERIFY(LazyCondition(arg(0))(true));
	QVERIFY(!LazyCondition(arg(0))(false));
	QCOMPARE((arg(0) == arg(1))(a, b), a == b);
	QCOMPARE((arg(0) != arg(1))(a, b), a != b);
	
}

void LazyEvaluationTest::stlAlgorithm () {
	using namespace Nuria;
	
	QVector< int > vector { 0, 1, 2, 3, 4, 5 };
	QVector< int > expected { 3, 4, 5 };
	QVector< int > target (3);
	
	std::copy_if(vector.begin (), vector.end (), target.begin (), arg(0) >= 3);
	QCOMPARE(target, expected);
}

QTEST_MAIN(LazyEvaluationTest)
#include "tst_lazyevaluation.moc"
