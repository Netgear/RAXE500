#!/bin/sh


###############################################################################
#
# This is the build script for cross-compiling an ARMv7 based docker daemon and
# client from a x86_64 Ubuntu machine.
#
###############################################################################


CURDIR=${PWD}
README_ORIG=${CURDIR}/README.txt
README_NEW=${CURDIR}/README.txt.mod
GITHUB=https://github.com


# =================
# PRIVATE FUNCTIONS
# =================

# Helper function for extracting and decompressing the downloaded
# cross-compilers
decompress_tools() {

  decompress=0
  attempt=3

  # This local function checks if all the files in docker_ws/tools
  # are already in executable format, if not chances are that they
  # are compressed and require decompression.
  check_compressed_files() {
    decompress=0
    for filename in ${CURDIR}/docker_ws/tools/*; do
      if [ -z "$(file ${filename} | grep executable)" ]; then
        decompress=1
      fi
    done
  }

  # Check if all files in the tools folder are in executable format
  check_compressed_files

  # If there are files that are NOT in executable format, check
  # if they are in gzip or tar format and try to decompress them.
  # This will be done recursively until all files in the folder
  # are in executable format. In order to avoid getting into
  # infinite loop, the logic will stop after 3 iterations and the
  # flag $decompress flag will be set to 1
  while [[ ${decompress} == 1 ]] && [[ ${attempt} != 0 ]]; do
    for filename in ${CURDIR}/docker_ws/tools/*; do
      if [ ! -z "$(file ${filename} | grep gzip)" ]; then
        gunzip ${filename}
        if [ -f ${filename} ]; then
          rm ${filename}
        fi
      fi
    done
    for filename in ${CURDIR}/docker_ws/tools/*; do
      if [ ! -z "$(file ${filename} | grep tar)" ]; then
        tar --strip-components=1 -xf ${filename}
        rm ${filename}
        if [ -f ${filename} ]; then
          rm ${filename}
        fi
      fi
    done
    check_compressed_files
    attempt=${attempt}-1;
  done
}


# ======================
# PARSE INPUT PARAMETERS
# ======================
check_dependency=1
while getopts "h?q" opt; do
  case "$opt" in
  q)
    check_dependency=0
    ;;
  h|\?)
    echo "Usage: $0 {-q}"
    echo "  - q: don't ask depdency questions"
    exit
    ;;
  esac
done
echo ""


# ===========================
# CHECK DEPENDENCY (OPTIONAL)
# ===========================

if [[ ${check_dependency} == 1 ]]; then
  echo "0) Dependency Questions..."
  echo "--------------------------"
  echo ""
  echo "a. Have you installed docker in your build machine?"
  echo "   Reference: https://www.digitalocean.com/community/tutorials/how-to-install-and-use-docker-on-ubuntu-16-04"
  select yn in "Yes" "No"; do
    case $yn in
      Yes ) break;;
      No ) 
        echo "ERROR: Dependency not met!!!"
        exit;;
    esac
  done
  echo ""
  echo "b. Have you updated the DNS setting of your docker?"
  echo "   Reference: https://robinwinslow.uk/2016/06/23/fix-docker-networking-dns/"
  select yn in "Yes" "No"; do
    case $yn in
      Yes ) break;;
      No ) 
        echo "ERROR: Dependency not met!!!"
        exit;;
    esac
  done
  echo ""
  echo "c. Have you updated your server such that docker can be run without sudo?"
  echo "   Reference: https://github.com/sindresorhus/guides/blob/master/docker-without-sudo.md"
  select yn in "Yes" "No"; do
    case $yn in
      Yes ) break;;
      No ) 
        echo "ERROR: Dependency not met!!!"
        exit;;
    esac
  done
  echo ""
fi



# ==================
# SCRIPT STARTS HERE
# ==================

# If ${CURDIR}/docker_ws/docker/bundles exits, it implies that docker was built
# successfully once.  We need to run docker to clean up the folder before we
# can delete the docker_ws folder...
if [[ -d ${CURDIR}/docker_ws/docker/bundles ]]; then
  cd ${CURDIR}/docker_ws/docker
  docker run --cpuset-cpus 0 --cpuset-mems 0 -v $PWD:/go/src/github.com/docker/docker --privileged -it docker.armhf "rm -rf bundles"
fi
rm -rf ${CURDIR}/docker_ws
mkdir ${CURDIR}/docker_ws
cd ${CURDIR}/docker_ws

# Make a copy of README.txt because README.txt itself is a patch file and we will
# manipulate it if a git mirror set up for github.com is detected
cp ${README_ORIG} ${README_NEW}


# 1) Parse README.txt in which it contains all the versioning info of where to
#    get the docker source code and the cross compilers

git_url=$(grep -m 1 "<git>" ${README_NEW} | sed 's/^[^:]*: //g')
branch=$(grep -m 1 "<branch>" ${README_NEW} | sed 's/^[^:]*: //g')
tag=$(grep -m 1 "<tag>" ${README_NEW} | sed 's/^[^:]*: //g')
qemu=$(grep -m 1 "<qemu-arm-static>" ${README_NEW} | sed 's/^[^:]*: //g')
rexin=$(grep -m 1 "<resin-xbuild>" ${README_NEW} | sed 's/^[^:]*: //g')

echo "1) Parsing README.txt for docker build info..."
echo "----------------------------------------------"
echo "  - The following information will be used to build docker..."
echo "    git URL         : ${git_url}"
echo "    branch          : ${branch}"
echo "    tag             : ${tag}"
echo "    qemu-arm-static : ${qemu}"
echo "    rexin-xbuild    : ${rexin}"
echo "  - Parsing completed!"
echo ""



# 2) Check if git mirror has been set up to expedite the download

echo "2) Checking if a git mirror is set up for github.com..."
echo "-------------------------------------------------------"
mirror_url=$(grep $GITHUB ~/.gitconfig | sed /^#/d)
if [ -z "${mirror_url}" ]; then
  echo "  - WARNING: No git mirror detected for $GITHUB"
else
  echo "  - git mirror for github detected in ~/.gitconfig:"
  echo "      ${mirror_url}";
  mirror_url=$(grep $GITHUB ~/.gitconfig | sed /^#/d | sed 's/.*\(git:.*com\)"].*/\1/')
  mirror_url_escaped=$(echo ${mirror_url} | sed -e 's|\/|\\/|g')
  echo "  - git mirror url extracted:"
  echo "      ${mirror_url}"

  # Check if the patch file contains a line that specifies a 
  # git mirror can be used
  patch_line=$(grep -nr "\[git mirror URL\]" ${README_NEW} | grep insteadof)

  if [ ! -z "${patch_line}" ]; then

    echo -e "  - Patching READM.txt to use git mirror...\c"

    # Get the line number to be patched
    patch_line_number=$(echo ${patch_line} | sed 's/:.*//')

    # Manipulate the line to be patched such that it uses
    # the extracted URL of the git mirror

    # Remove the line number and uncomment the line.
    patch_line=$(echo ${patch_line} | sed 's/^[^:]*://')
    patch_line=$(echo ${patch_line} | sed -e 's|#||')

    # Escape all "\"s
    patch_line=$(echo ${patch_line} | sed -e 's|\\|\\\\|g')
    # Insert the git mirror URL 
    patch_line=$(echo ${patch_line} | sed -e "s|\[git mirror URL\]|${mirror_url_escaped}|g")
    # Escape all "/"s
    patch_line=$(echo ${patch_line} | sed -e 's|\/|\\/|g')

    # replace line with the manipulated string
    sed -i -e "${patch_line_number}s/.*/${patch_line}/" ${README_NEW}

    echo "Patching completed!"

  fi
fi
echo "  - Check completed!"
echo ""



# 3) Basic sanity test to check if the server has the basic requirements
#    for building docker daemon and docker client

echo "3) Checking basic system requirements..."
echo "----------------------------------------"
if [ -z "$(uname -a | grep x86_64)" ]; then
  echo "  - ERROR: Build server isn't x86_64..."
  exit
else
  echo "  - GOOD: build server is a x86_64 machine..."
fi 
if [ -z "$(which docker)" ]; then
  echo "  - ERROR: Docker has NOT been installed in the server..."
  exit
else
  echo "  - GOOD: docker is installed in the server..."
fi 
if [ -z "$(which wget)" ]; then
  echo "  - ERROR: wget has NOT been installed in the server..."
  exit
else
  echo "  - GOOD: wget found in the system..."
fi
if [ -z "$(which git)" ]; then
  echo "  - ERROR: git has NOT been installed in the server..."
  exit
else
  echo "  - GOOD: git found in the system..."
fi
if [ -z "$(which tar)" ]; then
  echo "  - ERROR: tar has NOT been installed in the server..."
  exit
else
  echo "  - GOOD: tar found in the system..."
fi
if [ -z "$(which gunzip)" ]; then
  echo "  - ERROR: gunzip has NOT been installed in the server..."
  exit
else
  echo "  - GOOD: gunzip found in the system..."
fi
echo "  - Check completed!"
echo ""



# 4) Get the cross-compilers.

echo "4) Downloading cross-compilers..."
echo "---------------------------------"

rm -rf ${CURDIR}/docker_ws/tools
mkdir ${CURDIR}/docker_ws/tools
cd ${CURDIR}/docker_ws/tools
echo "  - Downloading qemu-arm-static..."
wget -q ${qemu}
echo "  - Downloading rexin-xbuild..."
wget -q ${rexin}
if [[ $(ls | wc -l) != 2 ]]; then
  echo "  - ERROR: Download failed..."
  exit
fi
echo "  - Download completed!"
echo "  - Checking downloaded files and their formats..."
# The downloaded files may be compressed in tar/gzip/etc.
# the following function would check the downloaded
# files and try to decompress them if needed 
decompress_tools
if [[ $decompress != 0 ]]; then
  echo "  - ERROR: Downloaded files are NOT in the right format!"
  exit
elif [ ! -f ${CURDIR}/docker_ws/tools/qemu-arm-static ]; then
  echo "  - ERROR: qemu-arm-static not downloaded successfully!"
  exit
elif [ ! -f ${CURDIR}/docker_ws/tools/resin-xbuild ]; then
  echo "  - ERROR: resin-xbuild not downloaded successfully!"
  exit
else
  echo "  - Check completed!"
fi
chmod a+x ${CURDIR}/docker_ws/tools/*
echo ""



# 5) Fetch docker source code
echo "5) Fetching docker's source code..."
echo "-----------------------------------"

cd ${CURDIR}/docker_ws
git clone -v ${git_url}
cd docker
git checkout ${branch}
git reset --hard ${tag}

# Check if the git operations are successful
if [ ! -d "${CURDIR}/docker_ws/docker" ]; then
  echo "  - ERROR: git operations failed - git clone failed!"
  exit
elif [[ $(git log -n 1 | grep commit | sed -e 's/commit //g') != ${tag} ]]; then
  echo "  - ERROR: git operations failed - git tag mismatch!"
  exit
else
  echo "  - git operations completed!"
fi
echo ""



# 6) Apply patch to the docker source code to allow cross compilation
echo "6) Applying patch to docker's source code to allow cross compilation..."
echo "-----------------------------------------------------------------------"

patch -s -f < ${README_NEW}
if [[ $? == 0 ]]; then
  echo "  - Patching completed!"
else
  echo "  - ERROR: Patching was unsuccessful!"
  exit
fi
echo ""



# 7) Build docker's cross-compilation build environment
echo "7) Building docker's cross-compilation build environment..."
echo "-----------------------------------------------------------"

cp ${CURDIR}/docker_ws/tools/* ${CURDIR}/docker_ws/docker
docker build --cpuset-cpus 0 --cpuset-mems 0 -t docker.armhf -f Dockerfile.armhf .
if [[ $? == 0 ]]; then
  echo "  - Docker cross-compilation environment was created successfully!"
else
  echo "  - ERROR: Failed to create docker cross-compilation environment!"
  exit
fi
echo ""



# 8) Launch docker's build environment in bash
echo "8) Building docker daemon and client in docker's cross-compilation environment..."
echo "---------------------------------------------------------------------------------"

docker run --cpuset-cpus 0 --cpuset-mems 0 -v $PWD:/go/src/github.com/docker/docker --privileged -it docker.armhf hack/make.sh
if [ ! -d "${CURDIR}/docker_ws/docker/bundles/latest/binary-client" ]; then
  echo "  - ERROR: Failed to build docker client!"
  exit
elif [ ! -d "${CURDIR}/docker_ws/docker/bundles/latest/binary-daemon" ]; then
  echo "  - ERROR: Failed to build docker daemon!"
  exit
else
  echo "  - docker daemon and clients were built successfully!"
fi
echo ""


# 9) Copy the resulting binaries to the bin folder
echo "9) Copying the resulting binaries to the bin folder..."
echo "------------------------------------------------------"

cp -rf ${CURDIR}/docker_ws/docker/bundles/latest/binary-client/* ${CURDIR}/docker-binaries
if [[ $? != 0 ]]; then
  echo "  - ERROR: Failed to copy binary-client!"
  exit
fi
cp -rf ${CURDIR}/docker_ws/docker/bundles/latest/binary-daemon/* ${CURDIR}/docker-binaries
if [[ $? != 0 ]]; then
  echo "  - ERROR: Failed to copy binary-daemon!"
  exit
fi
echo "  - Done copying!"
echo ""

echo "!!! BUILD COMPLETED !!!"
echo ""
