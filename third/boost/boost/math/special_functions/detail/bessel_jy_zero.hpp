//  Copyright (c) 2013 Christopher Kormanyos
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// This work is based on an earlier work:
// "Algorithm 910: A Portable C++ Multiple-Precision System for Special-Function Calculations",
// in ACM TOMS, {VOL 37, ISSUE 4, (February 2011)} (C) ACM, 2011. http://doi.acm.org/10.1145/1916461.1916469
//
// This header contains implementation details for estimating the zeros
// of cylindrical Bessel and Neumann functions on the positive real axis.
// Various methods are used to estimate the roots. These include
// empirical curve fitting and McMahon's asymptotic approximation
// for small order, and uniform asymptotic expansion for large order.
//
#ifndef _BESSEL_JY_ZERO_2013_01_18_HPP_
  #define _BESSEL_JY_ZERO_2013_01_18_HPP_

  #include <algorithm>
  #include <boost/math/constants/constants.hpp>
  #include <boost/math/special_functions/math_fwd.hpp>
  #include <boost/math/special_functions/cbrt.hpp>
  #include <boost/math/special_functions/detail/airy_ai_bi_zero.hpp>

  namespace boost { namespace math {
  namespace detail
  {
    // Forward declarations of the needed Bessel function implementations.
    template <class T, class Policy>
    T cyl_bessel_j_imp(T v, T x, const bessel_no_int_tag&, const Policy& pol);
    template <class T, class Policy>
    T cyl_neumann_imp(T v, T x, const bessel_no_int_tag&, const Policy& pol);

    namespace bessel_zero
    {
      template<class T>
      T equation_nist_10_21_19(const T& v, const T& a)
      {
        // Get the initial estimate of the m'th root of Jv or Yv.
        // This subroutine is used for the order m with m > 1.
        // The order m has been used to create the input parameter a.

        // This is Eq. 10.21.19 in the NIST Handbook.
        const T mu                  = (v * v) * 4U;
        const T mu_minus_one        = mu - T(1);
        const T eight_a_inv         = T(1) / (a * 8U);
        const T eight_a_inv_squared = eight_a_inv * eight_a_inv;

        const T term3 = ((mu_minus_one *  4U) *     ((mu *    7U) -     T(31U) )) / 3U;
        const T term5 = ((mu_minus_one * 32U) *   ((((mu *   83U) -    T(982U) ) * mu) +    T(3779U) )) / 15U;
        const T term7 = ((mu_minus_one * 64U) * ((((((mu * 6949U) - T(153855UL)) * mu) + T(1585743UL)) * mu) - T(6277237UL))) / 105U;

        return a + ((((                      - term7
                       * eight_a_inv_squared - term5)
                       * eight_a_inv_squared - term3)
                       * eight_a_inv_squared - mu_minus_one)
                       * eight_a_inv);
      }

      template<typename T>
      class equation_as_9_3_39_and_its_derivative
      {
      public:
        equation_as_9_3_39_and_its_derivative(const T& zt) : zeta(zt) { }

        boost::math::tuple<T, T> operator()(const T& z) const
        {
          BOOST_MATH_STD_USING // ADL of std names, needed for acos, sqrt.

          // Return the function of zeta that is implicitly defined
          // in A&S Eq. 9.3.39 as a function of z. The function is
          // returned along with its derivative with respect to z.

          const T zsq_minus_one_sqrt = sqrt((z * z) - T(1));

          const T the_function(
              zsq_minus_one_sqrt
            - (  acos(T(1) / z) + ((T(2) / 3U) * (zeta * sqrt(zeta)))));

          const T its_derivative(zsq_minus_one_sqrt / z);

          return boost::math::tuple<T, T>(the_function, its_derivative);
        }

      private:
        equation_as_9_3_39_and_its_derivative& operator = (const equation_as_9_3_39_and_its_derivative&);
        const T zeta;
      };

      template<class T>
      static T equation_as_9_5_26(const T& v, const T& ai_bi_root)
      {
        BOOST_MATH_STD_USING // ADL of std names, needed for log, sqrt.

        // Obtain the estimate of the m'th zero of Jv or Yv.
        // The order m has been used to create the input parameter ai_bi_root.
        // Here, v is larger than about 2.2. The estimate is computed
        // from Abramowitz and Stegun Eqs. 9.5.22 and 9.5.26, page 371.
        //
        // The inversion of z as a function of zeta is mentioned in the text
        // following A&S Eq. 9.5.26. Here, we accomplish the inversion by
        // performing a Taylor expansion of Eq. 9.3.39 for large z to order 2
        // and solving the resulting quadratic equation, thereby taking
        // the positive root of the quadratic.
        // In other words: (2/3)(-zeta)^(3/2) approx = z + 1/(2z) - pi/2.
        // This leads to: z^2 - [(2/3)(-zeta)^(3/2) + pi/2]z + 1/2 = 0.
        //
        // With this initial estimate, Newton-Raphson iteration is used
        // to refine the value of the estimate of the root of z
        // as a function of zeta.

        const T v_pow_third(boost::math::cbrt(v));
        const T v_pow_minus_two_thirds(T(1) / (v_pow_third * v_pow_third));

        // Obtain zeta using the order v combined with the m'th root of
        // an airy function, as shown in  A&S Eq. 9.5.22.
        const T zeta = v_pow_minus_two_thirds * (-ai_bi_root);

        const T zeta_sqrt = sqrt(zeta);

        // Set up a quadratic equation based on the Taylor series
        // expansion mentioned above.
        const T b = -((((zeta * zeta_sqrt) * 2U) / 3U) + boost::math::constants::half_pi<T>());

        // Solve the quadratic equation, taking the positive root.
        const T z_estimate = (-b + sqrt((b * b) - T(2))) / 2U;

        // Establish the range, the digits, and the iteration limit
        // for the upcoming root-finding.
        const T range_zmin = (std::max<T>)(z_estimate - T(1), T(1));
        const T range_zmax = z_estimate + T(1);

        const int digits2_of_t = int(float(std::numeric_limits<T>::digits)
                                     * (  log(float(std::numeric_limits<T>::radix))
                                        / log(float(2))));

        const int digits2_for_root = (std::min)(digits2_of_t, std::numeric_limits<double>::digits);

        boost::uintmax_t iteration_count = boost::uintmax_t(std::numeric_limits<T>::digits10 * 2);

        // Calculate the root of z as a function of zeta.
        const T z = boost::math::tools::newton_raphson_iterate(
          boost::math::detail::bessel_zero::equation_as_9_3_39_and_its_derivative<T>(zeta),
          z_estimate,
          range_zmin,
          range_zmax,
          digits2_for_root,
          iteration_count);

        static_cast<void>(iteration_count);

        // Continue with the implementation of A&S Eq. 9.3.39.
        const T zsq_minus_one      = (z * z) - T(1);
        const T zsq_minus_one_sqrt = sqrt(zsq_minus_one);

        // This is A&S Eq. 9.3.42.
        const T b0_term_5_24 = T(5) / ((zsq_minus_one * zsq_minus_one_sqrt) * 24U);
        const T b0_term_1_8  = T(1) / ( zsq_minus_one_sqrt * 8U);
        const T b0_term_5_48 = T(5) / ((zeta * zeta) * 48U);

        const T b0 = -b0_term_5_48 + ((b0_term_5_24 + b0_term_1_8) / zeta_sqrt);

        // This is the second line of A&S Eq. 9.5.26 for f_k with k = 1.
        const T f1 = ((z * zeta_sqrt) * b0) / zsq_minus_one_sqrt;

        // This is A&S Eq. 9.5.22 expanded to k = 1 (i.e., one term in the series).
        return (v * z) + (f1 / v);
      }

      namespace cyl_bessel_j_zero_detail
      {
        template<class T>
        T equation_nist_10_21_40_a(const T& v)
        {
          const T v_pow_third(boost::math::cbrt(v));
          const T v_pow_minus_two_thirds(T(1) / (v_pow_third * v_pow_third));

          return v * (((((                         + T(0.043)
                          * v_pow_minus_two_thirds - T(0.0908))
                          * v_pow_minus_two_thirds - T(0.00397))
                          * v_pow_minus_two_thirds + T(1.033150))
                          * v_pow_minus_two_thirds + T(1.8557571))
                          * v_pow_minus_two_thirds + T(1));
        }

        template<class T>
        T initial_guess(const T& v, unsigned m)
        {
          // Compute an estimate of the m'th root of cyl_bessel_j.

          T guess;

          if(m == 0U)
          {
            // Requesting an estimate of the zero'th root is an error.
            // Return zero.
            guess = T(0);
          }
          else if(m == 1U)
          {
            // Get the initial estimate of the first root.

            if(v < T(2.2))
            {
              // For small v, use the results of empirical curve fitting.
              // Mathematica(R) session for the coefficients:
              //  Table[{n, BesselJZero[n, 1]}, {n, 0, 22/10, 1/10}]
              //  N[%, 20]
              //  Fit[%, {n^0, n^1, n^2, n^3, n^4, n^5, n^6}, n]
              guess = (((((    - T(0.0008342379046010)
                           * v + T(0.007590035637410))
                           * v - T(0.030640914772013))
                           * v + T(0.078232088020106))
                           * v - T(0.169668712590620))
                           * v + T(1.542187960073750))
                           * v + T(2.4048359915254634);
            }
            else
            {
              // For larger v, use the first line of Eqs. 10.21.40 in the NIST Handbook.
              guess = boost::math::detail::bessel_zero::cyl_bessel_j_zero_detail::equation_nist_10_21_40_a(v);
            }
          }
          else
          {
            if(v < T(2.2))
            {
              // Use Eq. 10.21.19 in the NIST Handbook.
              const T a(((v + T(m * 2U)) - T(0.5)) * boost::math::constants::half_pi<T>());

              guess = boost::math::detail::bessel_zero::equation_nist_10_21_19(v, a);
            }
            else
            {
              // Get an estimate of the m'th root of airy_ai.
              const T airy_ai_root(boost::math::detail::airy_zero::airy_ai_zero_detail::initial_guess<T>(m));

              // Use Eq. 9.5.26 in the A&S Handbook.
              guess = boost::math::detail::bessel_zero::equation_as_9_5_26(v, airy_ai_root);
            }
          }

          return guess;
        }

        template<class T, class Policy>
        class function_object
        {
        public:
          function_object(const T& v,
                          const Policy& pol) : my_v(v),
                                               my_pol(pol) { }

          boost::math::tuple<T, T> operator()(const T& x) const
          {
            const bool order_is_zero = (   (my_v > -boost::math::tools::epsilon<T>())
                                        && (my_v < +boost::math::tools::epsilon<T>()));

            // Obtain Jv(x) and Jv'(x).
            T j_v;
            T j_v_prime;

            if(order_is_zero)
            {
              j_v       =  boost::math::detail::cyl_bessel_j_imp(T(0), x, boost::math::detail::bessel_no_int_tag(), my_pol);
              j_v_prime = -boost::math::detail::cyl_bessel_j_imp(T(1), x, boost::math::detail::bessel_no_int_tag(), my_pol);
            }
            else
            {
                      j_v       = boost::math::detail::cyl_bessel_j_imp(  my_v,      x, boost::math::detail::bessel_no_int_tag(), my_pol);
              const T j_v_m1     (boost::math::detail::cyl_bessel_j_imp(T(my_v - 1), x, boost::math::detail::bessel_no_int_tag(), my_pol));
                      j_v_prime = j_v_m1 - ((my_v * j_v) / x);
            }

            // Return a tuple containing both Jv(x) and Jv'(x).
            return boost::math::make_tuple(j_v, j_v_prime);
          }

        private:
          const T my_v;
          const Policy& my_pol;
          function_object& operator = (const function_object&);
        };
      } // namespace cyl_bessel_j_zero_detail

      namespace cyl_neumann_zero_detail
      {
        template<class T>
        T equation_nist_10_21_40_b(const T& v)
        {
          const T v_pow_third(boost::math::cbrt(v));
          const T v_pow_minus_two_thirds(T(1) / (v_pow_third * v_pow_third));

          return v * (((((                         - T(0.001)
                          * v_pow_minus_two_thirds - T(0.0060))
                          * v_pow_minus_two_thirds + T(0.01198))
                          * v_pow_minus_two_thirds + T(0.260351))
                          * v_pow_minus_two_thirds + T(0.9315768))
                          * v_pow_minus_two_thirds + T(1));
        }

        template<class T>
        T initial_guess(T v, unsigned m)
        {
          // Compute an estimate of the m'th root of cyl_neumann.

          T guess;

          if(m == 0U)
          {
            // Requesting an estimate of the zero'th root is an error.
            // Return zero.
            guess = T(0);
          }
          else if(m == 1U)
          {
            // Get the initial estimate of the first root.

            if(v < T(2.2))
            {
              // For small v, use the results of empirical curve fitting.
              // Mathematica(R) session for the coefficients:
              //  Table[{n, BesselYZero[n, 1]}, {n, 0, 22/10, 1/10}]
              //  N[%, 20]
              //  Fit[%, {n^0, n^1, n^2, n^3, n^4, n^5, n^6}, n]
              guess = (((((    - T(0.0025095909235652)
                           * v + T(0.021291887049053))
                           * v - T(0.076487785486526))
                           * v + T(0.159110268115362))
                           * v - T(0.241681668765196))
                           * v + T(1.4437846310885244))
                           * v + T(0.89362115190200490);
            }
            else
            {
              // For larger v, use the second line of Eqs. 10.21.40 in the NIST Handbook.
              guess = boost::math::detail::bessel_zero::cyl_neumann_zero_detail::equation_nist_10_21_40_b(v);
            }
          }
          else
          {
            if(v < T(2.2))
            {
              // Use Eq. 10.21.19 in the NIST Handbook.
              const T a(((v + T(m * 2U)) - T(1.5)) * boost::math::constants::half_pi<T>());

              guess = boost::math::detail::bessel_zero::equation_nist_10_21_19(v, a);
            }
            else
            {
              // Get an estimate of the m'th root of airy_bi.
              const T airy_bi_root(boost::math::detail::airy_zero::airy_bi_zero_detail::initial_guess<T>(m));

              // Use Eq. 9.5.26 in the A&S Handbook.
              guess = boost::math::detail::bessel_zero::equation_as_9_5_26(v, airy_bi_root);
            }
          }

          return guess;
        }

        template<class T, class Policy>
        class function_object
        {
        public:
          function_object(const T& v,
                          const Policy& pol) : my_v(v),
                                               my_pol(pol) { }

          boost::math::tuple<T, T> operator()(const T& x) const
          {
            // Obtain Yv(x) and Yv'(x).
            const T y_v      (boost::math::detail::cyl_neumann_imp(  my_v,      x, boost::math::detail::bessel_no_int_tag(), my_pol));
            const T y_v_m1   (boost::math::detail::cyl_neumann_imp(T(my_v - 1), x, boost::math::detail::bessel_no_int_tag(), my_pol));
            const T y_v_prime(y_v_m1 - ((my_v * y_v) / x));

            // Return a tuple containing both Yv(x) and Yv'(x).
            return boost::math::make_tuple(y_v, y_v_prime);
          }

        private:
          const T my_v;
          const Policy& my_pol;
          function_object* operator = (const function_object&);
        };
      } // namespace cyl_neumann_zero_detail
    } // namespace bessel_zero
  } } } // namespace boost::math::detail

#endif // _BESSEL_JY_ZERO_2013_01_18_HPP_
