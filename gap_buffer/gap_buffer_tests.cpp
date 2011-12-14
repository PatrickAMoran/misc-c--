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



// Assert that a gap buffer is empty and check the properties of that
template<class TGapBuffer>
void assert_properties_size(TGapBuffer & buf, size_t len)
{
  const TGapBuffer & buf_const = buf;
  BOOST_CHECK_EQUAL( buf.empty(), !len );
  BOOST_CHECK_EQUAL( (buf == TGapBuffer()), !len );
  BOOST_CHECK_EQUAL( buf.size(), len );
  BOOST_CHECK( advance_copy(buf.begin(), len) == buf.end() );
  BOOST_CHECK( advance_copy(buf.rbegin(), len) == buf.rend() );
  BOOST_CHECK( advance_copy(buf_const.begin(), len)  == buf_const.end() );
  BOOST_CHECK( advance_copy(buf_const.rbegin(), len) == buf_const.rend() );
  BOOST_CHECK( advance_copy(buf_const.begin(), len) == buf.end() );
  BOOST_CHECK( advance_copy(buf.begin(), len) == buf_const.end() );
  BOOST_CHECK( advance_copy(buf_const.rbegin(), len) == buf.rend() );
  BOOST_CHECK( advance_copy(buf.rbegin(), len) == buf_const.rend() );
  
  // (end - begin) == size
  BOOST_CHECK_EQUAL( (std::distance(buf.begin(), buf.end())),
		     len );
  BOOST_CHECK_EQUAL( (std::distance(buf_const.begin(), buf_const.end())),
		     len );

  // (rend - rbegin) == size
  BOOST_CHECK_EQUAL( (std::distance(buf.rbegin(), buf.rend())),
		     len );
  BOOST_CHECK_EQUAL( (std::distance(buf_const.rbegin(), buf_const.rend())),
		     len );

  // (here - begin) + (end - here) == size
  BOOST_CHECK_EQUAL( (std::distance(buf.begin(), buf.here()) +
		      std::distance(buf.here(), buf.end())),
		     len );
  BOOST_CHECK_EQUAL( (std::distance(buf_const.begin(), buf_const.here()) +
		      std::distance(buf_const.here(), buf_const.end())),
		     len );

  // (rhere - rbegin) + (rend - rhere) == size
  BOOST_CHECK_EQUAL( (std::distance(buf.rbegin(), buf.rhere()) +
		      std::distance(buf.rhere(), buf.rend())),
		     len );
  BOOST_CHECK_EQUAL( (std::distance(buf_const.rbegin(), buf_const.rhere()) +
		      std::distance(buf_const.rhere(), buf_const.rend())),
		     len );
}

// ----- ----- ------ Constructors ----- ----- -----

BOOST_AUTO_TEST_CASE(default_constructor)
{
}
BOOST_AUTO_TEST_SUITE_END()
