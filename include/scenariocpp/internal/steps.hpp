#pragma once

#include "stringutils.hpp"
#include "parameter_printer.hpp"
#include "result.hpp"

#include <string>
#include <functional>
#include <sstream>

namespace scenariocpp
{

template<typename Fixture, typename Parameters>
struct Step
{
    Step(std::string step,
         std::string keyword)
        : step_(std::move(step)),
          keyword_(std::move(keyword))
    {}

    virtual ~Step() = default;

    virtual std::string GetName() const =0;
    virtual std::string GetDescription(const Parameters&) const =0;

    virtual void Execute(Fixture&, const Parameters&) =0;

    std::string GetStep() const { return step_; }
    std::string GetKeyword() const { return keyword_; }

private:
    std::string step_;
    std::string keyword_;
};

template<typename Keyword, typename Fixture>
struct ParameterlessStepHelper
{
    std::string name_;
    std::function<void(Fixture&)> func_;
};

template<typename Fixture, typename Parameters>
struct ParameterlessStep : public Step<Fixture, Parameters>
{
    template<typename Keyword>
    ParameterlessStep(std::string step,
                      std::string keyword,
                      ParameterlessStepHelper<Keyword, Fixture>&& helper)
        : Step<Fixture, Parameters> (std::move(step), std::move(keyword)),
          name_(std::move(helper.name_)),
          func_(std::move(helper.func_))
    {}

    ~ParameterlessStep() override = default;

    std::string GetName() const override { return name_ ;}

    std::string GetDescription(const Parameters&) const override
    {
        std::stringstream ss;
        ss << Step<Fixture, Parameters>::GetStep()
           << " "
           << stringutils::Humanise(name_);

        return ss.str();
    }

    void Execute(Fixture& aFixture, const Parameters&) override { func_(aFixture); }

private:
    std::string name_;
    std::function<void(Fixture&)> func_;
};

template<typename Keyword, typename Fixture, typename T>
struct ContainedParameterStepHelper
{
    std::string name_;
    T param_;
    std::function<void(Fixture&, T)> func_;
};

template<typename Fixture, typename Parameters, typename T>
struct ContainedParameterStep : public Step<Fixture, Parameters>
{
    template<typename Keyword>
    ContainedParameterStep(std::string step,
                           std::string keyword,
                           ContainedParameterStepHelper<Keyword, Fixture,T>&& helper)
        : Step<Fixture, Parameters> (std::move(step), std::move(keyword)),
          name_(std::move(helper.name_)),
          param_(std::move(helper.param_)),
          func_(std::move(helper.func_))
    {}

    ~ContainedParameterStep() override = default;

    std::string GetName() const override { return name_ ;}

    std::string GetDescription(const Parameters&) const override
    {
        std::stringstream ss;
        ss << Step<Fixture, Parameters>::GetStep()
           << " "
           << stringutils::Humanise(name_)
           << " [";
           PrintParam(ss, param_);
           ss << "]";
        return ss.str();
    }

    void Execute(Fixture& aFixture, const Parameters&) override
    {
        func_(aFixture, param_);
    }

private:
    std::string name_;
    T param_;
    std::function<void(Fixture&, T)> func_;
};

template<typename Keyword, typename Fixture, typename Parameters, typename T, typename U>
struct ParameterisedStepHelper
{
    std::string name_;
    U Parameters::* Tp_;
    std::function<void(Fixture&, T)> func_;
};

template<typename Fixture, typename Parameters, typename T, typename U>
struct ParameterisedStep : public Step<Fixture, Parameters>
{
    template<typename Keyword>
    ParameterisedStep(std::string step,
                      std::string keyword,
                      ParameterisedStepHelper<Keyword, Fixture, Parameters, T, U>&& helper)
        : Step<Fixture, Parameters> (std::move(step), std::move(keyword)),
          name_(std::move(helper.name_)),
          Tp_(std::move(helper.Tp_)),
          func_(std::move(helper.func_))
    {}

    ~ParameterisedStep() override = default;

    std::string GetName() const override { return name_ ;}

    std::string GetDescription(const Parameters& aParameters) const override
    {
        std::stringstream ss;
        ss << Step<Fixture, Parameters>::GetStep()
           << " "
           << stringutils::Humanise(name_)
           << " [";
           PrintParam(ss, aParameters.*(Tp_));
           ss << "]";
        return ss.str();
    }

    void Execute(Fixture& aFixture, const Parameters& aParameters) override
    {
        func_(aFixture, aParameters.*(Tp_));
    }

private:
    std::string name_;
    U Parameters::* Tp_;
    std::function<void(Fixture&, T)> func_;
};

/* step keywords */
struct Precondition {};
struct Action {};
struct ExpectedAction {};
struct PostCondition {};

} /* namespace scenariocpp */

#undef DECL_STEP_CLASS
