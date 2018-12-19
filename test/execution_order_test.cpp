#include "scenariocpp/scenariocpp.hpp"

namespace scenariocpp
{
namespace test
{

enum class step
{
    Precondition,
    Action,
    ExpectedAction,
    Postcondition
};

std::ostream& operator <<(std::ostream& os, step aStep)
{
    switch(aStep)
    {
    case step::Precondition:
        return os << "Precondition";
    case step::Action:
        return os << "Action";
    case step::ExpectedAction:
        return os << "ExpectedAction";
    case step::Postcondition:
        return os << "Postcondition";
    }
}

class ExecutionOrderTest : public ::testing::Test,
                           public ::scenariocpp::ExtendsFixture<ExecutionOrderTest>
{
public:
    ExecutionOrderTest() = default;
    ~ExecutionOrderTest() override = default;

    PRECONDITION_STEP(ANonParameterisedPreconditionStep)
    {
        GetSteps().push_back(step::Precondition);
    }

    ACTION_STEP(ANonParameterisedActionStep)
    {
        GetSteps().push_back(step::Action);
    }

    EXPECTED_ACTION_STEP(ANonParameterisedExpectedActionStep)
    {
        GetSteps().push_back(step::ExpectedAction);
    }

    POSTCONDITION_STEP(ANonParameterisedPostconditionStep)
    {
        GetSteps().push_back(step::Postcondition);
    }

    PRECONDITION_STEP_P(AParameterisedPreconditionStep, step aStep)
    {
        GetSteps().push_back(aStep);
    }

    ACTION_STEP_P(AParameterisedActionStep, step aStep)
    {
        GetSteps().push_back(aStep);
    }

    EXPECTED_ACTION_STEP_P(AParameterisedExpectedActionStep, step aStep)
    {
        GetSteps().push_back(aStep);
    }

    POSTCONDITION_STEP_P(AParameterisedPostconditionStep, step aStep)
    {
        GetSteps().push_back(aStep);
    }

    static std::vector<step>& GetSteps()
    {
        static std::vector<step> steps;
        return steps;
    }
};

SCENARIO_F ( ExecutionOrderTest, ANonParameterisedScenario )
    .Given ( ANonParameterisedPreconditionStep() )
    .When  ( ANonParameterisedActionStep() )
    .Then  ( ANonParameterisedPostconditionStep() )
    .And   ( ANonParameterisedExpectedActionStep() );

TEST_F ( Scenario_ExecutionOrderTest, CheckANonParameterisedScenario )
{
    const auto& steps = ExecutionOrderTest::GetSteps();

    EXPECT_EQ(steps.size(), 4);

    EXPECT_EQ(steps.at(0), step::Precondition);
    EXPECT_EQ(steps.at(1), step::ExpectedAction);
    EXPECT_EQ(steps.at(2), step::Action);
    EXPECT_EQ(steps.at(3), step::Postcondition);
}

TEST_F(Scenario_ExecutionOrderTest, DummyClearSteps)
{
    auto& steps = ExecutionOrderTest::GetSteps();

    steps.clear();
    EXPECT_TRUE(steps.empty());
}

SCENARIO_F ( ExecutionOrderTest, ParameterisedScenario )
    .Given ( AParameterisedPreconditionStep (step::Precondition) )
    .When  ( AParameterisedActionStep (step::Action) )
    .Then  ( AParameterisedPostconditionStep (step::Postcondition) )
    .And   ( AParameterisedExpectedActionStep (step::ExpectedAction) );

TEST_F ( Scenario_ExecutionOrderTest, CheckParameterisedScenario )
{
    const auto& steps = ExecutionOrderTest::GetSteps();

    EXPECT_EQ(steps.size(), 4);

    EXPECT_EQ(steps.at(0), step::Precondition);
    EXPECT_EQ(steps.at(1), step::ExpectedAction);
    EXPECT_EQ(steps.at(2), step::Action);
    EXPECT_EQ(steps.at(3), step::Postcondition);
}

TEST_F ( Scenario_ExecutionOrderTest, DummyClearSteps2 )
{
    auto& steps = ExecutionOrderTest::GetSteps();

    steps.clear();
    EXPECT_TRUE(steps.empty());
}

SCENARIO_F ( ExecutionOrderTest, MixedParameterisedScenario )
    .Given ( AParameterisedPreconditionStep (step::Precondition) )
    .And   ( ANonParameterisedPreconditionStep () )
    .When  ( AParameterisedActionStep (step::Action) )
    .And   ( ANonParameterisedActionStep() )
    .Then  ( AParameterisedPostconditionStep (step::Postcondition) )
    .And   ( ANonParameterisedPostconditionStep() )
    .And   ( AParameterisedExpectedActionStep (step::ExpectedAction) )
    .And   ( ANonParameterisedExpectedActionStep() );

TEST_F ( Scenario_ExecutionOrderTest, CheckMixedParameterisedScenario )
{
    const auto& steps = ExecutionOrderTest::GetSteps();

    EXPECT_EQ(steps.size(), 8);

    EXPECT_EQ(steps.at(0), step::Precondition);
    EXPECT_EQ(steps.at(1), step::Precondition);
    EXPECT_EQ(steps.at(2), step::ExpectedAction);
    EXPECT_EQ(steps.at(3), step::ExpectedAction);
    EXPECT_EQ(steps.at(4), step::Action);
    EXPECT_EQ(steps.at(5), step::Action);
    EXPECT_EQ(steps.at(6), step::Postcondition);
    EXPECT_EQ(steps.at(7), step::Postcondition);
}

TEST_F ( Scenario_ExecutionOrderTest, DummyClearSteps3 )
{
    auto& steps = ExecutionOrderTest::GetSteps();

    steps.clear();
    EXPECT_TRUE(steps.empty());
}

} /* namespace test */
} /* namespace scenariocpp */
