>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
            README for esdcan CAN driver
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

**This document describes the esdcan driver of version > 4.0.0.**

.. This document is formatted as reStructuredText (*.rst).

.. contents:: Table of Contents

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    _`0)` Introduction
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

The files contained in this archive belong to *esd*'s new **esdcan** CAN driver.
It is developed with a wide variety of operating-systems and architectures in
mind and is flexible enough to meet the requirements of future CAN protocols.

The default Linux driver archive supports Linux kernels from 2.6.10 on and many
revisions later. It is built regularly against the head of Linus Torvald's
`kernel tree`_ (which is at the time of this writing @ 5.1-rc4).

.. _kernel tree: https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git/

The **esdcan** Linux driver is delivered in different versions, please use the
correct archive. The archive name is created by the following naming scheme:

    esdcan-<card-id>-linux-<arch>-<ver>.tgz

On 32-Bit x86 systems <arch> is "x86". On 64-Bit x86 systems <arch> is "x86_64"

Please see section `2)`_ for the <card-id> used in the naming scheme of driver
archives.

For kernels 2.4.x the driver can still be used but has to be prepared on demand.
Talk to our sales department (sales@esd.eu) if you need this.

Your application program will not be talking directly with the driver but with
the NTCAN API library that wraps the driver's interface to the known *esd* NTCAN
API interface. It is strongly recommended to use the ``libntcan.so`` shared
library provided in the driver archive and to avoid any static ``libntcan.a``
binaries you may have from older installations.

++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    _`1)` Archive Content
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Example file tree for a 64-bit multi-arch release. In a 32-bit only
release the ``bin64`` and ``lib64`` directories and their content is not present.

::

    release/esdcan-pci331-linux-x86_64-3.10.7
    |-- bin32
    |   |-- cantest
    |   `-- updpci331
    |-- bin64
    |   |-- cantest
    |   `-- updpci331
    |-- example
    |   `-- cantest.c
    |-- include
    |   `-- ntcan.h
    |-- lib32
    |   `-- libntcan.so.4.0.0
    |-- lib64
    |   `-- libntcan.so.4.0.0
    |-- src
    |   |-- miscelleaneous sources ...
    |   |-- :
    |   :
    |-- udev
    |   `-- esdcan-pci331-dev.rules
    |-- config.mk
    |-- Makefile
    |-- README
    `-- RelnotesLinux.html


----------------------------------------------------------------------
_`1.1)` Detailed File Description
----------------------------------------------------------------------

