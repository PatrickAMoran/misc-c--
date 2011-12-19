#define BOOST_TEST_MODULE gap_buffer
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>

#include <boost/concept_check.hpp>

#include "gap_buffer.hpp"

#include <deque>
#include <list>
#include <vector>
#include <iterator>
#include <string>

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

template<class T>
std::ostream & operator<<(std::ostream & os, gap_buffer<T> const & buf)
{
  os << '"';
  for(typename gap_buffer<T>::const_iterator iter = buf.begin(), 
	end = buf.end(); iter != end; ++iter)
    os << (*iter);
  os << '"' << std::endl;
  return os;
}


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
  return a.size() == b.size() &&
    std::mismatch( a.begin(), a.end(),
		   b.begin() ).first == a.end();
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

// Assert that a gap buffer is nonempty and check the properties of that
template<class TGapBuffer>
void assert_properties_nonempty(TGapBuffer & buf)
{
  const TGapBuffer & buf_const = buf;
  BOOST_CHECK( !buf.empty() );
  BOOST_CHECK( (buf != TGapBuffer()) );
  BOOST_CHECK_NE( buf.size(), 0 );

  BOOST_CHECK( buf.begin() != buf.end() );
  BOOST_CHECK( buf_const.begin() != buf_const.end() );
  BOOST_CHECK( buf.rbegin() != buf.rend() );
  BOOST_CHECK( buf_const.rbegin() != buf_const.rend() );

  BOOST_CHECK( buf.begin() != buf_const.end() );
  BOOST_CHECK( buf_const.begin() != buf.end() );
  BOOST_CHECK( buf.rbegin() != buf_const.rend() );
  BOOST_CHECK( buf_const.rbegin() != buf.rend() );
  
  // (end - begin) > 0
  BOOST_CHECK_GT( (std::distance(buf.begin(), buf.end())),
		  0 );
  BOOST_CHECK_GT( (std::distance(buf_const.begin(), buf_const.end())),
		  0 );

  // (rend - rbegin) > 0
  BOOST_CHECK_GT( (std::distance(buf.rbegin(), buf.rend())),
		  0 );
  BOOST_CHECK_GT( (std::distance(buf_const.rbegin(), buf_const.rend())),
		  0 );

  // (here - begin) + (end - here) > 0
  BOOST_CHECK_GT( (std::distance(buf.begin(), buf.here()) +
		   std::distance(buf.here(), buf.end())),
		  0 );
  BOOST_CHECK_GT( (std::distance(buf_const.begin(), buf_const.here()) +
		   std::distance(buf_const.here(), buf_const.end())),
		  0 );

  // (rhere - rbegin) + (rend - rhere) > 0
  BOOST_CHECK_GT( (std::distance(buf.rbegin(), buf.rhere()) +
		   std::distance(buf.rhere(), buf.rend())),
		  0 );
  BOOST_CHECK_GT( (std::distance(buf_const.rbegin(), buf_const.rhere()) +
		   std::distance(buf_const.rhere(), buf_const.rend())),
		  0 );
}

template<class TGapBuffer>
void assert_position_end(TGapBuffer & buf)
{
  // Demonstrate that the cursor is at the end
  BOOST_CHECK_EQUAL( buf.position(), buf.size() );
  BOOST_CHECK( buf.here() == buf.end() );
  BOOST_CHECK( buf.rhere() == buf.rbegin() );
}

typedef buffer_t::iterator                 iterator;
typedef buffer_t::const_iterator           const_iterator;
typedef buffer_t::reverse_iterator         reverse_iterator;
typedef buffer_t::const_reverse_iterator   const_reverse_iterator;


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
  BOOST_CHECK_EQUAL( copy_constructed, copied_value );
  BOOST_CHECK( seq_eq(copy_constructed, copied_value) );
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
  BOOST_CHECK_EQUAL( fill_constructed_1, fill_constructed_2 );

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

// ----- ----- ------ Size Reporting ----- ----- -----
/*
  The meaning of size_type max_size() const is not really clear.  What is clear
  is that for a container adapter, it can only be reasonably defined in terms of
  its adapted container.  It is not tested here because I haven't yet found a way
  of exposing it to the unit tests, and I doubt if this member function is
  actually of any use.
*/

