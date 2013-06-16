#! /bin/sh
lupdate `find . -name \*.mm -or -name \*.cpp -or -name \*.h -or -name \*.ui` -locations none -no-obsolete -codecfortr UTF-8 -ts translations/*.ts

