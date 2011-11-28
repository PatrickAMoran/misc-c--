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
This particular implementation is in an active-development status,  It is not
stable or finished.  It is likely that it contains errors.  The absence of any
testing should hint you as to the quality of this code.  However, watch this
repository for improvement.

TODO
----
Planned improvements to this class template include:

* General cleanup / reorganization of the ipp file.
* Make gap_buffer model Sequence
* Make doxygen report 'boost::enable_if_c<condition, type>' as 'type'
* Add lots more testing and verification
* Use Boost.ConceptCheck to provide users with better error messages