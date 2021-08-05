# scpshell
A POSIX shell written in C with the scplib \
It's purpos is mainly to train my skills in C and test the scplib.

## Credits
* [Build Your Own Shell](https://github.com/tokenrove/build-your-own-shell) by [Julian Squires](https://github.com/tokenrove)

## Makefile

### Tests
Ths shell is tested with the validation script of [Build Your Own Shell](https://github.com/tokenrove/build-your-own-shell). \
Run `make validate_scpshell` or `make validate_strict_scpshell` for pedantic, to execute the script. \
You can also test the shell yourself by running `make run_scpshell` or `make run_strict_scpshell` for pedantic. \
Alternatifely you can simply compile the shell by running `make scpshell` of `make strict_scpshell` for pedantic.

### Installation
Run `make install` as root, it will copy the `scpshell` file in your `/usr/bin/`. \
You can then run the shell directly as `scpshell`.
