/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#define _CXXTEST_HAVE_STD
#define _CXXTEST_HAVE_EH
#define _CXXTEST_ABORT_TEST_ON_FAIL
#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>
#include <cxxtest/ErrorPrinter.h>

int main() {
 return CxxTest::ErrorPrinter().run();
}
#include "basictests.h"

static MyTestSuite suite_MyTestSuite;

static CxxTest::List Tests_MyTestSuite = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_MyTestSuite( "basictests.h", 11, "MyTestSuite", suite_MyTestSuite, Tests_MyTestSuite );

static class TestDescription_MyTestSuite_testCreateDanglingAttribute : public CxxTest::RealTestDescription {
public:
 TestDescription_MyTestSuite_testCreateDanglingAttribute() : CxxTest::RealTestDescription( Tests_MyTestSuite, suiteDescription_MyTestSuite, 15, "testCreateDanglingAttribute" ) {}
 void runTest() { suite_MyTestSuite.testCreateDanglingAttribute(); }
} testDescription_MyTestSuite_testCreateDanglingAttribute;

static class TestDescription_MyTestSuite_testGetSetOnDanglingAttributes : public CxxTest::RealTestDescription {
public:
 TestDescription_MyTestSuite_testGetSetOnDanglingAttributes() : CxxTest::RealTestDescription( Tests_MyTestSuite, suiteDescription_MyTestSuite, 24, "testGetSetOnDanglingAttributes" ) {}
 void runTest() { suite_MyTestSuite.testGetSetOnDanglingAttributes(); }
} testDescription_MyTestSuite_testGetSetOnDanglingAttributes;

static class TestDescription_MyTestSuite_testGetSetNodeValue : public CxxTest::RealTestDescription {
public:
 TestDescription_MyTestSuite_testGetSetNodeValue() : CxxTest::RealTestDescription( Tests_MyTestSuite, suiteDescription_MyTestSuite, 40, "testGetSetNodeValue" ) {}
 void runTest() { suite_MyTestSuite.testGetSetNodeValue(); }
} testDescription_MyTestSuite_testGetSetNodeValue;

static class TestDescription_MyTestSuite_testOverlayDanglingAttributes : public CxxTest::RealTestDescription {
public:
 TestDescription_MyTestSuite_testOverlayDanglingAttributes() : CxxTest::RealTestDescription( Tests_MyTestSuite, suiteDescription_MyTestSuite, 57, "testOverlayDanglingAttributes" ) {}
 void runTest() { suite_MyTestSuite.testOverlayDanglingAttributes(); }
} testDescription_MyTestSuite_testOverlayDanglingAttributes;

static class TestDescription_MyTestSuite_testOverwriteDanglingAttributes : public CxxTest::RealTestDescription {
public:
 TestDescription_MyTestSuite_testOverwriteDanglingAttributes() : CxxTest::RealTestDescription( Tests_MyTestSuite, suiteDescription_MyTestSuite, 84, "testOverwriteDanglingAttributes" ) {}
 void runTest() { suite_MyTestSuite.testOverwriteDanglingAttributes(); }
} testDescription_MyTestSuite_testOverwriteDanglingAttributes;

static class TestDescription_MyTestSuite_testSetValue : public CxxTest::RealTestDescription {
public:
 TestDescription_MyTestSuite_testSetValue() : CxxTest::RealTestDescription( Tests_MyTestSuite, suiteDescription_MyTestSuite, 111, "testSetValue" ) {}
 void runTest() { suite_MyTestSuite.testSetValue(); }
} testDescription_MyTestSuite_testSetValue;

static class TestDescription_MyTestSuite_testNonExistantGetNode : public CxxTest::RealTestDescription {
public:
 TestDescription_MyTestSuite_testNonExistantGetNode() : CxxTest::RealTestDescription( Tests_MyTestSuite, suiteDescription_MyTestSuite, 123, "testNonExistantGetNode" ) {}
 void runTest() { suite_MyTestSuite.testNonExistantGetNode(); }
} testDescription_MyTestSuite_testNonExistantGetNode;

static class TestDescription_MyTestSuite_testSetGetOffRoot : public CxxTest::RealTestDescription {
public:
 TestDescription_MyTestSuite_testSetGetOffRoot() : CxxTest::RealTestDescription( Tests_MyTestSuite, suiteDescription_MyTestSuite, 132, "testSetGetOffRoot" ) {}
 void runTest() { suite_MyTestSuite.testSetGetOffRoot(); }
} testDescription_MyTestSuite_testSetGetOffRoot;

static class TestDescription_MyTestSuite_testNodePaths : public CxxTest::RealTestDescription {
public:
 TestDescription_MyTestSuite_testNodePaths() : CxxTest::RealTestDescription( Tests_MyTestSuite, suiteDescription_MyTestSuite, 142, "testNodePaths" ) {}
 void runTest() { suite_MyTestSuite.testNodePaths(); }
} testDescription_MyTestSuite_testNodePaths;