*config.mk:*
        The configuration file for the compile process. You might need to
        edit this file, in order to suit any peculiarities of your system.
        (mainly the KERNELPATH, you'll see later)


*Makefile:*
        Lets you build the driver without any programming knowledge and
        without the need to touch any of the source files.


*README.rst:*
        This file


*lib(32|64)/libntcan.so.v.mv.r:*
        The ntcan library as dynamic-shared-library (``.so``).

        _`Note on dynamic shared library`:
        The ``.so`` suffix is followed by three dot-separated numbers (.v.mv.r).
        These are version (.v), minor-version (.mv, which has **nothing** to
        do with a device's minor number) and a release-number (.r).
        The version (.v) changes, if there are incompatible changes in the
        library's interface. We at *esd* think this shouldn't change at all,
        providing you as a user with a consistent interface.
        Minor-version (.mv) will be changed, if there are major changes in
        functionality or additional features.
        Bugfixes will lead to an increasing release-number (.r).
        WARNING:  See section `8)`_ below for a binary incompatibility issue.


*include/ntcan.h:*
        Header for the NTCAN API / library

        THIS IS THE ONLY HEADER, YOU SHOULD INCLUDE IN YOUR APPLICATIONS
        (with the exception of ``irigb.h``, see below).
        PLEASE DO NOT USE ANY DEFINES LOCATED IN ANY OF THE OTHER HEADERS,
        IN ORDER TO KEEP YOUR APPLICATIONS WORKING WITH FUTURE VERSIONS OF
        THE DRIVER.


*example/cantest.c:*
        Source of an example application.


*bin{32|64}/cantest:*
        The 32-bit and the 64-bit variants of ``cantest`` were built from the
        same source file provided as example/cantest.c.
        Binary of example programm, see section `7)`_ for further explanation.

        .. NOTE::
            ``cantest`` needs the ``libntcan.so``, please assure, that it is
            able to find the correct version. Refer to installation in section
            `3.3)`_ or set your LD_LIBRARY_PATH appropriately. In any case you
            need to make sure, that the latest delivered version is used.


*src/\*:*
        Source / object / config files:

        This driver is released as a combination of binary-objects (``*.o``) and
        source files (``*.c`` and ``*.h``). This way *esd* can provide a CAN
        driver working with many different Linux kernels. The source files are
        **NOT** under the GPL! You're not allowed to modify, redistribute or
        sell the files. They are intellectual property of *esd electronics GmbH*.

        .. CAUTION::
            Don't try to use any defines or data structures located in these
            files in your own sources. This will lead to non-working
            applications in the future.


*bin{32|64}/upd<card-id>:*
        The optional firmware update tool. This tool is only delivered with
        boards that have an updateable firmware. Such boards are
        CAN-PCI*/402(-FD), CAN-(C)PCI/331, CAN-(C)PCI/360 and CAN-ISA/331.

        With some boards this tool can be used to switch between CAN-2.0-A /
        CAN-2.0-B firmware mode.

        Replace <card-id> with the appropriate card-id from the table in section
        `2)`_ to derive the program's name (e.g. it's called ``updpci331`` for a
        CAN-PCI/331).

        .. NOTE::
            Use this tool in combination with the delivered driver, only!

            This tool can also be used to switch the firmware mode of cards
            (only (c)pci331, (c)pci360 and isa331 need such switching) between
            CAN-2.0-A-firmware (processes CAN-messages with 11-bit-identifiers,
            only) and CAN-2.0-B-firmware (processes both CAN-messages with
            11-bit- and 29-bit-identifiers) mode.

        USAGE examples:

        To update the firmware of a CAN-PCI/331 replace <net> with a net
        number assigned to your CAN-PCI/331 in this command:

            "updpci331 <net>"

        To switch a CAN-PCI/331 between 2.0-A and 2.0-B mode replace <x> with
        "a" or "b" depending on the firmware mode you want to use and <net>
        with a net-number assigned to your CAN-PCI/331:

            "updpci331 -t<x> <net>"

        .. NOTE::
            The mode switching is needed only on CAN-(C)PCI/331, CAN-(C)PCI/360
            and CAN-ISA/331. It is not possible nor needed on CAN-PCIe/402.

        .. admonition:: Note for CAN-CPCI/360 only

            Normally you'll use the tool as described above. There's one
            exception: With certain old CAN-CPCI/360 boards the update might
            fail and you end up with a CAN board with only one CAN net. For this
            case there's a second version of the update tool with the suffix
            "rt". The update will take significantly longer, but this will fix
            the problem.


----------------------------------------------------------------------
_`1.2)` Detailed File Description for (optional) IRIG-B files
----------------------------------------------------------------------

Only old driver releases for CAN-PCI/400 (especially PMC-CAN/400-4I) contained
the IRIG-B library. But current driver archives don't contain the IRIG-B libary
any more. It's provided as an extra download or on the CAN CD.

You'll only need the IRIG-B library, if you have a IRIG-B capable CAN-board,
like PMC-CAN/400-4I. For reference only the provided files are listed below.

*include/irigb.h:*
        Header for the IRIG-B API library

*lib{32|64}/libirigb.so.v.mv.r:*
        IRIG-B library as dynamic-shared-library (``*.so``).
        See `Note on dynamic shared library`_ on ``libntcan.so`` above.

*doc/irigb_esd.html:*
        API manual for IRIG-B library.


++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    _`2)` Card ID's and default driver parameters
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

The Card IDs are card family names used in the naming scheme of driver archives
and for the kernel thread utilized by the driver. The major numbers shown in the
tables are the default values used by the driver for a certain board. The driver
name from the table is used as part of a shell script's name used to initialize
the device inodes (see sections `3.2.2)`_ or `3.6.5)`_).

The following table shows PCI/PCIe bus based CAN interface boards:

=================  ===========  ======  ===============================
esd card name      card-id      major   driver name
=================  ===========  ======  ===============================
AMC-CAN4           amc4         54      CAN_ESDACC
CAN-CPCI/331       pci331       50      CAN_PCI331
CAN-PCI/331        pci331       50      "
CAN-PMC/331        pci331       50      "
CAN-PCI/360        pci360       51      CAN_PCI360
CAN-CPCI/360       pci360       51      "
CAN-CPCI/200       pci200       54      CAN_PCI2xx
CAN-PCIe/200       pci200       54      "
CAN-PCI/200        pci200       54      "
CAN-PCI/266        pci200       54      "
CAN-CPCI/405       pci405       53      CAN_PCI405
CAN-PCI/405        pci405       53      "
CAN-PCI/400        esdaccbm     55      CAN_ESDACC
CAN-PCIe/400       esdaccbm     55      "
CPCI-CAN/400       esdaccbm     55      "
PMC-CAN/400        esdaccbm     55      "
CAN-PCIe\*/402\*   pcie402      52      CAN_PCIe402
CAN-PCI/402*       pcie402      52      "
CPCI\*-CAN/402\*   pcie402      52      "
=================  ===========  ======  ===============================


The following table shows ISA/PC104 bus based CAN interface boards and the
default values for the IO address and the IRQ number used by the driver for
the first board.

=================  ===========  ======  =========== ======  ==========
esd card name      card-id      major   driver name io      irq
=================  ===========  ======  =========== ======  ==========
CAN-ISA/331        isa331       52      CAN_ISA331  0x1e0   5
CAN-PC104/331      isa331       52      "           0x1e0   5
CAN-ISA/200        isa200       53      CAN_ISA200  0x1e8   5
CAN-PC104/200-I    pc104_200i   54      PC104_200I  0x1e8   5
=================  ===========  ======  =========== ======  ==========


The following table shows USB bus based CAN interfaces:

=================  ===========  ======  ===============================
esd device name    card-id      major
=================  ===========  ======  ===============================
CAN-USB-Mini       usb331       50      Support dropped (GPL only symbol issues)
CAN-USB/2          ---          --      SocketCAN support only
CAN-USB/Micro      ---          --      SocketCAN support only
CAN-USB/400        ---          --      unsupported
=================  ===========  ======  ===============================


*esd* also provides a virtual CAN driver that implements a virtual CAN interface
for testing purposes without requiring a real CAN hardware. This driver can be
downloaded from the website under "Support"->"Downloads"->"Software". In the
filter mask enter "virtual" to get access to the virtual CAN driver.

=================  ===========  ======  ===============================
esd card name      card-id      major   driver name
=================  ===========  ======  ===============================
CNULL              null_fd      51      CAN_VIRTUALFD
=================  ===========  ======  ===============================


The major number that the driver uses is shown on startup in the system log
file. The default major number almost always works.



++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    _`3)` Installation
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

During the installation process you will compile and install the driver itself
and eventually some configuration files.

But to use it from an user space program you also need to install the NTCAN
API library with its (optional) configuration files.

As prerequisites to compile the driver you will need to have these packages
installed on your Debian / Ubuntu machine:

  - make
  - gcc
  - libelf-dev

The following steps are needed to accomplish the installation:

  - `3.1)`_   Unpack the archive
  - `3.2)`_   Recommended driver installation using DKMS

    - `3.2.1)`_ Install the driver using the DKMS framework
    - `3.2.2)`_ Prepare system to have device nodes /dev/can*
    - `3.2.3)`_ Prepare to load the driver module (automatically)

  - `3.3)`_   Installation of the NTCAN library
  - `3.4)`_   Installation of the cantest tool
  - `3.5)`_   Adjust the driver's thread priority

  - `3.6)`_   Manual driver installation in case of problems



----------------------------------------------------------------------
_`3.1)` Unpack the archive
----------------------------------------------------------------------

As a common first step extract the driver from the archive:

All drivers released after July 2012 are delivered in a ``*.tgz`` archive
instead of a password protected ZIP file.

Extract the driver directory from the ``*.tgz`` file::

    tar xzf esdcan-<card-id>-linux-<arch>-<ver><-ext>.tgz

card-id
    card-id (e.g.: "pci200" or "pci405" ...)

arch
    host-architecture (e.g.: "x86" or "x86_64")

ver
    driver version (e.g.: 3.7.2)

ext
    extension (applicable to certain cards and old releases only,
    e.g.: "-gcc2", "-gcc_4.6" or "-smp")


You will end up with a directory named as the archive's basename.



----------------------------------------------------------------------
_`3.2)` Recommended driver installation using DKMS
----------------------------------------------------------------------

With most Linux distributions (tested with Debian/Ubuntu) you should be able to
build and install the driver using the DKMS framework. This will have the
benefit that on a kernel update the DKMS framework will automatically compile
and install the driver for the new kernel.

Another benefit is that on a machine which has UEFI Secure Boot enabled the DKMS
framework will sign the driver with an appropriate key.


_`3.2.1)` Install the driver using the DKMS framework
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Change into the directory that was created by the unpacking action::

    cd ./esdcan-<card-id>-linux-<arch>-<ver>-<ext>

For the next steps you need to be **root**. In the mentioned directory enter the
command

::

    dkms add ./src

which utilizes the ``./src/dkms.conf`` file to add a new package to the DKMS
framework. The next command should build the driver which uses from
``dkms.conf`` the *PACKAGE_NAME* as <pkg-name> parameter and *PACKAGE_VERSION*
as <pkg-ver> parameter.

The commmand pattern ``dkms build <pkg-name>/<pkg-ver>`` results then for
example in::

    dkms build esdcan-pcie402-linux-x86_64/4.0.1

The next step is to install the driver for the currently running kernel in the
``/lib/modules/`` tree by issuing a command pattern
``dkms install <pkg-name>/<pkg-ver>``. The same example results then in the
command::

    dkms install esdcan-pcie402-linux-x86_64/4.0.1

This should also do any further installation work like ``depmod``. The
installation directory for the kernel module depends on the configuration of the
DKMS framework.

If you at some time want to get rid of the installed driver for all kernel
versions then issue a command following the pattern
``dkms remove <pkg-name>/<pkg-ver> --all``::

    dkms remove esdcan-pcie402-linux-x86_64/4.0.1 --all



_`3.2.2)` Prepare system to have device nodes ``/dev/can*``
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

To enable the system to create the needed device nodes automatically you must
drop the udev rules file from the udev subdirectory of the release archive in
the directory ``/etc/udev/rules.d/`` of your system.

After that you should try to load the kernel module manually with modprobe to
test the setup. Use ``modprobe -v esdcan-<card-id>``. Then examine the ``dmesg``
output if the driver was loaded successfully. At last you should check if the
needed ``/dev/can*`` inodes are present now.


.. note::
    The provided rules file makes the udev daemon execute a script file that is
    exported by the esdcan driver under ``/proc/bus/can/<driver-name>/inodes``
    to create the necessary inodes with the necessary major number. See section
    `2)`_ for <driver-name> values.



_`3.2.3)` Prepare to load the driver module (automatically)
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

For the PCI based boards the esdcan CAN driver will be loaded automatically by the
Linux kernel's PCI subsystem.

This is not the case for ISA/PC104 based boards. This holds true also for the the
the virtual CAN driver because it has no associated hardware. For automatic
loading of the kernel module please have a look at the man-pages of "modules" and
"depmod" or "modules.conf".

Automatic loading of the kernel module can be accomplished on current systems
using the ``/etc/modules`` configuration file or by dropping a
``esdcan-<card-id>.conf`` in the directory ``/etc/modules-load.d/`` if it
exists. ::

    # /etc/modules-load.d/esdcan-<card-id>.conf: load esdcan driver module at boot time
    esdcan-<card-id>


.. note::
    For the pci200 card family also a Linux SocketCAN driver exists in the
    mainline kernel. This driver may get preferred and therefore the loading of
    the esdcan driver may fail.

In this case you could blacklist the SocketCAN driver by installing a file
``blacklist-socketcan.conf`` under ``/etc/modprobe.d/`` with the following
content::

    # Prevent modules of Socket CAN from grabbing esd CAN boards
    blacklist	plx_pci
    blacklist	sja1000
    blacklist	can_dev


----------------------------------------------------------------------
_`3.3)` Installation of the NTCAN library
----------------------------------------------------------------------

.. NOTE::
    Since there's a vast variety of different linux distributions out there,
    following installation notes are a suggestion, only.

The installation needs to be performed as **root** user. The 32-bit library is
provided in the ``lib32`` directory and the 64-bit library in the ``lib64``
directory, respectively. At the moment there is no installation tool and you
need to copy the files manually.

The shared library should belong to user and group **root** with the following
file access permissions: u=rwx, g=rx, o=rx (i. e. 0755). Don't forget to call
``ldconfig`` to complete the installation.

The resulting tree after the installation of the libraries, include files,
creating links and concluding ``ldconfig`` call should look like this on either
a 32-bit or a 64-bit system::

    /
    `-- usr
        `-- local
            |-- include
            |   `-- ntcan.h
            `-- lib
                |-- libntcan.so -> libntcan.so.4
                |-- libntcan.so.4 -> libntcan.so.4.0.0
                `-- libntcan.so.4.0.0

If you install on a 64-bit multi-arch system you may want to install
additionally the 32-bit libraries. The resulting tree will look as shown below.
Note the addition of the file ``esd-usr-local-lib32.conf`` at
``/etc/ld.so.conf.d/`` that adds the ``/usr/local/lib32/`` path to the ``ld.so``
search path for libraries.

::

    /
    |-- etc
    |   |-- esd-plugin		(optional, see note on plugin interface below)
    |   `-- ld.so.conf.d
    |       `-- esd-usr-local-lib32.conf
    `-- usr
        `-- local
            |-- include
            |   `-- ntcan.h
            |-- lib
            |   |-- libntcan.so -> libntcan.so.4
            |   |-- libntcan.so.4 -> libntcan.so.4.0.0
            |   `-- libntcan.so.4.0.0
            `-- lib32
                |-- libntcan.so -> libntcan.so.4
                |-- libntcan.so.4 -> libntcan.so.4.0.0
                `-- libntcan.so.4.0.0


Content of ``esd-usr-local-lib32.conf``::

    # 32-bit only configuration (esd 32-bit libraries)
    /usr/local/lib32

.. note::
    You need to think about the file */etc/esd-plugin* only if you use the
    plugin interface of the NTCAN library either for the SocketCAN wrapper
    plugin library or the EtherCAN plugin library. Please refer to the
    documentation of these plugin libraries for details.

Please note also that the NTCAN library delivered with this esdcan driver was
compiled without the plugin interface and you **MUST** use the NTCAN library
distributed with the mentioned plugin libraries to make them work.


----------------------------------------------------------------------
_`3.4)` Installation of the cantest tool
----------------------------------------------------------------------

Provided that the NTCAN library was installed correctly you can install the
``cantest`` tool anywhere in your PATH. Find the documentation on the use of
``cantest`` in the NTCAN API Manual (Part 1).

This tool can be used to verify the installation / operating state of the CAN
driver and ``libntcan.so`` installation.


----------------------------------------------------------------------
_`3.5)` Adjust the driver's thread priority
----------------------------------------------------------------------

As a last step you should ponder on the driver's thread priority if it fits your
and your application's needs. Please refer to section `5)`_ for details.

At last at this point the CAN driver and library are completely prepared and the
installation is done.


----------------------------------------------------------------------
_`3.6)` Manual driver installation in case of problems
----------------------------------------------------------------------

The manual installation has the advantage that you have more control over the
intermediate steps and you will see any error messages more directly.

_`3.6.1)` Compile the driver:
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Enter the directory that was created by extracting the driver's archive::

    cd ./esdcan-<card-id>-linux-<arch>-<ver>-<ext>

In some cases you need to edit a configuration-file for the compilation:
In ``config.mk`` you need to set the variable KERNELPATH correctly.
Normally the default path ``/usr/src/linux/`` should be correct. If your
linux configration differs from the standard, correct the line accordingly::

    KERNELPATH = /your-path-to-the-kernel-source

You might need to be **root** to compile the driver.

Compiling the driver is simple by typing::

    make

For some card's there are warnings please refer to the section `9)`_ Known
Issues.

If you build against a Linux kernel 2.6.x or later the driver file is called
``esdcan-<card-id>.ko`` and is generated inside of the ``src`` subdirectory.

For example the CAN-PCI/331 the driver file is found at::

    ./src/esdcan-pci331.ko


_`3.6.2)` Install the driver:
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

The driver-module should be placed somewhere in ``/lib/modules/<kernel-release>/``.
The <kernel-release> has to be replaced by the according string of your system,
which you can get by::

    uname -r

After doing this you need to run ``depmod -a`` to update the kernel's
database of known modules.


_`3.6.3)` Install the NTCAN library
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

The ``libntcan.so.v.mv.r`` should be installed like it is shown in section `3.3)`_.


_`3.6.4)` Installation of the cantest tool
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Please refer to section `3.4)`_.


_`3.6.5)` Prepare the device inodes ``/dev/can*`` (as superuser):
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

The libntcan.so library uses the ``/dev/can*`` device nodes to access the driver.
Depending on the age of your Linux distribution there are three methods around
to get the device nodes available at run time:

    a) Automatic creation by the udev daemon on driver load
    b) Automatic (re-)creation of manually added template inodes by the udev daemon
    c) Manual creation of ``/dev/can*``

If your Linux distribution uses an udev daemon you must utilize one of the
methods a) or b) because the inodes in ``/dev`` vanish during reboot. The
method c) is only suitable if your Linux distribution is old enough so that
the device inodes stay around during a system reboot.

To assist with this task the esdcan driver exports a script file under
``/proc/bus/can/<driver-name>/inodes`` that can be utilized to create the inodes
with the correct major number. Refer to section `2)`_ for <driver-name> values.
But to look at that script you must load the driver at first as described in
section `3.6.6)`_ below.


Detailed description of the three methods:

    a)  Automatic creation by udev rule on load of esdcan-<card-id>

        The nowadays recommended way to automatically add the ``/dev/can*``
        inodes is to use an udev rule which is provided in the file
        ``esdcan-<card-id>.rules``. This file must be installed in one of the
        directories the udev daemon looks into.

        Under Debian / Ubuntu the directory for local rules is
        ``/etc/udev/rules.d/``. The provided ``esdcan-<card-id>.rules`` file
        contains a similar rule as this example for the pci331 card family::

            # udev rule to create device nodes for the esdcan CAN driver: esdcan-pci331.ko
            ACTION=="add", SUBSYSTEM=="drivers", KERNEL=="CAN_PCI331", RUN+="/bin/sh /proc/bus/can/%k/inodes"


    b)  Semi-automatic recreation of device node after reboot

        If you have dynamically created device nodes (i. e. any manually created
        ``/dev/can*`` vanish after a reboot) then you may get the device nodes
        created semi-automatic if you do create the device nodes in the special
        directory ``/lib/udev/devices/``. Refer to the exported ``inodes``
        script for correct parameters for the ``mknod`` command. This is only
        possible on medium aged systems if ``/lib/udev/devices/`` is present.


    c)  Manually creating a the CAN device inodes

        The the last resort to create the device nodes is to do it manually like
        this after you loaded the esdcan driver (see section `3.6.6)`_)::

            sudo /bin/sh /proc/bus/can/<driver-name>/inodes <base-net>

        It works if your kernel doesn't dynamically create the device nodes at
        boot time. If this is the case and udev is not an option that command
        needs to be executed from one of the system initialisation scripts.

        The parameter <base-net> for the ``inodes`` script specifies the first
        network number that the script should use for this driver. It defaults
        to zero. The parameter may be needed if you have *esd* CAN interfaces of
        different type in your system that need different drivers to work.

        Example:

        You have a CAN-PCIe/402 and a CAN-PCI405-B4 board with each 4 CAN
        interfaces installed. Then you could call the scripts like this to have
        the CAN-PCIe/402 networks start at 0 and the CAN-PCI405 networks start
        at 4::

            sudo /bin/sh /proc/bus/can/CAN_PCIe402/inodes
            sudo /bin/sh /proc/bus/can/CAN_PCI405/inodes 4



_`3.6.6)` Testwise load the driver (as superuser):
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Now you should load the driver manually as **root** to check if any problems
show up. You use the ``modprobe`` command as follows::

    modprobe -v esdcan-<card-id>.ko [verbose=V] [mode=MO]

:verbose:   verbose mask (e.g. verbose=0x00000003, default 0x00000000)
:mode:      mode flags (e.g. mode=0x20000000, default 0x00000000)

The verbose output will show you the resulting ``insmod`` command with all
parameters coming eventually from any installed option files (refer to
section `4)`_ Driver parameters).

Look in your system log or inspect the ``dmesg`` output for any messages of the
driver!



++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    _`4)` Driver parameters
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

These driver parameters can be given as parameters for the ``insmod`` command.
Or if the module is loaded automatically or manually by ``modprobe`` you can
create a file ``/etc/modprobe.d/esdcan-options.conf`` which is evaluated by the
module loader. Please see "man modprobe.d" for details.

*major*
    May be used to set a non-default major number for the driver. If you don't
    know, what major numbers are or have no problems loading the driver and/or
    using the driver, then leave this parameter alone. The major number that the
    driver uses is shown on startup in the system log file. The default major
    number almost always works.

*verbose*
    By setting the verbose mask (value range 0x00000000 to 0x000000FF) you can
    influence the amount of messages the driver writes to your system log on
    startup. The value is interpreted bitwise, every additional bit may add more
    output.

*mode*
    This parameter is hardware dependent. Do not set any bits, which are not
    valid for your hardware, as this might lead to unexpected results. On most
    CAN boards there should be no need to set any bit at all.

    + Global mode settings for the esdcan driver:

        :0x00000040: Force LOM (off by default)

            All baudrates set will get the ListenOnlyMode flag added, i. e. the
            board will never drive the CAN bus.

        :0x00000080: Smart disconnect (off by default)

            After the last NTCAN handle is closed for a CAN network the baudrate
            will automatically be set to NTCAN_NO_BAUDRATE, i. e. the CAN
            controller enters **RESET** mode.


    + All boards with esdACC controller:

        :0x20000000:
            Select RX Timestamping mode (0: EOF (default), 1: SOF) Available
            since firmware version > 0.34d.

        :0x02000000:
            Select calculation mode for measured autobaud (0: Rounded bitlength
            (default), 1: Table based). Available since firmware version > 0.47d
            but may be unsupported by the driver.


    + All boards utilizing the pcie402 driver (i. e. CAN-PCI\*/402\* ):

        :0x40000000:
            Don't use MSI interrupts but fall back to legacy interrupts.
            Available since driver 3.10.5.


    + CPCI-CAN/400-4I-PXI:

        :0x10000000:
            May be used to choose the timestamp source on driver startup
            (0: IRIG-B (default), 1: PXI)

        :0x08000000: May be used to invert the Start-Trigger-Input


*io, io1, io2, io3, ...*
    Provide the base adresses of I/O space for the 1st, 2nd, 3rd and so on CAN
    card on the ISA/PC104 bus.


*irq, irq1, irq2, irq3, ...*
    Provide the interrupt number for the 1st, 2nd, 3rd and so on CAN card on the
    ISA/PC104 bus.

This is an example to enable Start Of Frame (SOF) timestamping for the PCIe402.
Put this in the file ``/etc/modprobe.d/esdcan-options.conf``::

    # Enable Start Of Frame (SOF) timestamping for PCIe402
    options esdcan-pcie402 mode=0x20000000



++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    _`5)` Driver's thread priorities
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

----------------------------------------------------------------------
_`5.1)` Basic information on the driver's kernel thread
----------------------------------------------------------------------

The driver starts a kernel thread (dpc-thread) that distributes the received
CAN frames to the user handles. If a process is waiting (in a *canRead()* for
example) it is also released from its wait state.

The driver's kernel thread gets its name derived from the name of the kernel
module that contains the driver for your board. Examples::

    esdcan-amc4.ko          kesdcan_amc4
    esdcan-esdaccbm.ko      kesdcan_esdaccbm
    esdcan-pci405.ko        kesdcan_pci405
    esdcan-<card-id>        kesdcan_<card-id>

If you install three cards of the same type you will still have a single kernel
thread. But if you install different types of boards you will get a different
named kernel thread for each type of board.

The different names make it easier to distinguish which thread is responsible
for which group of cards (of the same type).

Older drivers used always the name "dpc thread" for their kernel threads (with
a literal blank in the name).


----------------------------------------------------------------------
_`5.2)` Advice for priority selection for the driver's kernel thread
----------------------------------------------------------------------

The default configuration of our CAN driver's kernel thread (dpc-thread) is to
give it no special priority. So it runs with the default priority as all other
user processes in the time shared class (nice NI=0, priority PRI=19 under x86
Linux).

This is suitable for a desktop system that only monitors some CAN buses. If you
want to do some control business you may need better priorities.

As a rule of thumb the dpc-thread should have a better priority than any of the
user processes that do CAN I/O. Especially this is true if your user processes
run in the FIFO scheduler class.

Every control application is different. Therefore you have to fine tune the
priority of the dpc-thread so that it fits to your needs. You can do that
with the Linux command line tools ``ps``, ``renice`` and ``chrt``.

To get a sorted by priority list of threads you can enter the following command
at the **root** prompt::

|   # ps -eo pid,tid,class,rtprio,ni,pri,psr,pcpu,stat,wchan:24,comm Oy


Identify the dpc-thread and your user program in the list. To only increase the
priority in the TimeSharing class you may use commands like these::

|   # ps -e | grep "kesdcan_pci405"
|     897 ?        00:00:00 kesdcan_pci405
|   # renice -n -15 -p 897
|   897 (process ID) old priority 0, new priority -15


If you need to reach the FIFO scheduling class you can use the ``chrt`` tool. An
example to change the thread's priority to FIFO priority 12 see below::

|   # chrt -v -p -f 12 897
|   pid 897's current scheduling policy: SCHED_OTHER
|   pid 897's current scheduling priority: 0
|   pid 897's new scheduling policy: SCHED_FIFO
|   pid 897's new scheduling priority: 12


Be aware of the fact that the given priorities are valid only in the selected
"scheduler class" and the kernel's priority is also dependend from the "base
priority" for a scheduler class.


The ``ps`` command cited before gives you a by priority sorted list of threads
and a good overview of the priority layout currently in use. This should enable
you to adjust the priorities as needed. The necessary priority adjustment
command can be embedded in one of the system's start scripts.



++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    _`6)` CAN-API and CAN-FD support
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

After successful installation you can access the CAN bus with the NTCAN API (link
``libntcan.so`` with your application using "-lntcan").

Starting with the NTCAN library version 4.2.0 the library provides the
additional functions and definitions needed to support the transfer of CAN-FD
frames.

The bigger CAN-FD frames now are transferred using the *CMSG_X* structure. These
are the new functions to handle the bigger messages and control the additional
bitrate data::

    canSetBaudrateX()
    canGetBaudrateX()
    canTakeX()
    canReadX()
    canSendX()
    canWriteX()
    canGetOverlappedResultX()

For a detailed description of the usage of these functions please refer to the
NTCAN-API manual part 1 which describes the API itself.



++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    _`7)` Sample-Programm "cantest"
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

.. attention::
    The enclosed binary version of cantest needs to be able to find the dynamic
    shared library ``libntcan.so``. Please, assure, that your LD_LIBRARY_PATH is
    set correctly if you didn't install the ``libntcan.so`` on your system as
    depicted in section `3.5)`_.

The program ``cantest`` is provided as an extremely basic tool to test if you
can receive and transmit CAN frames with the just now installed CAN driver. For
a CAN-FD capable driver the ``cantest`` tool enclosed in the driver's archive is
also compiled to support the transfer of CAN-FD frames. The program shows how it
is compiled in the displayed heading when it starts.

A CAN-FD capable ``cantest`` program displays this::

    CAN Test FD Rev 3.0.10  -- (c) 1997 - 2018 esd electronics gmbh

A ``cantest`` program without CAN-FD support displays this::

    CAN Test Rev 3.0.10  -- (c) 1997 - 2018 esd electronics gmbh

If called without parameters an overview of all available CAN networks and their
associated version numbers will be shown and a short syntax help screen is
printed!

The calling syntax of ``cantest`` with almost all parameters being optional is::

    cantest test-number   [net id-first id-last count
                          txbuf rxbuf txtout rxtout baud
                          testcount data0 data1 ... data7 ... data63]

See the following table for a short parameter description.

+----------------+---------------------------------------+---------------------+
| parameter      | description                           | default             |
+================+=======================================+=====================+
| net            | logical net-number(canOpen())         | 0                   |
+----------------+---------------------------------------+---------------------+
| id-first       | enabled CAN-id's start-id             | 0                   |
+----------------+---------------------------------------+---------------------+
| id-last        | enabled CAN-id's stop-id              | 0                   |
+----------------+---------------------------------------+---------------------+
| count          | count of CMSG packets                 | 1                   |
+----------------+---------------------------------------+---------------------+
| txbuf          | TX queue-size of handle(canOpen)      | 10                  |
+----------------+---------------------------------------+---------------------+
| rxbuf          | RX queue-size of handle(canOpen)      | 100                 |
+----------------+---------------------------------------+---------------------+
| txtout         | TX timeout of handle                  | 1000                |
+----------------+---------------------------------------+---------------------+
| rxtout         | RX timeout of handle                  | 5000                |
+----------------+---------------------------------------+---------------------+
| baud           | baudrate index or other accepted value| 2    (500 kbit/s)   |
+----------------+---------------------------------------+---------------------+
| testcount      | count of NTCAN API calls              | 10   (-1 => forever)|
+----------------+---------------------------------------+---------------------+
| data0 to data7 | Up to  8 bytes TX with CMSG / CMSG_T  | CAN frame filled to |
|   or up        +---------------------------------------+                     |
|      to data63 | Up to 64 bytes TX with CMSG_X         | maximum size (8/64) |
+----------------+---------------------------------------+---------------------+

The following table gives a short overview of the basic test-numbers
implemented. The shown commands operate on CMSG structures. For an equivalent
command operating on CMSG_T structures add 20 to the test number (i. e. 1 ==
canWrite(), 20+1 == canWriteT()). For an equivalent command operating on CMSG_X
structures add 60 to the test number.

To set a CAN-FD arbitration and data baudrate you provide a pair of suitable
bitrate indices (see test-number -4) for the *baud* parameter glued together
with a colon like this: "2:17".

For a detailed description of the operation please refer to the NTCAN-API manual
part 1.

======= =======================================================
test    description
======= =======================================================
  -2    Overview without syntax help

  -3    Overview without syntax help but with feature flags details

  -4    Overview without syntax help but with bit rate index table

   0    canSend():  send CAN frames without confirmation

   1    canWrite(): send CAN frames and wait for acknowledge

   2    canTake():  reading CAN frames without wait

   3    canRead():  reading CAN frames with wait

   4    canRead():  reading events only (emulate canReadEvent())

   5    canSend():  sending events only (emulate canSendEvent())
======= =======================================================

.. note::
    If you try to transmit data from CMSG_X structures (test 60 / 61) you must
    configure a CAN-FD data bitrate because *cantest* tries to send a CAN-FD
    frame with 64 data bytes by default.



++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    _`8)` Binary incompatiblities on libntcan level!
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

Starting with driver release 3.10.0 we deliver the ``libntcan.so.4``, i. e. a
NTCAN library with a major version of 4.

This change was enforced because we had to change some NTCAN_IOCTL_* values
which make the versions before and after that change binary incompatible! Find
the details in the last paragraph of this section (`Incompatibility details`_).
The change had to be done in preparation of the upcoming CAN-FD support.

Because the binary incompatiblity can not be detected at runtime we were forced
to change the library version of ``libntcan.so``. Starting with the release
4.0.0 the shared library will also use symbol versioning!

To assist you with the administration of the <ntcan.h> headers new headers will
contain a comment line like below to show that they belong to a
``libntcan.so.4`` release::

/* Released with libntcan.so.4.0.0 */

What does that mean for you on your host (development) system and on your
target system?

-----

In the easy and recommended case, you migrate completely to the new driver and
``libntcan.so.4``. Then do the following:

- On your host system remove all instances of the old <ntcan.h> file.
- On your host system remove all instances of the ``libntcan.so.3*`` files.
- On your host system install the new <ntcan.h> and the ``libntcan.so.4*``
  files at the appropriate places.
- If needed install the current releases of *esd* libraries that depend on
  ``libntcan.so`` and are also needed by your application (see section `8.1)`_
  below).
- Rebuild your application.
- Install the new driver kernel module on the target system (and optionally
  on the development system).
- Install ``libntcan.so.4*`` (+optional libaries) and your rebuilt application
  on the target system.
- You're done.


-----

You need the new driver but don't need any extensions of later *libntcan*
versions and you're not willing or capable to rebuild your application.

- On your host system keep the old <ntcan.h> and the old ``libntcan.so.3*``
  files.
- Do **NOT** install any of the new <ntcan.h> and ``libntcan.so.4*`` files!
- Install the new driver kernel module on the target system (and optionally
  on the development system).
- You're done.


-----

The complicated case. You need to support target systems with old
``libntcan.so.3`` and want to use the current release ``libntcan.so.4`` on new
designs. Be warned that this is error prone and can lead to silent failures
of your application. Then do the following:

- On your host system rename all instances of the old <ntcan.h> file to
  the new name <ntcan3.h>.
- Remove all symbolic links ``libntcan.so`` that point to a ``libntcan.so.3*``
  library.
- On your host system install the new <ntcan.h> and the ``libntcan.so.4*``
  files at the appropriate places.
- Run ``ldconfig``. If not already there create a symbolic link ``libntcan.so``
  that points to the symbolic link ``libntcan.so.4`` to make ``libntcan.so.4``
  the library version to be linked with by default.
- Install the new driver kernel module and the old ``libntcan.so.3`` or new
  ``libntcan.so.4`` on the target systems as needed.

To use the new version 4 library simply rebuild your application and
install it on the target which has the new ``libntcan.so.4`` installed.


To use the old version 3 library you have to do some additional tasks.

- Add a code snippet like below to your source code to include the
  old <ntcan3.h> for sure:

    #include "ntcan3.h"

- Link explicitely with the old library version by adding the following
  link option when linking your application: "-l:libntcan.so.3"
  This tells the linker to explitely link with version 3 of libntcan.
  Please observe the colon ":" that marks the beginning of an explicit name.
- Then you can install your application on the target system with the
  ``libntcan.so.3``.

.. note::
    You **MUST NOT** build an application with <ntcan3.h> and link with
    ``libntcan.so.4`` or vice versa. This may lead to silent malfunctions!!


----------------------------------------------------------------------
_`8.1)` Effect on depending Libaries
----------------------------------------------------------------------

There are some *esd* libraries that depend directly on the ``libntcan.so`` and
are linked against either version 3 or 4. Therefore a matching version of the
optional libraries needs to be installed on your host (development) system
and your target system.

The directly depending libraries are::

    libirigb.so     Access IRIG-B time data with CAN boards that support it.
    libcalcan.so    CAN base libarary for the esd CANopen master / slave.

If you're using the ``libntcan.so`` also directly in your application make
sure that you use a library version of the optional libraries that is
linked against the same ``libntcan.so`` version that you're using for your
application!

You may display the link dependencies of the libary by using ``ldd`` or "objdump
-x".


---- _`Incompatibility details`

The following NTCAN_IOCTL_* defines changed their values from libntcan.so.3 to
libntcan.so.4::

#define NTCAN_IOCTL_SET_TX_TS_WIN         0x001B
#define NTCAN_IOCTL_GET_TX_TS_WIN         0x001C
#define NTCAN_IOCTL_SET_TX_TS_TIMEOUT     0x001D
#define NTCAN_IOCTL_GET_TX_TS_TIMEOUT     0x001E
#define NTCAN_IOCTL_SET_HND_FILTER        0x001F

If you use these old defines on a new ``libntcan.so.4`` implementation you will
get malfunctions because these values mean different things in the
``libntcan.so.4`` implementation. The big deal is that using such a old define
could change something completely different on the new implemenation even
without provoking the return of an error code. This way the malfunction is
**SILENTLY** triggered and the application has no means to detect it!



++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    _`9)` Known Issues
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

- While building the esdcan CAN driver against your Linux kernel headers a warning
  like this shows up::

    WARNING: could not find /var/lib/dkms/esdcan-pcie402-linux-x86_64/3.10.7/build/.nucleus.o.cmd
    for /var/lib/dkms/esdcan-pcie402-linux-x86_64/3.10.7/build/nucleus.o

  This warning message is there because we don't deliver the \*.o.cmd files of
  our precompiled objects by intention. These files only contain information
  generated by the kernel's Makefiles on how the objects were compiled and are
  used by the kernel's Makefiles to decide if these objects need a
  recompilation. Because you have no source for the precompiled objects it
  makes no sense to provide the "missing" \*.o.cmd files.

- When loading the module, kernels with version > 2.4.9 generate a warning about
  a tainted kernel::

    Warning: loading ./esdcan-xyz will taint the kernel: no license

  This isn't an error message and doesn't concern the functionality of CAN-driver
  or kernel.

- Attention must be paid if you are upgrading from very old drivers with version
  numbers below 3.0.0 (e.g. 2.5.x) which were called **candev** drivers. With
  NTCAN version >= 3.0.0 *esd* now supports an additional dynamic version of the
  NTCAN API library. This NTCAN version supports to talk to 2.x (**candev**) and
  3.x++ (**esdcan**) drivers. It is recommended to link applications against the
  new dynamic library.

  If you want to use an application that has been linked against the old static
  library (revision < 3.x) please recompile this application and link against
  the new library.

- Another caveat comes up with the NTCAN version >= 4.1.2. The support for the
  old **candev** driver was removed from the NTCAN library.

- A SocketCAN driver can prevent the loading of the esdcan driver. See note in
  section `3.2.3)`_.

- Install script for the libraries is still missing.



++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	_`10)` Revision and Contact
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
::

  2019-06-13 / MJ
  ____________________________________
  (c) esd electronics gmbh 2004 - 2019

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
