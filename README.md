# scop
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

## Temporary dependency
```
cd scop/include
git clone https://github.com/g-truc/glm.git tmp
mv tmp/glm .
rm -rf tmp
```