BOOST_AUTO_TEST_CASE(size)
{
  buffer_t default_constructed;
  
  BOOST_CHECK_EQUAL( default_constructed.size(), 0 );

  // Demonstrate that inserting a single element raises the size by 1
  default_constructed.insert(' ');
  assert_properties_size( default_constructed, 1 );

  // Demonstrate that moving the cursor does not change the size
  default_constructed.advance(-1);
  assert_properties_size( default_constructed, 1 );
  default_constructed.advance(1);
  assert_properties_size( default_constructed, 1 );

  // Demonstrate that inserting 4 elements raises the size by 4
  default_constructed.insert(default_constructed.begin(), 4, 'a');
  assert_properties_size( default_constructed, 5 );

  // Demonstrate that removing 2 elements lowers the size by 2
  default_constructed.erase(-2);
  assert_properties_size( default_constructed, 3 );
}

BOOST_AUTO_TEST_CASE(empty)
{
  buffer_t buffer;
  assert_properties_empty( buffer );

  buffer.insert(buffer.end(), 1, '\n');
  assert_properties_nonempty( buffer );

  buffer.clear();
  assert_properties_empty( buffer );
}


// ----- ----- ------ Swapping ----- ----- -----
BOOST_AUTO_TEST_CASE(swap)
{
  std::string const strA("this is the first test buffer");
  std::string const strB("the second test buffer am I");

  buffer_t bufferA(strA.begin(), strA.end());
  buffer_t bufferB(strB.begin(), strB.end());
  
  bufferA.advance(-7);
  size_t const positionA = bufferA.position();

  bufferB.advance(-2);
  size_t const positionB = bufferB.position();

  BOOST_REQUIRE( seq_eq(bufferA, strA) );
  BOOST_REQUIRE( seq_eq(bufferB, strB) );
  BOOST_REQUIRE_EQUAL( bufferA.position(), positionA );
  BOOST_REQUIRE_EQUAL( bufferB.position(), positionB );

  bufferA.swap( bufferB );

  BOOST_CHECK( seq_eq(bufferA, strB) );
  BOOST_CHECK( seq_eq(bufferB, strA) );
  BOOST_CHECK_EQUAL( bufferA.position(), positionB );
  BOOST_CHECK_EQUAL( bufferB.position(), positionA );

  bufferB.swap( bufferA );

  BOOST_CHECK( seq_eq(bufferA, strA) );
  BOOST_CHECK( seq_eq(bufferB, strB) );
  BOOST_CHECK_EQUAL( bufferA.position(), positionA );
  BOOST_CHECK_EQUAL( bufferB.position(), positionB );
}


// ----- ----- ------ Cursor Manipulation ----- ----- -----
BOOST_AUTO_TEST_CASE(advance)
{
  std::string const strA("this is the first test buffer");
  buffer_t buffer(strA.begin(), strA.end());

  size_t const position_1 = buffer.position();

  // Assert that advancing 0 does nothing
  buffer.advance(0);
  BOOST_CHECK_EQUAL(buffer.position(), position_1);

  // Assert that moving backwards does work
  buffer.advance(-3);
  size_t const position_2 = position_1 - 3;
  BOOST_CHECK_EQUAL(buffer.position(), position_2);

  // Assert that moving forwards does work
  buffer.advance(1);
  size_t const position_3 = position_2 + 1;
  BOOST_CHECK_EQUAL(buffer.position(), position_3);
}


