curr_dir=$(pwd $(dirname $0))
root_dir=${curr_dir}
build_dir=${root_dir}/build

compile_env_check(){
    if [ ! -d ${build_dir} ];then
        mkdir ${build_dir}
    else
        cd ${build_dir}
        rm -rf ./*
    fi
}

compile(){
    compile_env_check

    cd ${build_dir}
    cmake ..
    make -j4
}

compile
