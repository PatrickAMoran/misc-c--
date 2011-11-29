#define BOOST_TEST_MODULE gap_buffer
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <boost/concept_check.hpp>

#include "gap_buffer.hpp"

#include <deque>
#include <list>
#include <vector>

// First, our static assertions
struct Concept_Checks
{
  typedef gap_buffer<std::deque<char> > random_access_t;
  BOOST_CONCEPT_ASSERT(( boost::Container<              random_access_t >));
  BOOST_CONCEPT_ASSERT(( boost::ForwardContainer<       random_access_t >));
  BOOST_CONCEPT_ASSERT(( boost::DefaultConstructible<   random_access_t >));
  BOOST_CONCEPT_ASSERT(( boost::Sequence<               random_access_t >));

  typedef gap_buffer<std::list<char> > forward_t;
  BOOST_CONCEPT_ASSERT(( boost::Container<              forward_t >));
  BOOST_CONCEPT_ASSERT(( boost::ForwardContainer<       forward_t >));
  BOOST_CONCEPT_ASSERT(( boost::DefaultConstructible<   forward_t >));
  BOOST_CONCEPT_ASSERT(( boost::Sequence<               forward_t >));
};

BOOST_AUTO_TEST_SUITE(gap_buffer_tests)

BOOST_AUTO_TEST_CASE(constructors)
{

}

BOOST_AUTO_TEST_SUITE_END()
