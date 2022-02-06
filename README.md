# simplegdb
Simplest Distributed Graph Databases

## dependencies
Mercury

`wget https://github.com/mercury-hpc/mercury/releases/download/v2.1.0/mercury-2.1.0.tar.bz2`

cd mercury-X
mkdir build
cd build
ccmake .. (where ".." is the relative path to the mercury-X directory)

make
make install


berkeleyDB 4.8+

wget http://download.oracle.com/berkeley-db/db-4.8.30.zip
unzip db-4.8.30.zip
cd db-4.8.30
cd build_unix
../dist/configure --prefix=/usr/local --enable-cxx
make
make install


libconfigure 1.0+
`sudo apt-get install -y libconfig-dev`
