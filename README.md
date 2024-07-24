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

 - Generator: `g++ testcase_generator/generator.cpp -o generator`
 - Validator: `g++ testcase_generator/validator.cpp -o validator`

### Running
 
 - Generate testcase: `./generator <testcase number>` 
 - Validate testcase: `./validator <testcase number>`

## Schedulers

Custom schedulers can be added in any language.
APIs for the implementation are currently avalaible for the following languages:

 - C++

## Viewers
Install dependencies:
`pip install networkx matplotlib`

Run:
 - Testcase visualiser: `python3 graph_viewer/main.py <testcase number>`
 - Schedule visualiser: `python3 schedule_viewer/main.py <testcase number>`
