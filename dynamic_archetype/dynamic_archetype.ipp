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




/* \/-\/ ----- \/-\/ ----- Dependencies ----- \/-\/ ----- \/-\/ */
#include <boost/preprocessor/seq/enum.hpp>

#include <boost/mpl/not.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/or.hpp>
#include <boost/mpl/list.hpp>
#include <boost/mpl/pop_front.hpp>
#include <boost/mpl/front.hpp>
#include <boost/mpl/fold.hpp>

#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/add_const.hpp>
#include <boost/type_traits/add_reference.hpp>

#include <boost/utility/enable_if.hpp>

#include <boost/config.hpp>

#include <boost/tuple/tuple.hpp>


#ifdef BOOST_NO_VARIADIC_TEMPLATES
#  error "Non-variadic template version not written yet"
#endif


/* \/-\/ ----- \/-\/ ----- Preprocessor Shorthand ----- \/-\/ ----- \/-\/ */

// Generate the enable_if argument by checking the condition named cond
// against the type sequence tested, with type as the result on success


// Shortcut for template parameters
#define TPARAMS template<class ...Concepts>	\
  template<class Policy, class Parent>


// MPL/Type Traits shorthands
#define AND  boost::mpl::and_
#define OR   boost::mpl::or_
#define NOT  boost::mpl::not_
#define SAME boost::is_same


/* \/-\/ ----- \/-\/ ----- Metaprogramming Code ----- \/-\/ ----- \/-\/ */

namespace dynamic_archetypes {


  // This struct exists primarily to get the templates out of the header
  template<class ...Concepts>
  struct dynamic_archetype<Concepts...>::impl
  {
    template<class A1, class A2> struct combine_concepts;
    template<class Policy,
	     class Parent,
	     bool has_def_const,
	     bool has_copy_constr,
	     bool copy_is_explicit>
    class constructor_gen;
    template<class... Concs> struct to_list;

    typedef typename to_list<Concepts...>::type concept_list;

    typedef typename
    boost::mpl::fold<typename boost::mpl::pop_front<concept_list>::type,
		     typename boost::mpl::front<concept_list>::type,
		     combine_concepts<boost::mpl::_1, boost::mpl::_2> >::type
    combined_concept;

    typedef
    constructor_gen<combined_concept,
		    typename combined_concept::Parent,
		    combined_concept::has_default_constructor::value,
		    combined_concept::has_copy_constructor::value,
		    combined_concept::copy_is_explicit::value>
    unspecified_type;

    template<class T>
    static
    boost::tuple<typename boost::add_reference<
		   typename boost::add_const<T>::type>::type>
    make_single_tuple(T const & elem)
    {
      return boost::tuple<typename boost::add_reference<
	typename boost::add_const<T>::type>::type>(elem);
    }
  };

  template<class ...Concepts>
  template<class T, class ...Concs>
  struct dynamic_archetype<Concepts...>::impl::to_list<T, Concs...> {
    typedef typename 
    boost::mpl::push_front<T, typename to_list<Concs...>::type >::type type;
  };

  template<class ...Concepts>
  template<class T>
  struct dynamic_archetype<Concepts...>::impl::to_list<T>{
    typedef boost::mpl::list<T> type;
  };



#define COMBINE(name, oper)					\
  typedef oper<typename A1::name, typename A2::name> name


  /*
    Given two architecture descriptions (A1 and A2), produce a type that
    represents their union.  The union is defined as the archetype such that if
    either of the two input archetypes have a constructor, their union has it as
    well.  Additionally, if one has an explicit single-parameter constructor and
    the other has a constructor with the same signature but not explicit, then
    the result is non-explicit.
  */
  template<class ...Concepts>
  template<class A1, class A2>
  struct dynamic_archetype<Concepts...>::impl::combine_concepts
  {
    COMBINE( has_default_constructor, OR  );
    COMBINE( has_copy_constructor,    OR  );
    COMBINE( copy_is_explicit,        AND );

    template<class T> struct construct_from : public
    OR<typename A1::template construct_from<T>,
       typename A2::template construct_from<T> >
    {};

    template<class T> struct explicitly_construct_from : public 
    AND<NOT<construct_from<T> >,
	OR<typename A1::template explicitly_construct_from<T>,
	   typename A2::template explicitly_construct_from<T> > >
    {};
  };


}


// Pull in the refactored constructor_gen specializations
#include "detail/constructor_gen.ipp"

#undef TPARAMS
#undef AND
#undef NOT
#undef SAME
#undef COMBINE
