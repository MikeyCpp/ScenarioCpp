#include "scenariocpp.hpp"

#include <gtest/gtest.h>

namespace test
{

class TestStrings : public ::scenariocpp::Fixture<TestStrings>
{
public:
    TestStrings() = default;
    ~TestStrings() override = default;

    PRECONDITION_STEP_P(AStringWithInitialValue, std::string initial)
    {
        str_ = initial;
    }

    ACTION_STEP_P(TheStringIsAppendedWith, std::string appended)
    {
        str_ += appended;
    }

    POSTCONDITION_STEP_P(TheStringNowEquals, std::string expected)
    {
        EXPECT_EQ(str_, expected);
    }

    std::string str_;
};

SCENARIO_F ( TestStrings, AValueIsAppendedToAString )
    .Given  ( AStringWithInitialValue("Hello") )
    .When   ( TheStringIsAppendedWith("World") )
    .Then   ( TheStringNowEquals("HelloWorld") );

struct Params
{
    const char* initalValue_;
    std::string appendedValue_;
    std::string expectedValue_;
};

SCENARIO_P ( TestStrings, AValueIsAppendedToAnotherString, Params )
    .Given  ( AStringWithInitialValue(&Params::initalValue_) )
    .When   ( TheStringIsAppendedWith(&Params::appendedValue_) )
    .Then   ( TheStringNowEquals(&Params::expectedValue_) );

std::vector<Params> MyTestParams =
{
    { "Hello" , "World", "HelloWorld" },
    { "Flex" , "Trade", "FlexTrade" }
};

INSTANTIATE_SCENARIO_P(AValueIsAppendedToAnotherString, MyTestParams);

} /* namespace test */