BOOST_AUTO_TEST_CASE(position)
{
  std::string const strA("this is the first test buffer");

  buffer_t buffer(strA.begin(), strA.end());
  assert_position_end( buffer );
  size_t base_position = buffer.size();

  { // ----- Advance
    buffer.advance(-10);
    base_position -= 10;
    BOOST_CHECK_EQUAL(buffer.position(), base_position);

    buffer.advance(1);
    base_position += 1;
    BOOST_CHECK_EQUAL(buffer.position(), base_position);

    buffer.advance(0);
    BOOST_CHECK_EQUAL(buffer.position(), base_position);
  }

  { // ----- At cursor insert
    // Assert that the at-cursor insert moves the cursor along
    std::string const add_string("fizbuzz");
    base_position += add_string.length();
    buffer.insert(add_string);
    BOOST_CHECK_EQUAL(buffer.position(), base_position);

    // Assert that the at-cursor insert moves the cursor along
    base_position += 1;
    buffer.insert('z');
    BOOST_CHECK_EQUAL(buffer.position(), base_position);
  }

  { // ----- At cursor erase
    // Assert that the at-cursor erase ahead of the cursor does not move it
    buffer.erase(2);
    BOOST_CHECK_EQUAL(buffer.position(), base_position);

    // Assert that a null at-cursor erase does not move the cursor
    buffer.erase(0);
    BOOST_CHECK_EQUAL(buffer.position(), base_position);
  
    // Assert that an at-cursor erase backwards moves the cursor back
    buffer.erase(-2);
    base_position -= 2;
    BOOST_CHECK_EQUAL(buffer.position(), base_position);
  }

  { // ----- Other inserts
    std::string const insert_str("insert");

    // Assert that inserting data at the cursor moves it forward
    base_position += 1;
    buffer.insert(buffer.here(), 'x');
    BOOST_CHECK_EQUAL(buffer.position(), base_position);
    base_position += 2;
    buffer.insert(buffer.here(), 2, 'x');
    BOOST_CHECK_EQUAL(buffer.position(), base_position);

    base_position += insert_str.length();
    buffer.insert(buffer.here(), insert_str.begin(), insert_str.end());
    BOOST_CHECK_EQUAL(buffer.position(), base_position);

    // Assert that inserting data after the cursor does not
    buffer.insert(buffer.here() + 1, 3, 'b');
    BOOST_CHECK_EQUAL(buffer.position(), base_position);
    buffer.insert(buffer.here() + 1, 'b');
    BOOST_CHECK_EQUAL(buffer.position(), base_position);
    buffer.insert(buffer.here() + 1, insert_str.begin(), insert_str.end());
    BOOST_CHECK_EQUAL(buffer.position(), base_position);

    // Assert that inserting data before the cursor moves it
    buffer.insert(buffer.begin(), 3, 'b');
    base_position += 3;
    BOOST_CHECK_EQUAL(buffer.position(), base_position);

    buffer.insert(buffer.begin(), 'b');
    base_position += 1;
    BOOST_CHECK_EQUAL(buffer.position(), base_position);

    base_position += insert_str.length();
    buffer.insert(buffer.begin(), insert_str.begin(), insert_str.end());
    BOOST_CHECK_EQUAL(buffer.position(), base_position);
  }
}

BOOST_AUTO_TEST_CASE(insert_elem_at_cursor)
{
  std::string const strA("this is the first test buffer");
  buffer_t buffer(strA.begin(), strA.end());
  BOOST_CHECK_EQUAL(buffer.size(), strA.size() );
  buffer.advance(-10);
  BOOST_CHECK_EQUAL(buffer.size(), strA.size() );

  buffer.insert('T');
  BOOST_CHECK_EQUAL(buffer.size(), strA.size() + 1);

  std::string const strB("this is the first tTest buffer");
  BOOST_CHECK( seq_eq(strB, buffer) );

  buffer.insert('U');
  std::string const strC("this is the first tTUest buffer");
  BOOST_CHECK( seq_eq(strC, buffer) );
}


BOOST_AUTO_TEST_CASE(insert_range_at_cursor)
{
  std::string const strA("this is the first test buffer");
  buffer_t buffer(strA.begin(), strA.end());
  BOOST_CHECK_EQUAL(buffer.size(), strA.size() );
  buffer.advance(-10);
  BOOST_CHECK_EQUAL(buffer.size(), strA.size() );

  std::string const insA("T");
  buffer.insert(insA);
  BOOST_CHECK_EQUAL(buffer.size(), strA.size() + insA.size());

  std::string const strB("this is the first tTest buffer");
  BOOST_CHECK( seq_eq(strB, buffer) );

  std::string const insB("UV");
  buffer.insert(insB);
  std::string const strC("this is the first tTUVest buffer");
  BOOST_CHECK( seq_eq(strC, buffer) );
}


