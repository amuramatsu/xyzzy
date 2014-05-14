#! /usr/bin/env python

import os
import sys
import re
import io
import glob

if sys.version_info[0] >= 3:
    unicode = str

STOPFILE = {
    'ed.h': '$(ED_H)',
    '../ed.h': '$(ED_H)',
    'mainframe.h': '$(MAINFRAME_H)',
    'stdafx.h': '$(OUTDIR)/xyzzy.pch',
    'lucida-width.h': 'gen/lucida-width.h',
    'jisx0212-width.h': 'gen/jisx0212-width.h',
    'fontrange.h': 'gen/fontrange.h',
}
INCLUDES = {
    'gen': '$(GENDIR)',
    'privctrl': '$(PRIVCTRLDIR)',
    'dsfmt': '$(DSFMTDIR)',
    'zlib': '$(ZLIBDIR)',
}
VIRTUALDIR = '$(GENDIR)'

def canonicize_filepath(filename, dirname):
    if os.path.isabs(filename):
        return os.path.normpath(filename).replace("\\", "/")
    return os.path.normpath(os.path.join(dirname, filename)).replace("\\", "/")

INCLUDE_RE = re.compile(r'^\s*\#\s*include\s+\"([^\"]+)\"')
def get_include_files(filename, curdir, already=None):
    if already is None:
        already = set()

    incs = []
    try:
        f = io.open(canonicize_filepath(filename, curdir), "r",
                    encoding="latin-1")
    except:
        #return ([ filename ], already)
        raise
    in_comment = False
    for line in f:
        line = re.sub(r"\*.*\*", "", line)
        if not in_comment:
            m = INCLUDE_RE.search(line)
            if m:
                incs.append(m.group(1))
        if in_comment and line.find('*/') >= 0:
            in_comment = False
        if not in_comment and line.find('/*') >= 0:
            in_comment = True
    f.close()

    result = []
    for newfile in incs:
        if newfile in already:
            continue
        if newfile in STOPFILE:
            result.append(STOPFILE[newfile])
            already.add(newfile)
        else:
            for path in [ curdir ] + list(INCLUDES.keys()):
                f = canonicize_filepath(newfile, path)
                if os.path.exists(f):
                    new_result, already = get_include_files(
                        f, curdir, already)
                    result.extend(new_result)
                    break
            else:
                p = canonicize_filepath(newfile, VIRTUALDIR)
                result.append(p)
                already.add(p)
    return ([ filename ] + result, already)

def make_depends(filename, curdir, header=False):
    if header:
        target = filename.replace(".", "_").upper()
        target = re.sub(r"^.*[/\\]", "", target)
    else:
        target = filename.replace(".cc", ".obj")
        target = re.sub(r"^.*[/\\]", "", target)

    incs = set()
    for dep in get_include_files(filename, curdir)[0]:
        if dep == filename:
            continue
        for key in INCLUDES.keys():
            dep = re.sub(r"^%s" % key, INCLUDES[key], dep)
        incs.add(dep)
    
    if len(incs) > 0:
        if header:
            output = target + "="
            for f in sorted(incs):
                output += f
                if len(output) >= 66:
                    print(output + " \\")
                    output = ""
                else:
                    output += " "
            print(output)
        else:
            print("$(OUTDIR)/%s: %s" % (target, " ".join(sorted(incs))))

if __name__ == "__main__":
    for arg in sys.argv[1:]:
        for f in glob.glob(arg): 
            if f.endswith(".h"):
                make_depends(f, ".", True)
            else:
                make_depends(f, ".")
