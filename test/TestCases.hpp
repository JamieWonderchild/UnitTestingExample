//
// Created by Jamie Aronson on 13/10/2023.
//

#ifndef TESTINGPROJECT_TESTCASES_HPP
#define TESTINGPROJECT_TESTCASES_HPP

#include "../include/AutoBreak.h"
#include "UnitTests.hpp"

void initialSpeedIsZero() {
    MockServiceBus bus{};
    AutoBreak auto_break(bus);
    assert_that(auto_break.getSpeedMps() == 0L, "Initial speed is not 0");
}

void initialSensitivityIsFive() {
    MockServiceBus bus{};
    AutoBreak auto_break(bus);
    assert_that(auto_break.getCollisionThresholdS() == 5l, "Initial Sensitivity is not 5");
}

void sensitivityGreaterThanOne() {
    try {
        MockServiceBus bus{};
        AutoBreak autoBreak(bus);
        autoBreak.setCollisionThresholdS(0.5L);
        /*
         * Explanation:
         * If calling the setCollisionThresholdS fnc does not throw an exception, there will be nothing to catch, so
         * automatically the test will have failed.
         * If you do not have a case that you want to test, then there is no need to make a test
         * Hence, we have the requirement for sensitivity > 1. If this is on satisfied when setting sensitivity, we throw
         * in the set function
         */
    }
    catch (const std::exception &ex) {
        return;
    }
    assert_that(false, "No exception thrown"); //this will execute if no exception is thrown
}

void speedIsSaved() {
    MockServiceBus bus{};
    AutoBreak auto_break(bus);
    bus.speed_update_callback(SpeedUpdate{100});
    assert_that(auto_break.getSpeedMps() == 100, "speed update not saved to 100");
    bus.speed_update_callback(SpeedUpdate{50});
    assert_that(auto_break.getSpeedMps() == 50, "speed update not saved to 50");
    bus.speed_update_callback(SpeedUpdate{0});
    assert_that(auto_break.getSpeedMps() == 0, "speed update not saved to 0");
}

void alertWhenImminent() {
    // requirement -> time to collision < collision threshold should publish warning
    int commands_published{0};
    MockServiceBus bus{};
    AutoBreak auto_break(bus);
    auto_break.setCollisionThresholdS(10);
    bus.speed_update_callback(SpeedUpdate{100});
    bus.car_detected_update_callback(CarDetected{100, 0});
    assert_that(bus.number_commands_published == 1, "break commands published not 1");
};

void noAlertWhenImminent() {
    // requirement -> time to collision < collision threshold should publish warning
    int commands_published{0};
    MockServiceBus bus{};
    AutoBreak auto_break(bus);
    auto_break.setCollisionThresholdS(2);
    bus.speed_update_callback(SpeedUpdate{100});
    bus.car_detected_update_callback(CarDetected{1000, 0});
    assert_that(bus.number_commands_published == 0, "break commands published not 0");
};
#endif //TESTINGPROJECT_TESTCASES_HPP
