g++ testcase_generator/generator.cpp -o testcase_generator/generator
g++ testcase_generator/validator.cpp -o testcase_generator/validator
g++ schedule_validator/validator.cpp -o schedule_validator/validator

for scheduler in $(ls -d schedulers/*/);
do
    cd $scheduler
    ./compile.sh
    cd ../..
done