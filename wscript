from waflib import Options
import sys

top = '.'
out = 'build'
subdirs = ['app', 'lib', 'third_party']

def options(opt):
    opt.load('compiler_cxx unittest_gtest')
    opt.recurse(subdirs)

def configure(conf):
    conf.env.CXXFLAGS += ['-W', '-Wall', '-Wextra', '-Wold-style-cast', '-g', '-O2', '-fopenmp']
    conf.load('compiler_cxx unittest_gtest')
    conf.recurse(subdirs)

def build(bld):
    bld.recurse(subdirs)
