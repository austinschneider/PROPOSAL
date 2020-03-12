# see https://stackoverflow.com/a/48015772/3838691
import os
from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
import pathlib
import subprocess as sp
import sysconfig


def get_cmake():
    ''' On centos 7, cmake is cmake 2.x but we need > 3.8'''
    for exe in ['cmake3', 'cmake']:
        try:
            sp.check_call([exe, '--version'])
            return exe
        except (FileNotFoundError, sp.CalledProcessError):
            pass
    raise OSError('You need cmake >= 3.8')


class CMakeExtension(Extension):
    def __init__(self, name, sourcedir):
        self.sourcedir = os.path.abspath(sourcedir)
        # don't invoke the original build_ext for this special extension
        super().__init__(name, sources=[])


class build_ext_cmake(build_ext):
    def run(self):
        for ext in self.extensions:
            if isinstance(ext, CMakeExtension):
                self.build_cmake(ext)
        super().run()

    def build_cmake(self, ext):
        cwd = pathlib.Path().absolute()
        tmp = pathlib.Path(self.build_temp)
        extdir = pathlib.Path(self.get_ext_fullpath(ext.name))
        for d in (tmp, extdir):
            d.mkdir(parents=True, exist_ok=True)

        build_type = 'Debug' if self.debug else 'Release'
        os.chdir(tmp)

        cmake = get_cmake()
        python_lib = pathlib.Path(
            sysconfig.get_config_var('LIBDIR'),
            sysconfig.get_config_var('INSTSONAME')
        )
        cmake_call = [
            cmake,
            ext.sourcedir,
            '-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=' + str(extdir.parent.absolute()),
            '-DCMAKE_BUILD_TYPE=' + build_type,
            '-DBUILD_TESTING=OFF',
            '-DADD_PYTHON=ON',
            '-DPYTHON_LIBRARY=' + str(python_lib),
            '-DPYTHON_INCLUDE_DIR=' + sysconfig.get_path('include'),
        ]
        self.spawn(cmake_call)
        build_call = [
            cmake,
            '--build', '.',
            '--config', build_type,
            '--', '-j{}'.format(os.getenv('PROPOSAL_BUILD_CORES', ''))
        ]
        self.spawn(build_call)

        os.chdir(cwd)


setup(
    name='proposal',
    version='6.0.0',
    ext_modules=[CMakeExtension('pyPROPOSAL', os.path.dirname(__file__))],
    cmdclass={
        'build_ext': build_ext_cmake,
    },
    zip_safe=False,
)
