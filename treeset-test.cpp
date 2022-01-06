#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <list>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

// Include the testing-logger library from
// the CS70 system directory in Docker.
#include <cs70/testinglogger.hpp>
#include <cs70/randuint32.hpp>

#include "treeset.hpp"

using namespace std;

///////////////////////////////////////////////////////////
//  TESTING
///////////////////////////////////////////////////////////


bool insertTest() {
    TestingLogger log("insert");

    TreeSet<string> mySet;

    mySet.insert("Isaac");
    affirm(mySet.size() == 1);

    mySet.insert("Tejus");
    affirm(mySet.exists("Tejus") == true);
    affirm(mySet.size() == 2);

    return log.summarize();
}

bool existsTest() {
    TestingLogger log("exists");

    TreeSet<int> mySet;

    affirm(mySet.exists(7) == false);

    mySet.insert(2);
    affirm(mySet.exists(2) == true);

    return log.summarize();
}


bool printTest() {
    TestingLogger log("print");

    TreeSet<double> mySet;

    mySet.insert(2.1);

    stringstream ss;
    ss << mySet;

    string expected = "(-, 2.1, -)";

    affirm(ss.str() == expected);

    return log.summarize();
}

bool beginTest() {
    TestingLogger log("begin");

    TreeSet<int> mySet;
    mySet.insert(42);

    TreeSet<int>::iterator iter = mySet.begin();

    affirm(iter == mySet.begin());

    return log.summarize();
}

bool endTest() {
    TestingLogger log("end");

    TreeSet<string> mySet;
    mySet.insert("42");
    mySet.insert("43");
    mySet.insert("41");

    TreeSet<string>::iterator iter = mySet.end();

    affirm(iter == mySet.end());

    return log.summarize();
}

bool starTest() {
    TestingLogger log("star");

    TreeSet<char> mySet;
    mySet.insert('2');
    mySet.insert('3');
    mySet.insert('1');

    TreeSet<char>::iterator iter = mySet.begin();

    affirm(*iter == '2');

    return log.summarize();
}

bool iteratorEqualsTest() {
    TestingLogger log("iterator equals");

    TreeSet<int> mySet;
    mySet.insert(42);
    mySet.insert(43);
    mySet.insert(41);

    TreeSet<int>::iterator iterOne = mySet.begin();
    TreeSet<int>::iterator iterTwo = mySet.begin();

    affirm(iterOne == iterTwo);

    ++iterOne;

    affirm(!(iterOne == iterTwo));

    return log.summarize();
}

bool iteratorPlusTest() {
    TestingLogger log("iterator plus");

    TreeSet<float> mySet;
    mySet.insert(4.2);
    mySet.insert(4.3);
    mySet.insert(4.1);

    TreeSet<float>::iterator iter = mySet.begin();

    ++iter;
    float x = 4.1;
    affirm(*iter == x);

    ++iter;
    ++iter;
    affirm(iter == mySet.end());

    return log.summarize();
}

bool treeEqualsTest() {
    TestingLogger log("tree equals");

    TreeSet<int> mySetOne;
    TreeSet<int> mySetTwo;

    affirm(mySetOne == mySetTwo);
    mySetOne.insert(42);
    mySetOne.insert(43);
    mySetOne.insert(41);

    mySetTwo.insert(41);
    mySetTwo.insert(42);
    mySetTwo.insert(43);

    affirm(mySetOne == mySetTwo);

    mySetOne.insert(44);

    affirm(!(mySetTwo == mySetOne));

    return log.summarize();
}

bool heightTest() {
    TestingLogger log("height");

    TreeSet<bool> mySet;
    mySet.insert(true);
    mySet.insert(false);
    mySet.insert(true);

    affirm(mySet.height() == 1);

    return log.summarize();
}


bool depthTest() {
    TestingLogger log("depth");

    TreeSet<int> mySet;
    affirm(mySet.averageDepth() == 0);
    mySet.insert(42);
    mySet.insert(43);
    mySet.insert(41);

    affirm(mySet.averageDepth() == 2.0/3.0);

    return log.summarize();
}

bool showStatisticsTest() {
    TestingLogger log("print");

    TreeSet<string> mySet;
    mySet.insert("42");
    mySet.insert("43");
    mySet.insert("41");
    mySet.insert("40");

    stringstream ss;
    mySet.showStatistics(ss);

    string expected = "4 nodes, height 2, average depth 1\n";

    affirm(ss.str() == expected);

    return log.summarize();
}

bool randomInsertTest() {
    TestingLogger log("random insert");

    TreeSet<string> mySet(treetype::RANDOMIZED);
    mySet.insert("42");
    mySet.insert("43");
    mySet.insert("41");
    mySet.insert("40");

    affirm(mySet.size() == 4);

    return log.summarize();
}

bool rootInsertTest() {
    TestingLogger log("root insert");

    TreeSet<int> mySet(treetype::ROOT);
    mySet.insert(1);
    mySet.insert(3);
    mySet.insert(2);
    mySet.insert(4);

    affirm(mySet.size() == 4);
    affirm(mySet.height() == 2);

    return log.summarize();
}

bool seedCreation() {
    TestingLogger log("seed");

    TreeSet<int> mySet(treetype::RANDOMIZED, 17);
    mySet.insert(1);
    mySet.insert(3);
    mySet.insert(2);
    mySet.insert(4);

    affirm(mySet.size() == 4);

    return log.summarize();
}

/*
 * Test the TreeSet
 */
int main(int, char**) {
    TestingLogger alltests("All tests");


    affirm(insertTest());

    affirm(existsTest());

    affirm(beginTest());

    affirm(endTest());

    affirm(starTest());

    affirm(iteratorEqualsTest());

    affirm(iteratorPlusTest());

    affirm(printTest());

    affirm(treeEqualsTest());

    affirm(heightTest());

    affirm(depthTest());

    affirm(showStatisticsTest());

    affirm(randomInsertTest());

    affirm(rootInsertTest());

    affirm(seedCreation());

    if (alltests.summarize(true)) {
        return 0;  // Error code of 0 == Success!
    } else {
        return 2;  // Arbitrarily chosen exit code of 2 means tests failed.
    }
}
