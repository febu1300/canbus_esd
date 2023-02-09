# Install Driver
The Linux kernel that supports the driver we're about to install is  5.4.207.  
Check the kernel version using uname -r then we download the driver from esd.eu repository and build it with make command. 

If the kernel version is greater than 5.4.207, you must compile it from the source.

```bash

wget https://esd.eu/fileadmin/esd/software/drivers/can/linux/c402/esdcan-pcie402-linux-x86_64-4.1.4.tgz

cd esdcan-pcie402-linux-x86_64-4.1.4

make

```

After a successful build you should find a file that ends with .ko

Move this file to the corrosponding Kernel moudles directory. 

```bash

 sudo cp esdcan-pcie402.ko /lib/modules/5.4.207/

```

In the same directory, lib subdirectory you'll find a file libntcan.so.x.x.x .
Move this file to a standard shared library directory. 

```bash
sudo cp libntcan.so.4.2.9 /usr/local/lib64

ldconfig -n /usr/local/lib64

```

# Compile and run test files
You can find the test files to send and recieve can messages using esdcan pci adopter in our git repository .

```bash
git clone https://git.hs-coburg.de/Testfahrzeug_LuCCa/esdcan.git
cd esdcan

``` 
Run the following command to compile the source files. 

```bash
mkdir build
cd build
cmake ..
sudo make install

```

## Receive and Send
If the build process went smoothly, you should find the executables in build/bin sub-directory.
you can cd into the directory and run the programs. 
```bash
./canreceive

./cansend
```
**_NOTE:_** : Do not forget to connect the pcan device using a terminator adopter to the computer.
