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
	
	void parseVerb_data ();
	void parseVerb ();
	
	void parseRangeHeaderValueHappyPath ();
	void parseRangeHeaderValueBadData_data ();
	void parseRangeHeaderValueBadData ();
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

void HttpParserTest::parseVerb_data () {
	QTest::addColumn< QString > ("verb");
	QTest::addColumn< HttpClient::HttpVerb > ("result");
	
	QTest::newRow ("GET") << "GET" << HttpClient::GET;
	QTest::newRow ("POST") << "POST" << HttpClient::POST;
	QTest::newRow ("HEAD") << "HEAD" << HttpClient::HEAD;
	QTest::newRow ("PUT") << "PUT" << HttpClient::PUT;
	QTest::newRow ("DELETE") << "DELETE" << HttpClient::DELETE;
	QTest::newRow ("invalid") << "get" << HttpClient::InvalidVerb;
}

void HttpParserTest::parseVerb () {
	HttpParser parser;
	QFETCH(QString, verb);
	QFETCH(HttpClient::HttpVerb, result);
	QByteArray data = verb.toLatin1 ();
	
	QCOMPARE(parser.parseVerb (data), result);
	
}

void HttpParserTest::parseRangeHeaderValueHappyPath () {
	HttpParser parser;
	QByteArray data = "bytes=50-60";
	
	int begin = 0;
	int end = 0;
	QVERIFY(parser.parseRangeHeaderValue (data, begin, end));
	
	QCOMPARE(begin, 50);
	QCOMPARE(end, 60);
}

void HttpParserTest::parseRangeHeaderValueBadData_data () {
	QTest::addColumn< QString > ("range");
	
	QTest::newRow ("empty") << "";
	QTest::newRow ("no bytes=") << "12-34";
	QTest::newRow ("missing minus") << "bytes=1234";
	QTest::newRow ("missing begin") << "bytes=-34";
	QTest::newRow ("missing end") << "bytes=12-";
	QTest::newRow ("bad begin") << "bytes=0xaf-500";
	QTest::newRow ("bad end") << "bytes=12-0xaf";
	QTest::newRow ("begin < end") << "bytes=34-12";
	QTest::newRow ("begin == end") << "bytes=12-12";
	QTest::newRow ("end is negative") << "bytes=0--12";
	
}

void HttpParserTest::parseRangeHeaderValueBadData () {
	HttpParser parser;
	QFETCH(QString, range);
	QByteArray data = range.toLatin1 ();
	
	int begin = 0;
	int end = 0;
	QVERIFY(!parser.parseRangeHeaderValue (data, begin, end));
}

QTEST_MAIN(HttpParserTest)
#include "tst_httpparser.moc"
