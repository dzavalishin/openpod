=========
Rationale
=========


There are tens of operating system projects in the world. From tiny experimental works 
to medium size projects to big and big old ones with a bing community.

All of them need drivers. All of OS authors work on quite the same drivers from early start
and for ever and ever.

It seems just natural for OS authors community to define a standard for a driver API to
share work around and help each other.

This document is an attempt to start such effort. 

Goal is to define API and driver structure that is:

* **Simple enough.** There were previous efforts to define portable driver API which required
  driver (even simplest one) to be unnecessarily complex. This specification lets driver 
  author to start from a simple skeleton and make trivial driver in a day or so.

* **Extencible.** It must let complex and specific driver to have reach API into the kernel which
  does not limit driver designer too much and does not enforce him to come around and add non-standard
  entry points.

* **Support different device classes naturally.** Video and network drivers are really different and
  provide APIs that have quite nothing in common. But drivers in each class generally provide identical 
  entry points.

* **Support runtime device addition and removal.** Driver must be able to inform kernel about status change
  and add or remove devices in run time.

* **Support drivers providing different types of devices.** PS2 driver exports both keyboard and mouse. Video
  card driver can provide sound device for an HDMI port, and so on.  

* **Let driver to be both statically and dynamically linked.** Though, current specification is not really checked
  in a project with a dynamic driver loading, there is corresponding support in API and data structures.

Current state of specification: It is partially complete and waits for your comments.

The best way to comment is to create an issue in project's `GitHub repository <https://github.com/dzavalishin/openpod/issues>`_.

=============
What's inside
=============

This specification covers:

* Services provided to the driver by kernel
* Driver start / device discover / stop procedures (driver lifecycle)
* Device announce to the kernel (device lifecycle)
* Device API 
* Configuration (PCI) information access
* Asyncronous IO requests 
