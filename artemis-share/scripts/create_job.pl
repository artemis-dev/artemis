#!/usr/bin/perl
# Requirement: perl-YAML
# 
# standard input and output file is defined
# treeout output/?REVISION?/?RUN?/?EPOCH?.root
# histout output/?REVISION?/?RUN?/?EPOCH?.root
# 
# ^NUM^ in anchor will be replaced by run number
# ^NAME^ in anchor will be replaced by run name
# if anchor has ridf1=ridf/^NAME^/CAT1_^
# 
# Anchor template is prior to be used if it is passed.
# The format of anchor template is assumed as below.
# Anchor values are treated as schalar or array
# <Example>
# Anchor:
#   anchor1: value of anchor
#   anchor2: value of anchor2
# 
# in order to check required anchors, write anchor name at the beginning of template file.
# The anchor name is assumed to start with &.and to end with : (collon).
# the comments should be one block, where all the line start with '#' (no empty line exists).
# If non-comment line comes, this script finish searching the required anchors.
#
# <Example>
# # required anchors
# # &treeout:
# # &histout:
# 
# # &ignored: this and following anchors will be ignored due to the empty line
#

use Getopt::Long qw(:config posix_default no_ignore_case gnu_compat);
use strict;
use YAML;
use Data::Dumper;

use constant {
    OUTPUT_DIR      => 'output/@ATIME@_@REV@/@TIME@',
    COMMON_NAME     => '@TIME@_@NUM@.@STEERING@',
    JOB_SUFFIX	    => "sh",
    STEERING_SUFFIX => "yaml",
    TREEOUT_SUFFIX  => "tree.root",
    HISTOUT_SUFFIX  => "hist.root"
};

my @anchors;
my @jobParam;
my @errors;
my $git_rev;
my $git_at;
my $run_num;
my $steering_basename;
my $steering_filename;
my %opts = ();
my $logfile = "create_job.log";
my $output_basedir = "output";
my $job_time = time();
my $log_time = $job_time;
my $output_filename_fmt   = OUTPUT_DIR."/".COMMON_NAME;
my $job_filename_fmt	  = OUTPUT_DIR."/".COMMON_NAME.".".JOB_SUFFIX;
my $steering_filename_fmt = OUTPUT_DIR."/".COMMON_NAME.".".STEERING_SUFFIX;
my $treeout_filename_fmt  = OUTPUT_DIR."/".COMMON_NAME.".".TREEOUT_SUFFIX;
my $histout_filename_fmt  = OUTPUT_DIR."/".COMMON_NAME.".".HISTOUT_SUFFIX;
my $output_dir = OUTPUT_DIR;
my $job_filename = $job_filename_fmt;
my $steering_filename = $steering_filename_fmt;
my $treeout_filename = $treeout_filename_fmt;
my $histout_filename = $histout_filename_fmt;
my $job_file_template = "batch/run.tmpl.sh";
my $output_filename = $output_filename_fmt;
my $pwd = "../../..";
my %values;
$values{time} = $job_time;
$values{pwd} = $pwd;

my %reservedKeys = (
    'mode'		=> ['resource group name','u-debug'],
    'node'		=> ['number of node','4'],
    'cpu'		=> ['number of cpu in one node','36'],
    'walltime'		=> ['limit time for execution','00:30:00'],
    'user'		=> ['user name for batch job','system("ssh reedbuch-u.cc.u-tokyo.ac.jp \"echo \$\{USER\}\"'],
    'mailto'		=> ['mail address',''],
    'udgit'		=> ['update git if "yes"','no'],
    'rev'		=> ['revision of working directory (internally set)','`git --no-pager log -1 --format=\'%h\'`'],
    'atime'		=> ['commit time of working directory (internally set)','`git --no-pager log -1 --format=\'%at\'`'],
    'name'		=> ['run name (required)',''],
    'num'		=> ['run number (required)',''],
    'time'		=> ['job time (internally set)','time()'],
    'steering'		=> ['base filename (internally set)','input steering file template without .tmpl.yaml'],
    'steering_filename' => ['name of steering file',$steering_filename_fmt],
    'output_filename'	=> ['name of job script file',$job_filename_fmt],
    'pwd'		=> ['relative path to current directory respect to the steering file (internally set)',$pwd]
    );



my @replaceKeys = keys(%reservedKeys);

#= (
## for job control    
#    'mode',
#    'node',
#    'cpu',
#    'walltime',
#    'user',
#    'mailuser',
#    'udgit',
## for steering file and job    
#    'rev',
#    'atime',
#    'name',
#    'num',
#    'time',
#    'steering',
#    'steering_filename',
#    'output_filename',
#    'pwd'
#    );

    

