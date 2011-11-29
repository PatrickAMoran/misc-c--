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

// Include boost::begin and boost::end from Boost.Range
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

// Include boost::iterator_facade from Boost.Iterator
#include <boost/iterator/iterator_facade.hpp>

#include <algorithm>
#include <functional>
#include <limits>

template<class TContainer>
template<class TUnderlying>
struct gap_buffer<TContainer>::iterator_impl
{
public:
  iterator_impl(TUnderlying here,
		bool is_before,
		TUnderlying bef_end,
		TUnderlying aft_beg)
    : is_before(is_before)
    , location(here)
    , before_end(bef_end)
    , after_begin(aft_beg)
  {}
protected:
  struct Incomplete_Type;

  // Grant access to Boost.Iterator
  friend class boost::iterator_core_access;
  // If the \a location iterator belongs to the before container of the
  // gap_buffer
  bool is_before;
  // The underlying iterator into either before or after
  TUnderlying location;
  // The end iterator of before
  TUnderlying before_end;
  // The begin iterator of after
  TUnderlying after_begin;

  // Compare the iterator for equality, as a callback to Boost.Iterator
  bool equal(iterator_impl const & other) const
  {
    return location==other.location;
  }
  // Increment the iterator, as a callback to Boost.Iterator
  void increment()
  {
    ++location;
    if(is_before && location == before_end)
      location = after_begin;
  }
  // Decrement the iterator, as a callback to Boost.Iterator
  void decrement()
  {
    if(!is_before && location == after_begin)
      location = before_end;
    --location;
  }
  // Advance the iterator, as a callback to Boost.Iterator
  void advance(size_type n)
  {
    if(is_before){
      size_type const dist = distance(location, before_end);
      if(dist <= n){
	n -= dist;
	location = after_begin;
      }
    }
    std::advance(location, n);
  }
  // Measure distance between to iterators as callback to Boost.Iterator
  difference_type distance_to(iterator_impl const & other) const
  {
    if(is_before == other.is_before)
      return std::distance(location, other.location);
    else if(is_before)
      return std::distance(location, before_end) + 
	std::distance(after_begin, other.location);
    else
      return std::numeric_limits<difference_type>::max();
  }
};

template<class TContainer>
template<class TUnderlying>
struct gap_buffer<TContainer>::const_iterator_impl
  : private iterator_impl<TUnderlying>
  , public boost::iterator_facade<const_iterator_impl<TUnderlying>,
				  typename std::iterator_traits<TUnderlying>::value_type,
				  iterator_category,
				  const_reference>
{
public:
  const_iterator_impl(TUnderlying here,
		      bool is_before,
		      TUnderlying bef_end,
		      TUnderlying aft_beg)
    : iterator_impl<TUnderlying>(here, is_before, bef_end, aft_beg)
  {}
  
private:
  // Grant access to Boost.Iterator
  friend class boost::iterator_core_access;
  friend class gap_buffer<TContainer>;

  const_reference dereference() const
  {
    return *this->location;
  }
};

template<class TContainer>
template<class TUnderlying, class TConstIter>
struct gap_buffer<TContainer>::nonconst_iterator_impl
  : iterator_impl<TUnderlying>
  , boost::iterator_facade<nonconst_iterator_impl<TUnderlying, TConstIter>,
			   typename std::iterator_traits<TUnderlying>::value_type,
			   iterator_category>
{
public:
  nonconst_iterator_impl(TUnderlying here,
			 bool is_before,
			 TUnderlying bef_end,
			 TUnderlying aft_beg)
    : iterator_impl<TUnderlying>(here, is_before, bef_end, aft_beg)
  {}
  
private:
  // Grant access to Boost.Iterator
  friend class boost::iterator_core_access;
  friend class gap_buffer<TContainer>;

  reference dereference() const
  {
    return *this->location;
  }

  operator TConstIter() const
  {
    return TConstIter(this->location, this->is_before,
		      this->before_end, this->after_begin);
  }
};





template<class TContainer>
template<class TSinglePassCharRange>
typename gap_buffer<TContainer>::size_type
gap_buffer<TContainer>::
insert(TSinglePassCharRange const & rng)
{
  before.insert(before.end(), boost::begin(rng), boost::end(rng));
  return position();
}

template<class TContainer>
typename gap_buffer<TContainer>::size_type
gap_buffer<TContainer>::
insert(value_type const c)
{
  before.insert(before.end(), c);
  return position();
}



template<class TContainer>
typename gap_buffer<TContainer>::size_type
gap_buffer<TContainer>::
position() const
{
  return before.size();
}

template<class TContainer>
typename gap_buffer<TContainer>::size_type
gap_buffer<TContainer>::
size() const
{
  return before.size() + after.size();
}

