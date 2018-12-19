#pragma once

#include "steps.hpp"
#include "scenario_step.hpp"

#include "scenario_builder_base.hpp"

#include "no_params.hpp"

#include "chained_forward_iterator.hpp"
#include "stringutils.hpp"

#include "logger.hpp"

#include "gtest/gtest.h"

#include <string>
#include <vector>
#include <memory>
#include <list>

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
        : base_(std::move(builder.base_)) {}

    void ExecuteScenario(Fixture& aFixture,
                         const Parameters& aParameters,
                         bool verbose) const
    {
        for(auto& step : detail::MakeChainedForwardIterator(base_->PreScenarios_,
                                                            base_->Preconditions_,
                                                            base_->ExpectedActions_,
                                                            base_->Actions_,
                                                            base_->PostConditions_))
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
        description += stringutils::CapitaliseFirst(stringutils::Humanise(base_->scenarioName_));

        for(auto& step : base_->allSteps_)
        {
            description += "\n  ";
            description += step->GetDescription(aParameters);
        }

        return description;
    }

    const std::string& GetName() const
    {
        return base_->scenarioName_;
    }

    const std::string& GetFixtureName() const
    {
        return base_->fixtureName_;
    }

private:
    std::shared_ptr<ScenarioBase<Fixture, Parameters>> base_;
};

} /* namespace scenariocpp */
