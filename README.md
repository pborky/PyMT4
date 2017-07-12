PyMT4 - Python bindings for the Metatrader 4 trading platform  ( rmawatson [ at ] hotmail. com )

Overview -

This project provides bindings for using the Metatrader4 MQL API in python.

The bindings are provided through a tcp connection with the metatrader 4 client
via pymt4_host.mq4 using boost::asio. The python session connects to the host,
and can then make API calls natively in python. An experimental OnTick handler is
provided to register charts to notify the python session of new ticks.

A simple example in python:

Prerequisites:
 - Install Boost 32-bit prebuilt binaries
 - Install Python 32-bit
 - Compile MSVC to Release Win32 configuration.
 - Metatrader has a chart running pymt4_host.mq4, allowing execution of 32-bit dll calls.
 - PyMT4.pyd is available on PYTHONPATH

Launch Python 32-bit and enter the following commands:
from PyMT4 import *

Connect()
Print('OK')
AccountNumber()
Disconnect()


The PyMT4 module contains most of the API. Custom Indicators are not yet supported, but
should be simple enough to add in the future.

The network port is currently hard coded in pymt4_common.h and can be changed here. This will require a
compile of both the Client and Server, and should be made configurable at both ends (simple do to), as
is the address the client listens on (localhost).

The bindings are written such that they could easily be exposed to any other language.
pymt4_client_command.cpp contains the stub functions for the currently exposed API.
This could be called directly by including the header in a C++ application (and linking to the dll),
or wrapped for some other scripting language.

This is exposed to Python using boost::python in pymt4_client_module.cpp including the OnTickHandler
implementation.

Compiling -

This project uses boost heavily and you will need to optain a copy from www.boost.org. Look for pre-built binaries.
PyMT4 was built using Visual Studio 2015 and most of customization of your paths
should be:
- Modify the PyMT4_Properties.props property sheet
- For server build, remember to change the copy file path to Metatrader libraries in MSVC Post-Build Event.

Other thoughts -

For me, the ideal solution would be to keep python for the glue logic and utility of the mass
of code out there and speed of development, but allow exposing custom routines that prove too
slow to proxy (too many MQL calls), in addition to the raw API. And allow these routines to be
dynamically exposed to Python via this framework. This would only be of use if the routine
in question actually needed to make a significant amount of MQL api calls.

Last tested the command execution rate is averaging from 500 to 588 command per second. Quite impressive!

The precompiled samples are 32-bit and compiled against Boost 32-bit and Python 2.7 32-bit.

Good luck in your trading.