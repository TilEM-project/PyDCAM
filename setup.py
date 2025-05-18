from skbuild import setup

setup(
    name="PyDCAM",
    version="0.1.0",
    description="A library for interfacing with SmarAct MCS controllers.",
    author="Cameron Devine",
    author_email="cameron.devine@alleninstitute.org",
    license="MIT",
    packages=["PyDCAM"],
    package_dir={"": "src"},
    cmake_install_dir="src/PyDCAM",
    python_requires=">=3.10",
)
