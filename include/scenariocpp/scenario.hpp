#pragma once

#include "steps.hpp"
#include "scenario_step.hpp"

#include "scenario_builder_base.hpp"

#include "no_params.hpp"

#include "stringutils.hpp"

#include "logger.hpp"

#include "gtest/gtest.h"

#include <string>
#include <vector>
#include <memory>

namespace scenariocpp
{

class ScenarioException : public std::logic_error
{
public:
    ScenarioException(const std::string& error)
        : logic_error(error) {}
};

template<typename Fixture, typename Parameters>
class Scenario
{
public:
    Scenario(ScenarioBuilderBase<Fixture, Parameters>&& builder)
        : fixtureName_(std::move(builder.steps_->fixtureName_)),
          scenarioName_(std::move(builder.steps_->scenarioName_))
    {
        AddStepsInExecutionOrder(std::move(builder));
    }

    void ExecuteScenario(Fixture& aFixture,
                         const Parameters& aParameters,
                         bool verbose) const
    {
        for(auto& step : steps_)
        {
            if(verbose)
            {
                using namespace stringutils;
                auto keyword = ToUpper(Humanise(step->GetKeyword()));

                Logger::Log(keyword, ansi::Colour::Amber)
                        << GetFixtureName() << "." << step->GetName()
                        << std::endl;
            }

            step->Execute(aFixture, aParameters);

            if(::testing::Test::HasFailure())
            {
                std::stringstream ss;

                ss << "The " << stringutils::Humanise(step->GetKeyword())
                   << " " << GetFixtureName() << "::" << step->GetName()
                   << " -> \"" << step->GetDescription(aParameters) << "\""
                   << ", in scenario '" << GetName()
                   << "' failed. ";

                throw ScenarioException(ss.str());
            }
        }
    }

    std::string GenerateDescription(const Parameters& aParameters) const
    {
        std::string description {"Scenario: "};
        description += stringutils::CapitaliseFirst(stringutils::Humanise(scenarioName_));

        for(auto& step : steps_)
        {
            description += "\n  ";
            description += step->GetDescription(aParameters);
        }

        return description;
    }

    const std::string& GetName() const
    {
        return scenarioName_;
    }

    const std::string& GetFixtureName() const
    {
        return fixtureName_;
    }

private:
    void AddStepsInExecutionOrder(ScenarioBuilderBase<Fixture, Parameters>&& builder)
    {
        AddSteps(std::move(builder.steps_->PreScenarios_));
        AddSteps(std::move(builder.steps_->Preconditions_));
        AddSteps(std::move(builder.steps_->ExpectedActions_));
        AddSteps(std::move(builder.steps_->Actions_));
        AddSteps(std::move(builder.steps_->PostConditions_));
    }

    void AddSteps(std::vector<std::unique_ptr<Step<Fixture, Parameters>>>&& steps)
    {
        for(auto& step : steps)
            steps_.push_back(std::move(step));
    }

    std::vector<std::unique_ptr<Step<Fixture, Parameters>>> steps_;

    std::string fixtureName_;
    std::string scenarioName_;
};

/*
Scenario halted, because: The postcondition TestString::TheStringNowEquals -> "the string now equals [helloworldmoto]", in scenario AppendingValue failed.
*/

} /* namespace scenariocpp */
