#pragma once

#include "stringutils.hpp"
#include "parameter_printer.hpp"
#include "result.hpp"

#include <string>
#include <functional>
#include <sstream>

namespace scenariocpp
{

enum class Step
{
    Given,
    AndGiven,
    When,
    AndWhen,
    Then,
    AndThen,
};

enum class Keyword
{
    Precondition,
    Action,
    ExpectedAction,
    PostCondition
};

template<typename Fixture>
struct StepExecutor
{
    StepExecutor(
            Step step,
            Keyword keyword)
        : step_(step),
          keyword_(keyword)
    {}

    virtual ~StepExecutor() = default;

    virtual void Execute(Fixture& aFixture) = default;

    Step GetStep() const
    {
        return step_;
    }

    Keyword GetKeyword() const
    {
        return keyword_;
    }

private:
    Step step_;
    Keyword keyword_;
};

template<typename Fixture, Keyword keyword>
struct NoArgumentsStepHelper
{
    std::string name_;
    std::function<void(Fixture&)> fixtureStep_;
};

template<typename Fixture, typename ArgumentType, Keyword keyword>
struct SingleArgumentStepHelper
{
    std::string name_;
    ArgumentType argument_;
    std::function<void(Fixture&, ArgumentType)> fixtureStep_;
};

template<typename Fixture>
struct NoArgumentsStepExecutor : public StepExecutor<Fixture>
{
    template<Keyword keyword>
    NoArgumentsStepExecutor(
            Step aStep,
            NoArgumentsStepHelper<Fixture, keyword> aHelper)
        : StepExecutor<Fixture>(aStep, keyword),
          name_(aHelper.name_),
          fixtureStep_(aHelper.fixtureStep_)
    {}

    ~NoArgumentsStepExecutor() override = default;

    void Execute(Fixture& fixture) final
    {
        fixtureStep_(fixture);
    }

private:
    std::string name_;
    std::function<void(Fixture&)> fixtureStep_;
};

template<typename Fixture, typename ArgumentType>
struct SingleArgumentStepExecutor : public StepExecutor<Fixture>
{
    template<Keyword keyword>
    SingleArgumentStepExecutor
    (
            Step aStep,
            SingleArgumentStepHelper<Fixture, ArgumentType, keyword> aHelper)
        : StepExecutor<Fixture>(aStep, keyword),
          name_(aHelper.name),
          argument_(std::move(aHelper.argument_)),
          fixtureStep_(aHelper.fixtureStep_)
    {}

    ~SingleArgumentStepExecutor() override = default;

    void Execute(Fixture& aFixture) final
    {
        fixtureStep_(aFixture, argument_);
    }

private:
    std::string name_;
    ArgumentType argument_;
    std::function<void(Fixture&, ArgumentType)> fixtureStep_;
};

template<typename Fixture, typename ArgumentType, typename Parameters, typename ParameterType, Keyword keyword>
struct ParameterisedStepHelper
{
    std::string name_;
    ParameterType Parameters::* parameterPointer_;
    std::function<void(Fixture&, ArgumentType)> fixtureStep_;
};

template<typename Fixture, typename Parameters>
struct ParameterisedStepExecutor
{
    ParameterisedStepExecutor(
            Step step,
            Keyword keyword)
        : step_(step),
          keyword_(keyword)
    {}

    virtual ~ParameterisedStepExecutor() = default;

    virtual void Execute(Fixture& aFixture, const Parameters& aParameters) =0;

    Step GetStep() const
    {
        return step_;
    }

    Keyword GetKeyword() const
    {
        return keyword_;
    }

private:
    Step step_;
    Keyword keyword_;
};

template<typename Fixture, typename Parameters>
struct NonParameterisedStepExecutorWrapper : public ParameterisedStepExecutor<Fixture, Parameters>
{
    NonParameterisedStepExecutorWrapper(std::unique_ptr<StepExecutor<Fixture>> step)
        : step_(std::move(step))
    {}

    ~NonParameterisedStepExecutorWrapper() override = default;

    void Execute(Fixture& aFixture, const Parameters&) final
    {
        step_->Execute(aFixture);
    }

private:
    std::unique_ptr<StepExecutor<Fixture>> step_;
};

template<typename Fixture, typename ArgumentType, typename Parameters, typename ParameterType>
struct SingleArgumentParameterisedStepExecutor : public ParameterisedStepExecutor<Fixture, Parameters>
{
    template<Keyword keyword>
    SingleArgumentParameterisedStepExecutor
    (
            Step step,
            ParameterisedStepHelper<Fixture, ArgumentType, Parameters, ParameterType, keyword> helper)
        : ParameterisedStepExecutor<Fixture, Parameters>(step, keyword),
          name_(std::move(helper.name_)),
          parameterPointer_(helper.parameterPointer_),
          fixtureStep_(helper.fixtureStep_)
    {}

    ~SingleArgumentParameterisedStepExecutor() override = default;

