# Building WordleMax

* Clone the repository
* Navigate to the repository on Terminal/Command Line using `cd`

Based on your OS, do the following:

## Linux

### Ubuntu / Debian

```bash
sudo apt update
sudo apt install build-essential libcurl4-openssl-dev
g++ index.cpp -o wordle -lcurl
```

### Fedora / RHEL

```bash
sudo dnf install gcc-c++ libcurl-devel
g++ index.cpp -o wordle -lcurl
```

### Arch Linux

```bash
sudo pacman -S base-devel curl
g++ index.cpp -o wordle -lcurl
```

### openSUSE

```bash
sudo zypper install gcc-c++ libcurl-devel
g++ index.cpp -o wordle -lcurl
```


### Run the Executable

```bash
./wordle
```

## Windows

1. **Install MSYS2**

   * Download from [https://www.msys2.org](https://www.msys2.org) and follow the installer.

2. **Update packages**
   Open the MSYS2 terminal and run:

   ```bash
   pacman -Syu
   ```

3. **Install build tools and libcurl**

   ```bash
   pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-curl
   ```

4. **Compile**
   Use the MinGW64 shell (not the MSYS shell) and run:

   ```bash
   g++ index.cpp -o wordle.exe -lcurl
   ```

5. **Run**

   ```bash
   ./wordle.exe
   ```




