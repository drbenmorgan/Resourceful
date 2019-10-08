Resourceful
===========
Resourceful is a project to study cross-platform techniques for
building C/C++ applications and libraries that are *relocatable*. That is,
the installed package can be copied from one location on the filesystem
to another (maintaining relative links between package components) and
still work without recompilation or changes to the environment. To
this end, Resourceful aims to illustrate some of the issues and possible
solutions using the following basic package layout:

```
+- <prefix>
   +- bin/
   |  +- rsf_simple (self contained program)
   |  +- rsf_dynamic (links to dynamic libResourceful)
   |  +- rsf_static (linked to static libResourceful)
   +- lib/
   |  +- libResourceful.{so|dylib,a} (basic library)
   |  +- rsf/
   |  |  +- <plugins> (example of package allowing pluggable modules)
   |  +- cmake/
   |  |  +- Resourceful (allowing Cmake build clients to use libResourceful)
   |  +- pkgconfig/
   |     +- Resourceful.pc (allowing general build clients to use libResourceful)
   +- share/
      +- Resourceful/
         +- Resourceful.json (example "resource" file)
```

In addition, `libResourceful` uses Boost's filesystem library as a simple example
of an external dependency. Together, this serves to illustrate:

* Working with *internal* paths to resources/plugins without hard coding those paths

* Using *internal* paths to link dependencies, avoiding hard coding where possible

* Using *external* paths to link dependencies, avoiding hard coding where possible

* Causes of, and ways round, hard-coded paths in CMake/pkg-config support files

The first two items should not be considered as a "best/good practice" demonstration,
as they are use case specific, with many additional techniques possible. The
last two are more closely related to packaging and CMake/pkg-config, and try to
follow known good/recommended practice here.

Binary Self Location
====================
Resources are connected with an application and/or library, so a key issue
for avoiding hard coded paths is for these binaries to be able to
locate themselves on the filesystem. If this path can be retrieved, then
we can get resource files if they are located at a known location *relative*
to the binary. This make the package of binary and resource(s) *relocatable*.

C/C++ Applications
------------------
Though the C/C++ `main` function *may* get passed a string *representing* the program
name as the zeroth element of the `argv` array, this is not *guaranteed* to
be the actual filesystem location of the program (see, for example [this discussion](http://stackoverflow.com/questions/2050961/is-argv0-name-of-executable-an-accepted-standard-or-just-a-common-conventi)). 
In particular, the actual invocation of a program may be through a soft or 
hard link whose filesystem location is completely separate from that of the executable.
Though links can be followed to some extent, hardlinks in particular cannot
easily be resolved. A program may also be directly executed using its name
only, with the system finding the first matching entry in `PATH`, and
running that.

On Unices, the `binreloc` code used in the [Listaller](http://listaller.tenstral.net/index.html) project provides an API for retrieving the location
of an executable (program or shared library) on the filesystem.
Mac OS X is Unix based, with binreloc using the [\_NSGetExecutablePath](https://developer.apple.com/library/mac/#documentation/Darwin/Reference/ManPages/man3/dyld.3.html) function to locate executables, but we should also note
the API provided by [Cocoa and Core Foundation](https://developer.apple.com/library/mac/#documentation/CoreFoundation/Conceptual/CFBundles/Introduction/Introduction.html) for application and resource location. The [Qt for Mac OS X](http://qt-project.org/doc/qt-4.8/mac-differences.html) guide also has
some useful information here.

At present Resourceful does not consider Windows platforms, although
some information has been found.
In particular, the [GetModuleFileName](http://msdn.microsoft.com/en-us/library/windows/desktop/ms683197%28v=vs.85%29.aspx) function can be used to determine the full path to the file containing a given module.
If it's called with a NULL module handle, it will retrieve the path
to the executable of the current process. That can be used for programs.
For DLLs, it's mentioned that [DllMain](http://msdn.microsoft.com/en-us/library/windows/desktop/ms682583%28v=vs.85%29.aspx) can be used as this gets passed a handle to the DLL module. That handle can be passed to
GetModuleFileName. **Note the warning** on the [MSDN Reference](http://msdn.microsoft.com/en-us/library/windows/desktop/ms682583%28v=vs.85%29.aspx) about the limitations of what can be done in this entry point function!
Whilst a few years old now, MSDN's [Best Practice for Creating Dlls](http://msdn.microsoft.com/en-us/windows/hardware/gg487379.aspx) discusses these limitations. One recommendation seen is to store the module handle as static
data of the DLL, setting it in `DllMain`, and provide a public API that will
call `GetModuleFileName` using this handle.

C/C++ Libraries
---------------
For dynamic libraries, the same techniques of binreloc or GetModuleFileName can be used. The only aspect to
take care of is if initialization is required (e.g. `br_init_lib` in binreloc), as the client may need to
do this. "Initialize on first use" techniques can help here.

With static libraries, the situation is less clear because the library code becomes part of the final executable.
This almost certainly (NB, to be tested!) results in self location resolving to the path to the executable, and hence
breaking any relative path from library to resources. Probably requires fallback to environment variables, configuration
information from the library install or copying of the resource data into a location relative to the static exe. Which is best
is dependent on the client's use case for static linking. 


Self Location in other Languages
================================
Many supply builtin constructs/syntax for file self location (e.g. `__file__` in Python). These are
generally much easier to use than in compiled/linked languages.

