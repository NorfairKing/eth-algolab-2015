# Algorithms Lab

This repository contains everything required to build a comprehensive pdf guide to bring to the ETH Algorithms lab all-you-can-carry exam.

- A set of step-by-step instructions to solve an exam question
- A section to help you to model solutions and get started on coding them
- A troubleshooting section: What to do when your code inexplicably doesn't work.
- A few pieces of math that could come in handy.
- My entire _collection of solutions_ including writeups, indexed by solution strategy.
- A build system to use while practicing. This should take care of building, running and testing your solutions. No need to waste time doing that manually.

## Building the guide

`make` should do the trick.

Make sure that you hove these dependencies:
- `enscript`
- `latexmk`
- `pygmentize`

## Contributing

Pull-requests are welcome.
Before contributing to this project, make sure you install the git hook for the pre-commit checks:

Using [Super User Spark](https://github.com/NorfairKing/super-user-spark):
```
spark deploy hooks.sus
```

## Build System Usage

When in the directory for a particular problem, the following command will help you.
```
../algo
```

This script will build, run and test the solution you want with `../algo <solution>` and `../algo publish` will compile the writeup.
