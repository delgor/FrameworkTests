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

#include <nuria/httpparser.hpp>

#include <QtTest/QtTest>
#include <QObject>

using namespace Nuria;

class HttpParserTest : public QObject {
	Q_OBJECT
private slots:
	
	void parseFirstLineHappyPath ();
	void parseFirstLineBadData_data ();
	void parseFirstLineBadData ();
	
	void parseHeaderLineHappyPath_data ();
	void parseHeaderLineHappyPath ();
	void parseHeaderLineBadData_data ();
	void parseHeaderLineBadData ();
	
	void parseVersion_data ();
	void parseVersion ();
};

void HttpParserTest::parseFirstLineHappyPath () {
	HttpParser parser;
	QByteArray data = "GET foo/bar HTTP/1.1";
	
	QByteArray verb;
	QByteArray path;
	QByteArray version;
	QVERIFY(parser.parseFirstLine (data, verb, path, version));
	
	QCOMPARE(verb, QByteArray ("GET"));
	QCOMPARE(path, QByteArray ("foo/bar"));
	QCOMPARE(version, QByteArray ("1.1"));
}

void HttpParserTest::parseFirstLineBadData_data () {
	QTest::addColumn< QString > ("line");
	
	QTest::newRow ("empty") << "";
	QTest::newRow ("verb") << " foo/bar HTTP/1.0";
	QTest::newRow ("empty path") << "GET  HTTP/1.0";
	QTest::newRow ("empty version") << "GET foo/bar ";
	QTest::newRow ("version") << "GET foo/bar";
	QTest::newRow ("broken version") << "GET foo/bar 1.1";
	
}

void HttpParserTest::parseFirstLineBadData () {
	HttpParser parser;
	QFETCH(QString, line);
	QByteArray data = line.toLatin1 ();
	
	QByteArray verb;
	QByteArray path;
	QByteArray version;
	QVERIFY(!parser.parseFirstLine (data, verb, path, version));
}

void HttpParserTest::parseHeaderLineHappyPath_data () {
	QTest::addColumn< QString > ("line");
	
	QTest::newRow ("colon") << "Foo: Bar: Baz";
	QTest::newRow ("colon space") << "Foo:Bar: Baz";
}

void HttpParserTest::parseHeaderLineHappyPath () {
	HttpParser parser;
	QFETCH(QString, line);
	QByteArray data = line.toLatin1 ();
	
	QByteArray name;
	QByteArray value;
	QVERIFY(parser.parseHeaderLine (data, name, value));
	
	QCOMPARE(name, QByteArray("Foo"));
	QCOMPARE(value, QByteArray("Bar: Baz"));
}

void HttpParserTest::parseHeaderLineBadData_data () {
	QTest::addColumn< QString > ("line");
	
	QTest::newRow ("empty") << "";
	QTest::newRow ("no colon") << "Foo=Bar";
	QTest::newRow ("empty name") << ": ";
	QTest::newRow ("empty value") << "Foo: ";
}

void HttpParserTest::parseHeaderLineBadData () {
	HttpParser parser;
	QFETCH(QString, line);
	QByteArray data = line.toLatin1 ();
	
	QByteArray name;
	QByteArray value;
	QVERIFY(!parser.parseHeaderLine (data, name, value));
}

Q_DECLARE_METATYPE(Nuria::HttpClient::HttpVersion)
void HttpParserTest::parseVersion_data () {
	QTest::addColumn< QString > ("version");
	QTest::addColumn< HttpClient::HttpVersion > ("result");
	
	QTest::newRow ("0.9") << "0.9" << HttpClient::HttpUnknown;
	QTest::newRow ("1.0") << "1.0" << HttpClient::Http1_0;
	QTest::newRow ("1.1") << "1.1" << HttpClient::Http1_1;
	QTest::newRow ("2.0") << "2.0" << HttpClient::HttpUnknown;
}

void HttpParserTest::parseVersion () {
	HttpParser parser;
	QFETCH(QString, version);
	QFETCH(HttpClient::HttpVersion, result);
	QByteArray data = version.toLatin1 ();
	
	QCOMPARE(parser.parseVersion (data), result);
	
}

QTEST_MAIN(HttpParserTest)
#include "tst_httpparser.moc"
