# Prelim Sys Id Prototype Stuff

This is just a code snapshot from Anzu, some stuff not yet exported. Consider
this "for demonstration purposes" / "in the noise" until it's proven
effective on hardware (or discarded otherwise).

Still needs code review, etc. etc.

## Setup

Only tested on Ubuntu 22.04, CUDA 11.8 (though GPU isn't strictly necessary).

Should install prereqs for Drake: <https://drake.mit.edu>
s
## Build and Test

```sh
cd drake_sys_id
bazel test //...
```

Will take a while because Python bindings have to build.

WARNING: For some reason, `//sys_id:drake_torch_sys_id_test` seems to run 5-10x
slower than it does in internal Anzu repository. Unclear why.
