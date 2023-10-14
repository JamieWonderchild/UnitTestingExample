//
// Created by Jamie Aronson on 13/10/2023.
//

#ifndef TESTINGPROJECT_UNITTESTS_HPP
#define TESTINGPROJECT_UNITTESTS_HPP

#include <stdexcept>

constexpr void assert_that(bool statement, const char *message) {
    if (!statement) {
        throw std::runtime_error(message);
    }
}

//signature takes in a ptr to a function that returns void. as a parameter, this is called 'unit_test'
void runTest(void(*unit_test)(), const char *name) {
    try {
        unit_test(); //runs the unit test. If no throw, success!!
        printf("[+] test %s successful \n", name);
    }
    catch (const std::exception &e) {
        printf("[-] test %s failure. %s \n", name, e.what());
    }
}

#endif //TESTINGPROJECT_UNITTESTS_HPP
