#ifndef GAP_BUFFER_HPP_INCLUDED_
#define GAP_BUFFER_HPP_INCLUDED_


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


#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>
#include <iterator>


/**
   @brief A gap buffer container adapter in C++
   @details
   This class template implements a gap-buffer, as documented on
   http://en.wikipedia.org/wiki/Gap_buffer.  This data structure represents a
   buffer of elements with efficient insertion and removal at a point known as
   the cursor.  This data structure is commonly used in text editors where these
   performance characteristics are desirable.

   A gap_buffer is an STL container.  It models the STL concepts Container,
   Forward Container, and Reversible Container.

   @tparam TContainer The underlying container type of the gap buffer.  A single
                      gap buffer will own two instances of TContainer - the area
		      before the cursor and the area after the cursor.
		      TContainer must be a model of the C++ STL concept
		      "Reversible Container".  While the still function
		      correctly, desired performance characteristics will only
		      be met if it also models "Front Insertion Sequence" and
		      "Back Insertion Sequence".
*/
template<class TContainer>
class gap_buffer
{
private:
  // This iterator template uses Boost.Iterator to produce the four iterator
  // types for gap_buffer.  It manages the process of jumping from the end of
  // the before container to the beginning of the after container, and vice
  // versa.
  template<class TUnderlying>
  struct iterator_impl;

  typedef typename std::iterator_traits<typename TContainer::iterator>::
  iterator_category iterator_category;

  TContainer before;
  TContainer after;

public:
  /// @name Other Container requirements
  //@{
  /// The value_type of this container
  typedef typename TContainer::value_type                  value_type;
  /// value_type *
  typedef typename TContainer::pointer                     pointer;
  /// value_type &
  typedef typename TContainer::reference                   reference;
  /// value_type const &
  typedef typename TContainer::const_reference             const_reference;
  /// The size_type of this container
  typedef typename TContainer::size_type                   size_type;
  /// The difference_type of this container
  typedef typename TContainer::difference_type             difference_type;

  /// Return the number of elements in the gap_buffer
  size_type size() const;
  /// Return the maximum number of elements a gap_buffer might hold
  size_type max_size() const;
  /// Return if the gap_buffer is empty
  bool empty() const;
  /// Swap this gap_buffer with another
  void swap(gap_buffer & other);
  //@}


  ///@name Iterator access
  //@{
  typedef iterator_impl<typename TContainer::iterator>         iterator;
  typedef iterator_impl<typename TContainer::const_iterator>   const_iterator;
  typedef iterator_impl<typename TContainer::reverse_iterator> reverse_iterator;
  typedef iterator_impl<typename TContainer::const_reverse_iterator>
  const_reverse_iterator;

  /// Return the cursor position as an iterator
  iterator here();
  /// Return the cursor position as an iterator
  const_iterator here() const;
  /// Return the cursor position as a reverse iterator
  reverse_iterator rhere();
  /// Return the cursor position as a reverse iterator
  const_reverse_iterator rhere() const;

  /// Get an iterator to the beginning of the gap_buffer
  iterator begin();
  /// Get an iterator to one element past the end of the gap_buffer
  iterator end();
  /// Get a const iterator to the beginning of the gap_buffer
  const_iterator begin() const;
  /// Get a const iterator to one element past the end of the gap_buffer
  const_iterator end() const;

  /// Get a reverse iterator to the beginning of the reversed gap_buffer
  reverse_iterator rbegin();
  /// @brief Get a reverse iterator to one element past the end of the reversed
  /// gap_buffer
  reverse_iterator rend();
  /// Get a const reverse iterator to the beginning of the reversed gap_buffer
  const_reverse_iterator rbegin() const;
  /// @brief Get a reverse const iterator to one element past the end of the
  /// reversed gap_buffer
  const_reverse_iterator rend() const;
  //@}

  /// @name Cursor Handling
  //@{
  /// Return the cursor position of the gap buffer
  size_type position() const;
  
  /// Move the cursor position
  void advance(difference_type const);

  /// @brief Remove data from this position.
  /// @details A positive value erases the given number of values from ahead of
  /// the cursor.  A negative value erases the absolute value of the given
  /// number of cursors from behind the cursor.
  void erase(difference_type const);

  /// Insert an element at the cursor
  size_type insert(value_type const);

  /// @brief Insert a range of elements at the cursor
  /// @param range Any range of elements Boost.Range recognizes as a Single Pass
  ///              Range.
  template<class TSinglePassRange>
  size_type insert(TSinglePassRange const & range);
  //@}


};

///@name Comparisons
//@{
template<class TCont>
bool operator==(gap_buffer<TCont> const &, gap_buffer<TCont> const &);
template<class TCont>
bool operator!=(gap_buffer<TCont> const &, gap_buffer<TCont> const &);
template<class TCont>
bool operator<(gap_buffer<TCont> const &, gap_buffer<TCont> const &);
template<class TCont>
bool operator>(gap_buffer<TCont> const &, gap_buffer<TCont> const &);
template<class TCont>
bool operator<=(gap_buffer<TCont> const &, gap_buffer<TCont> const &);
template<class TCont>
bool operator>=(gap_buffer<TCont> const &, gap_buffer<TCont> const &);
//@}


#include "gap_buffer.ipp"
#endif
