#pragma once

#include "steps.hpp"
#include "scenario_step.hpp"
#include "failure.hpp"

#include "scenario_builder_base.hpp"

#include "no_params.hpp"

#include "chained_forward_iterator.hpp"
#include "stringutils.hpp"

#include "logger.hpp"

#include "gtest/gtest.h"

#include <string>
#include <memory>

namespace scenariocpp
{

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
        if (failure_.HasFailure(&aParameters))
        {
            Logger::LogError() << "The scenario " << GetFixtureName() << "::" << GetName()
                               << " has previously failed.";
            FAIL();
            return;
        }

        for (auto& step : detail::MakeChainedForwardIterator(base_->PreScenarios_,
                                                             base_->Preconditions_,
                                                             base_->ExpectedActions_,
                                                             base_->Actions_,
                                                             base_->PostConditions_))
        {
            if (verbose)
            {
                Logger::LogInfo(stringutils::ToUpper(stringutils::Humanise(step->GetKeyword())), ansi::Colour::Amber)
                        << GetFixtureName() << "." << step->GetName();
            }

            step->Execute(aFixture, aParameters);

            if (::testing::Test::HasFailure())
            {
                failure_.SetFailure(&aParameters);

                Logger::LogError() << "The " << stringutils::Humanise(step->GetKeyword())
                   << " " << GetFixtureName() << "::" << step->GetName()
                   << " -> \"" << step->GetDescription(aParameters) << "\""
                   << ", in scenario '" << GetName()
                   << "' failed. ";

                return;
            }
        }
    }

    std::string GenerateDescription(const Parameters& aParameters) const
    {
        std::string description {"Scenario: "};
        description += stringutils::CapitaliseFirst(stringutils::Humanise(base_->scenarioName_));

        for (auto& step : base_->stepsInOrderOfDeclaration_)
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
    mutable Failure<Parameters> failure_;
    std::shared_ptr<ScenarioBase<Fixture, Parameters>> base_;
};

} /* namespace scenariocpp */
