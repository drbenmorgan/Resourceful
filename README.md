Resourceful
===========
Resourceful is a project to study of cross-platform techniques for
building applications that use resource files (e.g. icons, configuration,
data). These resources may be distributed with the application and/or 
supplied by the user. This leads to a couple of main topics for study:

* Working with paths to resources without hard coding.

* Working with paths supplied through the environment (canonical example 
being PYTHONPATH).

* Resource formats, from plain text to compiled binary formats (one 
example being Qt's QResource).


Executable Self Location
------------------------
On Unices, the binreloc code used in the [Listaller](http://listaller.tenstral.net/index.html) project provides an API for determining the location 
of an executable (program or shared library) on the filesystem.

