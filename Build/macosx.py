#!/usr/bin/env python
# From Joey Mukherjee (joey@swri.edu), 2005

"""Provides tools for building Mac application bundles."""

from os.path import *
from os import listdir, stat
import re
import shutil
import glob

from SCons.Builder import *
from SCons.Defaults import SharedCheck, ProgScan
from SCons.Script.SConscript import SConsEnvironment

def TOOL_BUNDLE(env):
    if 'BUNDLE' in env['TOOLS']:
        return
    if env['PLATFORM'] == 'darwin':
        env.Append(TOOLS = 'BUNDLE')
        # This is like the regular linker, but uses different vars.
        LinkBundle = SCons.Builder.Builder(action=[SharedCheck, "$BUNDLECOM"],
                                           emitter="$SHLIBEMITTER",
                                           prefix = '$BUNDLEPREFIX',
                                           suffix = '$BUNDLESUFFIX',
                                           target_scanner = ProgScan,
                                           src_suffix = '$BUNDLESUFFIX',
                                           src_builder = 'SharedObject')
        env['BUILDERS']['LinkBundle'] = LinkBundle
        env['BUNDLEEMITTER'] = None
        env['BUNDLEPREFIX'] = ''
        env['BUNDLESUFFIX'] = ''
        env['BUNDLEDIRSUFFIX'] = '.bundle'
        env['BUNDLE'] = '$SHLINK'
        env['BUNDLEFLAGS'] = ' -bundle'
        env['BUNDLECOM'] = '$BUNDLE $BUNDLEFLAGS -o ${TARGET} $SOURCES $_LIBDIRFLAGS $_LIBFLAGS $FRAMEWORKS'

        def MakeBundle(env, bundledir, app,
                       key, info_plist,
                       typecode='BNDL', creator='SapP',
                       resources=None, frameworks=None):
            """Install a bundle into its dir, in the proper format"""
            resources = resources or []
            # Substitute construction vars:
            for a in [bundledir, key, info_plist, typecode, creator]:
                a = env.subst(a)


            for f in resources:
                if f.find(".xib") != -1:
                    newF = f.replace(".xib", ".nib")
                    env.CompileXIB(f)
                    resources.append(newF)
                    resources.remove(f)

            if SCons.Util.is_List(app):
                app = app[0]

            if SCons.Util.is_String(app):
                app = env.subst(app)
                appbase = basename(app)
            else:
                appbase = basename(str(app))
            if not ('.' in bundledir):
                bundledir += '.$BUNDLEDIRSUFFIX'
            bundledir = env.subst(bundledir) # substitute again
            suffix=bundledir[bundledir.rfind('.'):]
            if (suffix=='.app' and typecode != 'APPL' or
                suffix!='.app' and typecode == 'APPL'):
                raise SCons.Errors.UserError, "MakeBundle: inconsistent dir suffix %s and type code %s: app bundles should end with .app and type code APPL."%(suffix, typecode)
            else:
                env.SideEffect (bundledir, app)
            inst = env.Install(bundledir+'/Contents/MacOS', app)
            inf = env.InstallAs(bundledir+'/Contents/Info.plist', info_plist)
            env.WriteVal(target=bundledir+'/Contents/PkgInfo',
                         source=SCons.Node.Python.Value(typecode+creator))
            for r in resources:
                if SCons.Util.is_List(r):
                    env.InstallAs(join(bundledir+'/Contents/Resources', r[1]),
                                  r[0])
                else:
                    env.Install(bundledir+'/Contents/Resources', r)
            for r in frameworks:
                if SCons.Util.is_List(r):
                    env.InstallAs(join(bundledir+'/Contents/Frameworks', r[1]),
                                  r[0])
                else:
                    env.Install(bundledir+'/Contents/Frameworks', r)
            return [ SCons.Node.FS.default_fs.Dir(bundledir) ]

        # This is not a regular Builder; it's a wrapper function.
        # So just make it available as a method of Environment.
        SConsEnvironment.MakeBundle = MakeBundle
