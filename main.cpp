#include <iostream>
#include "test/UnitTests.hpp"
#include "test/TestCases.hpp"

int main() {
    runTest(initialSpeedIsZero, "initial speed is zero");
    runTest(initialSensitivityIsFive, "initial sensitivity is 5");
    runTest(sensitivityGreaterThanOne, "Sensitivity cannot be less than one"); // we give a ptr to this func to run test
    runTest(speedIsSaved, "Speed is saved");
    runTest(alertWhenImminent, "Alerted when imminent collision");
    runTest(noAlertWhenImminent, "No alert when non-imminent collision");
    return 0;
}