template<class TContainer>
typename gap_buffer<TContainer>::size_type
gap_buffer<TContainer>::
max_size() const
{
  return before.max_size();
}

template<class TContainer>
bool
gap_buffer<TContainer>::
empty() const
{
  return before.empty() && after.empty();
}

template<class TContainer>
void
gap_buffer<TContainer>::
swap(gap_buffer<TContainer> & other)
{
  other.before.swap(before);
  other.after.swap(after);
}


template<class TContainer>
void
gap_buffer<TContainer>::
advance(typename gap_buffer<TContainer>::difference_type const d)
{
  size_type const magnitude = abs(d);
  if(d == 0)
    return;
  else if(d < 0)
    {
      after.insert(after.begin(), before.end() - magnitude, before.end());
      before.erase(before.end() - magnitude, before.end());
    }
  else
    {
      before.insert(before.end(), after.begin(), after.begin() + magnitude);
      after.erase(after.begin(), after.begin() + magnitude);
    }
}


template<class TContainer>
void
gap_buffer<TContainer>::
erase(typename gap_buffer<TContainer>::difference_type const d)
{
  size_type const magnitude = abs(d);
  if(d == 0)
    return;
  else if(d < 0)
    {
      before.erase(before.end() - magnitude, before.end());
    }
  else
    {
      after.erase(after.begin(), after.begin() + magnitude);
    }
}

template<class TContainer>
typename gap_buffer<TContainer>::iterator
gap_buffer<TContainer>::
here()
{
  return iterator(after.begin(), false, before.end(), after.begin());
}

template<class TContainer>
typename gap_buffer<TContainer>::const_iterator
gap_buffer<TContainer>::
here() const
{
  return const_iterator(after.begin(), false, before.end(), after.begin());
}


template<class TContainer>
typename gap_buffer<TContainer>::reverse_iterator
gap_buffer<TContainer>::
rhere()
{
  return reverse_iterator(after.rend()-1, false, after.rend(), before.rbegin());
}

template<class TContainer>
typename gap_buffer<TContainer>::const_reverse_iterator
gap_buffer<TContainer>::
rhere() const
{
  return const_reverse_iterator(after.rend()-1, false, after.rend(),
				before.rbegin());
}


template<class TContainer>
typename gap_buffer<TContainer>::iterator
gap_buffer<TContainer>::
begin()
{
  return iterator(before.begin(), true, before.end(), after.begin());
}

template<class TContainer>
typename gap_buffer<TContainer>::const_iterator
gap_buffer<TContainer>::
begin() const
{
  return const_iterator(before.begin(), true, before.end(), after.begin());
}

template<class TContainer>
typename gap_buffer<TContainer>::iterator
gap_buffer<TContainer>::
end()
{
  return iterator(after.end(), false, before.end(), after.begin());
}

template<class TContainer>
typename gap_buffer<TContainer>::const_iterator
gap_buffer<TContainer>::
end() const
{
  return const_iterator(after.end(), false, before.end(), after.begin());
}




template<class TContainer>
typename gap_buffer<TContainer>::reverse_iterator
gap_buffer<TContainer>::
rbegin()
{
  return reverse_iterator(after.rbegin(), true, after.rend(), before.rbegin());
}

template<class TContainer>
typename gap_buffer<TContainer>::const_reverse_iterator
gap_buffer<TContainer>::
rbegin() const
{
  return const_reverse_iterator(after.rbegin(), true, after.rend(), 
				before.rbegin());
}

template<class TContainer>
typename gap_buffer<TContainer>::reverse_iterator
gap_buffer<TContainer>::
rend()
{
  return reverse_iterator(before.rend(), false, after.rend(), before.rbegin());
}

template<class TContainer>
typename gap_buffer<TContainer>::const_reverse_iterator
gap_buffer<TContainer>::
rend() const
{
  return const_reverse_iterator(before.rend(), false, after.rend(), 
				before.rbegin());
}


template<class TContainer>
gap_buffer<TContainer>::gap_buffer()
{}

template<class TContainer>
gap_buffer<TContainer>::gap_buffer(gap_buffer const & other)
  : before(other.before)
  , after(other.after)
{}

template<class TContainer>
gap_buffer<TContainer> &
gap_buffer<TContainer>::operator=(gap_buffer const & other)
{
  before = other.before;
  after = other.after;
  return *this;
}


template<class TContainer>
gap_buffer<TContainer>::gap_buffer(size_type n, value_type e)
  : before(n, e)
{}

template<class TContainer>
template<class InputIterator>
gap_buffer<TContainer>::gap_buffer(InputIterator const & i,
				   InputIterator const & j)
  : before(i, j)
{}

