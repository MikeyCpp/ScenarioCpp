#pragma once

#include "steps.hpp"

#include <memory>

namespace scenariocpp
{

template<typename Fixture, typename Parameters>
struct Steps
{
    Steps(std::string fixtureName,
          std::string scenarioName)
        : fixtureName_(std::move(fixtureName)),
          scenarioName_(std::move(scenarioName))
    {}

    std::string fixtureName_;
    std::string scenarioName_;

    std::vector<std::unique_ptr<Step<Fixture, Parameters>>> PreScenarios_;
    std::vector<std::unique_ptr<Step<Fixture, Parameters>>> Preconditions_;
    std::vector<std::unique_ptr<Step<Fixture, Parameters>>> ExpectedActions_;
    std::vector<std::unique_ptr<Step<Fixture, Parameters>>> Actions_;
    std::vector<std::unique_ptr<Step<Fixture, Parameters>>> PostConditions_;
};

template<typename Fixture, typename Parameters>
struct ScenarioBuilderBase
{
    ScenarioBuilderBase(std::shared_ptr<Steps<Fixture, Parameters>> steps)
        : steps_(steps)
    {}

    std::shared_ptr<Steps<Fixture, Parameters>> steps_;
};

} /* namespace scenariocpp */
