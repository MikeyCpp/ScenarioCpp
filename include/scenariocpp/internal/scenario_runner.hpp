#pragma once

#include "scenario.hpp"
#include "logger.hpp"

#include <gtest/gtest.h>

namespace scenariocpp
{

class GTestScenarioRunner
{
    template<typename Fixture>
    class GTestFixtureHelper : public Fixture
    {
    public:
        void SetUp() override { Fixture::SetUp(); }
        void TearDown() override { Fixture::TearDown(); }
        void TestBody() override {}
    };

public:
    template<typename Fixture, typename Parameters>
    void Run(const Scenario<Fixture, Parameters>& aScenario,
             const Parameters& aParameters)
    {
        Logger::LogInfo("BEGIN SCENARIO", ansi::Colour::Green) << aScenario.GetFixtureName()
                                                               << "."
                                                               << aScenario.GetName()
                                                               << "\n"
                                                               << aScenario.GenerateDescription(aParameters);

        {
            Logger::LogInfo() << "Create test fixture";

            GTestFixtureHelper<Fixture> aFixture;
            aFixture.SetUp();

            Logger::LogInfo() << "Execute scenario";

            const bool verbose{true};
            aScenario.ExecuteScenario(aFixture, aParameters, verbose);

            Logger::LogInfo() << "Checking expected actions were fulfilled";
            aFixture.TearDown();
        } /* scope */

        if (::testing::Test::HasFailure())
        {
            Logger::LogInfo("SCENARIO FAILED", ansi::Colour::Red) << aScenario.GetName();
        }
        else
        {
            Logger::LogInfo("SCENARIO PASSED", ansi::Colour::Green) << aScenario.GetName();
        }
    }
};

} /* namespace scenariocpp */
