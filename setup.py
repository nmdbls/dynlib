from setuptools import setup, Extension
import sys

extra_compile_args = []
if sys.platform != "win32":
    extra_compile_args = ["-std=c99", "-O3"]

module = Extension(
    "mylib",
    sources=["main.c"],
    extra_compile_args=extra_compile_args,
)

setup(name="mylib", version="0.1", ext_modules=[module])
