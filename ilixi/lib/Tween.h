/*
 Copyright 2010-2012 Tarik Sekmen.

 All Rights Reserved.

 Written by Tarik Sekmen <tarik@ilixi.org>.

 This file is part of ilixi.

 ilixi is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 ilixi is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with ilixi.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef ILIXI_TWEEN_H_
#define ILIXI_TWEEN_H_

namespace ilixi
{

class Tween
{
    friend class TweenAnimation;

public:

    enum Transition
    {
        LINEAR,
        SINE,
        QUINT,
        QUART,
        QUAD,
        EXPO,
        ELASTIC,
        CUBIC,
        CIRCLE,
        BOUNCE,
        BACK
    };

    enum Equation
    {
        EASE_IN, EASE_OUT, EASE_IN_OUT
    };

    /*!
     * Creates a new tween.
     *
     * @param transition
     * @param equation
     * @param variable
     * @param initialValue
     * @param endValue
     */
    Tween(Transition transition, Equation equation, float initialValue = 0,
          float endValue = 1);

    /*!
     * Copy constructor.
     */
    Tween(const Tween& tween);

    /*!
     * Destructor.
     */
    virtual
    ~Tween();

    /*!
     * Returns the type of transition.
     */
    Transition
    transition() const;

    /*!
     * Returns the type of equation.
     */
    Equation
    equation() const;

    /*!
     * Returns initial value.
     */
    float
    initialValue() const;

    /*!
     * Returns end value.
     */
    float
    endValue() const;

    /*!
     * Returns current tween value.
     */
    float
    value() const;

    /*!
     * Sets the type of transition.
     */
    void
    setTransition(Transition transition);

    /*!
     * Sets the type of equation.
     */
    void
    setEquation(Equation equation);

    /*!
     * Sets the initial value of tweened variable.
     */
    void
    setInitialValue(float initialValue);

    /*!
     * Sets end value of tweened variable.
     */
    void
    setEndValue(float endValue);

private:
    //! This property stores transition.
    Transition _transition;
    //! This property stores equation.
    Equation _equation;
    //! This is the initial values of tweened variable.
    float _initialValue;
    //! This is the required change in value.
    float _change;
    //! This property stores tweened variable.
    float _value;

    //! Updates using transition and equation.
    void
    runEase(float time, float duration);
};

inline float
Tween::value() const
{
    return _value;
}

}
#endif /* ILIXI_TWEEN_H_ */
