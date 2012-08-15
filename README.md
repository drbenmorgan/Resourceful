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
On Unices, the binreloc code used in the [Listaller](http://listaller.tenstral.net/index.html) project provides an API for retrieving the location 
of an executable (program or shared library) on the filesystem.

On Win32, it looks like the [GetModuleFileName](http://msdn.microsoft.com/en-us/library/windows/desktop/ms683197%28v=vs.85%29.aspx) function can be 
used to determine the full path to the file containing a given module.
If it's called with a NULL module handle, it will retrieve the path
to the executable of the current process. That can be used for programs.
For DLLs, it's mentioned that [DllMain](http://msdn.microsoft.com/en-us/library/windows/desktop/ms682583%28v=vs.85%29.aspx) can be used as this gets 
passed a handle to the DLL module. That handle can be passed to 
GetModuleFileName. **Note the warning** on the [MSDN Reference](http://msdn.microsoft.com/en-us/library/windows/desktop/ms682583%28v=vs.85%29.aspx) 
about the limitations of what can be done in this entry point function!
Whilst a few years old now, I found a 
[Best Practice for Creating Dlls](http://msdn.microsoft.com/en-us/windows/hardware/gg487379.aspx) on MSDN that discusses these limitations.
One recommendation that I've seen is to store the module handle as static 
data of the DLL, setting it in DllMain, and provide a public API that will 
call GetModuleFileName using this handle.

