Docker Binaries Summary:
========================

Version       : 18.03.1-ce
Release Notes : https://docs.docker.com/release-notes/docker-ce/#18031-ce-2018-04-26
URL           : https://download.docker.com/linux/static/stable/aarch64/docker-18.03.1-ce.tgz
                https://download.docker.com/linux/static/stable/armhf/docker-18.03.1-ce.tgz
License       : All docker-ce binaries mentioned and the patch listed below are
                licensed under the terms as noted by Docker, Inc. below:

                Docker Engine
                Copyright 2013-2018 Docker, Inc.

                Licensed under the Apache License, Version 2.0 (the "License");
                you may not use this file except in compliance with the License.
                You may obtain a copy of the License at

                    http://www.apache.org/licenses/LICENSE-2.0

                Unless required by applicable law or agreed to in writing, software
                distributed under the License is distributed on an "AS IS" BASIS,
                WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
                See the License for the specific language governing permissions and
                limitations under the License.


How to enable Docker in your image:
===================================

Note that docker is currently ONLY enabled on ARMv7 and ARMv8 based platforms
and it has only been tested on 963148GW, and 96858GWO. It should be noted that
despite 963138GW being an ARMv7 based SoC, docker CANNOT be supported on that
platform due to the lack of NEON coprocessor in one of the ARM cores. 

1. Docker is integrated with BEEP, therefore BEEP must be enabled alongside with
   docker.  Use the ./release/maketargets script to create your target profile
   with _DOCKER and _BEEP. For example:

   ./release/maketargets 963148GW_DOCKER_BEEP

2. Build your target with the newly created profile. For example:

   ./make PROFILE=963148GW_DOCKER_BEEP


How to recreate the binaries from a Ubuntu 16.04 x86_64 machine:
================================================================

NOTE 1: Generally there is no need for creating your own docker binaries unless
        you want to make custom changes to the docker daemons or client. A
        docker client/daemon should be portable between platforms as long as
        they are built for the same architecture. For instance, if you find on
        internet a set of docker images that were built for ARMv7, they should
        work as-is on our ARMv7 based platforms as well. The following website
        archives all the official docker binaries built for the various CPU
        architectures that docker supports:
        URL: https://download.docker.com/linux/static/stable/

NOTE 2: A build script (e.g. build.sh) found in the same folder as this
        README.txt automates the steps below. As you will see from the following
        instructions, there are a lot of dependencies that must be met in order
        for the build process to complete successfully. The build script tests
        for those dependencies before building. If the script fails to build,
        please cross-reference the following instructions with the error
        messages and output of the script to understand the potential sources of
        failures.

NOTE 3: The following instructions and build.sh were put together based on the
        docker version and tools as specified by the tags below. It's possible
        that they do not work as-is with newer docker version, but the general
        concept should still apply.

        <git>             : https://github.com/docker/docker.git
        <branch>          : 17.03.x
        <tag>             : c6d412e329c85f32a4b2269b49aaa0794affcf88
        <qemu-arm-static> : https://github.com/resin-io/qemu/releases/download/qemu-2.5.0-resin-rc3-arm/qemu-2.5.0-resin-rc3-arm.tar.gz
        <resin-xbuild>    : https://github.com/resin-io-projects/armv7hf-debian-qemu/raw/6fe6a15fd81b28959d11cdee69930136a2ff85b4/bin/resin-xbuild

1. To build docker, you need to install docker in your Ubuntu server first.
   Install docker on your own Ubuntu Server as per the following instructions:

   https://www.digitalocean.com/community/tutorials/how-to-install-and-use-docker-on-ubuntu-16-04

2. Change your Ubuntu server such that docker can be run without sudo:

   https://github.com/sindresorhus/guides/blob/master/docker-without-sudo.md

3. Fix DNS setting for docker in your own Ubuntu build server: 

   https://robinwinslow.uk/2016/06/23/fix-docker-networking-dns/

4. Fetch the docker source code from github.com as follows, by replacing all
   <?> with the corresponding value as listed in Docker Binaries Summary 
   section above.

   mkdir ~/docker_ws
   cd ~/docker_ws
   git clone <git>
   cd docker
   git checkout <branch>
   git reset --hard <tag>

   Note that the github server may timeout or reset your git connection
   depending on how loaded the server is.  It's recommended to set up a local
   git mirror to mirror all the git repos that are fetched; this will ensure
   all developers to have a reliable way of accessing the source code.

5. Download qemu-arm-static and resin-xbuild from the sources as indicated
   above and put them under ~/docker_ws/docker/. These are tools for cross-
   compiling docker binaries for ARMv7 from your x86_64 Ubuntu machine. 

   Broadcom Internal Note: A copy of these tools are archived and can be found
   under /projects/comengine/linux/tools/docker.

6. Apply the patch at the bottom of this page in ~/docker_ws/docker. The patch
   modifies Dockerfile.armhf such that it will do cross-compilation using the
   tools that you just downloaded. It also contains the necessary changes
   (currently commented out) for fetching the source code from a git mirror as
   recommended in step 4 above. In the case if a local git mirror is used, don't
   forget to uncomment the corresponding block and fill in the correct URL of
   your git mirror.

   Note that this README.txt is structured such that you can simply use it as a
   patch. For example:

   cd ~/docker_ws/docker
   patch < <path_to_this file>/README.txt

