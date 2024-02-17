#!/bin/sh -ex

find . -name '*.[hc]' -print0 | xargs -0 -n 1 clang-format --dry-run --Werror

make tests