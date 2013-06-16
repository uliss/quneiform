#! /bin/sh
lupdate `find . -name \*.cpp` -codecfortr UTF-8 -locations none -no-obsolete -ts i18n/ksane_*.ts

