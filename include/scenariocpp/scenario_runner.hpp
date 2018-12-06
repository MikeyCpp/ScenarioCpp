#pragma once

#include "scenario.hpp"

#include "logger.hpp"

#include "gtest/gtest.h"

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
    void Run(
            const Scenario<Fixture, Parameters>& aScenario,
            const Parameters& aParameters)
    {
        Logger::Log("BEGIN SCENARIO", ansi::Colour::Green) << aScenario.GetFixtureName()
                                                           << "."
                                                           << aScenario.GetName()
                                                           << "\n"
                                                           << aScenario.GenerateDescription(aParameters)
                                                           << std::endl;

        {
            Logger::LogInfo() << "Set-up test fixture" << std::endl;

            GTestFixtureHelper<Fixture> aFixture;
            aFixture.SetUp();

            Logger::LogInfo() << "Execute scenario" << std::endl;

            try
            {
                aScenario.ExecuteScenario(aFixture, aParameters, true);
                Logger::LogInfo() << "Checking expected actions were fulfilled" << std::endl;
            }
            catch (ScenarioException& e)
            {
                std::cout << ansi::Colour::Red << "Scenario halted, because: " << e.what() << std::endl;
            }

            aFixture.TearDown();
        }

        if(::testing::Test::HasFailure())
        {
            Logger::Log("SCENARIO FAILED", ansi::Colour::Red) << aScenario.GetName() << std::endl;
        }
        else
        {
            Logger::Log("SCENARIO PASSED", ansi::Colour::Green) << aScenario.GetName() << std::endl;
        }
    }
};

} /* namespace scenariocpp */
