================================================================================
                    Image similarity experiments
================================================================================

I've been using geeqie__ for years; this is a nice, well written image viewer.

__ http://geeqie.org/

Among many other features, **geeqie** implements "image similarity" function.
A user can drag images and find visually similar ones. I seldom use this
function, but as a programmer was curious how it's done. When I looked at
the sources I found that the code can be boosted, i.e. vectorized.

To make experiments easier I do them in separate repository, but in the
way they can be easily included into the main program (if the author would
like to do this, of course).

Overview
--------------------------------------------------------------------------------

The main input to similarity find is a list of 32 x 32 images, the scaled
input pictures. These *fingerprints* are RGB images where each (?) channel
is **normalized**.

32 x 32 = 1024 is a nice, SIMD-friendly number.


Normalization
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Normalization is done in two steps:

* Fand min and max value in image.
* Scale values to range [0 .. 255]; for instance, if a channel happens
  to have just three values 5,6,7, they are transformed into 0, 127 and 255.
  The expression is like this: ``(x[i] - min) * 255.0 / (max - min)``.

At the moment the above expression is written directly; a compiler
is able to vectorize such expression, but is forced to perform all
operations using floating point instruction.

Since range (``max - min``) can be 0 .. 255, I propose to precompile
specialisations for each value. A specialisations can use:

* instruction ``PSHUFB`` for ranges 1 .. 16;
* approximate ``255.0 / range`` with integer multiplication followed
  by shift right (``PMULL{LO/HI}``, ``PSRLI``); the multiplier
  and shift amount were found by a program (not all ranges are
  covered, though);
* if range = 0, simple ``memset(data, 0, size)`` is sufficient;
* if range == 255 nothing has to be done.

The above methods were implemented, here are results from my
computer (output from ``normalization/speed``)::

    empty   :     0.210 cycle/op (best)    0.305 cycle/op (avg)
    scalar  :     2.333 cycle/op (best)    2.393 cycle/op (avg)
    SSE     :     0.794 cycle/op (best)    0.825 cycle/op (avg)

SSE version is ~2.5 times faster.


Fingerprint comparison
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

During similarity check, the two fingerprints are compared. The comparison
calculates sum of absolute values. This is what the dedicated instruction
``PSADBW`` does. [Not started yet]
