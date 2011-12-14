#define BOOST_TEST_MODULE gap_buffer
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <boost/concept_check.hpp>

#include "gap_buffer.hpp"

#include <deque>
#include <list>
#include <vector>
#include <iterator>

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

typedef gap_buffer<std::deque<char> > buffer_t;


template<class TIter>
TIter advance_copy(TIter const & iter, size_t n)
{
  TIter rtn = iter;
  std::advance(rtn, n);
  return rtn;
}

template<class TSeqA, class TSeqB>
bool seq_eq(TSeqA const & a, TSeqB const & b)
{
  return std::mismatch( a.begin(), a.end(),
			b.begin() ) ==
    std::make_pair(a.end(), b.end());
}


// ----- ----- ------ Constructors ----- ----- -----

BOOST_AUTO_TEST_CASE(default_constructor)
{
}
BOOST_AUTO_TEST_SUITE_END()