BOOST_AUTO_TEST_CASE(erase_elem_at_cursor)
{
  std::string const strA("this is the first test buffer");
  buffer_t buffer(strA.begin(), strA.end());
  BOOST_CHECK_EQUAL(buffer.size(), strA.size());
  buffer.advance(-10);
  BOOST_CHECK_EQUAL(buffer.size(), strA.size() );

  buffer.erase(1);
  BOOST_CHECK_EQUAL(buffer.size(), strA.size() - 1);

  std::string const strB("this is the first tst buffer");
  BOOST_CHECK( seq_eq(strB, buffer) );

  buffer.erase(-5);
  std::string const strC("this is the fist buffer");
  BOOST_CHECK( seq_eq(strC, buffer) );
}

// ----- ----- ------ Operators ----- ----- -----
BOOST_AUTO_TEST_CASE(compare_operators)
{
  std::string const strA("Something to compare against");
  std::string const strB("Something else to compare against");
  std::string const strC("HAHAHAHAHA");

  buffer_t const bufferA(strA.begin(), strA.end());
  buffer_t const bufferB(strB.begin(), strB.end());
  buffer_t const bufferC(strC.begin(), strC.end());
  buffer_t bufferD(strA.begin(), strA.end());

  // Assert that two gap buffers always equal each other
  BOOST_CHECK( bufferA == bufferA );
  BOOST_CHECK( bufferB == bufferB );
  BOOST_CHECK( bufferC == bufferC );
  BOOST_CHECK( bufferD == bufferD );
  // Check that A == D
  BOOST_CHECK( bufferA == bufferD );

  // Assert that != equates to !(==)
  BOOST_CHECK( !(bufferA != bufferA) );
  BOOST_CHECK( !(bufferB != bufferB) );
  BOOST_CHECK( !(bufferC != bufferC) );
  BOOST_CHECK( !(bufferD != bufferD) );
  BOOST_CHECK( !(bufferA != bufferD) );

  // Assert that == implies !<
  BOOST_CHECK( !(bufferA < bufferA) );
  BOOST_CHECK( !(bufferB < bufferB) );
  BOOST_CHECK( !(bufferC < bufferC) );
  BOOST_CHECK( !(bufferD < bufferD) );
  BOOST_CHECK( !(bufferA < bufferD) );

  // Assert that == implies !>
  BOOST_CHECK( !(bufferA > bufferA) );
  BOOST_CHECK( !(bufferB > bufferB) );
  BOOST_CHECK( !(bufferC > bufferC) );
  BOOST_CHECK( !(bufferD > bufferD) );
  BOOST_CHECK( !(bufferA > bufferD) );

  // Assert that == implies <=
  BOOST_CHECK( bufferA <= bufferA );
  BOOST_CHECK( bufferB <= bufferB );
  BOOST_CHECK( bufferC <= bufferC );
  BOOST_CHECK( bufferD <= bufferD );
  BOOST_CHECK( bufferA <= bufferD );

  // Assert that == implies >=
  BOOST_CHECK( bufferA >= bufferA );
  BOOST_CHECK( bufferB >= bufferB );
  BOOST_CHECK( bufferC >= bufferC );
  BOOST_CHECK( bufferD >= bufferD );
  BOOST_CHECK( bufferA >= bufferD );

  // Assert that position is ignored
  bufferD.advance(-4);
  BOOST_CHECK( bufferA == bufferD );
  BOOST_CHECK( !(bufferA != bufferD) );

  // Check the ordering
  BOOST_CHECK_EQUAL( (bufferA < bufferA),
		     (strA < strA) );
  BOOST_CHECK_EQUAL( (bufferB < bufferB),
		     (strB < strB) );
  BOOST_CHECK_EQUAL( (bufferC < bufferC),
		     (strC < strC) );
  BOOST_CHECK_EQUAL( (bufferA > bufferA),
		     (strA > strA) );
  BOOST_CHECK_EQUAL( (bufferB > bufferB),
		     (strB > strB) );
  BOOST_CHECK_EQUAL( (bufferC > bufferC),
		     (strC > strC) );

  BOOST_CHECK_EQUAL( (bufferA <= bufferA),
		     (strA <= strA) );
  BOOST_CHECK_EQUAL( (bufferB <= bufferB),
		     (strB <= strB) );
  BOOST_CHECK_EQUAL( (bufferC <= bufferC),
		     (strC <= strC) );
  BOOST_CHECK_EQUAL( (bufferA >= bufferA),
		     (strA >= strA) );
  BOOST_CHECK_EQUAL( (bufferB >= bufferB),
		     (strB >= strB) );
  BOOST_CHECK_EQUAL( (bufferC >= bufferC),
		     (strC >= strC) );

  BOOST_CHECK_EQUAL( (bufferA < bufferB),
		     (strA < strB) );
  BOOST_CHECK_EQUAL( (bufferB < bufferC),
		     (strB < strC) );
  BOOST_CHECK_EQUAL( (bufferA < bufferC),
		     (strA < strC) );
  BOOST_CHECK_EQUAL( (bufferA > bufferB),
		     (strA > strB) );
  BOOST_CHECK_EQUAL( (bufferB > bufferC),
		     (strB > strC) );
  BOOST_CHECK_EQUAL( (bufferA > bufferC),
		     (strA > strC) );

  BOOST_CHECK_EQUAL( (bufferA <= bufferB),
		     (strA <= strB) );
  BOOST_CHECK_EQUAL( (bufferB <= bufferC),
		     (strB <= strC) );
  BOOST_CHECK_EQUAL( (bufferA <= bufferC),
		     (strA <= strC) );
  BOOST_CHECK_EQUAL( (bufferA >= bufferB),
		     (strA >= strB) );
  BOOST_CHECK_EQUAL( (bufferB >= bufferC),
		     (strB >= strC) );
  BOOST_CHECK_EQUAL( (bufferA >= bufferC),
		     (strA >= strC) );
}

