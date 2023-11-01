def _make_solib_name(name):
    pieces = (
        [native.repository_name()] +
        native.package_name().split("/") +
        [name]
    )
    solib = "lib{}.so.1".format("-".join(pieces))
    return solib

def cc_shared_library(
        name,
        hdrs = [],
        srcs = [],
        deps = [],
        data = [],
        **kwargs):
    """Creates a shared library, primarily for usage with Python bindings.
    Suggested to be named `shared_library`.

    Arguments:
        srcs:
            C++ source files.
        hdrs:
            C++ header files.
    """
    solib = _make_solib_name(name)

    # Create main shared library.
    native.cc_binary(
        name = solib,
        srcs = srcs + hdrs,
        linkshared = 1,
        linkstatic = 1,
        data = data,
        deps = deps,
        **kwargs
    )

    # Expose shared library and headers for transitive dependencies.
    native.cc_library(
        name = name,
        hdrs = hdrs,
        srcs = [solib],
        deps = deps,
        **kwargs
    )
