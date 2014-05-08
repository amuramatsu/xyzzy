#! /usr/bin/env perl

my $GIT = "git";
my $LAST_APPLY_PATCH = "";

sub usage {
    print "Usage: $0 FROM_TAG TO_TAG\n";
    exit 1;
}

sub get_commits {
    my ($from, $to) = @_;
    open my $fh, "$GIT log --no-merges --reverse ${from}..${to} |";
    my @logs = grep /^commit /, <$fh>;
    close $fh;
    return map { chomp; s/^commit //; $_; } @logs;
}

sub cherry_pick {
    my ($id) = @_;
    system("$GIT log --color -n 1 ${id} | cat");
    print "\n\n--- apply this patch (Y/n)\n";
    $_ = <STDIN>;
    unless (/^[nN]/) {
	if (system("$GIT cherry-pick -x ${id}") != 0) {
	    print "\ncherry-pick ${id} is faild\n\n";
	    return;
	}
    }
    $LAST_APPLY_PATCH = $id;
}
    
usage() if ($#ARGV < 1);
for my $id (get_commits($ARGV[0], $ARGV[1])) {
    cherry_pick($id);
}
