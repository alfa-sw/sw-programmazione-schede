# coding: utf-8

# pylint: disable=missing-docstring
# pylint: disable=invalid-name

import os
import glob

from setuptools import setup, find_packages
from runpy import run_path

here = os.path.abspath(os.path.dirname(__file__))

with open(os.path.join(here, '__version__'), encoding='utf-8') as f:
    __version__ = f.read().strip()

with open(os.path.join(here, 'README.md'), encoding='utf-8') as f:
    __readme__ = f.read().strip()

__app_name__ = 'alfa_fw_upgrader'


def main():
    setup(
        name=__app_name__,
        version=__version__,
        description='',
        long_description_content_type='text/x-rst',
        long_description=__readme__,
        url='',
        classifiers=[
            'Development Status :: 3 - Alpha',
            'Programming Language :: Python :: 3',
            "License :: OSI Approved :: MIT License",
            "Operating System :: POSIX :: Linux",
            "Operating System :: Microsoft :: Windows :: Windows Vista",
            "Operating System :: Microsoft :: Windows :: Windows 7",
            "Operating System :: Microsoft :: Windows :: Windows 10",
        ],
        packages=find_packages(where='src'),
        package_dir={'': 'src'},
        include_package_data=True,
        scripts=[
            'bin/alfa_fw_upgrader',
            'bin/alfa_fw_upgrader_launcher.py',
        ],
        install_requires=[
            'pyusb',
            'rs485_master',
            'eel',
            'PyYAML',
            'AppDirs',
            'importlib_resources ; python_version<"3.9"',
            'importlib_metadata ; python_version<"3.8"',
            'crc'
        ],


    )


if __name__ == '__main__':
    main()
