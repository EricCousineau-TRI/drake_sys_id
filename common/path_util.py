import os
from os.path import (
    abspath,
    expanduser,
    expandvars,
)

from bazel_tools.tools.python.runfiles.runfiles import Create as _Create

_runfiles = _Create()


def Rlocation(respath, executable=False):
    """Returns the pathname of a declared data dependency of a py_binary (or
    py_library, etc.).  The result is guaranteed to exist and be readable, and
    be under the `.runfiles` directory for use with relative paths.

    The `respath` looks like "drake_sys_id/pkg/file.ext" or "drake/pkg/file.ext"
    or etc.

    If `executable` is set to True, then the result is also guaranteed to be
    excutable.
    """
    result = os.path.join(_runfiles.EnvVars()["RUNFILES_DIR"], respath)
    result_manifest = _runfiles.Rlocation(respath)
    if result_manifest is None:
        raise RuntimeError(
            f"Resource path {respath} could not be resolved to a "
            "filesystem path; maybe there is a typo in the path, or a "
            "missing data = [] attribute in the BUILD.bazel file.")
    check_manifest_realpath = True

    if not os.path.exists(result):
        raise RuntimeError(
            ("Resource path {} resolved to filesystem path {} but that "
             "filesystem path does not exist.").format(
                 respath, result))
    if not os.access(result, os.R_OK):
        raise RuntimeError(
            ("Resource path {} resolved to filesystem path {} but that "
             "filesystem path does not allow read access.").format(
                 respath, result))
    if executable:
        if not os.access(result, os.X_OK):
            raise RuntimeError(
                ("Resource path {} resolved to filesystem path {} but that "
                 "filesystem path does not allow execute access.").format(
                     respath, result))
    if check_manifest_realpath:
        # As a sanity check, ensure that realpaths match.
        realpath_manifest = os.path.realpath(result_manifest)
        realpath_runfiles = os.path.realpath(result)
        if realpath_manifest != realpath_runfiles:
            raise RuntimeError(
                f"Resource path {respath} has a mismatch between runfiles "
                f"and manifest realpaths:\n"
                f"  runfiles: {result}\n"
                f"    realpath: {realpath_runfiles}\n"
                f"  manifest: {result_manifest}\n"
                f"    realpath: {realpath_manifest}\n")

    return result


def resolve_path(path):
    pkg_prefix = "package://"
    if path.startswith(pkg_prefix):
        return Rlocation(path[len(pkg_prefix) :])
    else:
        path = expandvars(expanduser(path))
        return abspath(path)
