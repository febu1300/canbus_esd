>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
        Information about esdcan CAN driver build workaround
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

**This document describes the esdcan driver revision 4.1.4-r1.**

.. This document is formatted as reStructuredText (*.rst).


_`1)` Necessity of a build workaround for Linux kernels >= 5.6*
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

The files contained in this archive belong to *esd*'s new **esdcan** CAN driver.
Due to changes in the build infrastructure of the Linux kernel with
kernel release 5.6 and later the delivered **esdcan** driver could not
be built at the customer's site (split build) to adjust the driver module's
interface to the running kernel.

This workaround release (-r1) only incorporates a workaround to mitigate
the build failures on these newer kernels.

No further change since the release of **esdcan** revision 4.1.4 is intended.
None of the C source files was changed.

**Note**:
    Please ignore the installation instructions from the manual
    "CAN-API, Part 2, Installation Manual". Use the installation instructions
    included in the README.rst. The recommmended way is to utilize the
    DKMS framework as described in section "3.2) Recommended driver 
    installation using DKMS".


_`2)` Revision and Contact
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
::

  2023-02-06 / MJ
  ____________________________________
  (c) esd electronics gmbh 2004 - 2023

  esd electronics gmbh
  Vahrenwalder Str. 207
  30165 Hannover
  GERMANY
  Phone: +49-511-372 98-0
  Fax: +49-511-372 98-68
  E-mail: info@esd.eu
  Internet: www.esd.eu
  ____________________________________
  Doc-Typ-No.: SD31200

.. Automatic section numbering only to check the manual one.
.. .. sectnum::
..     :suffix: )
..     :start: 0
