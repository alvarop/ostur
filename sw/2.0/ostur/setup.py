"""
Ostur Library
"""
from setuptools import setup, find_packages

with open("README.md", "r") as fh:
    long_description = fh.read()


setup(
    name="ostur",
    version="0.1",
    description="Ostur python libraries",
    long_description=long_description,
    long_description_content_type="text/markdown",
    url="https://github.com/alvarop/ostur",
    author="Alvaro Prieto",
    author_email="source@alvaroprieto.com",
    license="MIT",
    packages=find_packages(),
        classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: MIT License",
        "Operating System :: OS Independent",
    ],
    include_package_data=True,
    zip_safe=False,
    python_requires='>=3.6'
)
