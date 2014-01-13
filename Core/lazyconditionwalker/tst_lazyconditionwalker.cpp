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
#include <nuria/lazyconditionwalker.hpp>
#include <nuria/lazyevaluation.hpp>

class LazyConditionWalkerTest : public QObject {
	Q_OBJECT
private slots:
	
	void testLazyConditionHandlers ();
	void testFieldHandlers ();
	void testVariantHandlers ();
	
	void ensureDepthFirstOrder ();
	
	void doNotWalkArguments ();
	void doWalkArguments ();
};

void LazyConditionWalkerTest::testLazyConditionHandlers () {
	using namespace Nuria;
	
	// 
	LazyCondition condition (val (1) == val (2) && val (3) != val (4) &&
				 val (1) > val (2) && val (3) >= val (4) &&
				 val (1) < val (2) || val (3) <= val (4));
	
	QVector< int > result;
	QVector< int > expected {
		LazyCondition::Equal, LazyCondition::NonEqual, LazyCondition::LogicAnd, 
		LazyCondition::Greater, LazyCondition::LogicAnd,
		LazyCondition::GreaterEqual, LazyCondition::LogicAnd, LazyCondition::Less,
		LazyCondition::LogicAnd, LazyCondition::LessEqual, LazyCondition::LogicOr
	};
	
	
	Callback testFunc = Callback::fromLambda([&](QVariantList, LazyCondition c) {
			    result.append (c.type ());
	});
	
	// 
	LazyConditionWalker walker;
	walker.onCondition (LazyCondition::Empty, testFunc);
	walker.onCondition (LazyCondition::Single, testFunc);
	walker.onCondition (LazyCondition::Equal, testFunc);
	walker.onCondition (LazyCondition::NonEqual, testFunc);
	walker.onCondition (LazyCondition::Greater, testFunc);
	walker.onCondition (LazyCondition::GreaterEqual, testFunc);
	walker.onCondition (LazyCondition::Less, testFunc);
	walker.onCondition (LazyCondition::LessEqual, testFunc);
	walker.onCondition (LazyCondition::LogicAnd, testFunc);
	walker.onCondition (LazyCondition::LogicOr, testFunc);
	
	// 
	walker.walk (condition);
	QCOMPARE(result, expected);
	
}

void LazyConditionWalkerTest::testFieldHandlers () {
	using namespace Nuria;
	
	// 
	LazyCondition condition (val (1) == arg (0) && test ("foo") == Field (51, QVariant ()));
	
	QVector< int > result;
	QVector< int > expected {
		Field::Argument, Field::TestCall, 51 /* Custom type */
	};
	
	
	Callback testFunc = Callback::fromLambda([&](QVariantList, Field f) {
			    result.append (f.customType ());
	});
	
	// 
	LazyConditionWalker walker;
	walker.onField (Field::Value, testFunc);
	walker.onField (Field::Argument, testFunc);
	walker.onField (Field::TestCall, testFunc);
	walker.onField (51, testFunc);
	
	// 
	walker.walk (condition, false);
	
	QCOMPARE(result, expected);
	
}

void LazyConditionWalkerTest::testVariantHandlers () {
	using namespace Nuria;
	
	// 
	LazyCondition condition (arg (0) == val(5) && (val(QString()) == val(true)));
	
	QVector< int > result;
	QVector< int > expected {
		QMetaType::Int, QMetaType::QString, QMetaType::Bool
	};
	
	
	Callback testFunc = Callback::fromLambda([&](QVariantList, QVariant v) {
			    result.append (v.userType ());
	});
	
	// 
	LazyConditionWalker walker;
	walker.onVariant (qMetaTypeId< LazyCondition > (), testFunc); // Doesn't trigger
	walker.onVariant (qMetaTypeId< Field > (), testFunc); // Doesn't trigger
	walker.onVariant (QMetaType::Int, testFunc);
	walker.onVariant (QMetaType::QString, testFunc);
	walker.onVariant (QMetaType::Bool, testFunc);
	
	// 
	walker.walk (condition, false);
	QCOMPARE(result, expected);
	
}

void LazyConditionWalkerTest::ensureDepthFirstOrder () {
	using namespace Nuria;
	
	// 
	LazyCondition condition (test ("foo", 5));
	
	QVector< int > result;
	QVector< int > expected {
		QMetaType::Int, qMetaTypeId< Field > (), qMetaTypeId< LazyCondition > ()
	};
	
	// 
	Callback testFunc = Callback::fromLambda([&](QVariantList, QVariant v) {
			    result.append (v.userType ());
	});
	
	// 
	LazyConditionWalker walker;
	walker.onVariant (QMetaType::Int, testFunc);
	walker.onField (Field::TestCall, testFunc);
	walker.onCondition (LazyCondition::Single, testFunc);
	
	// 
	walker.walk (condition, true);
	QCOMPARE(result, expected);
	
}


void LazyConditionWalkerTest::doNotWalkArguments () {
	using namespace Nuria;
	
	// 
	LazyCondition condition (test ("foo", 5));
	
	// 
	int intSeen = 0;
	Callback testFunc = Callback::fromLambda([&](QVariantList, QVariant v) {
			    intSeen++;
	});
	
	// 
	LazyConditionWalker walker;
	walker.onVariant (QMetaType::Int, testFunc);
	
	// 
	walker.walk (condition, false);
	QCOMPARE(intSeen, 0);
}

void LazyConditionWalkerTest::doWalkArguments () {
	using namespace Nuria;
	
	// 
	LazyCondition condition (test ("foo", 5));
	
	// 
	int intSeen = 0;
	Callback testFunc = Callback::fromLambda([&](QVariantList, QVariant v) {
			    intSeen++;
	});
	
	// 
	LazyConditionWalker walker;
	walker.onVariant (QMetaType::Int, testFunc);
	
	// 
	walker.walk (condition, true);
	QCOMPARE(intSeen, 1);
}

QTEST_MAIN(LazyConditionWalkerTest)
#include "tst_lazyconditionwalker.moc"
