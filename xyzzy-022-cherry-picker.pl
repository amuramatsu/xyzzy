#! /usr/bin/env perl

use strict;

my $GIT = "git";
my $LAST_PATCH = "";

sub usage {
    print "Usage: $0 FROM_TAG TO_TAG\n";
    exit 1;
}

sub get_commits {
    my ($from, $to) = @_;
    open my $fh, "$GIT --no-pager log --no-merges --reverse ${from}..${to} |";
    my @logs = grep /^commit /, <$fh>;
    close $fh;
    return map { chomp; s/^commit //; $_; } @logs;
}

sub make_tag {
    my ($filename, $data) = @_;
    open my $fh, ">", $filename;
    print $fh "$data\n";
    close $fh;
}

sub cherry_pick {
    my ($id) = @_;
    system("$GIT --no-pager log --color -n 1 ${id}");
    print "\n\n--- apply this patch (Y/n)\n";
    $_ = <STDIN>;
    unless (/^[nN]/) {
	if (system("$GIT cherry-pick -x ${id}") != 0) {
	    print "\nLAST processed patch is ${LAST_PATCH}\n";
	    print "cherry-pick ${id} is faild\n\n";
	    make_tag("LAST_PATCH", $LAST_PATCH);
	    make_tag("ERROR_PATCH", $id);
	    exit 1;
	}
    }
    $LAST_PATCH = $id;
}
    
usage() if ($#ARGV < 1);
for my $id (get_commits($ARGV[0], $ARGV[1])) {
    cherry_pick($id);
}