GetOptions(\%opts,
	   'force|f',
	   'num|n=s',
	   'name|s=s',
	   'template|t=s',
	   'time=s',
	   'help|h',
	   'test',
	   'a_template|a=s',
	   'anchor=s' => \@anchors,
	   'jobprm|p=s' => \@jobParam,
	   'jobtmpl|j=s'
) or die "Error while getting options";



# while (my ($key, $value) = each(%opts)){
#     print "key=$key, value=$value\n";
# }
# 
# for (my $i = 0; $i < @anchors; $i++) {
#     print "anchor: $anchors[$i]\n";
# }

Usage() and exit 0 if ($opts{help});

# default values

foreach my $key ("mode", "node", "cpu", "walltime", "udgit") {
    $values{$key} = $reservedKeys{$key}[1];
}
$job_file_template = $opts{jobtmpl} if (exists($opts{jobtmpl}));
StoreJobPrm();
if (!exists($values{user})) {
    $values{user} = system("ssh reedbush-u.cc.u-tokyo.ac.jp \"echo \$\{USER\}\"");
}

######################################################################
# check required options
######################################################################
RequireOption("num");
RequireOption("name");
RequireOption("template");

HasError() and die "Error while checking required option";
######################################################################
# validate option 
######################################################################
Validate((-f $opts{template}),      sprintf("no such template file '%s'",$opts{template}));
Validate(($opts{num} =~ /^[0-9]+$/),"runnumber should contain only digits");
Validate((-f $opts{a_template}),    sprintf("no such anchor template file '%s'",$opts{a_template})) if (exists($opts{a_template}));
Validate((-f $opts{runtmpl}), sprintf("no such run template %s",$opts{runtmpl})) if (exists($opts{runtmpl}));
Validate((-f $job_file_template), sprintf("no such job script template %s",$job_file_template));

HasError() and die "Error while validation of option parameters";

$values{time} = $opts{time} if (exists($opts{time}));
$values{name} = $opts{name};
$values{num}  = sprintf("%04d",$opts{num});
$values{steering} = $opts{template};
$values{steering} =~ s/.tmpl.yaml//;
$values{steering} =~ s/\//_/g;
######################################################################
# get revision and atime (check git repository)
######################################################################
GetRev() or die "Error invalid revision";

######################################################################
# check anchors
######################################################################
my @required = RequiredAnchors($opts{template});
my %anchorArgs;
my %existAnchors;
my @outputLines;
push @outputLines, "# Created by using revision $values{rev}";
push @outputLines, "# Steering template: ".$opts{template};
push @outputLines, "# Anchor template  : ".$opts{a_template} if (exists($opts{a_template}));
######################################################################
# override for test
######################################################################
if (exists($opts{test})) {
    $output_dir = "output/test";
    $steering_filename = "output/test/test.yaml";
    $output_filename = $output_dir."/test";
    $job_filename = "output/test/test.sh";
    $treeout_filename = $output_dir."/test.tree.root";
    $histout_filename = $output_dir."/test.hist.root";
    $pwd = "../..";
}

ReplaceVariables($output_filename);


######################################################################
# anchor  template
######################################################################
# variable in template will be directory replaced
if (exists($opts{a_template})) {
    open(ATMP,$opts{a_template}) or die "Cannot open file ".$opts{a_template};
    while (<ATMP>) {
	# revision
	s/(.*)\n$/$1/;
	/^\#/ and (push @outputLines, $_) and next;
	my $line = $_;
	if ($line =~ /^[^&]+&(\S+)/) {
#	    print $1,"\n";
	    $existAnchors{$1} = 1;
	    printf("anchor: %s\n",$1);
	}
	ReplaceVariables($line);
	push @outputLines, $line;
    }
    close(ATMP);
} else {
    push @outputLines, "Anchor:\n";
}

######################################################################
# anchor arguments
######################################################################

for (my $i = 0; $i < @anchors; $i++) {
    my ($aname, $avalue) = $anchors[$i] =~ /^([^=]+)=(.*)/;
    $anchorArgs{$aname} = $avalue;
    $existAnchors{$aname} = 1;
    printf("%10s = %s\n",$aname,$avalue);
}

######################################################################
# default anchor 
######################################################################
if ((grep {$_ eq "treeout"} @required) && !exists($existAnchors{"treeout"})) {
    $anchorArgs{"treeout"} = sprintf($treeout_filename);
    ReplaceVariables($anchorArgs{"treeout"});
    $existAnchors{"treeout"} = 1;
    
}
if ((grep {$_ eq "histout"} @required) && !exists($existAnchors{"histout"})) {
    $anchorArgs{"histout"} = sprintf($histout_filename);
    ReplaceVariables($anchorArgs{"histout"});
    $existAnchors{"histout"} = 1;
}


