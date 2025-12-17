from setuptools import setup, Extension

module = Extension(
    "mylib",
    sources=["main.c"],
)

setup(name="mylib", version="0.1", ext_modules=[module])
