#! /usr/bin/env perl

use strict;

my %STOPFILE = (
    'ed.h'        => '$(ED_H)',
    '../ed.h'     => '$(ED_H)',
    'stdafx.h'    => '$(OUTDIR)/xyzzy.pch',
    'mainframe.h' => '$(MAINFRAME_H)',
    'lucida-width.h' => 'gen/lucida-width.h',
    'jisx0212-width.h' => 'gen/jisx0212-width.h',
    'fontrange.h' => 'gen/fontrange.h',
);
my %INCLUDES = (
    'gen' => '$(GENDIR)',
    'privctrl' => '$(PRIVCTRLDIR)',
    'dsfmt' => '$(DSFMT)',
    'zlib' => '$(ZLIBDIR)',
);
my $VIRTUALDIR = '$(GEN)';

sub canonicize_filepath {
    my ($filename, $dir) = @_;
    my $drive = "";
    $filename =~ s/\\/\//g;
    if ($filename =~ s/^([a-zA-Z]:)//) {
	$drive = uc($1);
    }
    my $abspath = $filename =~ /^\// ? 1 : 0;
    my $uncpath = ($drive eq "" && $filename =~ /^\/\//) ? 1 : 0;

    my @result;
    for my $elem (split "/", "$dir/$filename") {
	next if $elem eq "." || $elem eq "";
	if ($elem eq "..") {
	    if ($#result >= 0 && $result[$#result] ne "..") {
		pop @result;
	    }
	    elsif (! $abspath) {
		push @result, "..";
	    }
	}
	else {
	    push @result, $elem;
	}
    }
    return $drive . ($uncpath ? "/" : "") . ($abspath ? "/" : "") .
	join("/", @result);
}

sub get_include_files {
    my ($file, $curdir, $already) = @_;
    $already = [] unless $already;
    
    my @incs;
    open my $fh, "<", canonicize_filepath($file, $curdir) or 
	return ([ $file ], $already);
    my $in_comment = 0;
    while (<$fh>) {
	s,/\*.*\*/,,g;
	if (!$in_comment && /^\s*\#\s*include\s+\"([^\"]+)\"/) {
	    push @incs, $1;
	}
	if ($in_comment && /\*\//) {
	    $in_comment = 0;
	}
	if (!$in_comment && /\/\*/) {
	    $in_comment = 1;
	}
    }
    close $fh;

    my @result;
    for my $newfile (@incs) {
	next if grep /^$newfile$/, @$already;
	if ($STOPFILE{$newfile}) {
	    push @result, $STOPFILE{$newfile};
	    push @$already, $STOPFILE{$newfile};
	}
	else {
	    my $found = 0;
	    for my $path (($curdir, keys %INCLUDES)) {
		my $f = canonicize_filepath($newfile, $path);
		if (-e $f) {
		    my ($new_result, $already) =
			get_include_files($f, $curdir, $already);
		    @result = (@result, @$new_result);
		    $found = 1;
		    last;
		}
	    }
	    if (!$found) {
		my $p = canonicize_filepath($newfile, $VIRTUALDIR);
		push @result, $p;
		push @$already, $p;
	    }
	}
    }
    return ([ $file, @result ], $already );
}

sub make_depends {
    my ($file, $curdir) = @_;
    my $target = $file;
    $target =~ s/\.cc$/\.obj/;

    my @incs;
    for my $dep (@{(get_include_files($file, $curdir))[0]}) {
	next if $dep eq $file;
	for my $key (keys %INCLUDES) {
	    $dep =~ s,^$key/,$INCLUDES{$key}/,;
	}
	next if grep /^$dep$/, @incs;
	push @incs, $dep;
    }
    if ($#incs >= 0) {
	print "\$(OUTDIR)/$target: " . join(" ", @incs) . "\n";
    }
}
for my $f (@ARGV) {
    make_depends($f, ".");
}
