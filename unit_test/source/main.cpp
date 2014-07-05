//
//  main.cpp
//  GBJ
//
//  Created by Baptiste Dupy on 24/10/13.
//  Copyright (c) 2013 Baptiste Dupy. All rights reserved.
//

#include <iostream>

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/XmlOutputter.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/CompilerOutputter.h>

#include "unit_paths.hpp"

int main(int argc, const char * argv[])
{
    std::string xmlLoc = std::string(CYTOKGEN_UNIT_TEST_RESULTS_DIR)+"macosx.xml";
    
    CppUnit::TextTestRunner runner;
    
    std::ofstream ofs(xmlLoc);
    
    CppUnit::XmlOutputter* xml =
        new CppUnit::XmlOutputter(&runner.result() , ofs);
    runner.setOutputter(xml);
    
    //runner.addTest();
    runner.run();
}