# check existence 
for (my $i = 0; $i < @required; $i++) {
    Validate(exists($existAnchors{$required[$i]}), "Required anchor does not exists: ".$required[$i]);
}
HasError() and die "Error while validation of anchors";

foreach my $key (keys(%existAnchors)) {
    Validate((grep {$_ eq $key} @required), "Non-required anchor exists: ".$key);
}

if ($opts{force}) {
    HasError() and print "Forced continue (Error while validation of anchors)\n";
} else {
    HasError() and die "Error while validation of anchors";
}
# add anchors

foreach my $key (keys(%anchorArgs)) {
    push @outputLines, sprintf("  - &%s [%s]",$key,$anchorArgs{$key});
}

ReplaceVariables($output_dir);
ReplaceVariables($steering_filename);
ReplaceVariables($values{steering});
ReplaceVariables($job_filename);
$values{steering_filename} = $steering_filename;
$values{output_filename} = $job_filename;
$values{pwd} = $pwd;

	
system("mkdir -p $output_dir");

open (STEERING,">".$steering_filename);
open (TEMPLATE, $opts{template});
print STEERING join "\n", @outputLines;
print STEERING "\n";
while (<TEMPLATE>) {
    ReplaceVariables($_);
    print STEERING;
}
close (TEMPLATE);
close (STEERING);

open (JOB,">".$job_filename);
open (TEMPLATE, $job_file_template);
while (<TEMPLATE>) {
    ReplaceVariables($_);
    print JOB;
}
close (JOB);
close (TEMPLATE);
printf($job_filename."\n");
printf($steering_filename."\n");
WriteLog($logfile,sprintf("%s,%12s,%12s,%10s, %s,%10s%04d, %20s\n",
			  GetDateString($log_time),$log_time,$job_time,$values{rev},
			  GetDateString($values{atime}),$opts{name},$values{num},$steering_filename));
		 


sub ReplaceVariables
{

    foreach my $key (@replaceKeys) {
	my $ucKey = uc $key;
	do 1 while (@_[0] =~ s/\@$ucKey\@/$values{$key}/g);
    }

    
#    do 1 while(@_[0] =~ s/\@REV\@/$git_rev/g);
#    do 1 while(@_[0] =~ s/\@ATIME\@/$git_at/g);
#    do 1 while(@_[0] =~ s/\@NAME\@/$opts{name}/g);
#    do 1 while(@_[0] =~ s/\@NUM\@/$run_num/g);
#    do 1 while(@_[0] =~ s/\@TIME\@/$job_time/g);
#    do 1 while(@_[0] =~ s/\@STEERING\@/$steering_basename/g);
#    do 1 while(@_[0] =~ s/\@STEERING_FILENAME\@/$steering_filename/g);
#    do 1 while(@_[0] =~ s/\@OUTPUT_FILENAME\@/$output_filename/g);
#    do 1 while(@_[0] =~ s/\@PWD\@/$pwd/g);
}

sub Validate # logic, message
{
    if (@_ == 2) {
	my ($logic, $message) = @_;
	push @errors, $message if (!$logic);
    } else {
	# first argument may be missing due to the empty string returned by regular expression
	my ($message) = @_;
	push @errors, $message;
    }
}

sub GetDateString
{
    my $time = shift;
    my @youbi = ('日', '月', '火', '水', '木', '金', '土');
    my ($sec,$min,$hour,$mday,$mon,$year,$wday,$yday,$isdst) = localtime($time);
    $year += 1900;
    $mon += 1;
    return sprintf("%04d-%02d-%02d %02d:%02d:%02d",$year,$mon,$mday,$hour,$min,$sec);
}

sub GetOutputDirName
{
    return sprintf("%s/%s_%s",$output_basedir,$git_at,$git_rev);
}

sub GetRev
{
    $values{rev} = trim(`git --no-pager log -1 --format='%h'`);
    $values{atime}  = trim(`git --no-pager log -1 --format='%at'`);
}


sub RequireOption
{
    my $name = @_[0];
    push @errors, sprintf("option $name should be passed") if (!defined($opts{$name}) ||  $opts{$name} eq '') ;    
}



sub HasError
{
    return 0 if (@errors == 0);
    for (my $i =0; $i < @errors; $i++) {
	printf("[Error] $errors[$i]\n");
    }
    return 1;
}


sub StoreJobPrm
{
    for (my $i = 0; $i < @jobParam; ++$i) {
	my ($pname, $pvalue) = $jobParam[$i] =~ /^([^=]+)=(.*)/;
	$values{$pname} = $pvalue;
	push @replaceKeys, $pname if ( 0 == (grep {$_ eq $pname} @replaceKeys) );
	printf ("%20s = %s\n",$pname,$pvalue);
    }
}
    

