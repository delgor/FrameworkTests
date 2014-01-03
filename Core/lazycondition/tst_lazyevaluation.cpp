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

#include <nuria/lazyevaluation.hpp>

class LazyEvaluationTest : public QObject {
	Q_OBJECT
private slots:
	
	void singleCondition ();
	void basicConditions ();
	void simpleConjunction ();
	void passedArgument ();
	void stlAlgorithm ();
};

// 
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
