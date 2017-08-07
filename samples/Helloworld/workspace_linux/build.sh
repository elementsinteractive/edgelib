echo Compiling...
echo helloworld.cpp
g++ -O3 -I ../../../include -D__LINUX__ -c ../code/helloworld.cpp -o helloworld.o

echo

echo Linking...
g++ helloworld.o --whole-archive -ledge -ledgerender -lz -lX11 --no-whole-archive -L $HOME/Desktop/edge/lib/linux-x86 -o helloworld

echo

echo helloworld
