#! /usr/bin/env perl

use strict;

use Getopt::Long qw(:config posix_default no_ignore_case gnu_compat);

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

sub get_picked_cherries {
    open my $fh, "$GIT --no-pager log |";
    my @picks =	grep /\(cherry picked from commit [a-zA-Z0-9]{40}\)/, <$fh>;
    close $fh;
    return map { chomp; s/^.*commit ([a-zA-Z0-9]{40}).*$/$1/; $_; } @picks;
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

sub remain_patches {
    my ($from, $to) = @_;
    my %picked;
    for my $id (get_picked_cherries()) {
	$picked{$id} = 1;
    }
    my @commits = get_commits($from, $to);
    for my $id (@commits) {
	if (! $picked{$id}) {
	    system("$GIT --no-pager log --no-color -n 1 ${id}");
	    print "\n";
	}
    }
}

my $opt_r;
GetOptions("remain|r" => \$opt_r);
usage() if ($#ARGV < 1);

if ($opt_r) {
    remain_patches($ARGV[0], $ARGV[1]);
}
else {
    for my $id (get_commits($ARGV[0], $ARGV[1])) {
	cherry_pick($id);
    }
}
