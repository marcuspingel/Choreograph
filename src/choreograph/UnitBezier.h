/*
 * Copyright (c) 2016 David Wicks, sansumbrella.com
 * All rights reserved.
 *
 * Bezier math is taken from Webkit's UnitBezier.
 * Copyright (C) 2008 Apple Inc. All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or
 * without modification, are permitted provided that the following
 * conditions are met:
 *
 * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once
#include <limits>

namespace choreograph {

/// UnitBezier modified from WebKit’s implementation.
/// Implicit first and last control points are (0,0) and (1,1).
class BezierInterpolant {
public:
  #ifdef CINDER_CINDER
  using vec2 = cinder::vec2;
  #else
  struct vec2 {
    vec2() = default;
    explicit vec2(float fill):
      x(fill),
      y(fill)
    {}
    explicit vec2(float x, float y):
      x(x),
      y(y)
    {}
    float x = 0.0f;
    float y = 0.0f;
  };
  #endif // ! CINDER_CINDER

  BezierInterpolant():
    _control_1(0.5, 0.0),
    _control_2(0.5, 1.0)
  {
    calculateCoefficients();
  }

  BezierInterpolant(float x1, float y1, float x2, float y2):
    _control_1(x1, y1),
    _control_2(x2, y2)
  {
    calculateCoefficients();
  }

  /// Given a value x, solve for y on the curve.
  double solve( double x, double epsilon = static_cast<double>( std::numeric_limits<float>::epsilon() * 100.0f ) ) const {
    return curveY(timeAtX( static_cast<float>( x ), static_cast<float>( epsilon )));
  }

  vec2 control1() const { return _control_1; }
  vec2 control2() const { return _control_2; }

  /// Direct manipulation of control point.
  /// You could also use an immutable style and reassign a newly-constructed bezier.
  void setControlPoints(const vec2 &control_1, const vec2 &control_2) {
    _control_1 = control_1;
    _control_2 = control_2;
    calculateCoefficients();
  }

  void setControlPoint1(const vec2 &control) { setControlPoints(control, control2()); }
  void setControlPoint2(const vec2 &control) { setControlPoints(control1(), control); }

  float curveX(double t) const {
    // at^3 + bt^2 + ct expanded using Horner's rule.
    return static_cast<float>( ((ax * t + bx) * t + cx) * t );
  }

  float curveY(double t) const {
    return ((ay * t + by) * t + cy) * t;
  }

  float derivativeX(double t) const {
    // 3at^2 + 2bt + c
    return (3 * ax * t + 2 * bx) * t + cx;
  }

  /// Given an x value, find a parameter t that generates it.
  float timeAtX(float x, float epsilon) const;

private:
  vec2 _control_1, _control_2;
  float ax, bx, cx;
  float ay, by, cy;

  void calculateCoefficients() {
    cx = 3 * _control_1.x;
    bx = 3 * (_control_2.x - _control_1.x) - cx;
    ax = 1 - cx - bx;

    cy = 3 * _control_1.y;
    by = 3 * (_control_2.y - _control_1.y) - cy;
    ay = 1 - cy - by;
  }
};

} // namespace choreograph
