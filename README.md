# huawei-scheduler

## Testcase Generation

### Compile

 - Generator: `g++ generator.cpp -o generator`
 - Validator: `g++ validator.cpp -o validator`

### Running
 
 - Generate testcase: `./generator <testcase number>` 
 - Validate testcase: `./validator <testcase number>`

### Graph viewer
Install dependencies
`pip install networkx matplotlib`

Run:
`python3 graph_viewer/main.py <path to testcase>`