BOOST_AUTO_TEST_CASE(assign_operator)
{
  std::string const strA("Something to compare against");
  std::string const strB("Something else to compare against");

  buffer_t bufferA(strA.begin(), strA.end());
  buffer_t bufferB(strB.begin(), strB.end());
  buffer_t bufferC;

  BOOST_CHECK( bufferA != bufferB );
  BOOST_CHECK( bufferA != bufferC );
  BOOST_CHECK( bufferB != bufferC );

  bufferC = bufferA;
  BOOST_CHECK( bufferA != bufferB );
  BOOST_CHECK( bufferA == bufferC );
  BOOST_CHECK( bufferB != bufferC );

  bufferA = bufferB;
  BOOST_CHECK( bufferA == bufferB );
  BOOST_CHECK( bufferA != bufferC );
  BOOST_CHECK( bufferB != bufferC );

  bufferB = bufferC;
  BOOST_CHECK( bufferA != bufferB );
  BOOST_CHECK( bufferA != bufferC );
  BOOST_CHECK( bufferB == bufferC );
}


// ----- ----- ------ Iterators ----- ----- -----
BOOST_AUTO_TEST_CASE(iterator_comparisons)
{
  std::string const str("Some data to iterate over");

  buffer_t buffer(str.begin(), str.end());
  buffer_t const & cbuffer = buffer;

  { // Assert that we can compare iterators and const_iterators
    iterator iter        = buffer.begin();
    const_iterator citer = cbuffer.begin();

    BOOST_CHECK(  (iter == citer) );
    BOOST_CHECK( !(iter != citer) );
    BOOST_CHECK( !(iter >  citer) );
    BOOST_CHECK( !(iter <  citer) );
    BOOST_CHECK(  (iter >= citer) );
    BOOST_CHECK(  (iter <= citer) );

    ++iter;
    BOOST_CHECK( !(iter == citer) );
    BOOST_CHECK(  (iter != citer) );
    BOOST_CHECK(  (iter >  citer) );
    BOOST_CHECK( !(iter <  citer) );
    BOOST_CHECK(  (iter >= citer) );
    BOOST_CHECK( !(iter <= citer) );

    ++citer;
    BOOST_CHECK(  (iter == citer) );
    BOOST_CHECK( !(iter != citer) );
    BOOST_CHECK( !(iter >  citer) );
    BOOST_CHECK( !(iter <  citer) );
    BOOST_CHECK(  (iter >= citer) );
    BOOST_CHECK(  (iter <= citer) );

    ++citer;
    BOOST_CHECK( !(iter == citer) );
    BOOST_CHECK(  (iter != citer) );
    BOOST_CHECK( !(iter >  citer) );
    BOOST_CHECK(  (iter <  citer) );
    BOOST_CHECK( !(iter >= citer) );
    BOOST_CHECK(  (iter <= citer) );
  }

  { // Assert that we can compare iterators and const_iterators
    reverse_iterator riter        = buffer.rbegin();
    const_reverse_iterator rciter = cbuffer.rbegin();

    BOOST_CHECK(  (riter == rciter) );
    BOOST_CHECK( !(riter != rciter) );
    BOOST_CHECK( !(riter >  rciter) );
    BOOST_CHECK( !(riter <  rciter) );
    BOOST_CHECK(  (riter >= rciter) );
    BOOST_CHECK(  (riter <= rciter) );

    ++riter;
    BOOST_CHECK( !(riter == rciter) );
    BOOST_CHECK(  (riter != rciter) );
    BOOST_CHECK(  (riter >  rciter) );
    BOOST_CHECK( !(riter <  rciter) );
    BOOST_CHECK(  (riter >= rciter) );
    BOOST_CHECK( !(riter <= rciter) );

    ++rciter;
    BOOST_CHECK(  (riter == rciter) );
    BOOST_CHECK( !(riter != rciter) );
    BOOST_CHECK( !(riter >  rciter) );
    BOOST_CHECK( !(riter <  rciter) );
    BOOST_CHECK(  (riter >= rciter) );
    BOOST_CHECK(  (riter <= rciter) );

    ++rciter;
    BOOST_CHECK( !(riter == rciter) );
    BOOST_CHECK(  (riter != rciter) );
    BOOST_CHECK( !(riter >  rciter) );
    BOOST_CHECK(  (riter <  rciter) );
    BOOST_CHECK( !(riter >= rciter) );
    BOOST_CHECK(  (riter <= rciter) );
  }
}

