#pragma once

namespace helpers
{
    // Deduce the type of the parameter of a lambda or functor
    // See https://stackoverflow.com/questions/2562320/specializing-a-template-on-a-lambda-in-c0x
    // See https://stackoverflow.com/questions/7943525/is-it-possible-to-figure-out-the-parameter-type-and-return-type-of-a-lambda
    template<typename Fn>
    struct deduce_param : public deduce_param<decltype(&Fn::operator())>
    {
    };

    template<typename C, typename T>
    struct deduce_param<void(C::*)(T) const>
    {
        typedef T type;
    };

    template<typename C, typename T>
    struct deduce_param<void(C::*)(const T&) const>
    {
        typedef T type;
    };

    // TODO: Other function forms

    // Another helper
    template<typename Fn>
    struct deduce_result : public deduce_result<decltype(&Fn::operator())>
    {
    };

    template<typename R, typename C, typename...Args>
    struct deduce_result<R(C::*)(Args&&...) const>
    {
        typedef R type;
    };

    template<typename R, typename C, typename...Args>
    struct deduce_result<R(C::*)(Args...) const>
    {
        typedef R type;
    };


}