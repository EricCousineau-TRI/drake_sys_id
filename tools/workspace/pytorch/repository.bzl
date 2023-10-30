load("@drake//tools/workspace:pypi_wheel.bzl", "setup_pypi_wheel")

def _pytorch_impl(repository_ctx):
    mirrors = {
            "pypi_wheel": [
                "https://download.pytorch.org/whl/cu118/torch-2.0.0%2Bcu118-cp310-cp310-linux_x86_64.whl",  # noqa
            ],
    }

    result = setup_pypi_wheel(
        repository_ctx,
        package="torch",
        version="2.0.0",
        pypi_tag="torch-2.0.0+cu118-cp310-cp310-linux_x86_64.whl",
        blake2_256="0",  # Placeholder, replace with actual value if available
        sha256="4b690e2b77f21073500c65d8bb9ea9656b8cb4e969f357370bbc992a3b074764",  # Replace with actual sha256 of the wheel
        mirrors=mirrors,
        data=["torch/bin/torch_shm_manager"],
    )
    if result.error != None:
        fail("Unable to complete setup for @{} repository: {}".format(
            # (forced line break)
            repository_ctx.name,
            result.error,
        ))

pytorch_repository = repository_rule(
    implementation = _pytorch_impl,
)
