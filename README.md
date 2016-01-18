# Algorithms Lab

## Building the guide

`make` should do the trick.

## Contributing

Pull-requests are welcome.
Before contributing to this project, make sure you install the git hook for the pre-commit checks:

Using [Super User Spark](https://github.com/NorfairKing/super-user-spark):
```
spark deploy hooks.sus
```

Or just install `script/pre_commit_test.sh` into `.git/hooks/pre-commit`

## Depedencies

- `enscript`
- `latexmk`
- `pygmentize`

## Build System Usage

When in the directory for a particular problem, the following command will help you.
```
../algo
```

This script will build, run and test the solution you want with `../algo <solution>` and `../algo publish` will compile the writeup.
