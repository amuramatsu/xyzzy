#! /usr/bin/env python

import sys
import re
import subprocess
from getopt import getopt

if sys.version_info[0] >= 3:
    unicode = str
    raw_input = input

GIT = "git"
LAST_PATCH = ""

def usage():
    print("Usage: %s [-r] FROM_TAG TO_TAG\n" % sys.argv[0])
    sys.exit(1)

def get_commits(from_, to):
    global GIT
    git = subprocess.Popen(
        [ GIT, "--no-pager", "log", "--no-merges", "--reverse",
          "%s..%s" % (from_, to)], stdout=subprocess.PIPE)
    commits = []
    for line in git.stdout.readlines():
        line = unicode(line, "latin-1")
        if line.startswith("commit "):
            commits.append(line.replace("commit ", "").strip(" \r\n"))
    return commits

def get_picked_cherries():
    global GIT
    PICKED_RE = re.compile(r"\(cherry picked from commit ([a-zA-Z0-9]{40})\)")
    git = subprocess.Popen(
        [ GIT, "--no-pager", "log" ], stdout=subprocess.PIPE)
    picks = []
    for line in git.stdout.readlines():
        line = unicode(line, "latin-1")
        m = PICKED_RE.search(line)
        if m:
            picks.append(m.group(1))
    return picks;

def make_tag(filename, data):
    f = open(filename, "w")
    f.write(data + "\n")
    f.close()

def cherry_pick(gitid):
    global GIT, LAST_PATCH, ERROR_PATCH
    subprocess.call([ GIT, "--no-pager", "log", "--color", "-n", "1", gitid ])
    print("\n\n--- apply this patch (Y/n)")
    response = raw_input()
    if response[0] not in "nN":
        s = subprocess.call([ GIT, "cherry-pick", "-x", gitid ])
        if s != 0:
            print("\nLAST processed patch is ${LAST_PATCH}" % LAST_PATCH)
            print("cherry-pick %s is faild\n" % gitid)
            make_tag("LAST_PATCH", LAST_PATCH);
            make_tag("ERROR_PATCH", git);
            sys.exit(1)
    LAST_PATCH = gitid

def remain_patches(from_, to):
    global GIT
    picked = set()
    for gitid in get_picked_cherries():
        picked.add(gitid)
    
    for gitid in get_commits(from_, to):
        if gitid not in picked:
            subprocess.call(
                [ GIT, "--no-pager", "log", "--no-color", "-n", "1", gitid ])
            print("")

if __name__ == "__main__":
    opt_r = False
    opts, args = getopt(sys.argv[1:], "r", ["remain"])
    if len(args) != 2:
        usage()
    for o, v in opts:
        if o in ("-r", "--remain"):
            opt_r = True
    if opt_r:
        remain_patches(args[0], args[1])
    else:
        for gitid in get_commits(args[0], args[1]):
            cherry_pick(gitid)
