#pragma once

#include <list>
#include <functional>

namespace scenariocpp
{
namespace detail
{

template<typename T>
class ChainedForwardIterator
{
public:
    ChainedForwardIterator(std::list<std::reference_wrapper<T>> containers)
        : containers_(std::move(containers)) {}

    struct iterator
    {
        iterator()
            : end_(true)
        {}

        iterator(std::list<std::reference_wrapper<T>>& containers)
            : end_(false),
              listIter_(containers.begin()),
              endListIter_(containers.end()),
              iter_(containers.begin()->get().begin())
        {
            IncrementTillValid();
        }

        typename T::value_type& operator *()
        {
            return iter_.operator *();
        }

        typename T::value_type* operator ->()
        {
            return iter_.operator ->();
        }

        iterator& operator ++()
        {
            ++iter_;

            IncrementTillValid();

            return *this;
        }

        bool operator ==(const iterator& other) const
        {
            if (end_ != other.end_)
                return false;

            if (end_ == true)
                return true;

            return listIter_ == other.listIter_
                    && iter_ == other.iter_;
        }

        bool operator !=(const iterator& other) const
        {
            return !(*this == other);
        }

    private:
        void IncrementTillValid()
        {
            while (iter_ == listIter_->get().end())
            {
                ++listIter_;

                if (listIter_ == endListIter_)
                {
                    end_ = true;
                    break;
                }

                iter_ = listIter_->get().begin();
            }
        }

        bool end_;
        typename std::list<std::reference_wrapper<T>>::iterator listIter_;
        typename std::list<std::reference_wrapper<T>>::iterator endListIter_;
        typename T::iterator iter_;
    };

    iterator begin()
    {
        return iterator{containers_};
    }

    iterator end()
    {
        return iterator{};
    }

private:
    std::list<std::reference_wrapper<T>> containers_;
};

template<typename Container, typename ...Containers>
ChainedForwardIterator<typename std::remove_reference<Container>::type> MakeChainedForwardIterator(Container& container,
                                                                                                   Containers&... containers)
{
    return ChainedForwardIterator<typename std::remove_reference<Container>::type>({container, containers...});
}

} /* namespace detail */
} /* namespace scenariocpp */
