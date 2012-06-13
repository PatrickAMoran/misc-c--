Gap Buffer
==========

Description
-----------
This directory contains an implementation of the gap-buffer data structure.
This is implemented as a container adapter, much like std::stack or std::queue.
The gap_buffer container is a C++ STL container.

For more information on what a gap buffer is, check out the information
Wikipedia article at http://en.wikipedia.org/wiki/Gap_buffer.

This implementation is header-only, so no compilation is required.  It's only
dependencies are an STL implementation, Boost.Range and Boost.Iterator.  Boost
documentation suggests that this should work on any boost 1.32.0 or newer.  This
is a pure C++03 implementation, no C++11 features are required.


Status
------
This particular implementation lacks its first major user, but is expected to
perform correctly.  Feel free to use it, and if it breaks on you, I will fix
bugs prompty.


TODO
----
Planned improvements to this class template include:

* Make doxygen report 'boost::enable_if_c<condition, type>' as 'type'
* Alter member functions to not actually move data until the insertion point is
  used
* Employ code-coverage tools to ensure test coverage
* Profile the implementation