#ifndef ACTIVATION_H_INCLUDED
#define ACTIVATION_H_INCLUDED

#include <functional>
#include "Layer.h"

namespace RTNeural
{

template<typename T>
class Activation : public Layer<T>
{
public:
    Activation (size_t size, std::function<T(T)> func) :
        Layer<T> (size, size),
        func (func)
    {}

    virtual ~Activation() {}

    inline void forward (const T* input, T* out) override
    {
        for (size_t i = 0; i < Layer<T>::out_size; ++i)
            out[i] = func (input[i]);
    }

private:
    const std::function<T(T)> func;
};

} // namespace RTNeural

#ifdef USE_EIGEN
#include <Eigen/Dense>

namespace RTNeural
{

template<typename T>
class TanhActivation : public Activation<T>
{
public:
    TanhActivation (size_t size) :
        Activation<T> (size, {})
    {
        inVec.resize (size, 1);
        outVec.resize (size, 1);
    }

    inline void forward (const T* input, T* out) override
    {
        inVec = Eigen::Map<const Eigen::Matrix<T, Eigen::Dynamic, 1>> (input, Layer<T>::in_size, 1);
        outVec = inVec.array().tanh();

        std::copy (outVec.data(), outVec.data() + Layer<T>::in_size, out);
    }

    Eigen::Matrix<T, Eigen::Dynamic, 1> inVec;
    Eigen::Matrix<T, Eigen::Dynamic, 1> outVec;
};

template<typename T>
class ReLuActivation : public Activation<T>
{
public:
    ReLuActivation (size_t size) :
        Activation<T> (size, {})
    {
        inVec.resize (size, 1);
        outVec.resize (size, 1);
    }

    inline void forward (const T* input, T* out) override
    {
        inVec = Eigen::Map<const Eigen::Matrix<T, Eigen::Dynamic, 1>> (input, Layer<T>::in_size, 1);
        outVec = inVec.array().max ((T) 0);

        std::copy (outVec.data(), outVec.data() + Layer<T>::in_size, out);
    }

    Eigen::Matrix<T, Eigen::Dynamic, 1> inVec;
    Eigen::Matrix<T, Eigen::Dynamic, 1> outVec;
};

} // namespace RTNeural

#else
#include <cmath>

namespace RTNeural
{

template<typename T>
class TanhActivation : public Activation<T>
{
public:
    TanhActivation (size_t size) :
        Activation<T> (size, [] (T x) { return std::tanh (x); })
    {}
};

template<typename T>
class ReLuActivation : public Activation<T>
{
public:
    ReLuActivation (size_t size) :
        Activation<T> (size, [] (T x) { return std::max ((T) 0, x); })
    {}
};

} // namespace RTNeural

#endif // USE_EIGEN

#endif // ACTIVATION_H_INCLUDED