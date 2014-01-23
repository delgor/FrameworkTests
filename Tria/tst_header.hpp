/* Copyright (c) 2014, The Nuria Project
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *    1. The origin of this software must not be misrepresented; you must not
 *       claim that you wrote the original software. If you use this software
 *       in a product, an acknowledgment in the product documentation would be
 *       appreciated but is not required.
 *    2. Altered source versions must be plainly marked as such, and must not be
 *       misrepresented as being the original software.
 *    3. This notice may not be removed or altered from any source
 *       distribution.
 */

#ifndef TST_HEADER_HPP
#define TST_HEADER_HPP

#include <nuria/essentials.hpp>
#include <QObject>
#include <QString>

// Namespaces are allowed by using the qualified name of types.
namespace Test {

// Forward declarations don't bring tria down.
class A;
struct B;

// We can find A by looking for classes with the "DoesItWork" annotation.
class NURIA_INTROSPECT NURIA_ANNOTATE(DoesItWork, 1 + 2) A {
	
	// Private symbols are ignored.
	int ignoredField;
	enum IgnoredEnum { IgnoredElement };
	void ignoredMethod () {}
	static void ignoredStatic () {}
	A (int a) { (void)a; }
	
public:
	
	// Enums are sorted ..
	enum First {};
	enum Second {};
	
	// Test multiple annotations, test key <-> value
	enum
	NURIA_ANNOTATE(First, 0) NURIA_ANNOTATE(Second, 1)
	Numbers {
		One = 1,
		Two,
		Ten = 10,
		Hundred = 100
	};
	
	// 
	NURIA_ANNOTATE(IsACtor, true)
	A () { qDebug("ctor"); }
	~A () { qDebug("dtor"); }
	
	// We're using Nuria::Callback, thus we can do a simple test here.
	// If Nuria::Callback fails its own test will notice.
	
	NURIA_ANNOTATE(IsAStatic, true)
	static void callMeStatic () { qDebug("static"); }
	
	NURIA_ANNOTATE(IsAMember, true)
	void callMe () { qDebug("member"); }
	
	void callMe (int a) { qDebug("a=%i", a); }
	
	// Conversion operator
	operator Numbers () { return (Numbers)field; }
	
	// Will be ignored because return type or soem argument doesn't have value semantics.
	QObject foo ();
	void foo (QObject a);
	
	NURIA_ANNOTATE(IsAField, true)
	int field;
	
};

// B inherits A. We'll be able to find B through this.
struct NURIA_INTROSPECT B : public A {
	int value;
	
	// All elements are stored sorted
	int c, a, b;
	
	// Needed to enable value-semantics.
	B () : value (-1) { }
	
	// 
	int sum (int a = 5, int b = 4, int c = 3)
	{ return a + b + c; }
	
	// Test conversion detection
	B (QString s) { value = s.toInt (); }
	static B fromInt (int i) { B b; b.value = i; return b; }
	
	// to* member-converters, const vs non-const
	QByteArray toByteArray () { return QByteArray::number (value); }
	QString toString () const { return QString::number (value); }
	
};

// This struct will NOT be made available.
struct Ignored {};

}

// Tria won't declare already Q_DECLARE_METATYPE'd types.
Q_DECLARE_METATYPE(Test::A::Numbers)
Q_DECLARE_METATYPE(Test::A*)
Q_DECLARE_METATYPE(Test::A)
Q_DECLARE_METATYPE(Test::B*)
Q_DECLARE_METATYPE(Test::B)

#endif // TST_HEADER_HPP
