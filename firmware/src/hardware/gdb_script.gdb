target extended-remote localhost:3333
mon speed 10000
mon reset 0
layout split
focus cmd

break main
break main.cpp:21
