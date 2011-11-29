#ifndef DYNAMIC_ARCHETYPES_DEFAULT_CONSTRUCTIBLE_HPP_INCLUDED_
#define DYNAMIC_ARCHETYPES_DEFAULT_CONSTRUCTIBLE_HPP_INCLUDED_

/*
   This file is copyright (c) Patrick Moran 2011.  A license is granted to any
   party to use this file according to the terms of the Boost Software License
   version 1 as it appears below:


   Boost Software License - Version 1.0 - August 17th, 2003

   Permission is hereby granted, free of charge, to any person or organization
   obtaining a copy of the software and accompanying documentation covered by
   this license (the "Software") to use, reproduce, display, distribute,
   execute, and transmit the Software, and to prepare derivative works of the
   Software, and to permit third-parties to whom the Software is furnished to
   do so, all subject to the following:

   The copyright notices in the Software and this entire statement, including
   the above license grant, this restriction and the following disclaimer,
   must be included in all copies of the Software, in whole or in part, and
   all derivative works of the Software, unless such copies or derivative
   works are solely in the form of machine-executable object code generated by
   a source language processor.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
   SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
   FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
   DEALINGS IN THE SOFTWARE.
*/

#include "object_data.hpp"

namespace dynamic_archetypes {
  
  /// @brief A dynamic archetype of DefaultConstructible
  struct default_constructible {
    default_constructible();

  private:
    // DefaultConstructible does not mention the address-of operator, so we must
    // suppress it.
    struct Incomplete_Type;
    Incomplete_Type operator&() const;
    ~default_constructible();
  };

  /// @brief Test two objects for equivalence
  template<>
  bool object_data<default_constructible>::equivalent
  (default_constructible const &, default_constructible const &);
}

#endif
