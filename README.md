# ScenarioCPP
ScenarioCPP is a Behaviour Driven Development (BDD) extension to googletest for rapid development of extensible and human readable BDD tests

# Scenarios Fixtures
A **scenario fixture** contains the configuration of objects you're testing and definitions of the **steps** that can be performed in your scenario.

To start creating a scenario fixture, derive your fixture class from `::scenariocpp::Fixture<[Derived]>`

```cpp
class MyTest : public ::scenariocpp::Fixture<MyTest>,
{ ...
}
```

# Steps
Use the `_STEP` macros to name and define steps in your scenario fixture

| Step | macro | description |
|--|--| -- |
| Precondition | `PRECONDITION_STEP(name)` | An initial condition before the test is ran |
| Action | `ACTION_STEP(name)` | An action to be taken by the test |
| Postcondition | ` POSTCONDITION_STEP(name)` | An expected condition after the test has finished |
| Expected Action | `EXPECTED_ACTION_STEP(name)` | An action expected to be taken by the system under test, use this step instead of *Postcondition* when you want to set an `EXPECT_CALL()` |

```cpp
PRECONDITION_STEP (PreconditionName) {
 ... setup initial condition ...
}
```
# Paramertised Steps
Paramertised steps take an argument, and are defined using the `_STEP_P` macros

```cpp
PRECONDITION_STEP_P (PreconditionName, Arg aArg) {
 ... setup initial condition with aArg...
}
```

# Creating A Scenario
Use the `SCENARIO_F` macro to name and declare a scenario outside of the fixture

```cpp
SCENARIO_F ( ScenarioFixture, ScenarioName )
    ...
```

Define the steps of your scenario using `.` to access scenario **keywords**

| Keyword | Accepted Step |
| --- | --- |
| `Given` | *Precondition* |
| `When` | *Action* |
| `Then` | *Expected Action*, *Postcondition* |
| *`And`* | ( behaves the same as the previously accessed keyword ) |

```cpp
SCENARIO_F ( ScenarioFixture, ScenarioName )
    .Given ( PreconditionName() )
    .When  ( ActionName () )
    .And   ( AnotherActionName() )
    .Then  ( PostconditionName() );
```

Keywords are only accessable in the order Given -> When -> Then, and only accept a corresponding step

# Running a scenario
Scenarios are ran under the google testing framework, when a scenario is defined a google test is created and so may be filtered like regular google tests

Steps defined by the scenario are executed in the order
Preconditions -> Expected Actions -> Actions -> Postconditions

example:
```c++
class TestStrings : public scenariocpp::Fixture<MyTest>
{
public:
    PRECONDITION_STEP_P(AStringWithValue, std::string initialValue) {
        str_ = initialValue;
    }

    ACTION_STEP_P(TheStringIsAppendedWith, std::string appendedValue) {
        str_ += appendedValue;
    }

    POSTCONDITION_STEP_P(TheStringNowEquals, std::string expectedValue) {
        EXEPCT_EQ(str_, expectedValue);
    }
    std::string str_;
};

SCENARIO_F ( TestStrings, AValueIsAppendedToAString )
    .Given ( AStringWithInitialValue("Hello") )
    .When  ( TheStringIsAppendedWith("World") )
    .Then  ( TheStringNowEquals("HelloWorld") );
```

output:
```
[ RUN      ] Scenario_TestStrings.AValueIsAppendedToAString
[ BEGIN SCENARIO  ] TestStrings.AValueIsAppendedToAString
Scenario: A value is appended to a string
  Given a string with initial value ["Hello"]
  When the string is appended with ["World"]
  Then the string now equals ["HelloWorld"]
[-----------------] Set-up test fixture
[-----------------] Execute scenario
[ PRECONDITION    ] TestStrings.AStringWithInitialValue
[ ACTION          ] TestStrings.TheStringIsAppendedWith
[ POST CONDITION  ] TestStrings.TheStringNowEquals
[-----------------] Checking expected actions were fulfilled
[ SCENARIO PASSED ] AValueIsAppendedToAString
[       OK ] Scenario_TestStrings.AValueIsAppendedToAString (1 ms)
```

# Parameterised Scenarios
Paramerised Scenarios allow you to run the same scenario with different parameters for each step

Create a struct declaring the **step** arguments to be parameterised
Use the `SCENARIO_P` macro to name and declare a parameterised scenario
Pass the parameter to the step argument uby giving the address of the argument

```cpp
struct Params
{
    std::string initialValue_;
    std::string appendedValue_;
    std::string expectedValue_;
}
SCENARIO_P ( TestStrings, AValueIsAppendedToAString, Params )
    .Given ( AStringWithInitialValue(&Params::initialValue_) )
    .When  ( TheStringIsAppendedWith(&Params::appendedValue_) )
    .Then  ( TheStringNowEquals(&Params::expectedValue_) );
```

*note: parameterised steps may still take regular arguments*

To create your parameterised test, declare and initialise an iterable container of the parameters for the test.

Instantiate the test using the `INSTANTIATE_SCENARIO_P` macro, passing the name of the scenario and the parameter container

```cpp
const std::vector<Params> MyTestParams =
{
    { "Hello" , "World", "HelloWorld" },
    { "Foo" , "Bar", "FooBar" }
};

INSTANTIATE_SCENARIO_P (AValueIsAppendedToAString, MyTestParams);
```

# Scenario precondition
A non-parameterised scenario can be used as a precondition in scenario iff the scenario succeeded

```cpp
SCENARIO_F ( ScenarioFixture, FirstScenario )
    .Given ( PreconditionName() )
    .When  ( ActionName() )
    .Then  ( PostconditionName() );

SCENARIO_F ( ScenarioFixture, NextScenario )
    .Given ( FirstScenario() )
    .When  ( ActionName() )
    .Then  ( PostconditionName() );
```

# Parameter printers
By default step parameters in the scenario description will be printed using the function `std::ostream& operator<<(std::ostream& os, const ParameterType& t)`

Define your own printer by implimenting a `PrintParam` function in the `scenariocpp` namespace
```cpp
namespace scenariocpp
{
inline std::ostream& PrintParam(std::ostream& os, const ParameterType& t)
{
    ...
    return os;
}
} /* namespace scenariocpp */
```
