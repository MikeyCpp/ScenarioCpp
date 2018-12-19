#pragma once

#include "no_params.hpp"
#include "steps.hpp"

namespace scenariocpp
{

template<typename Fixture, typename Parameters>
class Scenario;

template<typename Fixture>
using NonParameterisedScenario = Scenario<Fixture, NoParams>;

template<typename Fixture, typename Parameters>
struct ScenarioStep : public Step<Fixture, Parameters>
{
    ScenarioStep(std::string step,
                 const NonParameterisedScenario<Fixture>& scenario)
        : Step<Fixture, Parameters>(std::move(step), "Scenario"),
          scenario_(scenario){}

    std::string GetName() const override { return scenario_.GetName(); }
    std::string GetDescription(const Parameters&) const override
    {
        std::stringstream ss;
        ss << Step<Fixture, Parameters>::GetStep()
           << " "
           << stringutils::Humanise(scenario_.GetName());

        return ss.str();
    }

    void Execute(Fixture& aFixture, const Parameters&) override
    {
        scenario_.ExecuteScenario(aFixture, {}, false);
    }

private:
    const NonParameterisedScenario<Fixture>& scenario_;
};

} /* namespace scenariocpp */