static class TestDescription_MyTestSuite_testRootNodeAttributes : public CxxTest::RealTestDescription {
public:
 TestDescription_MyTestSuite_testRootNodeAttributes() : CxxTest::RealTestDescription( Tests_MyTestSuite, suiteDescription_MyTestSuite, 158, "testRootNodeAttributes" ) {}
 void runTest() { suite_MyTestSuite.testRootNodeAttributes(); }
} testDescription_MyTestSuite_testRootNodeAttributes;

static class TestDescription_MyTestSuite_testMultiSectionPath : public CxxTest::RealTestDescription {
public:
 TestDescription_MyTestSuite_testMultiSectionPath() : CxxTest::RealTestDescription( Tests_MyTestSuite, suiteDescription_MyTestSuite, 175, "testMultiSectionPath" ) {}
 void runTest() { suite_MyTestSuite.testMultiSectionPath(); }
} testDescription_MyTestSuite_testMultiSectionPath;

static class TestDescription_MyTestSuite_testSerialize : public CxxTest::RealTestDescription {
public:
 TestDescription_MyTestSuite_testSerialize() : CxxTest::RealTestDescription( Tests_MyTestSuite, suiteDescription_MyTestSuite, 204, "testSerialize" ) {}
 void runTest() { suite_MyTestSuite.testSerialize(); }
} testDescription_MyTestSuite_testSerialize;

static class TestDescription_MyTestSuite_testOverlayAttributes : public CxxTest::RealTestDescription {
public:
 TestDescription_MyTestSuite_testOverlayAttributes() : CxxTest::RealTestDescription( Tests_MyTestSuite, suiteDescription_MyTestSuite, 251, "testOverlayAttributes" ) {}
 void runTest() { suite_MyTestSuite.testOverlayAttributes(); }
} testDescription_MyTestSuite_testOverlayAttributes;

static class TestDescription_MyTestSuite_testOverlayAttributes2 : public CxxTest::RealTestDescription {
public:
 TestDescription_MyTestSuite_testOverlayAttributes2() : CxxTest::RealTestDescription( Tests_MyTestSuite, suiteDescription_MyTestSuite, 272, "testOverlayAttributes2" ) {}
 void runTest() { suite_MyTestSuite.testOverlayAttributes2(); }
} testDescription_MyTestSuite_testOverlayAttributes2;

static class TestDescription_MyTestSuite_testOverlayAttributes3 : public CxxTest::RealTestDescription {
public:
 TestDescription_MyTestSuite_testOverlayAttributes3() : CxxTest::RealTestDescription( Tests_MyTestSuite, suiteDescription_MyTestSuite, 294, "testOverlayAttributes3" ) {}
 void runTest() { suite_MyTestSuite.testOverlayAttributes3(); }
} testDescription_MyTestSuite_testOverlayAttributes3;

static class TestDescription_MyTestSuite_testOverwriteAttributes : public CxxTest::RealTestDescription {
public:
 TestDescription_MyTestSuite_testOverwriteAttributes() : CxxTest::RealTestDescription( Tests_MyTestSuite, suiteDescription_MyTestSuite, 311, "testOverwriteAttributes" ) {}
 void runTest() { suite_MyTestSuite.testOverwriteAttributes(); }
} testDescription_MyTestSuite_testOverwriteAttributes;

static class TestDescription_MyTestSuite_testOverwriteAttributes2 : public CxxTest::RealTestDescription {
public:
 TestDescription_MyTestSuite_testOverwriteAttributes2() : CxxTest::RealTestDescription( Tests_MyTestSuite, suiteDescription_MyTestSuite, 331, "testOverwriteAttributes2" ) {}
 void runTest() { suite_MyTestSuite.testOverwriteAttributes2(); }
} testDescription_MyTestSuite_testOverwriteAttributes2;

static class TestDescription_MyTestSuite_testOverwriteAttributes3 : public CxxTest::RealTestDescription {
public:
 TestDescription_MyTestSuite_testOverwriteAttributes3() : CxxTest::RealTestDescription( Tests_MyTestSuite, suiteDescription_MyTestSuite, 352, "testOverwriteAttributes3" ) {}
 void runTest() { suite_MyTestSuite.testOverwriteAttributes3(); }
} testDescription_MyTestSuite_testOverwriteAttributes3;

static class TestDescription_MyTestSuite_testAttribProxyMock : public CxxTest::RealTestDescription {
public:
 TestDescription_MyTestSuite_testAttribProxyMock() : CxxTest::RealTestDescription( Tests_MyTestSuite, suiteDescription_MyTestSuite, 392, "testAttribProxyMock" ) {}
 void runTest() { suite_MyTestSuite.testAttribProxyMock(); }
} testDescription_MyTestSuite_testAttribProxyMock;

static class TestDescription_MyTestSuite_testAttribProxyDangling : public CxxTest::RealTestDescription {
public:
 TestDescription_MyTestSuite_testAttribProxyDangling() : CxxTest::RealTestDescription( Tests_MyTestSuite, suiteDescription_MyTestSuite, 408, "testAttribProxyDangling" ) {}
 void runTest() { suite_MyTestSuite.testAttribProxyDangling(); }
} testDescription_MyTestSuite_testAttribProxyDangling;

#include <cxxtest/Root.cpp>