sub WriteLog
{
    my ($logfile, $message) = @_;
    open(LOG,"+>>",$logfile) or die "cannot open file $logfile";
    printf LOG $message;
    close(LOG);
}

sub trim {
    my $val = shift;
    $val =~ s/^\s*(.*?)\s*$/$1/;
    return $val;
}

sub RequiredAnchors
{
    my ($tmpl) = @_;
    my (@ret) = ();
    open (TMPL,$tmpl) or die "cannot open template file $tmpl";
    while (<TMPL>) {
	last if ($_ !~ /^#/);
	my ($anchor) = $_ =~ /^#\s+&(\S+):/;
	next if ($anchor eq "");
	push @ret,$anchor;
    }
    close(TMPL);
    return @ret;
}

sub Usage
{
    my $prog = "create_job.pl";
    my @help;
    my $indent = sprintf("%8s"," ");
    sub add {
	my $level = @_[0];
	my $message;
	for (my $i = 0; $i < $level; ++$i) {
	    $message .= $indent;
	}
	$message .= @_[1];

	push @help, $message;
    }

    sub blank { push @help,""; };
    add(0,"Name");
    add(1,"$prog - create batch job script and steering file");
    blank();
    add(0,"SYNOPSIS");
    add(1,"$prog --num runNum --name runName --template templateFileName [--test] [options]");
    blank();
    add(0,"DESCRIPTION");
    add(1,"$prog creates steering file and batch job script to run the created steering file.");
    add(1,"Each set of files are created in the output directory of output/\@ATIME\@_\@REV\@/\@TIME\@.");
    add(1,"Mandatory options are num, name, and template to indicate the run number, run name");
    add(1,"and steering template file name. Steering template file name should ends with '.tmpl.yaml'.");
    add(1,"Steering, anchor, and job script  template files can contain some keywords to be replaced.");
    add(1,"The keywords in template files should be UPCASE and enclosed with '@', e.g. \@NAME\@.");
    add(1,"The default keywords are listed below.");
    blank();
    add(1,'Default anchors named treeout and histout are automatically set if it is not defined by users.');
    add(1,'Anchor treeout is output/@ATIME@_@REV@/@TIME@/@TIME@_@NUM@.@STEERING@.tree.root.');
    add(1,'Anchor histout is output/@ATIME@_@REV@/@TIME@/@TIME@_@NUM@.@STEERING@.hist.root.');
    blank();
    add(0,"OPTIONS");
    add(1,"-n, --num runNumber");
    add(2,"run number");
    blank();
    add(1,"-s, --name runName");
    add(2,"run name");
    blank();
    add(1,"-t, --template templateFileName");
    add(2,"template file name with suffix of .tmpl.yaml");
    blank();
    add(1,"-h, --help");
    add(2,"show this help");
    blank();
    add(1,"--test");
    add(2,"force the output file name to output/test/test.{sh,yaml}");
    blank();
    add(1,"-a, --a_template templateFileName");
    add(2,"anchor template file name");
    blank();
    add(1,"--anchor name=value");
    add(2,"defines anchor named 'name' and having 'value'");
    blank();
    add(1,"-p --jobprm name=value");
    add(2,"defines replacement named 'name' and \@NAME\@ will be replaced by 'value'");
    add(2,"any keys other than reserved ones can be defined");
    blank();
    add(1,"-j --jobtmpl templateFileName");
    add(2,"job script template file name");
    blank();
    add(1,"reserved replacement keys");
    foreach my $key (sort(keys(%reservedKeys))) {
	add(2,sprintf("%-20s: %s",$key,$reservedKeys{$key}[0]));
	add(3,sprintf("%20s: %s","default",$reservedKeys{$key}[1]));
    }
    blank();
    add(0,"EXAMPLE");
    add(1,'perl share/scripts/create_job.pl --num 259 --name phys --template steering/cat/gainshifttable.tmpl.yaml');
    add(1,' --a_template steering/cat/gainshifttable.anchor.yaml  -p user=g42000 -p mailto=ota@cns.s.u-tokyo.ac.jp');
    blank();
    add(2,"will read steering/cat/gainshifttable.tmpl.yaml, steering/cat/gainshifttable.anchor.yaml and ");
    add(2,'default job script template (batch/run.tmpl.sh),');
    add(2,'and implicitly replace @USER@ with g4200 and @MAILTO@ with ota@cns.s.u-tokyo.ac.jp.');
    blank();
    add(0,'AUTHOR');
    add(1,'Written by Shinsuke OTA <ota@cns.s.u-tokyo.ac.jp>');
    blank();
    add(0,'COPYRIGHT');
    add(1,'Copyright (C) 2017-2018 Shinsuke OTA. There is NO WARRANTY, to the extent permitted by law.');
    blank();
    printf(join("\n",@help));
    return 1;
}