    void Execute(Fixture &aFixture, const Parameters &aParameters) override
    {
        fixtureStep(aFixture, aParameters.*(parameterPointer_));
    }

private:
    std::string name_;
    ParameterType Parameters::* parameterPointer_;
    std::function<void(Fixture&, ArgumentType)> fixtureStep_;
};

//template<typename Fixture, typename Parameters>
//struct Step
//{
//    Step(std::string step,
//         std::string keyword)
//        : step_(std::move(step)),
//          keyword_(std::move(keyword))
//    {}

//    virtual ~Step() = default;

//    virtual std::string GetName() const =0;
//    virtual std::string GetDescription(const Parameters&) const =0;

//    virtual void Execute(Fixture&, const Parameters&) =0;

//    std::string GetStep() const { return step_; }
//    std::string GetKeyword() const { return keyword_; }

//private:
//    std::string step_;
//    std::string keyword_;
//};

//template<typename Keyword, typename Fixture>
//struct ParameterlessStepHelper
//{
//    std::string name_;
//    std::function<void(Fixture&)> func_;
//};

//template<typename Fixture, typename Parameters>
//struct ParameterlessStep : public Step<Fixture, Parameters>
//{
//    template<typename Keyword>
//    ParameterlessStep(std::string step,
//                      std::string keyword,
//                      ParameterlessStepHelper<Keyword, Fixture>&& helper)
//        : Step<Fixture, Parameters> (std::move(step), std::move(keyword)),
//          name_(std::move(helper.name_)),
//          func_(std::move(helper.func_))
//    {}

//    ~ParameterlessStep() override = default;

//    std::string GetName() const override { return name_ ;}

//    std::string GetDescription(const Parameters&) const override
//    {
//        std::stringstream ss;
//        ss << Step<Fixture, Parameters>::GetStep()
//           << " "
//           << stringutils::Humanise(name_);

//        return ss.str();
//    }

//    void Execute(Fixture& aFixture, const Parameters&) override { func_(aFixture); }

//private:
//    std::string name_;
//    std::function<void(Fixture&)> func_;
//};

//template<typename Keyword, typename Fixture, typename T>
//struct ContainedParameterStepHelper
//{
//    std::string name_;
//    T param_;
//    std::function<void(Fixture&, T)> func_;
//};

//template<typename Fixture, typename Parameters, typename T>
//struct ContainedParameterStep : public Step<Fixture, Parameters>
//{
//    template<typename Keyword>
//    ContainedParameterStep(std::string step,
//                           std::string keyword,
//                           ContainedParameterStepHelper<Keyword, Fixture,T>&& helper)
//        : Step<Fixture, Parameters> (std::move(step), std::move(keyword)),
//          name_(std::move(helper.name_)),
//          param_(std::move(helper.param_)),
//          func_(std::move(helper.func_))
//    {}

//    ~ContainedParameterStep() override = default;

//    std::string GetName() const override { return name_ ;}

//    std::string GetDescription(const Parameters&) const override
//    {
//        std::stringstream ss;
//        ss << Step<Fixture, Parameters>::GetStep()
//           << " "
//           << stringutils::Humanise(name_)
//           << " [";
//           PrintParam(ss, param_);
//           ss << "]";
//        return ss.str();
//    }

//    void Execute(Fixture& aFixture, const Parameters&) override
//    {
//        func_(aFixture, param_);
//    }

//private:
//    std::string name_;
//    T param_;
//    std::function<void(Fixture&, T)> func_;
//};

//template<typename Keyword, typename Fixture, typename Parameters, typename T, typename U>
//struct ParameterisedStepHelper
//{
//    std::string name_;
//    U Parameters::* Tp_;
//    std::function<void(Fixture&, T)> func_;
//};

//template<typename Fixture, typename Parameters, typename T, typename U>
//struct ParameterisedStep : public Step<Fixture, Parameters>
//{
//    template<typename Keyword>
//    ParameterisedStep(std::string step,
//                      std::string keyword,
//                      ParameterisedStepHelper<Keyword, Fixture, Parameters, T, U>&& helper)
//        : Step<Fixture, Parameters> (std::move(step), std::move(keyword)),
//          name_(std::move(helper.name_)),
//          Tp_(std::move(helper.Tp_)),
//          func_(std::move(helper.func_))
//    {}

//    ~ParameterisedStep() override = default;

//    std::string GetName() const override { return name_ ;}

//    std::string GetDescription(const Parameters& aParameters) const override
//    {
//        std::stringstream ss;
//        ss << Step<Fixture, Parameters>::GetStep()
//           << " "
//           << stringutils::Humanise(name_)
//           << " [";
//           PrintParam(ss, aParameters.*(Tp_));
//           ss << "]";
//        return ss.str();
//    }

//    void Execute(Fixture& aFixture, const Parameters& aParameters) override
//    {
//        func_(aFixture, aParameters.*(Tp_));
//    }

//private:
//    std::string name_;
//    U Parameters::* Tp_;
//    std::function<void(Fixture&, T)> func_;
//};

///* step keywords */
//struct Precondition {};
//struct Action {};
//struct ExpectedAction {};
//struct PostCondition {};

} /* namespace scenariocpp */

#undef DECL_STEP_CLASS