BOOST_AUTO_TEST_CASE(iterator_movement)
{
  std::string const str("Some data to iterate over");
  buffer_t buffer(str.begin(), str.end());
  buffer_t const & cbuffer = buffer;

  iterator iter = buffer.end();
  const_iterator citer = buffer.end();
  BOOST_CHECK(  (iter == citer) );

  --iter;
  BOOST_CHECK_EQUAL(*iter, 'r');
  --iter;
  BOOST_CHECK_EQUAL(*iter, 'e');
  --iter;
  BOOST_CHECK_EQUAL(*iter, 'v');
  --iter;
  BOOST_CHECK_EQUAL(*iter, 'o');

  BOOST_CHECK(  (iter == (citer-4)) );
  citer = citer - 4;
  BOOST_CHECK(  (iter == citer) );
}

BOOST_AUTO_TEST_CASE(insert_at_iter)
{
  std::string str("Some data to iterate over");
  buffer_t buffer(str.begin(), str.end());

  buffer.advance(-13);

  {
    // Demonstrate that inserting data before the cursor works and does move
    // the cursor
    size_t const pos_before = buffer.position();
    buffer_t::iterator insert_pos = buffer.here();
    --insert_pos;
    buffer.insert(insert_pos, 'Z');
    str.insert(str.begin() + 11, 'Z');
    BOOST_CHECK( seq_eq(buffer, str) );
    BOOST_CHECK_EQUAL( pos_before + 1, buffer.position() );
  }
  {
    // Demonstrate that inserting data at the cursor works and does move
    // the cursor
    size_t const pos_before = buffer.position();
    buffer.insert(buffer.here(), 'Y');
    str.insert(str.begin() + 13, 'Y');
    BOOST_CHECK( seq_eq(buffer, str) );
    BOOST_CHECK_EQUAL( pos_before + 1, buffer.position() );
  }
  {
    // Demonstrate that inserting data after the cursor works and does not move
    // the cursor
    size_t const pos_before = buffer.position();
    buffer.insert(buffer.here() + 1, 'X');
    str.insert(str.begin() + 15, 'X');
    BOOST_CHECK( seq_eq(buffer, str) );
    BOOST_CHECK_EQUAL( pos_before, buffer.position() );
  }
}


