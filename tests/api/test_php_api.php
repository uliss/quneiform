<?php

require '@CMAKE_BINARY_DIR@/api/swig/php/cuneiform.php';

echo "PHP api test\n";
echo "version: " . CF_VERSION . "\n";
echo "build:   " . CF_BUILD_NUMBER . "\n";

$fopts = new cuneiform_format_options;
$ropts = new cuneiform_recognition_options;

$ropts->set_language(CF_LANGUAGE_ENGLISH);

$page = cuneiform::recognize("@CMAKE_SOURCE_DIR@/images/quneiform/english.png", $ropts, $fopts);
echo $page->str(CF_FORMAT_TEXT, $fopts);

?>
