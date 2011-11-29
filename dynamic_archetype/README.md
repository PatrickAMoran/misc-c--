Dynamic Archetypes
==================

Description
-----------
The Boost.ConceptCheck library defines an archetype as a class that minimally
covers a Concept.  That is, it provides the least-specific class possible that
is still a model of the concept.  They also provide a number of these archetypes
for STL concepts.

However, these archetypes are purely compile-time checks.  The actual
implementations of the member functions are stubbed out and may not do anything
related to what a reasonable implementation would do.  For example, the archetype
for EqualityComparable could simply always return true for operator==.

Dynamic Archetypes are an attempt to take archetypes one step further.  Their
purpose is also to minimally cover a concept, but to provide additional extrinsic
utilities that allow you to verify that they actually behave the way they ought
to.

The purpose of this is to provide a good tool for writing good tests of generic
algorithms.  For example, to test an implementation of std::sort, you could use
a dynamic archetype for RandomAccessIterator that dereferences to a dynamic
archetype for LessThanComparable.


Status
------
This code is in active development. It is not stable or finished.