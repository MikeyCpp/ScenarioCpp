#pragma once

#include "scenario_builder_base.hpp"

#include "steps.hpp"
#include "scenario_step.hpp"

namespace scenariocpp
{

template<typename Fixture, typename Parameters>
class ScenarioBuilder
{
    using BaseBuilder = ScenarioBuilderBase<Fixture, Parameters>;

    #define DECL_BUILDER_FUNC(name, keyword, next_step)  \
    \
    template<typename T, typename U> \
    next_step##Builder name(ParameterisedStepHelper<keyword, Fixture, Parameters, T, U> a##keyword) \
    { \
        BaseBuilder::steps_->keyword##s_.emplace_back(new ParameterisedStep<Fixture, Parameters, T, U>(#name, #keyword, std::move(a##keyword)));\
        return BaseBuilder::steps_; \
    } \
     \
    next_step##Builder name(ParameterlessStepHelper<keyword, Fixture> a##keyword) \
    { \
        BaseBuilder::steps_->keyword##s_.emplace_back(new ParameterlessStep<Fixture, Parameters>(#name, #keyword, std::move(a##keyword))); \
        return BaseBuilder::steps_; \
    } \
     \
    template<typename T> \
    next_step##Builder name(ContainedParameterStepHelper<keyword, Fixture, T> a##keyword) \
    { \
        BaseBuilder::steps_->keyword##s_.emplace_back(new ContainedParameterStep<Fixture, Parameters, T>(#name, #keyword, std::move(a##keyword))); \
        return BaseBuilder::steps_; \
    }

    struct ThenBuilder : public BaseBuilder
    {
        ThenBuilder(std::shared_ptr<Steps<Fixture, Parameters>> steps)
            : BaseBuilder(steps) {}

        DECL_BUILDER_FUNC(And, PostCondition, Then)
        DECL_BUILDER_FUNC(And, ExpectedAction, Then)
    };

    struct WhenBuilder : public BaseBuilder
    {
        WhenBuilder(std::shared_ptr<Steps<Fixture, Parameters>> steps)
            : BaseBuilder(steps) {}

        DECL_BUILDER_FUNC(And, Action, When)

        DECL_BUILDER_FUNC(Then, PostCondition, Then)
        DECL_BUILDER_FUNC(Then, ExpectedAction, Then)
    };

    struct GivenBuilder : public BaseBuilder
    {
        GivenBuilder(std::shared_ptr<Steps<Fixture, Parameters>> steps)
            : BaseBuilder(steps) {}

        GivenBuilder And(const NonParameterisedScenario<Fixture>& scenario)
        {
            BaseBuilder::steps_->PreScenarios_.emplace_back(new ScenarioStep<Fixture, Parameters>("And", scenario));
            return BaseBuilder::steps_;
        }

        DECL_BUILDER_FUNC(And, Precondition, Given)

        DECL_BUILDER_FUNC(When, Action, When)
        DECL_BUILDER_FUNC(When, ExpectedAction, When)
    };

    struct InitialBuilder : public BaseBuilder
    {
        InitialBuilder(std::shared_ptr<Steps<Fixture, Parameters>> steps)
            : BaseBuilder(steps) {}

        GivenBuilder Given(const NonParameterisedScenario<Fixture>& scenario)
        {
            BaseBuilder::steps_->PreScenarios_.emplace_back(new ScenarioStep<Fixture, Parameters>("Given", scenario));
            return BaseBuilder::steps_;
        }

        DECL_BUILDER_FUNC(Given, Precondition, Given)
    };

    #undef DECL_BUILDER_FUNC

public:
    InitialBuilder Build(const std::string& fixtureName,
                         const std::string& scenarioName)
    {
        return InitialBuilder(std::make_shared<Steps<Fixture, Parameters>>(fixtureName,
                                                                           scenarioName));
    }
};

template<typename Fixture>
using NonParameterisedScenarioBuilder = ScenarioBuilder<Fixture, NoParams>;

} /* namespace scenariocpp */
