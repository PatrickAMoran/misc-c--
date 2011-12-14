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


// Assert that a gap buffer is empty and check the properties of that
template<class TGapBuffer>
void assert_properties_empty(TGapBuffer & buf)
{
  assert_properties_size(buf, 0);
  // Check consequences of being empty
  BOOST_CHECK_EQUAL( buf.position(), 0u ); // It has position 0
}

template<class TGapBuffer>
void assert_position_end(TGapBuffer & buf)
{
  // Demonstrate that the cursor is at the end
  BOOST_CHECK_EQUAL( buf.position(), buf.size() );
  BOOST_CHECK( buf.here() == buf.end() );
  BOOST_CHECK( buf.rhere() == buf.rbegin() );
}

// ----- ----- ------ Constructors ----- ----- -----

BOOST_AUTO_TEST_CASE(default_constructor)
{
  buffer_t default_constructed;
  assert_properties_empty(default_constructed);
}

BOOST_AUTO_TEST_CASE(copy_constructor)
{
  buffer_t copied_value;
  {
    copied_value.insert('x');
    assert_properties_size(copied_value, 1);
  }
  
  buffer_t copy_constructed(copied_value);
  assert_properties_size(copy_constructed, 1);
  BOOST_CHECK(copy_constructed == copied_value);
  BOOST_CHECK(!(copy_constructed != copied_value));

  // Assert that absence of unintended aliasing
  copy_constructed.insert('y');
  BOOST_CHECK( copy_constructed != copied_value );
}

BOOST_AUTO_TEST_CASE(fill_constructor)
{
  size_t const first_len = 7;
  buffer_t fill_constructed_1( first_len );
  assert_properties_size( fill_constructed_1, first_len );
  
  buffer_t fill_constructed_2( first_len, '\0');
  assert_properties_size( fill_constructed_1, first_len );
  BOOST_CHECK( fill_constructed_1 == fill_constructed_2 );

  { // Demonstrate its equivalence to another container's fill constructor
    std::vector<char> other_cont(first_len, '\0');
    BOOST_CHECK( seq_eq(fill_constructed_1, other_cont) );
  }

  size_t const third_len = 22;
  char const third_fill_val = 'v';
  buffer_t fill_constructed_3( third_len, third_fill_val );
  assert_properties_size( fill_constructed_3, third_len );
  { // Demonstrate its equivalence to another container's fill constructor
    std::vector<char> other_cont(third_len, third_fill_val);
    BOOST_CHECK( seq_eq(fill_constructed_3, other_cont) );
  }
  
}

BOOST_AUTO_TEST_CASE(iter_pair_constructor)
{
  static size_t const first_len = 22;
  std::vector<char> other_cont(first_len, '\0');
  
  buffer_t iterator_constructed(other_cont.begin(), other_cont.end());
  // Demonstrate that the data is all there
  BOOST_CHECK( seq_eq(iterator_constructed, other_cont) );
  assert_position_end( iterator_constructed );
}

BOOST_AUTO_TEST_SUITE_END()