7. Build docker's cross-compilation build environment

   docker build --cpuset-cpus 0 --cpuset-mems 0 -t docker.armhf -f Dockerfile.armhf .
   
   Note: The options --cpuset-cpus and --cpuset-mems are needed as it seems
   that the qemu didn't like to run across multiple CPUs.

8. Launch docker's build environment in bash

   docker run --cpuset-cpus 0 --cpuset-mems 0 -v $PWD:/go/src/github.com/docker/docker --privileged -it docker.armhf bash

9. Build docker daemon and client inside the docker container:

   ./hack/make.sh

   As part of the build process, the script will fetch more packages from the
   github server. As noted previously, the github server may timeout or reset
   your git connection depending on how loaded the server is.  It's recommended
   to set up a local git mirror to mirror all the git repos that are fetched;
   this will ensure all developers to have a reliable way of accessing the
   source code.

10. Incorporate your own compiled binaries into your target image.

   The resulting docker daemon and client binaries are created in
   ~/docker_ws/docker/bundles/latest/. In order to incorporate your own
   compiled binaries into your image, simply copy both binary-client
   and binary-daemon from the aforementioned folder to the corresponding 
   locations in userspace/public/apps/docker/docker-binaries of your
   workspace and rebuild your target image.


How to build or package your own docker image:
==============================================

   If you had followed the instructions above, you should have everything
   you need to cross-compile or package your own docker image following the
   steps in https://docs.docker.com/engine/userguide/eng-image/baseimages/

   Note that you can easily package a docker image without actually involving
   docker's cross-compilation build environment. This can be accomplished by
   copying the applications of interests along with the libraries that they
   depend on into your docker image's filesystem by using the "COPY" command
   in the Dockerfile of your docker image.
   
   For instance, you can copy an iperf application that was compiled for ARMv7
   outside of docker's cross-compilation environment (e.g. iperf that was
   cross-compiled by the normal build process) into your docker image as long
   as the libraries that your iperf depends on are also in the docker image's
   filesystem.

   Note that when packaging a docker image, an ENTRYPOINT must be specified as
   all docker images are launched as detached and interactive containers.  If
   an ENTRYPOINT isn't specified when packaging your docker image, your docker
   container will NOT be launched.

   Here is a sample of the Dockerfile for packaging iperf:

      FROM armhf/alpine
      
      # copy iperf from local folder to alpine's filesystem
      COPY <path to local iperf>/iperf /bin/

      # copy libraries that iperf depends on to alpine's filesystem
      COPY <path to libs that iperf depends on>/iperf_lib/* /lib/

      # specify entry point of the docker image when it is launched.  
      ENTRYPOINT /bin/ash

   With the above Dockerfile, you can then create a docker image by invoking
   "docker build -f ./Dockerfile ."

   In the case you do want to compile your own docker image in the docker's
   cross-compilation environment, adjustment similar to the patch below must
   be made to the Dockerfile of your docker image so that qemu-arm-static
   and resin-xbuild are used.


Patch for enabling cross-compilation for ARMv7 in a x86_64 Ubuntu environment:
==============================================================================

diff --git a/Dockerfile.armhf b/Dockerfile.armhf
index 1aebc16..4331b6e 100644
--- a/Dockerfile.armhf
+++ b/Dockerfile.armhf
@@ -17,6 +17,15 @@
 
 FROM armhf/debian:jessie
 
+ENV QEMU_EXECVE 1
+
+COPY qemu-arm-static /usr/bin/qemu-arm-static
+COPY resin-xbuild /usr/bin/resin-xbuild
+
+RUN [ "qemu-arm-static", "/bin/sh", "-c", "ln -s resin-xbuild /usr/bin/cross-build-start; ln -s resin-xbuild /usr/bin/cross-build-end; ln /bin/sh /bin/sh.real" ]
+
+RUN [ "cross-build-start" ]
+
 # allow replacing httpredir or deb mirror
 ARG APT_MIRROR=deb.debian.org
 RUN sed -ri "s/(httpredir|deb).debian.org/$APT_MIRROR/g" /etc/apt/sources.list
@@ -55,6 +64,12 @@ RUN apt-get update && apt-get install -y \
        --no-install-recommends \
        && pip install awscli==1.10.15
 
+# If you have a local git mirror set up for github.com, you should uncomment
+# the following RUN command and replace [git mirror URL] with your local git
+# mirror's URL (e.g. git://a.b.c.d/github] in order for docker to take
+# advantage of it.
+#RUN echo "[url \"[git mirror URL]\"] insteadof = https://github.com" >> ~/.gitconfig
+
 # Get lvm2 source for compiling statically
 ENV LVM2_VERSION 2.02.103
 RUN mkdir -p /usr/local/lvm2 \
@@ -176,7 +191,7 @@ COPY hack/dockerfile/binaries-commits /tmp/binaries-commits
 COPY hack/dockerfile/install-binaries.sh /tmp/install-binaries.sh
 RUN /tmp/install-binaries.sh tomlv vndr runc containerd tini proxy
 
-ENTRYPOINT ["hack/dind"]
-
 # Upload docker source
 COPY . /go/src/github.com/docker/docker
+
+ENTRYPOINT ["/bin/sh", "-c"]
