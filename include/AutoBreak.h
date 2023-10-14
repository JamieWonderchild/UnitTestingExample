//
// Created by Jamie Aronson on 13/10/2023.
//

#ifndef TESTINGPROJECT_AUTOBREAK_H
#define TESTINGPROJECT_AUTOBREAK_H

#include <functional>

struct SpeedUpdate {
    double velocity_mps;
};

struct CarDetected {
    double distance_m;
    double speed_mps;
};

struct BreakCommand {
    double time_to_collision_s;
};

struct SpeedLimitDetected {
    double speed_limit;
};

/*
 * a function that returns void and accepts a speed update parameter
 * std::function needs to know type
 * function is defined by its return type and parameter types. Both must be defined in <>
 */
using SpeedUpdateCallback = std::function<void(const SpeedUpdate &)>;
using CarDetectedUpdateCallback = std::function<void(const CarDetected &)>;
using SpeedLimitCallback = std::function<void(const SpeedLimitDetected &)>;

struct I_ServiceBus {
    virtual ~I_ServiceBus() = default;

    virtual void subscribe(SpeedUpdateCallback) = 0;

    virtual void subscribe(CarDetectedUpdateCallback) = 0;

    virtual void subscribe(SpeedLimitCallback) = 0;

    virtual void publish(const BreakCommand &) = 0;
};

struct MockServiceBus : public I_ServiceBus {
    void publish(const BreakCommand &b_c) override {
        number_commands_published++;
        last_command = b_c;
    }

    void subscribe(CarDetectedUpdateCallback c_b) override {
        car_detected_update_callback = c_b;
    }

    void subscribe(SpeedUpdateCallback c_b) override {
        speed_update_callback = c_b;
    }

    void subscribe(SpeedLimitCallback c_b) override {
        speed_limit_detected_update_callback = c_b;
    }

    int number_commands_published;
    BreakCommand last_command;
    SpeedUpdateCallback speed_update_callback{};
    CarDetectedUpdateCallback car_detected_update_callback{};
    SpeedLimitCallback speed_limit_detected_update_callback{};
};

//autobreak is the class we are unit testing
struct AutoBreak {
public:
    explicit AutoBreak(I_ServiceBus &s) : collision_threshold_s{5}, last_known_speed_limit{39} {
        s.subscribe([this](const SpeedUpdate &speed_update) {
            /*
             * this defines what happens when speed_update_callback() is called. This is done on initialization
             * Remember we said that SpeedUpdateCallback is really std::function<void(const SpeedUpdate&)>
             * This is the function and the implementation we have defined it to update the current speed
             * We say that the callback should execute this function each time it is called ie update the speed
             */
            speed_mps = speed_update.velocity_mps;
        });

        s.subscribe([this, &s](const CarDetected &car_detected) {
            const auto relative_velocity = this->speed_mps - car_detected.speed_mps;
            const auto collision_time = car_detected.distance_m / relative_velocity;
            if (collision_time > 0 and collision_time < collision_threshold_s) {
                s.publish(BreakCommand{collision_threshold_s});
            }
        });

        s.subscribe([this, &s](const SpeedLimitDetected &new_speed_limit) {
            last_known_speed_limit = new_speed_limit.speed_limit;
            if (speed_mps > new_speed_limit.speed_limit) {
                const auto speed_diff = speed_mps - new_speed_limit.speed_limit;
                s.publish(BreakCommand{speed_diff});
            }
        });
    }

    void setCollisionThresholdS(const double &collision_threshold_S) {
        if (collision_threshold_S < 1) {
            throw std::exception();
        }
        this->collision_threshold_s = collision_threshold_S;
    }

    double getSpeedMps() {
        return speed_mps;
    }

    double getCollisionThresholdS() {
        return collision_threshold_s;
    }

    double getLastKnownSpeedLimit() {
        return last_known_speed_limit;
    }

private:
    double collision_threshold_s;
    double speed_mps{0};
    double last_known_speed_limit;
};


#endif //TESTINGPROJECT_AUTOBREAK_H
