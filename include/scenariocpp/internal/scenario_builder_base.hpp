#pragma once

#include "steps.hpp"

#include <memory>

namespace scenariocpp
{

template<typename Fixture, typename Parameters>
struct ScenarioBase
{
    ScenarioBase(std::string fixtureName,
                 std::string scenarioName)
        : fixtureName_(std::move(fixtureName)),
          scenarioName_(std::move(scenarioName))
    {}

    std::string fixtureName_;
    std::string scenarioName_;

    std::vector<std::shared_ptr<Step<Fixture, Parameters>>> PreScenarios_;
    std::vector<std::shared_ptr<Step<Fixture, Parameters>>> Preconditions_;
    std::vector<std::shared_ptr<Step<Fixture, Parameters>>> ExpectedActions_;
    std::vector<std::shared_ptr<Step<Fixture, Parameters>>> Actions_;
    std::vector<std::shared_ptr<Step<Fixture, Parameters>>> PostConditions_;

    std::vector<std::shared_ptr<Step<Fixture, Parameters>>> allSteps_;
};

template<typename Fixture, typename Parameters>
struct ScenarioBuilderBase
{
    ScenarioBuilderBase(std::shared_ptr<ScenarioBase<Fixture, Parameters>> base)
        : base_(base)
    {}

    std::shared_ptr<ScenarioBase<Fixture, Parameters>> base_;
};

} /* namespace scenariocpp */
