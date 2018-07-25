/*
 * Copyright (c) 2013-2015, Roland Bock
 * Copyright (c) 2018, Coloridad Ltd.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *   Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 *   Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef SQLPP11_DATE_FUNCTIONS_MONTH_H
#define SQLPP11_DATE_FUNCTIONS_MONTH_H

#include <sqlpp11/char_sequence.h>
#include <sqlpp11/data_types/integral/data_type.h>
#include <sqlpp11/alias_operators.h>
#include <sqlpp11/serialize.h>
#include <sqlpp11/wrap_operand.h>
#include <sqlpp11/data_types/integral/expression_operators.h>

namespace sqlpp
{
  struct day_alias_t
  {
    struct _alias_t
    {
      static constexpr const char _literal[] = "day_";
      using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;

      template <typename T>
      struct _member_t
      {
        T day;
        T& operator()()
        {
          return day;
        }

        const T& operator()() const
        {
          return day;
        }
      };
    };
  };

  template <typename Expr>
  struct day_t : public expression_operators<day_t<Expr>, integral>,
                   public alias_operators<day_t<Expr>>
  {
    using _traits = make_traits<integral, tag::is_expression>;

    using _nodes = detail::type_vector<Expr, date_function>;
    using _can_be_null = std::true_type;

    using _auto_alias_t = day_alias_t;

    day_t(const Expr expr) : _expr(expr)
    {
    }

    day_t(const day_t&) = default;
    day_t(day_t&&) = default;
    day_t& operator=(const day_t&) = default;
    day_t& operator=(day_t&&) = default;
    ~day_t() = default;

    Expr _expr;
  };

  template <typename Context, typename Expr>
  struct serializer_t<Context, day_t<Expr>>
  {
    using _serialize_check = consistent_t;
    using T = day_t<Expr>;

    static Context& _(const T& t, Context& context)
    {
      context << "DAY(";
      sqlpp::serialize(t._expr, context);
      context << ")";

      return context;
    }
  };

  template <typename T>
  auto day(T t) -> day_t<T>
  {
    static_assert(is_expression_t<wrap_operand_t<T>>::value, "day() requires an expression as argument");
    static_assert(is_column_t<wrap_operand_t<T>>::value, "day() requires a column as argument");
    static_assert(is_day_point_t<wrap_operand_t<T>>::value
                      or is_time_of_day_t<wrap_operand_t<T>>::value
                      or is_time_point_t<wrap_operand_t<T>>::value, "day() requires date or datetime value type as argument");
    return {t};
  }
}
#endif // SQLPP11_DATE_FUNCTIONS_MONTH_H