BOOST_AUTO_TEST_CASE(insert_n_at_iter)
{
  std::string str("Some data to iterate over");
  buffer_t buffer(str.begin(), str.end());

  buffer.advance(-13);

  {
    // Demonstrate that inserting data before the cursor works and does move
    // the cursor
    size_t const pos_before = buffer.position();
    buffer.insert(buffer.here()-1, 3, 'Z');
    str.insert(str.begin() + 11, 3, 'Z');
    BOOST_CHECK( seq_eq(buffer, str) );
    BOOST_CHECK_EQUAL( pos_before + 3, buffer.position() );
  }
  {
    // Demonstrate that inserting data at the cursor works and does move
    // the cursor
    size_t const pos_before = buffer.position();
    buffer.insert(buffer.here(), 4, 'Y');
    str.insert(str.begin() + 15, 4, 'Y');
    BOOST_CHECK( seq_eq(buffer, str) );
    BOOST_CHECK_EQUAL( pos_before + 4, buffer.position() );
  }
  {
    // Demonstrate that inserting data after the cursor works and does not move
    // the cursor
    size_t const pos_before = buffer.position();
    buffer.insert(buffer.here() + 1, 1, 'X');
    str.insert(str.begin() + 20, 1, 'X');
    BOOST_CHECK( seq_eq(buffer, str) );
    BOOST_CHECK_EQUAL( pos_before, buffer.position() );
  }
  {
    // Demonstrate that inserting no data has no effect
    size_t const pos_before = buffer.position();
    buffer.insert(buffer.here(), 0, 'X');
    BOOST_CHECK( seq_eq(buffer, str) );
    BOOST_CHECK_EQUAL( pos_before, buffer.position() );
  }
}


BOOST_AUTO_TEST_CASE(insert_range_at_iter)
{
  std::string str("Some data to iterate over");
  buffer_t buffer(str.begin(), str.end());

  buffer.advance(-13);

  {
    // Demonstrate that inserting data before the cursor works and does move
    // the cursor
    size_t const pos_before = buffer.position();
    std::string const insert_str("ZZZ");
    buffer.insert(buffer.here()-1, insert_str.begin(), insert_str.end());
    str.insert(str.begin() + 11, insert_str.begin(), insert_str.end());
    BOOST_CHECK( seq_eq(buffer, str) );
    BOOST_CHECK_EQUAL( pos_before + insert_str.size(), buffer.position() );
  }
  {
    // Demonstrate that inserting data at the cursor works and does move
    // the cursor
    size_t const pos_before = buffer.position();
    std::string const insert_str("YYYY");
    buffer.insert(buffer.here(), insert_str.begin(), insert_str.end());
    str.insert(str.begin() + 15, insert_str.begin(), insert_str.end());
    BOOST_CHECK( seq_eq(buffer, str) );
    BOOST_CHECK_EQUAL( pos_before + insert_str.size(), buffer.position() );
  }
  {
    // Demonstrate that inserting data after the cursor works and does not move
    // the cursor
    size_t const pos_before = buffer.position();
    std::string const insert_str("X");
    buffer.insert(buffer.here() + 1, insert_str.begin(), insert_str.end());
    str.insert(str.begin() + 20, insert_str.begin(), insert_str.end());
    BOOST_CHECK( seq_eq(buffer, str) );
    BOOST_CHECK_EQUAL( pos_before, buffer.position() );
  }
  {
    // Demonstrate that inserting no data has no effect
    size_t const pos_before = buffer.position();
    std::string const insert_str;
    buffer.insert(buffer.here(), insert_str.begin(), insert_str.end());
    BOOST_CHECK( seq_eq(buffer, str) );
    BOOST_CHECK_EQUAL( pos_before, buffer.position() );
  }
}


//@todo front(), front() const
//@todo erase(iter), erase(iter, iter)
//@todo clear()
//@todo resize(n)

BOOST_AUTO_TEST_SUITE_END()
