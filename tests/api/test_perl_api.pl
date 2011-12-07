#!/usr/bin/perl

BEGIN {
    push @INC, "@CMAKE_BINARY_DIR@/api/swig/perl";
}

use Cuneiform;

print "Cuneiform info:\n";
print "version: $Cuneiform::VERSION\n";
print "build:   $Cuneiform::BUILD_NUMBER\n";

my $fopt = new Cuneiform::FormatOptions();
my $ropt = new Cuneiform::RecognitionOptions();

$fopt->setLanguage($Cuneiform::LANGUAGE_ENGLISH);
$ropt->setLanguage($Cuneiform::LANGUAGE_ENGLISH);

$page = Cuneiform::recognize("@CMAKE_SOURCE_DIR@/images/quneiform/english.png", $ropt, $fopt);

$page->save("/dev/stdout", $Cuneiform::FORMAT_TEXT, $fopt);

