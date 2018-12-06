#pragma once

#include "steps.hpp"

#include "gtest/gtest.h"

#include <type_traits>

namespace scenariocpp
{

template<typename FixtureType>
struct Fixture : public ::testing::Test
{
    using ScenarioFixtureType = FixtureType;
};


template<typename FixtureType>
struct DerivesFixture
{
    using ScenarioFixtureType = FixtureType;
};

template<typename T>
struct ParamerterisedFunction {};

template<typename T>
struct ParamerterisedFunction<void(T)>
{
    using type = T;
};

} /* namespace scenariocpp */

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
    static ::scenariocpp::ParameterisedStepHelper<::scenariocpp::step, ScenarioFixtureType, Parameters, ::scenariocpp::ParamerterisedFunction<void(argument)>::type, U> name(U Parameters::* arg) \
    { \
    return \
        { #name, \
          arg, \
          [](ScenarioFixtureType& aFixture, ::scenariocpp::ParamerterisedFunction<void(argument)>::type&& aArg){ aFixture.step##_p_f_ ##name(aArg); }\
        }; \
    } \
    static ::scenariocpp::ContainedParameterStepHelper<::scenariocpp::step, ScenarioFixtureType, ::scenariocpp::ParamerterisedFunction<void(argument)>::type> name(::scenariocpp::ParamerterisedFunction<void(argument)>::type&& arg) \
    {\
    return \
        { #name,\
         std::forward<::scenariocpp::ParamerterisedFunction<void(argument)>::type>(arg),\
         [](ScenarioFixtureType& aFixture, ::scenariocpp::ParamerterisedFunction<void(argument)>::type&& aArg){ aFixture.step##_p_f_ ##name(aArg); }\
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

#include "scenario.hpp"
#include "scenario_builder.hpp"
#include "scenario_runner.hpp"

#include "gtest_fixture_setup_override.hpp"

#define SCENARIO_F(fixture, name)\
    using Scenario_##fixture = ::scenariocpp::GTestFixtureSetupOverride<fixture>; \
    struct fixture##_##name : public Scenario_##fixture { \
      static const ::scenariocpp::NonParameterisedScenario<fixture> s_; \
    }; \
    const ::scenariocpp::NonParameterisedScenario<fixture>& name() { return fixture##_##name ::s_; } \
    TEST_F(Scenario_##fixture, name) { \
        ::scenariocpp::GTestScenarioRunner runner; \
        runner.Run(fixture##_##name::s_, ::scenariocpp::NoParams{}); \
    } \
    const ::scenariocpp::NonParameterisedScenario<fixture> fixture##_##name::s_ \
    = ::scenariocpp::NonParameterisedScenarioBuilder<fixture>().Build(#fixture, #name)

#define SCENARIO_P(fixture, name, parameters)\
    struct name : public ::scenariocpp::GTestFixtureSetupOverride<fixture>, \
                  public ::testing::WithParamInterface<parameters> { \
    static const ::scenariocpp::Scenario<fixture, parameters> s_; \
    }; \
    TEST_P(name, Scenario_##fixture) { \
        ::scenariocpp::GTestScenarioRunner runner; \
        runner.Run(name::s_, GetParam()); \
    } \
    const ::scenariocpp::Scenario<fixture, parameters> name::s_ \
    = ::scenariocpp::ScenarioBuilder<fixture, parameters>().Build(#fixture, #name)

#define INSTANTIATE_SCENARIO_P(name, parameters) \
    INSTANTIATE_TEST_CASE_P(parameters, name, ::testing::ValuesIn(parameters))
