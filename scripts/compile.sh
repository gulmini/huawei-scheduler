skip_tools=false

for arg in "$@"; do
    if [ "$arg" = "--no-tools" ]; then
        skip_tools=true
        break
    fi
done

if [ "$skip_tools" = false ]; then
    g++ testcase_generator/generator.cpp -o testcase_generator/generator
    g++ testcase_generator/validator.cpp -o testcase_generator/validator
    g++ schedule_validator/validator.cpp -o schedule_validator/validator
fi

if [ $# -eq 0 ]; then
    for scheduler in $(ls -d schedulers/*/);
    do
        echo $scheduler
        cd $scheduler
        ./compile.sh
        cd ../..
    done
else
    for scheduler in "$@";
    do
        if [ "$scheduler" = "--no-tools" ]; then
            continue
        fi
        cd schedulers/$scheduler
        ./compile.sh
        cd ../..
    done
fi
