```perl
#!/usr/bin/perl -w
use strict;
use LWP::Simple;

# Just an example: the URL for the most recent /Fresh Air/ show 
my $url = 'http://freshair.npr.org/dayFA.cfm?todayDate=current';

my $content = get($url);
die "Couldn't get $url" unless defined $content;

# Do things with $content:
if ($content =~ m/jazz/i) {
    print "They're talking about jazz today on Fresh Air!\n";
} else { print "Fresh Air is apparently jazzless today.\n"; }
```


perl -MLWP::Simple -e "getprint 'http://cpan.org/RECENT'"
LWP是一个module，网络相关。