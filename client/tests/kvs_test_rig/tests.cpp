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
CxxTest::StaticSuiteDescription suiteDescription_MyTestSuite( "basictests.h", 9, "MyTestSuite", suite_MyTestSuite, Tests_MyTestSuite );

static class TestDescription_MyTestSuite_testDanglingAttribute : public CxxTest::RealTestDescription {
public:
 TestDescription_MyTestSuite_testDanglingAttribute() : CxxTest::RealTestDescription( Tests_MyTestSuite, suiteDescription_MyTestSuite, 13, "testDanglingAttribute" ) {}
 void runTest() { suite_MyTestSuite.testDanglingAttribute(); }
} testDescription_MyTestSuite_testDanglingAttribute;

static class TestDescription_MyTestSuite_testNonExistantGetNode : public CxxTest::RealTestDescription {
public:
 TestDescription_MyTestSuite_testNonExistantGetNode() : CxxTest::RealTestDescription( Tests_MyTestSuite, suiteDescription_MyTestSuite, 22, "testNonExistantGetNode" ) {}
 void runTest() { suite_MyTestSuite.testNonExistantGetNode(); }
} testDescription_MyTestSuite_testNonExistantGetNode;

static class TestDescription_MyTestSuite_testSetGetOffRoot : public CxxTest::RealTestDescription {
public:
 TestDescription_MyTestSuite_testSetGetOffRoot() : CxxTest::RealTestDescription( Tests_MyTestSuite, suiteDescription_MyTestSuite, 31, "testSetGetOffRoot" ) {}
 void runTest() { suite_MyTestSuite.testSetGetOffRoot(); }
} testDescription_MyTestSuite_testSetGetOffRoot;

static class TestDescription_MyTestSuite_testNodePaths : public CxxTest::RealTestDescription {
public:
 TestDescription_MyTestSuite_testNodePaths() : CxxTest::RealTestDescription( Tests_MyTestSuite, suiteDescription_MyTestSuite, 41, "testNodePaths" ) {}
 void runTest() { suite_MyTestSuite.testNodePaths(); }
} testDescription_MyTestSuite_testNodePaths;

static class TestDescription_MyTestSuite_testRootNodeAttributes : public CxxTest::RealTestDescription {
public:
 TestDescription_MyTestSuite_testRootNodeAttributes() : CxxTest::RealTestDescription( Tests_MyTestSuite, suiteDescription_MyTestSuite, 56, "testRootNodeAttributes" ) {}
 void runTest() { suite_MyTestSuite.testRootNodeAttributes(); }
} testDescription_MyTestSuite_testRootNodeAttributes;

#include <cxxtest/Root.cpp>
