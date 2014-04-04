experiments
===========

This is just a place for me to play around.  Some may turn out to be more useful than others.

functional
=========

Working on creating a sort of functional programming framework for C++.

Right now it's dependent upon C++14.  I may backport eventually.

As of this moment the direction I'm going defines a sort of raw type that
everything must adhere to.  All types must provide an operator() overload.
Any value types must also define a value() function.  Value types will
generally define the former in terms of the latter and return copies for both.
