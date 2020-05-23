###how to use docker to install linux system in macos
+ make sure docker has been successfully installed in advance;
+ command needing to input:
    - docker search centos;
    - docker pull centos ( pull the latest version of centos by default);
    - docker images(list the iso files installed);
    - docker run it -d -name mycentos -p 8088:80 centos
    (-name: define name of your container; -p: define port mappings: the former is virtual machine port,the latter is container port; -it: interaction model)
+ since the default linux system(centos etc...) just intalled is the simplest version by default; you need to intall the software you need by yourself; 
 such as:
    ```cpp
    sudo yum install gcc;
    sudo yum install gcc-c++;
    sudo yum install ncurses(if you     type 'clear',warning: "bash     command not found");
    ```
+ then you need to save the operations you typed to virtual system in the container:
    - open a new tab terminal and execute the command below:
        - ]# docker ps;
        ouput like this:
        ```cpp
        CONTAINER ID        IMAGE               COMMAND             CREATED             STATUS              PORTS                  NAMES
        1a0f0c50f255        470671670cac        "/bin/bash"         11 hours ago        Up 11 hours         0.0.0.0:8088->80/tcp   xina_centos

        ```
        - ]# docker commit 1a0f0c50f255 470671670cac ( docker commit [CONTAINER ID] [IMAGE])
        - turn to the former tab(which runs the linux system) and type the command:
        ]# exit
+ by the way you can inspect the particular infomation by typing:
    - ]# docker inspect [id] ( docker inspect '1a')