template<class TContainer>
typename gap_buffer<TContainer>::reference
gap_buffer<TContainer>::front()
{
  return before.front();
}

template<class TContainer>
typename gap_buffer<TContainer>::const_reference
gap_buffer<TContainer>::front() const
{
  return before.front();
}

template<class TContainer>
typename gap_buffer<TContainer>::iterator
gap_buffer<TContainer>::insert(iterator position, const_reference element)
{
  typename TContainer::iterator iter;
  bool is_before;
  if(position.location == after.begin()){
    before.insert(before.end(), element);
    iter = after.begin();
    is_before = false;
  }else if(position.is_before){
    iter = before.insert(position.location, element);
    is_before = true;
  }else{
    iter = after.insert(position.location, element);
    is_before = false;
  }
  return iterator(iter, is_before, before.end(), after.begin());
}

template<class TContainer>
void 
gap_buffer<TContainer>::insert(iterator position, size_type n,
			       const_reference element)
{
  if(position.location == after.begin())
    before.insert(before.end(), n, element);
  else if(position.is_before)
    before.insert(position.location, n, element);
  else
    after.insert(position.location, n, element);
}

template<class TContainer>
template<class InputIterator>
void
gap_buffer<TContainer>::insert(iterator position,
			       InputIterator const & i, InputIterator const & j)
{
  if(position.location == after.begin())
    before.insert(before.end(), i, j);
  else if(position.is_before)
    before.insert(position.location, i, j);
  else
    after.insert(position.location, i, j);
}

template<class TContainer>
typename gap_buffer<TContainer>::iterator
gap_buffer<TContainer>::erase(iterator position)
{
  bool is_before = position.is_before;
  TContainer & to_erase_from = (is_before ? before : after);
  typename TContainer::iterator iter = to_erase_from.erase(position.location);

  if(iter == before.end()){
    is_before = false;
    iter = after.begin();
  }
  return iterator(iter, is_before, before.end(), after.begin());
}

template<class TContainer>
typename gap_buffer<TContainer>::iterator
gap_buffer<TContainer>::erase(iterator start, iterator end)
{
  typename TContainer::iterator
    before_begin = (start.is_before ? start.location : before.end()   ),
    before_end   = (end.is_before   ? end.location   : before.end()   ),
    after_begin  = (start.is_before ? after.begin()  : start.location ),
    after_end    = (end.is_before   ? after.begin()  : end.location   );

  bool const use_after =
    (after_end != after.begin()) || 
    ((before_end == before.end()) && (before_begin != before.end()));
  
  typename TContainer::iterator
    before_rtn = before.erase( before_begin, before_end ),
    after_rtn  = after. erase( after_begin,  after_end  );

  return iterator((use_after ? after_rtn : before_rtn), use_after, 
		  before.end(), after.begin());
}

template<class TContainer>
void
gap_buffer<TContainer>::clear()
{
  before.clear();
  after.clear();
}


template<class TContainer>
void
gap_buffer<TContainer>::resize(size_type n, value_type const & e)
{
  if(n <= before.size() || after.empty()){
    after.clear();
    before.resize(n, e);
  }
  else
    after.resize(n - before.size());
}

template<class TContainer>
bool
operator==(gap_buffer<TContainer> const & lhs, 
	   gap_buffer<TContainer> const & rhs)
{
  return (lhs.size() == rhs.size()) && 
    std::mismatch(lhs.begin(), lhs.end(), rhs.begin()) == rhs.end();
}
template<class TContainer>
bool
operator!=(gap_buffer<TContainer> const & lhs, 
	   gap_buffer<TContainer> const & rhs)
{
  return !(lhs == rhs);
}

template<class TContainer>
bool
operator<(gap_buffer<TContainer> const & lhs, 
	  gap_buffer<TContainer> const & rhs)
{
  return std::lexicographical_compare(lhs.begin(), lhs.end(), 
				      rhs.begin(), rhs.end());
}
template<class TContainer>
bool
operator<=(gap_buffer<TContainer> const & lhs, 
	   gap_buffer<TContainer> const & rhs)
{
  return
    std::lexicographical_compare(lhs.begin(), lhs.end(), 
				 rhs.begin(), rhs.end(),
				 std::less_equal<
				   typename TContainer::value_type>());
}

template<class TContainer>
bool
operator>(gap_buffer<TContainer> const & lhs, 
	  gap_buffer<TContainer> const & rhs)
{
  return (rhs < lhs);
}
template<class TContainer>
bool
operator>=(gap_buffer<TContainer> const & lhs, 
	   gap_buffer<TContainer> const & rhs)
{
  return (rhs <= lhs);
}
