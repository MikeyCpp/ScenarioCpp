#pragma once

#include "internal/steps.hpp"
#include "internal/parameterised_function.hpp"

#define SCENARIOCPP_DECL_STEP(step, name) \
    static ::scenariocpp::ParameterlessStepHelper<::scenariocpp::step, ScenarioFixtureType> name() \
    { \
        return \
            { #name, \
              [](ScenarioFixtureType& f){ f.step##_f_##name(); } \
            };\
    } \
    void step##_f_##name()

#undef PRECONDITION_STEP
#define PRECONDITION_STEP(name) \
    SCENARIOCPP_DECL_STEP(Precondition, name)

#undef ACTION_STEP
#define ACTION_STEP(name) \
    SCENARIOCPP_DECL_STEP(Action, name)

#undef EXPECTED_ACTION_STEP
#define EXPECTED_ACTION_STEP(name) \
    SCENARIOCPP_DECL_STEP(ExpectedAction, name)

#undef POSTCONDITION_STEP
#define POSTCONDITION_STEP(name) \
    SCENARIOCPP_DECL_STEP(PostCondition, name)

#define SCENARIOCPP_DECL_STEP_P(step, name, argument) \
    template<typename Parameters, typename U> \
    static ::scenariocpp::ParameterisedStepHelper<::scenariocpp::step, ScenarioFixtureType, Parameters, ::scenariocpp::detail::ParameterisedFunction<void(argument)>::type, U> name(U Parameters::* arg) \
    { \
    return \
        { #name, \
          arg, \
          [](ScenarioFixtureType& aFixture, ::scenariocpp::detail::ParameterisedFunction<void(argument)>::type&& aArg){ aFixture.step##_p_f_ ##name(aArg); }\
        }; \
    } \
    static ::scenariocpp::ContainedParameterStepHelper<::scenariocpp::step, ScenarioFixtureType, ::scenariocpp::detail::ParameterisedFunction<void(argument)>::type> name(::scenariocpp::detail::ParameterisedFunction<void(argument)>::type&& arg) \
    {\
    return \
        { #name,\
         std::forward<::scenariocpp::detail::ParameterisedFunction<void(argument)>::type>(arg),\
         [](ScenarioFixtureType& aFixture, ::scenariocpp::detail::ParameterisedFunction<void(argument)>::type&& aArg){ aFixture.step##_p_f_ ##name(aArg); }\
        }; \
    }\
    void step##_p_f_ ##name(argument)

#undef PRECONDITION_STEP_P
#define PRECONDITION_STEP_P(name, argument ) \
    SCENARIOCPP_DECL_STEP_P(Precondition, name, argument)

#undef ACTION_STEP_P
#define ACTION_STEP_P(name, argument) \
    SCENARIOCPP_DECL_STEP_P(Action, name, argument)

#undef EXPECTED_ACTION_STEP_P
#define EXPECTED_ACTION_STEP_P(name, argument) \
    SCENARIOCPP_DECL_STEP_P(ExpectedAction, name, argument)

#undef POSTCONDITION_STEP_P
#define POSTCONDITION_STEP_P(name, argument) \
    SCENARIOCPP_DECL_STEP_P(PostCondition, name, argument)

#include "internal/fixture.hpp"
#include "internal/scenario.hpp"
#include "internal/scenario_builder.hpp"
#include "internal/scenario_runner.hpp"

#define SCENARIO_F(fixture, name)\
    struct fixture##_##name : public fixture { \
      static const ::scenariocpp::NonParameterisedScenario<fixture> s_; \
    }; \
    const ::scenariocpp::NonParameterisedScenario<fixture>& name() { return fixture##_##name ::s_; } \
    using Scenario_##fixture = ::scenariocpp::detail::EmptyFixture<fixture>; \
    TEST_F(Scenario_##fixture, name) { \
        ::scenariocpp::GTestScenarioRunner runner; \
        runner.Run(fixture##_##name::s_, ::scenariocpp::NoParams{}); \
    } \
    const ::scenariocpp::NonParameterisedScenario<fixture> fixture##_##name::s_ \
    = ::scenariocpp::NonParameterisedScenarioBuilder<fixture>().Build(#fixture, #name)

#define SCENARIO_P(fixture, name, parameters)\
    struct fixture##_##name : public fixture { \
      static const ::scenariocpp::Scenario<fixture, parameters> s_; \
    }; \
    using name = ::scenariocpp::detail::ParameterisedFixture<fixture, parameters>; \
    TEST_P(name, Scenario_##fixture) { \
        ::scenariocpp::GTestScenarioRunner runner; \
        runner.Run(fixture##_##name::s_, GetParam()); \
    } \
    const ::scenariocpp::Scenario<fixture, parameters> fixture##_##name::s_ \
    = ::scenariocpp::ScenarioBuilder<fixture, parameters>().Build(#fixture, #name)

#define INSTANTIATE_SCENARIO_P(name, parameters) \
    INSTANTIATE_TEST_CASE_P(parameters, name, ::testing::ValuesIn(parameters))
