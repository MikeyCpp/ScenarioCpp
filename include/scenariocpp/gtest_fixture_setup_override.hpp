#pragma once

namespace scenariocpp
{

template<typename Fixture>
class GTestFixtureSetupOverride : public Fixture
{
public:
    void SetUp() override {}
    void TearDown() override {}
};

} /* namespace scenariocpp */
