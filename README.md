# scop

.obj 3D viewer made with OpenGL in C++

## Subject

https://cdn.intra.42.fr/pdf/pdf/57124/en.subject.pdf

## Building the project
start by downloading the dependencies
```
brew install glew glfw
```
then clone the project
```
git clone https://github.com/usr99/scop -b macos
```
and launch the makefile
```
cd scop
make
```

## Usage
```
./scop [path/to/obj/file]
```

## Temporary dependency installation
```
cd scop/include
git clone https://github.com/g-truc/glm.git tmp
mv tmp/glm .
rm -rf tmp
```
