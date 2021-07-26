-Para compilar para o PC Host, utilize a "libethercat" e "CMakeLists.txt" que se encontram na pasta "SoftwareAGV". Com o termial em "SoftwareAGV" digite os comandos:

cmake .

make

cd bin

./AGVbeckhoff


-Para compilar cruzado, substitua "libethercat" e "CMakeLists.txt" que se encontram na pasta "SoftwareAGV" pelos arquivos presentes na pasta "Cross_Compile". Com o termial em "SoftwareAGV" digite os comandos:

source /usr/local/oecore-x86_64/environment-setup-armv7at2hf-neon-angstrom-linux-gnueabi

cmake -DOCVV=3_1 .

make

