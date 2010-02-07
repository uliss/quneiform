#!/bin/sh

find ../ -name '*.h' -or -name '*.cpp' | while read f; do basename $f; done | sort | uniq -c | sort -nr | head -60
