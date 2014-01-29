#ifndef _IO_TEST_HPP_
#define _IO_TEST_HPP_

#include <cppunit/extensions/HelperMacros.h>
#include <string>

class IOTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE( IOTest );
	CPPUNIT_TEST( testPcpInput );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	void testPcpInput();
private:
	std::string pcp_in;
};

#endif
