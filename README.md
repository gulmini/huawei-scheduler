# huawei-scheduler

## Configuration

By default the executables look for files in the directories specified in the environment variables.

To source defaults values for these variables: `export $(grep -v '^#' config.env | xargs)`.

The variables are:

 - `SCHED_GEN_FOLDER`: directory for generation files;
 - `SCHED_TC_FOLDER`: directory for generated testcases;
 - `SCHED_OUTPUT_FOLDER`: directory for schedules.
 - `SCHED_GRAPH_VIS_FOLDER`: directory for testcase visuals.
 - `SCHED_OUTPUT_VIS_FOLDER`: directory for schedule visuals.

## Testcase Generation

### Compile

 - Generator: `g++ testcase_generator/generator.cpp -o testcase_generator/generator`
 - Validator: `g++ testcase_generator/validator.cpp -o testcase_generator/validator`

### Running
 
 - Generate testcase: `testcase_generator/generator <testcase number>` 
 - Validate testcase: `testcase_generator/validator <testcase number>`

### Scripts

A script is provided to generate and validate testcases in bulk:

 - `scripts/generate.sh` generates from all gen files in `SCHED_GEN_FOLDER`.
 - `scripts/generate.sh <x>` generates only from the x-th gen file.
 - `scripts/generate.sh <l> <r>` generates from the gen files in the range [l, r].

## Schedulers

Custom schedulers can be added in any language.
Schedulers should be in their own subfolder of `schedulers` and expose an executable named `sched`.

APIs for the implementation are currently avalaible for the following languages:

 - C++

### Checker

The schedule checker can be compiled with: `g++ schedule_validator/validator.cpp -o schedule_validator/validator`.

A schedule can be checked with `schedule_validator/validator <testcase number>`

### Scripts

A script is provided to schedule testcases and validate outputs in bulk:

 - `scripts/schedule.sh <s>` schedules all tc files in `SCHED_TC_FOLDER` using the scheduler located at `schedulers/<s>/sched`
 - `scripts/schedule.sh <s> <x>` schedules only the x-th tc file.
 - `scripts/schedule.sh <s> <l> <r>` schedules the tc files in the range [l, r].

## Viewers
Install dependencies:
`pip install networkx matplotlib`

Run:
 - Testcase visualiser: `python3 graph_viewer/main.py <testcase number>`
 - Schedule visualiser: `python3 schedule_viewer/main.py <testcase number>`
