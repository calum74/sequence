#pragma once

namespace sequences
{
    namespace helpers
    {
        template<typename T>
        struct remove_all
        {
            typedef typename std::remove_reference<T>::type t1;
            typedef typename std::remove_cv<t1>::type type;
        };


        // Deduce the type of the parameter of a lambda or functor
        // See https://stackoverflow.com/questions/2562320/specializing-a-template-on-a-lambda-in-c0x
        // See https://stackoverflow.com/questions/7943525/is-it-possible-to-figure-out-the-parameter-type-and-return-type-of-a-lambda
        template<typename Fn>
        struct deduce_param : public deduce_param<decltype(&Fn::operator())>
        {
        };

        template<typename R, typename C, typename T>
        struct deduce_param<R(C::*)(T) const>
        {
            typedef typename remove_all<T>::type type;
        };

        // TODO: Other function forms

        // Another helper
        template<typename Fn>
        struct deduce_result : public deduce_result<decltype(&Fn::operator())>  // Deduce the return type of functors and lambdas
        {
        };

        template<typename R, typename C, typename...Args>
        struct deduce_result<R(C::*)(Args&&...) const>
        {
            typedef typename remove_all<R>::type type;
        };

        template<typename R, typename C, typename...Args>
        struct deduce_result<R(C::*)(Args...) const>
        {
            typedef typename remove_all<R>::type type;
        };

        template<typename R, typename...Args>
        struct deduce_result<R(Args...)>
        {
            typedef typename remove_all<R>::type type;
        };

        template<typename R, typename...Args>
        struct deduce_result<R(*)(Args...)>
        {
            typedef typename remove_all<R>::type type;
        };

        template<typename P>
        struct project_first
        {
        };

        template<typename T1, typename T2>
        struct project_first<std::pair<T1,T2>>
        {
            typedef typename remove_all<T1>::type type;
            const type &operator()(const std::pair<T1,T2> &p) const { return p.first; }
        };

        template<typename P>
        struct project_second
        {
        };

        template<typename T1, typename T2>
        struct project_second<std::pair<T1,T2>>
        {
            typedef typename remove_all<T2>::type type;
            const type &operator()(const std::pair<T1,T2> &p) const { return p.second; }
        };

    }
}