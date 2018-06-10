//
// Created by Alex Shaver on 10/8/17.
//

#include "OrbitalEngine.h"

namespace engine{

OrbitalHistory OrbitalEngine::generateOrbitalHistory(double numberOfPeriods,
                                                     int numberOfTimeDivisions,
                                                     const orbital::Orbit& orbit){
    OrbitalHistory history;
    return history;
}

}

#ifdef BUILD_TESTS

#include <gtest/gtest.h>

namespace engine{
namespace {

class OrbitTest : public ::testing::Test{

};

TEST_F(OrbitTest, dummy){
//    Orbit orbit(StateVector{}, 1);
}

} // anonymous namespace
} // namespace engine

#endif // BUILD_TESTS
