#pragma once

#include <gtest/gtest.h>

namespace scenariocpp
{

template<typename FixtureType>
struct Fixture : public ::testing::Test
{
    using ScenarioFixtureType = FixtureType;
};

template<typename FixtureType>
struct ExtendsFixture
{
    using ScenarioFixtureType = FixtureType;
};

namespace detail
{

template<typename Fixture>
struct EmptyFixture : public ::testing::Test {};

template<typename Fixture, typename Parameters>
struct ParameterisedFixture : public ::testing::TestWithParam<Parameters> {};

} /* namespace detail */

} /* namespace scenariocpp */
