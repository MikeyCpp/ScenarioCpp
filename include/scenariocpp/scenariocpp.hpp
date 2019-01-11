/*
 * MIT License
 *
 * Copyright (c) 2018 MikeyCpp (mikeycpp@gmail.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include "internal/steps.hpp"

#define SCENARIOCPP_FUNCTION_NAME(step, name) scenariocpp_##step##_##name

#define SCENARIOCPP_DECL_STEP(step, name) \
    static ::scenariocpp::ParameterlessStepHelper<::scenariocpp::step, ScenarioFixtureType> name() \
    { \
        return \
            { #name, \
                [](ScenarioFixtureType& f){ f.SCENARIOCPP_FUNCTION_NAME(step,name)(); } \
            }; \
    } \
    void SCENARIOCPP_FUNCTION_NAME(step,name)()

#define PRECONDITION_STEP(name) \
    SCENARIOCPP_DECL_STEP(Precondition, name)

#define ACTION_STEP(name) \
    SCENARIOCPP_DECL_STEP(Action, name)

#define EXPECTED_ACTION_STEP(name) \
    SCENARIOCPP_DECL_STEP(ExpectedAction, name)

#define POSTCONDITION_STEP(name) \
    SCENARIOCPP_DECL_STEP(PostCondition, name)

#include "internal/parameterised_function.hpp"

#define SCENARIOCPP_FUNCTION_NAME_P(step, name) scenariocpp_##step##_p_##name

#define SCENARIOCPP_DECL_STEP_P(step, name, argument) \
    template<typename Parameters, typename U> \
    static ::scenariocpp::ParameterisedStepHelper<::scenariocpp::step, \
                                                  ScenarioFixtureType, \
                                                  Parameters, \
                                                  ::scenariocpp::detail::ParameterisedFunction<void(argument)>::type, \
                                                  U> \
    name(U Parameters::* arg) \
    { \
        return \
            { #name, \
              arg, \
              [](ScenarioFixtureType& aFixture, ::scenariocpp::detail::ParameterisedFunction<void(argument)>::type&& aArg) \
              { \
                  aFixture.SCENARIOCPP_FUNCTION_NAME_P(step,name)(aArg); \
              } \
            }; \
    } \
    static ::scenariocpp::ContainedParameterStepHelper<::scenariocpp::step, \
                                                       ScenarioFixtureType, \
                                                       ::scenariocpp::detail::ParameterisedFunction<void(argument)>::type \
                                                       > \
    name(::scenariocpp::detail::ParameterisedFunction<void(argument)>::type&& arg) \
    { \
        return \
            { #name, \
             std::forward<::scenariocpp::detail::ParameterisedFunction<void(argument)>::type>(arg), \
             [](ScenarioFixtureType& aFixture, ::scenariocpp::detail::ParameterisedFunction<void(argument)>::type&& aArg) \
             { \
                 aFixture.SCENARIOCPP_FUNCTION_NAME_P(step,name)(aArg); \
             } \
            }; \
    }\
    void SCENARIOCPP_FUNCTION_NAME_P(step,name)(argument)

#define PRECONDITION_STEP_P(name, argument ) \
    SCENARIOCPP_DECL_STEP_P(Precondition, name, argument)

#define ACTION_STEP_P(name, argument) \
    SCENARIOCPP_DECL_STEP_P(Action, name, argument)

#define EXPECTED_ACTION_STEP_P(name, argument) \
    SCENARIOCPP_DECL_STEP_P(ExpectedAction, name, argument)

#define POSTCONDITION_STEP_P(name, argument) \
    SCENARIOCPP_DECL_STEP_P(PostCondition, name, argument)

#include "internal/fixture.hpp"
#include "internal/scenario.hpp"
#include "internal/scenario_builder.hpp"
#include "internal/scenario_runner.hpp"

#define SCENARIOCPP_FIXTURE_NAME(fixture) scenario_##fixture
#define SCENARIOCPP_SCENARIO_CONTAINER_NAME(fixture, name) fixture##_##name

#define SCENARIO_F(fixture, name)\
    struct SCENARIOCPP_SCENARIO_CONTAINER_NAME(fixture, name) : public fixture \
    { \
        static const ::scenariocpp::NonParameterisedScenario<fixture> s_; \
    }; \
    const ::scenariocpp::NonParameterisedScenario<fixture>& name() \
    { \
        return SCENARIOCPP_SCENARIO_CONTAINER_NAME(fixture, name)::s_; \
    } \
    using SCENARIOCPP_FIXTURE_NAME(fixture) = ::scenariocpp::detail::EmptyFixture<fixture>; \
    TEST_F(SCENARIOCPP_FIXTURE_NAME(fixture), name) \
    { \
        ::scenariocpp::GTestScenarioRunner runner; \
        runner.Run(SCENARIOCPP_SCENARIO_CONTAINER_NAME(fixture, name)::s_, ::scenariocpp::NoParams{}); \
    } \
    const ::scenariocpp::NonParameterisedScenario<fixture> SCENARIOCPP_SCENARIO_CONTAINER_NAME(fixture, name)::s_ \
        = ::scenariocpp::NonParameterisedScenarioBuilder<fixture>().Build(#fixture, #name)

#define SCENARIO_P(fixture, name, parameters)\
    struct SCENARIOCPP_SCENARIO_CONTAINER_NAME(fixture, name) : public fixture \
    { \
        static const ::scenariocpp::Scenario<fixture, parameters> s_; \
    }; \
    using name = ::scenariocpp::detail::ParameterisedFixture<fixture, parameters>; \
    TEST_P(name, fixture##_##name) \
    { \
        ::scenariocpp::GTestScenarioRunner runner; \
        runner.Run(SCENARIOCPP_SCENARIO_CONTAINER_NAME(fixture, name)::s_, GetParam()); \
    } \
    const ::scenariocpp::Scenario<fixture, parameters> SCENARIOCPP_SCENARIO_CONTAINER_NAME(fixture, name)::s_ \
        = ::scenariocpp::ScenarioBuilder<fixture, parameters>().Build(#fixture, #name)

#define INSTANTIATE_SCENARIO_P(name, parameters) \
    INSTANTIATE_TEST_CASE_P(parameters, name, ::testing::ValuesIn(parameters))
